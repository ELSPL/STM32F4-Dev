/******************************************************************//**
* @file   stm32f4_discovery_can.c
* @brief  This file contains implementation of STM32F4 CAN in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_can.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_CAN
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_CAN_Public_Types
 * @{
 */

CAN_HandleTypeDef hcan_bsp1;
CAN_HandleTypeDef hcan_bsp2;

static CanTxMsgTypeDef TxMsg1;
static CanRxMsgTypeDef RxMsg1;
static CanTxMsgTypeDef TxMsg2;
static CanRxMsgTypeDef RxMsg2;

/**
 * @} STM32F4_DISCOVERY_CAN_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_CAN_Private_Functions
 * @{
 */

/**
 * @brief This function enable the CAN by enabling the peripheral clock
 * @param hcan select the can handle
 *        @arg hcan_bsp1
 *        @arg hcan_bsp2
 */
static void BSP_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hcan == &hcan_bsp1)
  {
    /* Peripheral clock enable */
    __CAN1_CLK_ENABLE();

    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    __GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  }

  else if(hcan == &hcan_bsp2)
  {
    /* Peripheral clock enable */
    __CAN2_CLK_ENABLE();
    __CAN1_CLK_ENABLE();

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    __GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_TX_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
  }


}

/**
 * @} STM32F4_DISCOVERY_CAN_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_CAN_Public_Functions
 * @{
 */
/**
 * @brief This function initialize and configure CAN
 * @param hcan select can handle
 *        @arg hcan_bsp1
 *        @arg hcan_bsp2
 * @param can_mode Select the CAN mode
 *        @arg CAN_MODE_LOOPBACK
 *        @arg CAN_MODE_SILENT
 *        @arg CAN_MODE_SILENT_LOOPBACK
 *        @arg CAN_MODE_NORMAL
 * @param fifo_assignment select the fifo0 or fifo1
 */
void BSP_CAN_Init(CAN_HandleTypeDef* hcan, uint32_t can_mode, Fifo_Assignment_Typedef fifo_assignment)
{

  CAN_FilterConfTypeDef  sFilterConfig;

  if(hcan == &hcan_bsp1)
  {
    hcan_bsp1.Instance = CAN1;
    hcan_bsp1.pTxMsg = &TxMsg1;
    hcan_bsp1.pRxMsg = &RxMsg1;

    sFilterConfig.FilterNumber = 0;
  }
  else if (hcan == &hcan_bsp2)
  {
    hcan_bsp2.Instance = CAN2;
    hcan_bsp2.pTxMsg = &TxMsg2;
    hcan_bsp2.pRxMsg = &RxMsg2;

    sFilterConfig.FilterNumber = 14;
  }

  hcan->Init.Prescaler = 16;
  hcan->Init.Mode = can_mode;
  hcan->Init.SJW = CAN_SJW_1TQ;
  hcan->Init.BS1 = CAN_BS1_6TQ;
  hcan->Init.BS2 = CAN_BS2_8TQ;
  hcan->Init.TTCM = DISABLE;
  hcan->Init.ABOM = DISABLE;
  hcan->Init.AWUM = DISABLE;
  hcan->Init.NART = DISABLE;
  hcan->Init.RFLM = DISABLE;
  hcan->Init.TXFP = DISABLE;

  BSP_CAN_MspInit(hcan);
  HAL_CAN_Init(hcan);

  if(hcan == &hcan_bsp1)
  {
    sFilterConfig.FilterNumber = 0;   // 0-13
  }
  else if (hcan == &hcan_bsp2)
  {
    sFilterConfig.FilterNumber = 14;  // 14-27
  }
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = fifo_assignment;
  sFilterConfig.FilterActivation = ENABLE;

  HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
}

/**
 * @brief This function disable the CAN by disabling the peripheral clock
 * @param hcan select can handle
 *        @arg hcan_bsp1
 *        @arg hcan_bsp2
 */
void BSP_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{

  if (hcan == &hcan_bsp1)
  {
    /* Peripheral clock disable */
    __CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  }
  else if (hcan == &hcan_bsp2)
  {
    /* Peripheral clock disable */
    __CAN2_CLK_DISABLE();

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_SCE_IRQn);
  }
}

/**
 * @} STM32F4_DISCOVERY_CAN_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_CAN End
 */

 /* --------------------------------- End Of File ------------------------------ */
