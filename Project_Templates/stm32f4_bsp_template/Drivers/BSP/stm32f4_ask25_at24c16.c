/**
  ******************************************************************************
  * @file    stm32f4_ask25_at24c16.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file provides set of firmware functions to manage I2C EEPROM
  *          available on STM32F4-ASK25 Kit from Edutech.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_ask25_at24c16.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25_AT24C16
  * @{
  */

/** @defgroup STM32F4_ASK25_AT24C16
  * @brief This file provides set of firmware functions to manage I2C EEPROM
  *        available on STM32F4-ASK25 Kit from Edutech.
  * @{
  */


/** @defgroup STM32F4_ASK25_AT24C16_Exported_Handle
 * @{
 */
I2C_HandleTypeDef hi2c1_at24c16;

/**
  * @}
  */

/** @defgroup STM32F4_ASK25_AT24C16_Private_Functions
  * @{
  */

/**
 * @brief This function is support function for ASK25_I2C1_Init
 *        This function configures GPIO of I2C1
 * @param hi2c  Provide I2C handle
 *      @arg  &hi2c1_at24c16
 * @retval None
 */
static void ASK25_I2C1_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __I2C1_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  }
}


/**
  * @}
  */

/** @defgroup STM32F4_ASK25_AT24C16_Functions
  * @{
  */

/**
 * @brief This Function configures and Initializes I2C1
 * @retval None
 */
void ASK25_I2C1_Init(void)
{
  __GPIOB_CLK_ENABLE();

  hi2c1_at24c16.Instance = I2C1;
  hi2c1_at24c16.Init.ClockSpeed = 100000;
  hi2c1_at24c16.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1_at24c16.Init.OwnAddress1 = 102;
  hi2c1_at24c16.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1_at24c16.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1_at24c16.Init.OwnAddress2 = 0;
  hi2c1_at24c16.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1_at24c16.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  ASK25_I2C1_MspInit(&hi2c1_at24c16);
  HAL_I2C_Init(&hi2c1_at24c16);
}


/**
 * @brief This function is for De-Initializing I2C1
 * @param hi2c Provide I2C handle
 *      @arg  &hi2c1_at24c16
 * @retval None
 */
void ASK25_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
  }
}


/**
 * @brief This function Writes byte at given address
 * @param eep_address   Word Address range[0000 - 2047]
 * @param byte_data     Byte value
 * @return  Status
 */
HAL_StatusTypeDef ASK25_AT24C16_Write_Byte (uint16_t eep_address, uint8_t byte_data)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t set_addr;

  set_addr =(eep_address & 0x7FF) >> 8;

  status = HAL_I2C_Mem_Write_IT(&hi2c1_at24c16, (E2P24C16_ID|(set_addr<<1)), (uint16_t)(eep_address & 0xFF), I2C_MEMADD_SIZE_8BIT, &byte_data, 1);
  HAL_Delay(5);
  return (status);
}


/**
 * @brief This function Writes series of characters from given address
 * @param eep_address   Word Address range[0000 - 2047]
 * @param byte_data     Buffer (uint8_t size)
 * @param length        Length of buffer
 * @return  Status
 */
HAL_StatusTypeDef ASK25_AT24C16_Write (uint16_t eep_address, uint8_t* byte_data, uint16_t length)
{
  /* Transmit setup */
  HAL_StatusTypeDef status = HAL_OK;
  uint16_t ip_len,new_length,i;
  uint16_t new_address;
  uint8_t set_addr, I2C_Tx_Buf[17];

  /* Intern page length(ip_len) gives length from address that can be occupied in page */
  ip_len = 0x10 - (eep_address % 0x10);
  new_address = eep_address + ip_len;

  set_addr =(eep_address & 0x7FF) >> 8;

  if(length > ip_len)
  {
    new_length = length - ip_len;

    for(i=0; i<ip_len; i++)
    {
      I2C_Tx_Buf[i]=*byte_data++;
    }

    status = HAL_I2C_Mem_Write_IT(&hi2c1_at24c16, (E2P24C16_ID|(set_addr<<1)), (uint16_t)(eep_address & 0xFF), I2C_MEMADD_SIZE_8BIT, I2C_Tx_Buf, ip_len);

    if(status == HAL_ERROR) //return status
    {
      return (HAL_ERROR);
    }

    // Recursive function
    HAL_Delay(5);
    ASK25_AT24C16_Write(new_address, byte_data, new_length);
  }

  if(length <= ip_len)
  {
    for(i=0; i<length; i++)
    {
      I2C_Tx_Buf[i]=*byte_data++;
    }

    status = HAL_I2C_Mem_Write_IT(&hi2c1_at24c16, (E2P24C16_ID|(set_addr<<1)), (uint16_t)(eep_address & 0xFF), I2C_MEMADD_SIZE_8BIT, I2C_Tx_Buf, length);
    return(status);
  }
  return(status);
}


