/******************************************************************//**
* @file		stm32f4_discovery_timer.c
* @brief	This file contains implementation of STM32F4 timers in custom method.
* @version	V1.0
* @date		May 20, 2015
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/


/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_timer.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_TIMER
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_TIMER_Public_Types
 * @{
 */
TIM_HandleTypeDef htim1_mcpwm;
TIM_HandleTypeDef htim8_mcpwm;

TIM_HandleTypeDef htim2_gen;
TIM_HandleTypeDef htim3_gen;
TIM_HandleTypeDef htim4_gen;
TIM_HandleTypeDef htim5_gen;

TIM_HandleTypeDef htim6_basic;
TIM_HandleTypeDef htim7_basic;

TIM_HandleTypeDef htim10_cap;
TIM_HandleTypeDef htim11_cap;
TIM_HandleTypeDef htim13_cap;
TIM_HandleTypeDef htim14_cap;

TIM_HandleTypeDef htim9_pcap;
TIM_HandleTypeDef htim12_pcap;
/**
 * @} STM32F4_DISCOVERY_TIMER_Public_Types End
 */


/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_TIMER_Private_Functions
 * @{
 */

/**
 * @brief This function is used to configure timer hardware pins
 * @param htim_base   Select Timer handler
 * @param tim_ch      Select Timer channel
 */
static void BSP_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(htim_base->Instance==TIM1)
  {
    /* Peripheral clock enable */
    __TIM1_CLK_ENABLE();

    if(tim_ch != None)
    {
      /**TIM1 GPIO Configuration
      PE9     ------> TIM1_CH1
      PA7     ------> TIM1_CH1N
      PE11    ------> TIM1_CH2
      PB0     ------> TIM1_CH2N
      PE13    ------> TIM1_CH3
      PB1     ------> TIM1_CH3N
      */
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;

      if(tim_ch == TIM_CH1)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_9;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH2)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH3)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
      }
    }

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
  }
  else if(htim_base->Instance==TIM2)
  {
    /* Peripheral clock enable */
    __TIM2_CLK_ENABLE();

    if(tim_ch != None)
    {
      /**TIM2 GPIO Configuration
      PA5     ------> TIM2_CH1
      PB10     ------> TIM2_CH3
      PB11     ------> TIM2_CH4
      PB3     ------> TIM2_CH2
      */
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;

      if(tim_ch == TIM_CH1)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH2)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH3)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH4)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      }
    }

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }
  else if(htim_base->Instance==TIM3)
  {
    /* Peripheral clock enable */
    __TIM3_CLK_ENABLE();

    if(tim_ch != None)
    {
      /**TIM3 GPIO Configuration
      PA6     ------> TIM3_CH1
      PA7     ------> TIM3_CH2
      PB0     ------> TIM3_CH3
      PB1     ------> TIM3_CH4
      */
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;

      if(tim_ch == TIM_CH1)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH2)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH3)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH4)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      }
    }

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  }
  else if(htim_base->Instance==TIM4)
  {
    /* Peripheral clock enable */
    __TIM4_CLK_ENABLE();

    if(tim_ch != None)
    {
      /**TIM4 GPIO Configuration
      PD12     ------> TIM4_CH1
      PD13     ------> TIM4_CH2
      PD14     ------> TIM4_CH3
      PD15     ------> TIM4_CH4
      */
      if(tim_ch == TIM_CH1)
        GPIO_InitStruct.Pin = GPIO_PIN_12;
      else if(tim_ch == TIM_CH2)
        GPIO_InitStruct.Pin = GPIO_PIN_13;
      else if(tim_ch == TIM_CH3)
        GPIO_InitStruct.Pin = GPIO_PIN_14;
      else if(tim_ch == TIM_CH4)
        GPIO_InitStruct.Pin = GPIO_PIN_15;

      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  }
  else if(htim_base->Instance==TIM5)
  {
    /* Peripheral clock enable */
    __TIM5_CLK_ENABLE();

    if(tim_ch != None)
    {
      /**TIM5 GPIO Configuration
      PA0-WKUP     ------> TIM5_CH1
      PA1     ------> TIM5_CH2
      PA2     ------> TIM5_CH3
      PA3     ------> TIM5_CH4
      */
      if(tim_ch == TIM_CH1)
        GPIO_InitStruct.Pin = GPIO_PIN_0;
      else if(tim_ch == TIM_CH2)
        GPIO_InitStruct.Pin = GPIO_PIN_1;
      else if(tim_ch == TIM_CH3)
        GPIO_InitStruct.Pin = GPIO_PIN_2;
      else if(tim_ch == TIM_CH4)
        GPIO_InitStruct.Pin = GPIO_PIN_3;

      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
  }
  else if(htim_base->Instance==TIM6)
  {
    /* Peripheral clock enable */
    __TIM6_CLK_ENABLE();
  }
  else if(htim_base->Instance==TIM7)
  {
    /* Peripheral clock enable */
    __TIM7_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
  }
  else if(htim_base->Instance==TIM8)
  {
    /* Peripheral clock enable */
    __TIM8_CLK_ENABLE();

    if(tim_ch != None)
    {
      /**TIM8 GPIO Configuration
      PC6     ------> TIM8_CH1
      PA5     ------> TIM8_CH1N
      PC7     ------> TIM8_CH2
      PB14    ------> TIM8_CH2N
      PC8     ------> TIM8_CH3
      PB15    ------> TIM8_CH3N
      */
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;

      if(tim_ch == TIM_CH1)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_5;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH2)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_14;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      }
      else if(tim_ch == TIM_CH3)
      {
        GPIO_InitStruct.Pin = GPIO_PIN_8;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_15;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
      }
    }

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
    HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  }
  else if(htim_base->Instance==TIM9)
  {
    /* Peripheral clock enable */
    __TIM9_CLK_ENABLE();

    /**TIM9 GPIO Configuration
    PE5     ------> TIM9_CH1
              OR
    PE6     ------> TIM9_CH2
    */
    if(tim_ch == TIM_CH1)
      GPIO_InitStruct.Pin = GPIO_PIN_5;
    else if(tim_ch == TIM_CH2)
      GPIO_InitStruct.Pin = GPIO_PIN_6;

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
  }
  else if(htim_base->Instance==TIM10)
  {
    /* Peripheral clock enable */
    __TIM10_CLK_ENABLE();

    /**TIM10 GPIO Configuration
    PB8     ------> TIM10_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  }
  else if(htim_base->Instance==TIM11)
  {
    /* Peripheral clock enable */
    __TIM11_CLK_ENABLE();

    /**TIM11 GPIO Configuration
    PB9     ------> TIM11_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM11;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  }
  else if(htim_base->Instance==TIM12)
  {
    /* Peripheral clock enable */
    __TIM12_CLK_ENABLE();

    /**TIM12 GPIO Configuration
    PB14     ------> TIM12_CH1
               OR
    PB15     ------> TIM12_CH2
    */
    if(tim_ch == TIM_CH1)
      GPIO_InitStruct.Pin = GPIO_PIN_14;
    else if(tim_ch == TIM_CH2)
      GPIO_InitStruct.Pin = GPIO_PIN_15;

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
  }
  else if(htim_base->Instance==TIM13)
  {
    /* Peripheral clock enable */
    __TIM13_CLK_ENABLE();

    /**TIM13 GPIO Configuration
    PA6     ------> TIM13_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  }
  else if(htim_base->Instance==TIM14)
  {
    /* Peripheral clock enable */
    __TIM14_CLK_ENABLE();

    /**TIM14 GPIO Configuration
    PA7     ------> TIM14_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM14;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  }
}


/**
 * @} STM32F4_DISCOVERY_TIMER_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_TIMER_Public_Functions
 * @{
 */

/**
 * @brief Basic Timer configurations for time keeping purposes
 * @param htim_base   Timer handler
 *        @arg  htim6_basic   used with Polling
 *        @arg  htim7_basic   used with Interrupt
 * @param period_us   Provide time in microseconds for timer (range: 0.5us to 1 sec)
 * @return None
 */
void BSP_TIM_Basic_Config(TIM_HandleTypeDef* htim_base, float period_us)
{
  TIM_MasterConfigTypeDef sMasterConfig;

  if(htim_base == &htim6_basic)
    htim_base->Instance = TIM6;
  else if(htim_base == &htim7_basic)
    htim_base->Instance = TIM7;

  htim_base->Init.CounterMode = TIM_COUNTERMODE_UP;

  if(1000 <= period_us <= 1000000) // 1Hz to 1KHz
  {
    htim_base->Init.Prescaler = SystemCoreClock/132000;
    htim_base->Init.Period = (float)(65000/(1000000/period_us));
  }
  else if(5 <= period_us < 1000) //1KHz to 200KHz
  {
    htim_base->Init.Prescaler = SystemCoreClock/168000000;
    htim_base->Init.Period = (float)(41900000/(1000000/period_us));
  }
  else if(0.5 <= period_us < 5) //200KHz to 2MHz
  {
    htim_base->Init.Prescaler = 0;
    htim_base->Init.Period = (float)(83800000/(1000000/period_us));
  }

  BSP_TIM_Base_MspInit(htim_base, None);
  HAL_TIM_Base_Init(htim_base);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(htim_base, &sMasterConfig);

  if(htim_base->Instance == TIM6)
    HAL_TIM_Base_Start(htim_base);
  else if(htim_base->Instance == TIM7)
    HAL_TIM_Base_Start_IT(htim_base);
}


/**
 * @brief General Purpose Timers for frequency generation and time keeping
 * @param htim_base   Timer Handlers
 *        @arg  htim2_gen  All uses interrupt
 *        @arg  htim3_gen
 *        @arg  htim4_gen
 *        @arg  htim5_gen
 * @param tim_ch      Select Hardware channel or use without channel
 *        @arg  None      No channel        TIM2    TIM3    TIM4    TIM5
 *                                          ----------------------------
 *        @arg  TIM_CH1   Timer Channel 1   PA5     PA6     PD12    PA0
 *        @arg  TIM_CH2   Timer Channel 2   PB3     PA7     PD13    PA1
 *        @arg  TIM_CH3   Timer Channel 3   PB10    PB0     PD14    PA2
 *        @arg  TIM_CH4   Timer Channel 4   PB11    PB1     PD15    PA3
 * @param freq_hz     Frequency in Hz (range: 1Hz to 2MHz)
 * @param hw_toggle   Hardware Pin behavior on Match
 *        @arg  TIM_OC_MODE_NOTHING   User Pin
 *        @arg  TIM_OC_MODE_TOGGLE    Toggle on Match
 *        @arg  TIM_OC_MODE_LOW       Low on Match
 *        @arg  TIM_OC_MODE_HIGH      High on Match
 * @return  None
 */
void BSP_TIM_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, uint32_t freq_hz, TIM_OC_MODE_OPT hw_toggle)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  if(htim_base == &htim2_gen)
    htim_base->Instance = TIM2;
  else if(htim_base == &htim3_gen)
    htim_base->Instance = TIM3;
  else if(htim_base == &htim4_gen)
    htim_base->Instance = TIM4;
  else if(htim_base == &htim5_gen)
    htim_base->Instance = TIM5;

  if(freq_hz <= 1000) // 1HZ to 1KHZ
  {
    htim_base->Init.Prescaler = SystemCoreClock/132000;
    htim_base->Init.Period = 32500/freq_hz;
  }
  else if (freq_hz <=200000) //1KHZ to 200 KHZ
  {
    htim_base->Init.Prescaler = SystemCoreClock/168000000;
    htim_base->Init.Period = 20950000/freq_hz;
  }
  else if (freq_hz <= 2000000) //200KHZ to 2MHz
  {
    htim_base->Init.Prescaler = 0;
    htim_base->Init.Period = 41900000/freq_hz;
  }

  htim_base->Init.CounterMode = TIM_COUNTERMODE_UP;
  htim_base->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  BSP_TIM_Base_MspInit(htim_base, tim_ch);
  HAL_TIM_Base_Init(htim_base);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(htim_base, &sClockSourceConfig);

  HAL_TIM_OC_Init(htim_base);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(htim_base, &sMasterConfig);

  if(hw_toggle == TIM_OC_MODE_NOTHING)
    sConfigOC.OCMode = TIM_OCMODE_TIMING;
  else if(hw_toggle == TIM_OC_MODE_LOW)
    sConfigOC.OCMode = TIM_OCMODE_INACTIVE;
  else if(hw_toggle == TIM_OC_MODE_HIGH)
    sConfigOC.OCMode = TIM_OCMODE_ACTIVE;
  if(hw_toggle == TIM_OC_MODE_TOGGLE)
    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;

  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  if(tim_ch == TIM_CH1)
  {
    HAL_TIM_OC_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_OC_Start_IT(htim_base, TIM_CHANNEL_1);
  }
  else if(tim_ch == TIM_CH2)
  {
    HAL_TIM_OC_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_OC_Start_IT(htim_base, TIM_CHANNEL_2);
  }
  else if(tim_ch == TIM_CH3)
  {
    HAL_TIM_OC_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_OC_Start_IT(htim_base, TIM_CHANNEL_3);
  }
  else if(tim_ch == TIM_CH4)
  {
    HAL_TIM_OC_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_4);
    HAL_TIM_OC_Start_IT(htim_base, TIM_CHANNEL_4);
  }
}


