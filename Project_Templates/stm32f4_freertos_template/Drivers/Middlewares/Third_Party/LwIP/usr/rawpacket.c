/**
  ******************************************************************************
  * @file    rawpacket.c
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    29-September-2015
  * @brief   This file provides set of firmware functions to generate raw packet
  *          and initialization of Ethernet
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------- */
#include "rawpacket.h"

/** @addtogroup STM32F4_DISCOVERY_ETH
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_ETH
  * @brief This file provides set of firmware functions to generate raw packet
  *        and initialization of Ethernet
  *
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_ETH_Exported_Handle
 * @{
 */

/**
  * @}
  */
/* Constants ------------------------------------------------------------------*/
/* This is the MAC address of Target board */
#define EMAC_ADDR12   0x0000101F
#define EMAC_ADDR34   0x0000E012
#define EMAC_ADDR56   0x00001D0C

/* MAC address for destination device */
#define EMAC_DST_ADDR12   0x0000071D
#define EMAC_DST_ADDR34   0x00004D1F
#define EMAC_DST_ADDR56   0x0000001A

/* Private Function ------------------------------------------------------------*/

/** @defgroup STM32F4_DISCOVERY_ETH_Private_Functions
  * @{
  */

/**
 * @brief CRC_init calculation
 * @param pCRC pointer to the CRC area
 */
static void crc32_init(uint32_t *pCRC)
{
    *pCRC = 0xffffffff;
}

/**
 * @brief Calculate CRC value one at a time
 * @param pCRC Pointer to the CRC area
 * @param val8 Passing value to get the CRC
 */
static void crc32_add(uint32_t *pCRC, uint8_t val8)
{
    uint32_t i, poly;
    uint32_t entry;
    uint32_t crc_in;
    uint32_t crc_out;

    crc_in = *pCRC;
    poly = 0xEDB88320L;
    entry = (crc_in ^ ((uint32_t) val8)) & 0xFF;
    for (i = 0; i < 8; i++)
    {
        if (entry & 1)
            entry = (entry >> 1) ^ poly;
        else
            entry >>= 1;
    }
    crc_out = ((crc_in>>8) & 0x00FFFFFF) ^ entry;
    *pCRC = crc_out;
    return;
}

/**
 * @brief Finish CRC calculation
 * @param pCRC Pointer to the CRC area
 */
static void crc32_end(uint32_t *pCRC)
{
    *pCRC ^= 0xffffffff;
}

/**
 * @brief Get the CRC value based on size of the string
 * @param pBfr Pointer to the string
 * @param size Size of the string
 * @return CRC value
 */
static uint32_t crc32_bfr(void *pBfr, uint32_t size)
{
    uint32_t crc32;
    uint8_t  *pu8;

    crc32_init(&crc32);
    pu8 = (uint8_t *) pBfr;
    while (size-- != 0)
    {
        crc32_add(&crc32, *pu8);
        pu8++ ;
    }
    crc32_end(&crc32);
    return ( crc32 );
}
/**
 * @brief Hardware initialization function for Ehternet
 * @param heth Ethernet handle
 *        @arg heth
 */
static void BSP_ETH_MspInit(ETH_HandleTypeDef* heth)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(heth->Instance==ETH)
  {
  /* USER CODE BEGIN ETH_MspInit 0 */
    int i;

    __GPIOE_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
  /* USER CODE END ETH_MspInit 0 */
    /* Peripheral clock enable */
    __ETH_CLK_ENABLE();

    /**ETH GPIO Configuration
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PA2     ------> ETH_MDIO
    PA7     ------> ETH_CRS_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PB11    ------> ETH_TX_EN
    PB12    ------> ETH_TXD0
    PB13    ------> ETH_TXD1
    PE2     ------> ETH_RST
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PE2 */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* USER CODE BEGIN ETH_MspInit 1 */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
    for (i = 0; i < 100; i++);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
    for (i = 0; i < 100; i++);
  /* USER CODE END ETH_MspInit 1 */
  }
}

/**
  * @}
  */

/* Global Functions ------------------------------------------------*/

/** @defgroup STM32F4_DISCOVERY_ETH_Functions
  * @{
  */

/**
 * @brief Packet creation function
 * @param txptr pointer to the packet array
 */
void PacketGen(uint8_t *txptr, uint8_t *payload)
{
  int i;
  uint32_t crcValue;
  uint32_t BodyLength = TX_PACKET_SIZE - 14;

  // Dest address
  (txptr[0]) = EMAC_DST_ADDR56 & 0xFF;
  (txptr[1]) = (EMAC_DST_ADDR56 >> 0x08) & 0xFF;
  (txptr[2]) = EMAC_DST_ADDR34 & 0xFF;
  (txptr[3]) = (EMAC_DST_ADDR34 >> 0x08) & 0xFF;
  (txptr[4]) = EMAC_DST_ADDR12 & 0xFF;
  (txptr[5]) = (EMAC_DST_ADDR12 >> 0x08) & 0xFF;

  // Src address
  (txptr[6]) = EMAC_ADDR56 & 0xFF;
  (txptr[7]) = (EMAC_ADDR56 >> 0x08) & 0xFF;
  (txptr[8]) = EMAC_ADDR34 & 0xFF;
  (txptr[9]) = (EMAC_ADDR34 >> 0x08) & 0xFF;
  (txptr[10]) = EMAC_ADDR12 & 0xFF;
  (txptr[11]) = (EMAC_ADDR12 >> 0x08) & 0xFF;

  // Type or length, body length is TX_PACKET_SIZE - 14 bytes
  (txptr[12]) = BodyLength & 0xFF;
  (txptr[13]) = (BodyLength >> 0x08) & 0xFF;

  // Skip the first 14 bytes for dst, src, and type/length
  for ( i=0; i < BodyLength; i++ )
  {
    (txptr[i+14]) = payload[i];
  }

  // Calculate CRC
    crcValue = crc32_bfr( txptr, TX_PACKET_SIZE );

  // Add 4-byte CRC
  (txptr[TX_PACKET_SIZE]) = (0xff & crcValue);
  (txptr[TX_PACKET_SIZE+1]) = 0xff & (crcValue >> 8 );
  (txptr[TX_PACKET_SIZE+2]) = 0xff & (crcValue >> 16);
  (txptr[TX_PACKET_SIZE+3]) = 0xff & (crcValue >> 24);
}

/**
 * @brief Ethernet Initialization function for raw packet transmission
 * @param heth Ethernet handle
 *        @arg heth
 */
void BSP_ETH_Init(ETH_HandleTypeDef* heth)
{
  uint8_t MACAddr[6];
  heth->Init.MACAddr = &MACAddr[0];
  heth->Instance = ETH;
  heth->Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  heth->Init.Speed = ETH_SPEED_10M;
  heth->Init.DuplexMode = ETH_MODE_FULLDUPLEX;
  heth->Init.PhyAddress = 0;
  heth->Init.RxMode = ETH_RXPOLLING_MODE;
  heth->Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  heth->Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

  BSP_ETH_MspInit(heth);
  if (HAL_ETH_Init(heth) != HAL_OK)
  {
    while(1);
  }
  heth->Instance->DMAOMR  &= ~ETH_DMAOMR_OSF;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */



 /* --------------------------------- End Of File ------------------------------ */
