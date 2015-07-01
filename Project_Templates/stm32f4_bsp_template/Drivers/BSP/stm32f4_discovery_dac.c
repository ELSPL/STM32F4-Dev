/******************************************************************//**
* @file   stm32f4_discovery_dac.c
* @brief  This file contains implementation of STM32F4 DAC in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_dac.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_DAC
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_DAC_Public_Types
 * @{
 */

DAC_HandleTypeDef hdac_bsp;
uint8_t SqWaveFlag = SET, SinWaveFlag = SET ;
float Vout,dac[720];

/**
 * @} STM32F4_DISCOVERY_DAC_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_DAC_Private_Functions
 * @{
 */

/**
 * @brief This function enable the DAC by enabling the peripheral clock
 * @param hdac          DAC handle select
 *        @arg hdac_bsp DAC handle
 * @param DAC_Channel   DAC channel select
 *        @arg DAC_CHANNEL_1
 *        @arg DAC_CHANNEL_2
 */
static void BSP_DAC_MspInit(DAC_HandleTypeDef* hdac,uint8_t DAC_Channel)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hdac->Instance==DAC)
  {
    /* Peripheral clock enable */
    __DAC_CLK_ENABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    if (DAC_Channel == DAC_CHANNEL_1)
    {
      GPIO_InitStruct.Pin = GPIO_PIN_4;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    else if (DAC_Channel == DAC_CHANNEL_2)
    {
      GPIO_InitStruct.Pin = GPIO_PIN_5;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
  }
}

/**
 * @} STM32F4_DISCOVERY_DAC_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_DAC_Public_Functions
 * @{
 */

/**
 * @brief DAC Configuration function
 * @param DAC_Channel   DAC channel select
 *        @arg DAC_CHANNEL_1
 *        @arg DAC_CHANNEL_2
 * @param DAC_Trigger   Specifies the external/software trigger for the selected DAC channel
 *        @arg DAC_TRIGGER_NONE
 *        @arg DAC_TRIGGER_SOFTWARE
 *        @arg DAC_TRIGGER_T2_TRGO
 *        @arg DAC_TRIGGER_T4_TRGO
 *        @arg DAC_TRIGGER_T5_TRGO
 *        @arg DAC_TRIGGER_T6_TRGO
 *        @arg DAC_TRIGGER_T7_TRGO
 *        @arg DAC_TRIGGER_T8_TRGO
 *        @arg DAC_TRIGGER_EXT_IT9
 */
void BSP_DAC_Init(uint8_t DAC_Channel, uint8_t DAC_Trigger)
{
  DAC_ChannelConfTypeDef sConfig;

  /**DAC Initialization
   */
  hdac_bsp.Instance = DAC;
  BSP_DAC_MspInit(&hdac_bsp,DAC_Channel);
  HAL_DAC_Init(&hdac_bsp);

  if (DAC_Channel == DAC_CHANNEL_1)
  {
     /**DAC channel OUT1 config
     */
    sConfig.DAC_Trigger = DAC_Trigger;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    HAL_DAC_ConfigChannel(&hdac_bsp, &sConfig, DAC_Channel);
  }
  else if (DAC_Channel == DAC_CHANNEL_2)
  {
      /**DAC channel OUT2 config
      */
    sConfig.DAC_Trigger = DAC_Trigger;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    HAL_DAC_ConfigChannel(&hdac_bsp, &sConfig, DAC_CHANNEL_2);
  }
  /* Start DAC */
  HAL_DAC_Start(&hdac_bsp,DAC_Channel);

}

/**
 * @brief Square Wave generation function
 * @param DAC_Channel   DAC channel select
 *        @arg DAC_CHANNEL_1
 *        @arg DAC_CHANNEL_2
 */
void BSP_DAC_SquareWave(uint8_t DAC_Channel)
{
  if (SqWaveFlag == SET)
  {
    HAL_DAC_SetValue(&hdac_bsp,DAC_Channel,DAC_ALIGN_8B_R,0xFF);
    HAL_Delay(10);
    SqWaveFlag = RESET;
  }
  else if (SqWaveFlag == RESET)
  {
    HAL_DAC_SetValue(&hdac_bsp,DAC_Channel,DAC_ALIGN_8B_R,0);
    HAL_Delay(10);
    SqWaveFlag = SET;
  }
}

/**
 * @brief Triangular wave generation function
 * @param DAC_Channel   DAC channel select
 *        @arg DAC_CHANNEL_1
 *        @arg DAC_CHANNEL_2
 */
void BSP_DAC_SawtoothWave(uint8_t DAC_Channel)
{
  uint16_t i=0;
  for (i=0;i<=4095;i++)
  {
    HAL_DAC_SetValue(&hdac_bsp,DAC_Channel,DAC_ALIGN_12B_R,i);
    if(i== 4095)
    {
      i = 0;
    }
  }
}

/**
 * @brief Triangular wave generation function
 * @param DAC_Channel   DAC channel select
 *        @arg DAC_CHANNEL_1
 *        @arg DAC_CHANNEL_2
 */
void BSP_DAC_TriangularWave(uint8_t DAC_Channel)
{
  HAL_DACEx_TriangleWaveGenerate(&hdac_bsp, DAC_Channel, DAC_TRIANGLEAMPLITUDE_4095);
}

/**
 * @brief Sine wave generation function
 * @param DAC_Channel   DAC channel select
 *        @arg DAC_CHANNEL_1
 *        @arg DAC_CHANNEL_2
 */
void BSP_DAC_SineWave(uint8_t DAC_Channel)
{
  float j;
  float deg;
  uint16_t i = 0;
  if (SinWaveFlag == SET)
  {
    deg = 0.5;   // sample at this resolution
    for(j=0;j<360;j=j+deg)
    {
      Vout = (15+(15*sin((PI/180)*j)));       // Amplitude(1+sin(angle)), amplitude max 3v
      dac[i] = (Vout*1365)/10;                // max DAC value = 4095
      i++;
    }
    SinWaveFlag = RESET;
  }
  for(i=0;i<720;i++)
  {
    HAL_DAC_SetValue(&hdac_bsp,DAC_Channel,DAC_ALIGN_12B_R,(uint32_t)dac[i]);
  }
}

/**
 * @brief Disable the DAC feature by disabling DAC clock
 * @param hdac          DAC handle select
 *        @arg hdac_bsp DAC handle
 * @param DAC_Channel   DAC channel select
 *        @arg DAC_CHANNEL_1
 *        @arg DAC_CHANNEL_2
 */
void BSP_DAC_MspDeInit(DAC_HandleTypeDef* hdac,uint8_t DAC_Channel)
{
  if(hdac->Instance==DAC)
  {
    /* Peripheral clock disable */
    __DAC_CLK_DISABLE();

    /**DAC GPIO Configuration
    PA4     ------> DAC_OUT1
    PA5     ------> DAC_OUT2
    */
    if (DAC_Channel == DAC_CHANNEL_1)
    {
      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
    }
    else if (DAC_Channel == DAC_CHANNEL_2)
    {
      HAL_GPIO_DeInit(GPIOA,GPIO_PIN_5);
    }
  }
}

/**
 * @} STM32F4_DISCOVERY_DAC_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_DAC End
 */

 /* --------------------------------- End Of File ------------------------------ */