/**
 * @brief General Purpose PWM Timer Configurations
 * @param htim_base     Timer Handlers
 *        @arg  htim2_gen  All uses interrupt
 *        @arg  htim3_gen
 *        @arg  htim4_gen
 *        @arg  htim5_gen
 * @param tim_ch        Select Hardware channel or use without channel
 *        @arg  None      No channel        TIM2    TIM3    TIM4    TIM5
 *                                          ----------------------------
 *        @arg  TIM_CH1   Timer Channel 1   PA5     PA6     PD12    PA0
 *        @arg  TIM_CH2   Timer Channel 2   PB3     PA7     PD13    PA1
 *        @arg  TIM_CH3   Timer Channel 3   PB10    PB0     PD14    PA2
 *        @arg  TIM_CH4   Timer Channel 4   PB11    PB1     PD15    PA3
 * @param freq_hz       Frequency in Hz (range: 1Hz to 2MHz)
 * @param pulsewidth_us Provide Pulse width value in microseconds for respective frequency
 * @param pwm_mode      Select PWM mode
 *        @arg  PWM_EDGE_ALIGN_MODE     Default Mode to Select
 *        @arg  PWM_CENTER_ALIGN_MODE1  <Provide Desc>
 *        @arg  PWM_CENTER_ALIGN_MODE2  <Provide Desc>
 *        @arg  PWM_CENTER_ALIGN_MODE3  <Provide Desc>
 * @return  None
 */