/**
 * @brief Reads byte at given address
 * @param eep_address   Word Address range[0000 - 2047]
 * @return Byte Value
 */
uint8_t ASK25_AT24C16_Read_Byte (uint16_t eep_address)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t set_addr, RByte=0;

  set_addr = (eep_address & 0x7FF) >> 8;

  HAL_I2C_Mem_Read_IT(&hi2c1_at24c16, (E2P24C16_ID|(set_addr<<1)), (uint16_t)(eep_address & 0xFF), I2C_MEMADD_SIZE_8BIT, &RByte, 1);
  HAL_Delay(5);
  status = HAL_I2C_Mem_Read_IT(&hi2c1_at24c16, (E2P24C16_ID|(set_addr<<1)), (uint16_t)(eep_address & 0xFF), I2C_MEMADD_SIZE_8BIT, &RByte, 1);

  if (status == HAL_OK)
  {
    return (RByte);
  }
  else
  {
    return (HAL_ERROR);
  }
}


/**
 * @brief Reads array from given address
 * @param eep_address   Word Address range[0000 - 2047]
 * @param buf_data      Buffer (uint8_t size)
 * @param length        Length of Buffer
 * @return Status
 */
HAL_StatusTypeDef ASK25_AT24C16_Read (uint16_t eep_address, uint8_t* buf_data, uint16_t length)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t set_addr;

  set_addr = (eep_address & 0x7FF) >> 8;

  HAL_I2C_Mem_Read_IT(&hi2c1_at24c16, (E2P24C16_ID|(set_addr<<1)), (uint16_t)(eep_address & 0xFF), I2C_MEMADD_SIZE_8BIT, buf_data, 1);
  HAL_Delay(5);
  status = HAL_I2C_Mem_Read_IT(&hi2c1_at24c16, (E2P24C16_ID|(set_addr<<1)), (uint16_t)(eep_address & 0xFF), I2C_MEMADD_SIZE_8BIT, buf_data, length);

  return(status);
}


/**
 * @brief This function can be used to Display Read data stored in array
 * @param string  Buffer (uint8_t size)
 * @param length  Length of Buffer
 */
void ASK25_AT24C16_Display_Array (uint8_t *string, uint16_t length)
{
  while(length)
  {
    vuprintf("%02x  ",*string++);
    length--;
  }
}


/**
 * @brief This function Ask for Address Range you want to display
 * @param mem_start_address   starting address of EEPROM
 * @param mem_end_address     ending address of EEPROM
 * @retval None
 */
void ASK25_AT24C16_Display_Loc (uint16_t mem_start_address, uint16_t mem_end_address)
{
  uint8_t line=0,count=0;
  uint8_t dat;
  uint16_t addr;

  vuprintf("EEPROM Range = 0x000 - 0x7FF \r\n");

  vcp_clr_scr_rst_cur();
  vuprintf("Start: %x03   End: %x03 \r\n",mem_start_address,mem_end_address);

  for(addr=mem_start_address; addr<mem_end_address+1; addr++)
  {
    if(count == 0) vuprintf("%x03   ",addr);

    dat = ASK25_AT24C16_Read_Byte(addr);   // read byte from address

    vuprintf("%x02  ",dat);
    count++;

    if(count == 16)                    // check for last digit entered
    {
      line++;
      count = 0;
      vuprintf("\r\n");
    }

    if(line == 20 || addr == mem_end_address)
    {
      vuprintf("\x1b[24;01HPress any key to continue.");
      line = 0;
      vgetche(BLOCKING);
      vcp_clr_scr_rst_cur();
    }
  }
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

/**
  * @}
  */

/*****************************************END OF FILE******************************************/