void BSP_PWM_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, uint32_t freq_hz, float pulsewidth_us, PWM_ALIGN_MODE pwm_mode)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();

  if(htim_base == &htim2_gen)
    htim_base->Instance = TIM2;
  else if(htim_base == &htim3_gen)
    htim_base->Instance = TIM3;
  else if(htim_base == &htim4_gen)
    htim_base->Instance = TIM4;
  else if(htim_base == &htim5_gen)
    htim_base->Instance = TIM5;

  if(freq_hz <= 1000) // 1HZ to 1KHZ
  {
    htim_base->Init.Prescaler = SystemCoreClock/132000;

    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      htim_base->Init.Period = 65000/freq_hz;
    else
      htim_base->Init.Period = 32500/freq_hz;
  }
  else if (freq_hz <=200000) //1KHZ to 200000 KHZ
  {
    htim_base->Init.Prescaler = SystemCoreClock/168000000;

    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      htim_base->Init.Period = 41900000/freq_hz;
    else
      htim_base->Init.Period = 20950000/freq_hz;
  }
  else if (freq_hz <= 2000000) //200000KHZ to 2000000KHZ
  {
    htim_base->Init.Prescaler = 0;

    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      htim_base->Init.Period = 83800000/freq_hz;
    else
      htim_base->Init.Period = 41900000/freq_hz;
  }

  if(pwm_mode == PWM_EDGE_ALIGN_MODE)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_UP;
  else if(pwm_mode == PWM_CENTER_ALIGN_MODE1)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  else if(pwm_mode == PWM_CENTER_ALIGN_MODE2)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2;
  else if(pwm_mode == PWM_CENTER_ALIGN_MODE3)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED3;

  htim_base->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  BSP_TIM_Base_MspInit(htim_base, tim_ch);
  HAL_TIM_Base_Init(htim_base);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(htim_base, &sClockSourceConfig);

  HAL_TIM_PWM_Init(htim_base);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(htim_base, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  if(freq_hz <= 1000) // 1HZ to 1KHZ
  {
    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      sConfigOC.Pulse = (float)(65000/(1000000/pulsewidth_us));
    else
      sConfigOC.Pulse = (float)(32500/(1000000/pulsewidth_us));
  }
  else if (freq_hz <=200000) //1KHZ to 200000 KHZ
  {
    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      sConfigOC.Pulse = (float)(41900000/(1000000/pulsewidth_us));
    else
      sConfigOC.Pulse = (float)(20950000/(1000000/pulsewidth_us));
  }
  else if (freq_hz <= 2000000) //200000KHZ to 2000000KHZ
  {
    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      sConfigOC.Pulse = (float)(83800000/(1000000/pulsewidth_us));
    else
      sConfigOC.Pulse = (float)(41900000/(1000000/pulsewidth_us));
  }

  if(tim_ch == TIM_CH1)
  {
    HAL_TIM_PWM_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(htim_base, TIM_CHANNEL_1);
  }
  else if(tim_ch == TIM_CH2)
  {
    HAL_TIM_PWM_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start_IT(htim_base, TIM_CHANNEL_2);
  }
  else if(tim_ch == TIM_CH3)
  {
    HAL_TIM_PWM_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start_IT(htim_base, TIM_CHANNEL_3);
  }
  else if(tim_ch == TIM_CH4)
  {
    HAL_TIM_PWM_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start_IT(htim_base, TIM_CHANNEL_4);
  }
}


/**
 * @brief This is Motor Control PWM Timer Configuration function
 * @param htim_base     Timer Handler
 *        @arg  htim1_mcpwm
 *        @arg  htim2_mcpwm
 * @param tim_ch        Select Hardware channel or use without channel
 *        @arg  None      No channel            TIM1     |    TIM8
 *                                          -------------|-------------
 *        @arg  TIM_CH1   Timer Channel 1   PE9    PA7(N)| PC6   PA5 (N)
 *        @arg  TIM_CH2   Timer Channel 2   PE11   PB0(N)| PC7   PB14(N)
 *        @arg  TIM_CH3   Timer Channel 3   PE13   PB1(N)| PC8   PB15(N)
 * @param freq_hz       Frequency in Hz (range: 1Hz to 2MHz)
 * @param pulsewidth_us Provide Pulse width value in microseconds for respective frequency
 * @param deadtime      Provide deadtime (Range: 0 to 255)
 * @param pwm_mode      Select PWM mode
 *        @arg  PWM_EDGE_ALIGN_MODE     Default Mode to Select
 *        @arg  PWM_CENTER_ALIGN_MODE1  <Provide Desc>
 *        @arg  PWM_CENTER_ALIGN_MODE2  <Provide Desc>
 *        @arg  PWM_CENTER_ALIGN_MODE3  <Provide Desc>
 * @return  None
 */
void BSP_MCPWM_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, uint32_t freq_hz, float pulsewidth_us, uint8_t deadtime, PWM_ALIGN_MODE pwm_mode)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();

  if(htim_base == &htim1_mcpwm)
    htim_base->Instance = TIM1;
  else if(htim_base == &htim8_mcpwm)
    htim_base->Instance = TIM8;

  if(freq_hz <= 1000) // 1HZ to 1KHZ
  {
    htim_base->Init.Prescaler = SystemCoreClock/132000;

    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      htim_base->Init.Period = 130000/freq_hz;
    else
      htim_base->Init.Period = 65000/freq_hz;
  }
  else if (freq_hz <=200000) //1KHZ to 200000 KHZ
  {
    htim_base->Init.Prescaler = SystemCoreClock/168000000;

    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      htim_base->Init.Period = 84000000/freq_hz;
    else
      htim_base->Init.Period = 42000000/freq_hz;
  }
  else if (freq_hz <= 2000000) //200000KHZ to 2000000KHZ
  {
    htim_base->Init.Prescaler = 0;

    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      htim_base->Init.Period = 167600000/freq_hz;
    else
      htim_base->Init.Period = 83800000/freq_hz;
  }

  if(pwm_mode == PWM_EDGE_ALIGN_MODE)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_UP;
  else if(pwm_mode == PWM_CENTER_ALIGN_MODE1)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  else if(pwm_mode == PWM_CENTER_ALIGN_MODE2)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2;
  else if(pwm_mode == PWM_CENTER_ALIGN_MODE3)
    htim_base->Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED3;

  htim_base->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim_base->Init.RepetitionCounter = 0;

  BSP_TIM_Base_MspInit(htim_base, tim_ch);
  HAL_TIM_Base_Init(htim_base);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(htim_base, &sClockSourceConfig);

  HAL_TIM_PWM_Init(htim_base);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(htim_base, &sMasterConfig);

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = deadtime;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(htim_base, &sBreakDeadTimeConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  if(freq_hz <= 1000) // 1HZ to 1KHZ
  {
    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      sConfigOC.Pulse = (float)(130000/(1000000/pulsewidth_us));
    else
      sConfigOC.Pulse = (float)(65000/(1000000/pulsewidth_us));
  }
  else if (freq_hz <=200000) //1KHZ to 200000 KHZ
  {
    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      sConfigOC.Pulse = (float)(84000000/(1000000/pulsewidth_us));
    else
      sConfigOC.Pulse = (float)(42000000/(1000000/pulsewidth_us));
  }
  else if (freq_hz <= 2000000) //200000KHZ to 2000000KHZ
  {
    if(pwm_mode == PWM_EDGE_ALIGN_MODE)
      sConfigOC.Pulse = (float)(167600000/(1000000/pulsewidth_us));
    else
      sConfigOC.Pulse = (float)(83800000/(1000000/pulsewidth_us));
  }

  if(tim_ch == TIM_CH1)
  {
    HAL_TIM_PWM_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(htim_base, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start_IT(htim_base,TIM_CHANNEL_1);
  }
  else if(tim_ch == TIM_CH2)
  {
    HAL_TIM_PWM_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start_IT(htim_base, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start_IT(htim_base,TIM_CHANNEL_2);
  }
  else if(tim_ch == TIM_CH3)
  {
    HAL_TIM_PWM_ConfigChannel(htim_base, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start_IT(htim_base, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start_IT(htim_base,TIM_CHANNEL_3);
  }
}


/**
 * @brief This is Timer Capture configuration function
 *        Capture Channel Pins
 *        TIM10   PB8
 *        TIM11   PB9
 *        TIM13   PA6
 *        TIM14   PA7
 * @param htim_base   Timer Handler
 *        @arg  htim10_cap
 *        @arg  htim11_cap
 *        @arg  htim13_cap
 *        @arg  htim14_cap
 * @param capFreq_hz  <provide description>
 * @param capEdge     Capture Polarity
 *        @arg  TIM_CAP_RISING_EDGE
 *        @arg  TIM_CAP_FALLING_EDGE
 *        @arg  TIM_CAP_BOTH_EDGE
 * @return  None
 */
void BSP_TIM_Capture_Config(TIM_HandleTypeDef* htim_base, uint32_t capFreq_hz, TIM_CAP_POLARITY_Type capEdge)
{
  TIM_IC_InitTypeDef sConfigIC;

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  if(htim_base == &htim10_cap)
    htim_base->Instance = TIM10;
  else if(htim_base == &htim11_cap)
    htim_base->Instance = TIM11;
  else if(htim_base == &htim13_cap)
    htim_base->Instance = TIM13;
  else if(htim_base == &htim14_cap)
    htim_base->Instance = TIM14;

  htim_base->Init.Prescaler = (SystemCoreClock)/168000000;
  htim_base->Init.Period = 168000000/capFreq_hz;

  htim_base->Init.CounterMode = TIM_COUNTERMODE_UP;
  htim_base->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  BSP_TIM_Base_MspInit(htim_base, None);
  HAL_TIM_Base_Init(htim_base);

  HAL_TIM_IC_Init(htim_base);

  if(capEdge == TIM_CAP_RISING_EDGE)
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  else if(capEdge == TIM_CAP_FALLING_EDGE)
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  else if(capEdge == TIM_CAP_BOTH_EDGE)
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;

  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(htim_base, &sConfigIC, TIM_CHANNEL_1);

  HAL_TIM_IC_Start_IT(htim_base, TIM_CHANNEL_1);
}


/**
 * @brief This is PWM Capture Configuration function
 * @param htim_base   Timer Handler
 *        @arg  htim9_pcap
 *        @arg  htim12_pcap
 * @param tim_ch        Select Hardware channel or use without channel
 *        @arg  None      No channel        TIM9   TIM12
 *                                          ------------
 *        @arg  TIM_CH1   Timer Channel 1   PE5    PB14
 *        @arg  TIM_CH2   Timer Channel 2   PE6    PB15
 * @param capFreq_hz    <provide description>
 * @return  None
 */
void BSP_PWM_Capture_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, uint32_t capFreq_hz)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_IC_InitTypeDef sConfigIC;

  __GPIOE_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  if(htim_base == &htim9_pcap)
    htim_base->Instance = TIM9;
  else if(htim_base == &htim12_pcap)
    htim_base->Instance = TIM12;

  htim_base->Init.Prescaler = 0;
  htim_base->Init.Period = 0;

  htim_base->Init.CounterMode = TIM_COUNTERMODE_UP;
  htim_base->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  BSP_TIM_Base_MspInit(htim_base, tim_ch);
  HAL_TIM_Base_Init(htim_base);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(htim_base, &sClockSourceConfig);

  HAL_TIM_IC_Init(htim_base);

  if(tim_ch == TIM_CH1)
  {
    sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  }
  else if(tim_ch == TIM_CH2)
  {
    sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  }

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  sSlaveConfig.TriggerFilter = 0;
  HAL_TIM_SlaveConfigSynchronization(htim_base, &sSlaveConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(htim_base, &sConfigIC, TIM_CHANNEL_1);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  HAL_TIM_IC_ConfigChannel(htim_base, &sConfigIC, TIM_CHANNEL_2);

}

/**
 * @brief De-Initialize Timers
 * @param htim_base   Select Timer Handler
 * @return  None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
    /* Peripheral clock disable */
    __TIM1_CLK_DISABLE();

    /**TIM1 GPIO Configuration
    PA7     ------> TIM1_CH1N
    PB0     ------> TIM1_CH2N
    PB1     ------> TIM1_CH3N
    PE9     ------> TIM1_CH1
    PE11     ------> TIM1_CH2
    PE13     ------> TIM1_CH3
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
  }
  else if(htim_base->Instance==TIM2)
  {
    /* Peripheral clock disable */
    __TIM2_CLK_DISABLE();

    /**TIM2 GPIO Configuration
    PA5     ------> TIM2_CH1
    PB10     ------> TIM2_CH3
    PB11     ------> TIM2_CH4
    PB3     ------> TIM2_CH2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_3);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  }
  else if(htim_base->Instance==TIM3)
  {
    /* Peripheral clock disable */
    __TIM3_CLK_DISABLE();

    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PA7     ------> TIM3_CH2
    PB0     ------> TIM3_CH3
    PB1     ------> TIM3_CH4
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6|GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  }
  else if(htim_base->Instance==TIM4)
  {
    /* Peripheral clock disable */
    __TIM4_CLK_DISABLE();

    /**TIM4 GPIO Configuration
    PD12     ------> TIM4_CH1
    PD13     ------> TIM4_CH2
    PD14     ------> TIM4_CH3
    PD15     ------> TIM4_CH4
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  }
  else if(htim_base->Instance==TIM5)
  {
    /* Peripheral clock disable */
    __TIM5_CLK_DISABLE();

    /**TIM5 GPIO Configuration
    PA0-WKUP     ------> TIM5_CH1
    PA1     ------> TIM5_CH2
    PA2     ------> TIM5_CH3
    PA3     ------> TIM5_CH4
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM5_IRQn);
  }
  else if(htim_base->Instance==TIM6)
  {
    /* Peripheral clock disable */
    __TIM6_CLK_DISABLE();
  }
  else if(htim_base->Instance==TIM7)
  {
    /* Peripheral clock disable */
    __TIM7_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
  }
  else if(htim_base->Instance==TIM8)
  {
    /* Peripheral clock disable */
    __TIM8_CLK_DISABLE();

    /**TIM8 GPIO Configuration
    PA5     ------> TIM8_CH1N
    PB14     ------> TIM8_CH2N
    PB15     ------> TIM8_CH3N
    PC6     ------> TIM8_CH1
    PC7     ------> TIM8_CH2
    PC8     ------> TIM8_CH3
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14|GPIO_PIN_15);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
    HAL_NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  }
  else if(htim_base->Instance==TIM9)
  {
    /* Peripheral clock disable */
    __TIM9_CLK_DISABLE();

    /**TIM9 GPIO Configuration
    PE5     ------> TIM9_CH1
    PE6     ------> TIM9_CH2
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_5|GPIO_PIN_6);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
  }
  else if(htim_base->Instance==TIM10)
  {
    /* Peripheral clock disable */
    __TIM10_CLK_DISABLE();

    /**TIM10 GPIO Configuration
    PB8     ------> TIM10_CH1
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
  }
  else if(htim_base->Instance==TIM11)
  {
    /* Peripheral clock disable */
    __TIM11_CLK_DISABLE();

    /**TIM11 GPIO Configuration
    PB9     ------> TIM11_CH1
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  }
  else if(htim_base->Instance==TIM12)
  {
    /* Peripheral clock disable */
    __TIM12_CLK_DISABLE();

    /**TIM12 GPIO Configuration
    PB14     ------> TIM12_CH1
    PB15     ------> TIM12_CH2
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14|GPIO_PIN_15);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);
  }
  else if(htim_base->Instance==TIM13)
  {
    /* Peripheral clock disable */
    __TIM13_CLK_DISABLE();

    /**TIM13 GPIO Configuration
    PA6     ------> TIM13_CH1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
  }
  else if(htim_base->Instance==TIM14)
  {
    /* Peripheral clock disable */
    __TIM14_CLK_DISABLE();

    /**TIM14 GPIO Configuration
    PA7     ------> TIM14_CH1
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  }
}


/**
 * @} STM32F4_DISCOVERY_TIMER_Public_Functions End
 */


/**
 * @} STM32F4_DISCOVERY_TIMER End
 */


 /* --------------------------------- End Of File ------------------------------ */
