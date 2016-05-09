/******************************************************************//**
* @file   stm32f4_discovery_adc.c
* @brief  This file contains implementation of STM32F4 adc in custom method.
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_adc.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_ADC
 * @{
 */

/* Public Types --------------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_ADC_Public_Types
 * @{
 */

ADC_HandleTypeDef hadc_bsp1;
ADC_HandleTypeDef hadc_bsp2;
ADC_HandleTypeDef hadc_bsp3;

DMA_HandleTypeDef hdma_adcbsp1;
DMA_HandleTypeDef hdma_adcbsp2;
DMA_HandleTypeDef hdma_adcbsp3;

#define NS       10         // Number of samples to get from ADC
#define SR       4          // Samples removed after sorting, 4=(2 highest & 2 lowest)

volatile uint16_t ADC_Raw[NS] = {0};       // Updated 2000 times per second by DMA
uint16_t Sample_ADC_Raw[NS]   = {0};       // Non-volatile copy of ADC_Raw[NS]
uint32_t ADC_Average          = 0;         // Average of the samples
float Temp                    = 0;         // Temporary register
float Temp_Celsius            = 0;         // Temperature in Celsius
float Calibration_Value       = 11.0;      // For measuring absolute temperature



uint32_t ADC_Channel[19] = { ADC_CHANNEL_0, ADC_CHANNEL_1,ADC_CHANNEL_2,
                             ADC_CHANNEL_3,ADC_CHANNEL_4,ADC_CHANNEL_5,
                             ADC_CHANNEL_6,ADC_CHANNEL_7,ADC_CHANNEL_8,
                             ADC_CHANNEL_9,ADC_CHANNEL_10,ADC_CHANNEL_11,
                             ADC_CHANNEL_12,ADC_CHANNEL_13,ADC_CHANNEL_14,
                             ADC_CHANNEL_15,ADC_CHANNEL_16,ADC_CHANNEL_17,
                             ADC_CHANNEL_18 };

uint16_t GPIO_Pin[16] = { GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,
                          GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7,GPIO_PIN_0,GPIO_PIN_1,
                          GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,
                          GPIO_PIN_5 };
/**
 * @} STM32F4_DISCOVERY_ADC_Public_Types End
 */

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_ADC_Private_Functions
 * @{
 */

/**
 * @brief This function enable the ADC and its port pin by enabling the ADC clock and GPIO clock
 * @param hadc      ADC handle
 *        @arg hadc_bsp1
 *        @arg hadc_bsp2
 *        @arg hadc_bsp3
 * @param ADC_pin   Select ADC port pin
 *        @arg ADC_IN0 to ADC_IN15 for ADC1 and ADC2
 * NOTE        For ADC3 ADC_IN0 to ADC_IN3 and ADC_IN10 to ADC_IN13
 *
 *   ADC2 GPIO Configuration
 *
 *   PA0-WKUP     ------> ADC2_IN0
 *   PA1     ------> ADC2_IN1
 *   PA2     ------> ADC2_IN2
 *   PA3     ------> ADC2_IN3
 *   PA4     ------> ADC2_IN4
 *   PA5     ------> ADC2_IN5
 *   PA6     ------> ADC2_IN6
 *   PA7     ------> ADC2_IN7
 *   PB0     ------> ADC2_IN8
 *   PB1     ------> ADC2_IN9
 *   PC0     ------> ADC2_IN10
 *   PC1     ------> ADC2_IN11
 *   PC2     ------> ADC2_IN12
 *   PC3     ------> ADC2_IN13
 *   PC4     ------> ADC2_IN14
 *   PC5     ------> ADC2_IN15
 */
static void BSP_ADC_MspInit(ADC_HandleTypeDef* hadc,ADC_PortPin_Typedef ADC_pin)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
    /* Peripheral clock enable */
    __ADC1_CLK_ENABLE();

    /* Peripheral DMA init*/
    hdma_adcbsp1.Instance = DMA2_Stream4;
    hdma_adcbsp1.Init.Channel = DMA_CHANNEL_0;
    hdma_adcbsp1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcbsp1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcbsp1.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adcbsp1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adcbsp1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adcbsp1.Init.Mode = DMA_CIRCULAR;
    hdma_adcbsp1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adcbsp1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adcbsp1);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcbsp1);
  }
  else if(hadc->Instance==ADC2)
  {
    /* Peripheral clock enable */
    __ADC2_CLK_ENABLE();

    /* Peripheral DMA init*/
    hdma_adcbsp2.Instance = DMA2_Stream2;
    hdma_adcbsp2.Init.Channel = DMA_CHANNEL_1;
    hdma_adcbsp2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcbsp2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcbsp2.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adcbsp2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adcbsp2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adcbsp2.Init.Mode = DMA_CIRCULAR;
    hdma_adcbsp2.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adcbsp2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adcbsp2);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcbsp2);
  }
  else if(hadc->Instance==ADC3)
  {
    /* Peripheral clock enable */
    __ADC3_CLK_ENABLE();

    /* Peripheral DMA init*/
    hdma_adcbsp3.Instance = DMA2_Stream0;
    hdma_adcbsp3.Init.Channel = DMA_CHANNEL_2;
    hdma_adcbsp3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adcbsp3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adcbsp3.Init.MemInc = DMA_MINC_DISABLE;
    hdma_adcbsp3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adcbsp3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adcbsp3.Init.Mode = DMA_CIRCULAR;
    hdma_adcbsp3.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_adcbsp3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adcbsp3);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcbsp3);
  }


  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  if(ADC_pin == ADC_IN0 || ADC_pin == ADC_IN1 || ADC_pin == ADC_IN2 || ADC_pin == ADC_IN3 || ADC_pin == ADC_IN4
          || ADC_pin == ADC_IN5 || ADC_pin == ADC_IN6 || ADC_pin == ADC_IN7)
  {
    /* GPIO Ports Clock Enable */
    __GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_Pin[ADC_pin];
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if (ADC_pin == ADC_IN8 || ADC_pin == ADC_IN9)
  {
    /* GPIO Ports Clock Enable */
    __GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_Pin[ADC_pin];
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
  else if (ADC_pin == ADC_IN10 || ADC_pin == ADC_IN11 || ADC_pin == ADC_IN12 || ADC_pin == ADC_IN13
           || ADC_pin == ADC_IN14 || ADC_pin == ADC_IN15)
  {
    /* GPIO Ports Clock Enable */
    __GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_Pin[ADC_pin];
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }

  /* Peripheral interrupt init*/
   HAL_NVIC_SetPriority(ADC_IRQn, 4, 0);
   HAL_NVIC_EnableIRQ(ADC_IRQn);
}

/**
 * @brief DMA initialization for ADC
 */
static void BSP_ADC_DMA_Init(ADC_HandleTypeDef* hadc)
{
  /* DMA controller clock enable */
  __DMA2_CLK_ENABLE();

  if(hadc == &hadc_bsp1)
  {
    /* DMA interrupt init */
    HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
  }
  else if(hadc == &hadc_bsp2)
  {
    /* DMA interrupt init */
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  }
  else if(hadc == &hadc_bsp3)
  {
    /* DMA interrupt init */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  }
}


/**
 * @brief This function is used for sorting the value
 * @param A   Array data to be sorted
 * @param L   length of array
 */
static void BSP_Sort_Values(uint16_t A[], uint8_t L)
{
  uint8_t i = 0;
  uint8_t status = 1;

  while(status == 1)
  {
    status = 0;
    for(i = 0; i < L-1; i++)
    {
      if (A[i] > A[i+1])
      {
        A[i]^=A[i+1];
        A[i+1]^=A[i];
        A[i]^=A[i+1];
        status = 1;
      }
    }
  }
}

/**
 * @} STM32F4_DISCOVERY_ADC_Private_Functions End
 */

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_ADC_Public_Functions
 * @{
 */

/**
 * @brief ADC configuration function
 * @param hadc            ADC handle
 *        @arg hadc_bsp1
 *        @arg hadc_bsp2
 *        @arg hadc_bsp3
 * @param ADC_channel     Select ADC Channel
 *        @arg ADC_IN0 to ADC_IN15 for ADC1 and ADC2
 * NOTE        For ADC3 ADC_IN0 to ADC_IN3 and ADC_IN10 to ADC_IN13
 * @param rank            The rank in the regular group sequencer
 *        @arg   This parameter must be a number between Min_Data = 1 and Max_Data = 16
 * @param Multimode_Type  Configures the ADC to operate in independent or multi mode
 *        @arg Indepenent_Mode
 *        @arg Dual_Mode_REGSIMULT
 *        @arg Dual_Mode_INTERL
 *        @arg Triple_Mode_INTERL
 *        @arg Temperature_Measure ADC1 channel 16 is used to measure the Temperature
 */

/************************************************************************************
 * NOTE: There are 3 DMA Access mode
 *       DMA Access mode 1: This mode is used in ADC triple mode as shown below.
 *       DMA Access mode 2: This mode is used in interleaved mode(Dual and Triple ADC
 *                         mode) and in regular simultaneous mode (Dual ADC mode only).
 *       DMA Access mode 3: This mode is used in interleaved mode(Dual and Triple ADC
 *                         mode) in 6-bit and 8-bit resolutions.
 *
 * ADC_DUALMODE_REGSIMULT_INJECSIMULT       - DMAAccessMode (ADC_DMAACCESSMODE_2)
 * ADC_DUALMODE_REGSIMULT_ALTERTRIG         - DMAAccessMode (ADC_DMAACCESSMODE_2)
 * ADC_DUALMODE_REGSIMULT                   - DMAAccessMode (ADC_DMAACCESSMODE_2)
 * ADC_DUALMODE_INTERL                      - DMAAccessMode (ADC_DMAACCESSMODE_2)/(ADC_DMAACCESSMODE_3)
 * ADC_DUALMODE_INJECSIMULT
 * ADC_DUALMODE_ALTERTRIG
 * ADC_TRIPLEMODE_REGSIMULT_INJECSIMULT     - DMAAccessMode (ADC_DMAACCESSMODE_1)
 * ADC_TRIPLEMODE_REGSIMULT_AlterTrig       - DMAAccessMode (ADC_DMAACCESSMODE_1)
 * ADC_TRIPLEMODE_INTERL                    - DMAAccessMode (ADC_DMAACCESSMODE_2)/(ADC_DMAACCESSMODE_3)
 * ADC_TRIPLEMODE_INJECSIMULT
 * ADC_TRIPLEMODE_REGSIMULT                 - DMAAccessMode (ADC_DMAACCESSMODE_1)
 * ADC_TRIPLEMODE_ALTERTRIG
 *
 * NOTE: When more than 1 Channel is used for ADC conversion, change the
 *       NbrOfConversion as per the required channel and provide different rank to
 *       different channel so that it can scan one channel after another.
 ***********************************************************************************/

void BSP_ADC_Init(ADC_HandleTypeDef* hadc, ADC_PortPin_Typedef ADC_channel, uint8_t rank, ADC_ModeSel_Typedef multimode_type)
{
  ADC_ChannelConfTypeDef sConfig;
  if (hadc == &hadc_bsp1)
  {
    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    hadc->Instance = ADC1;
    if (multimode_type == Temperature_Measure)
    {
      hadc->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
    }
    else
    {
      hadc->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    }
    hadc->Init.Resolution = ADC_RESOLUTION12b;
    hadc->Init.ScanConvMode = ENABLE;
    hadc->Init.ContinuousConvMode = ENABLE;
    hadc->Init.DiscontinuousConvMode = DISABLE;
    hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc->Init.NbrOfConversion = 1;
    hadc->Init.DMAContinuousRequests = ENABLE;
    hadc->Init.EOCSelection = EOC_SEQ_CONV;

    if (multimode_type == Dual_Mode_REGSIMULT || multimode_type == Dual_Mode_INTERL || multimode_type == Triple_Mode_INTERL)
    {
      BSP_ADC_DMA_Init(hadc);   // Initialize DMA Clock and interrupt
    }
    BSP_ADC_MspInit(hadc,ADC_channel);
    HAL_ADC_Init(hadc);

    /**Configure the ADC multi-mode
     */
    ADC_MultiModeTypeDef multimode;

    if (multimode_type == Dual_Mode_REGSIMULT )
    {
      multimode.Mode = ADC_DUALMODE_REGSIMULT;
      multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
      multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;

      HAL_ADCEx_MultiModeConfigChannel(hadc, &multimode);
    }
    else if (multimode_type == Dual_Mode_INTERL)
    {
      multimode.Mode = ADC_DUALMODE_INTERL;
      multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
      multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;

      HAL_ADCEx_MultiModeConfigChannel(hadc, &multimode);
    }
    else if (multimode_type == Triple_Mode_INTERL)
    {
      multimode.Mode = ADC_TRIPLEMODE_INTERL;
      multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
      multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;

      HAL_ADCEx_MultiModeConfigChannel(hadc, &multimode);
    }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
    sConfig.Channel = ADC_Channel[ADC_channel];
    sConfig.Rank = rank;
    if(multimode_type == Temperature_Measure)
    {
      sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    }
    else
    {
      sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    }

    HAL_ADC_ConfigChannel(hadc, &sConfig);

  }
  else if (hadc == &hadc_bsp2)
  {
    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    hadc->Instance = ADC2;
    hadc->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hadc->Init.Resolution = ADC_RESOLUTION12b;
    hadc->Init.ScanConvMode = ENABLE;
    hadc->Init.ContinuousConvMode = ENABLE;
    hadc->Init.DiscontinuousConvMode = DISABLE;
    hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc->Init.NbrOfConversion = 1;
    hadc->Init.DMAContinuousRequests = ENABLE;
    hadc->Init.EOCSelection = EOC_SEQ_CONV;

    if (multimode_type == Dual_Mode_REGSIMULT || multimode_type == Dual_Mode_INTERL || multimode_type == Triple_Mode_INTERL)
    {
      BSP_ADC_DMA_Init(hadc);   // Initialize DMA Clock and interrupt
    }
    BSP_ADC_MspInit(hadc,ADC_channel);
    HAL_ADC_Init(hadc);

    ADC_MultiModeTypeDef multimode;
    /**Configure the ADC multi-mode
     */

    if (multimode_type == Dual_Mode_REGSIMULT )
    {
      multimode.Mode = ADC_DUALMODE_REGSIMULT;
      multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
      multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;

      HAL_ADCEx_MultiModeConfigChannel(hadc, &multimode);
    }
    else if (multimode_type == Dual_Mode_INTERL)
    {
      multimode.Mode = ADC_DUALMODE_INTERL;
      multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
      multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;

      HAL_ADCEx_MultiModeConfigChannel(hadc, &multimode);
    }
    else if (multimode_type == Triple_Mode_INTERL)
    {
      multimode.Mode = ADC_TRIPLEMODE_INTERL;
      multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
      multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;

      HAL_ADCEx_MultiModeConfigChannel(hadc, &multimode);
    }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
    sConfig.Channel = ADC_Channel[ADC_channel];
    sConfig.Rank = rank;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
  }
  else if (hadc == &hadc_bsp3)
  {
    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    hadc->Instance = ADC3;
    hadc->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    hadc->Init.Resolution = ADC_RESOLUTION12b;
    hadc->Init.ScanConvMode = ENABLE;
    hadc->Init.ContinuousConvMode = ENABLE;
    hadc->Init.DiscontinuousConvMode = DISABLE;
    hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc->Init.NbrOfConversion = 1;
    hadc->Init.DMAContinuousRequests = ENABLE;
    hadc->Init.EOCSelection = EOC_SEQ_CONV;

    BSP_ADC_DMA_Init();   // Initialize DMA Clock and interrupt
    BSP_ADC_MspInit(hadc,ADC_channel);
    HAL_ADC_Init(hadc);

    ADC_MultiModeTypeDef multimode;
    /**Configure the ADC multi-mode
     */

    if (multimode_type == Triple_Mode_INTERL)
    {
      multimode.Mode = ADC_TRIPLEMODE_INTERL;
      multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
      multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;

      HAL_ADCEx_MultiModeConfigChannel(hadc, &multimode);
    }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
    sConfig.Channel = ADC_Channel[ADC_channel];
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
  }
}

/**
 * @brief ADC threshold Configuration Function
 * @param hadc            ADC handle
 *        @arg hadc_bsp1
 *        @arg hadc_bsp2
 *        @arg hadc_bsp3
 * @param highthreshold   analog watchdog High threshold value
 *                        This value must be 12bit value(Max 4095)
 * @param lowthreshold    ADC analog watchdog Low threshold value.
 *                        This value must be 12bit value(Min 0)
 */
void BSP_ADC_Thr_Init(ADC_HandleTypeDef* hadc, uint16_t highthreshold, uint16_t lowthreshold)
{
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig;

  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.HighThreshold = highthreshold;
  AnalogWDGConfig.LowThreshold = lowthreshold;
  AnalogWDGConfig.ITMode = ENABLE;
  HAL_ADC_AnalogWDGConfig(hadc, &AnalogWDGConfig);
}

/**
 * @brief Temperature calculation
 * @return temperature value in DegCelcius
 */
float BSP_ADC_Get_Temperature(void)
{
  uint8_t i;

  for(i = 0; i < NS; i++)
  {
    Sample_ADC_Raw[i] = HAL_ADC_GetValue(&hadc_bsp1);
  }

  BSP_Sort_Values(Sample_ADC_Raw, NS);

  ADC_Average = 0;
  for(i = SR/2; i < NS-SR/2; i++)
  {
    ADC_Average += Sample_ADC_Raw[i];
  }

  ADC_Average /= (NS-SR);

  Temp += ADC_Average;
  Temp *= 3;
  Temp /= 4095;
  Temp -= (float)0.76;
  Temp /= (float)0.0025;
  Temp += (float)25.0;
  Temp -= Calibration_Value;

  return Temp;
}


/**
 * @brief This function disable the ADC and its port pin by disabling the ADC clock and GPIO clock
 * @param hadc      ADC handle
 *        @arg hadc_bsp1
 *        @arg hadc_bsp2
 *        @arg hadc_bsp3
 */
void BSP_ADC_MspDeInit(ADC_HandleTypeDef* hadc,ADC_PortPin_Typedef ADC_pin)
{
  if(hadc->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __ADC1_CLK_DISABLE();
  }
  else if(hadc->Instance==ADC2)
  {
    /* Peripheral clock disable */
    __ADC2_CLK_DISABLE();
  }

  else if(hadc->Instance==ADC3)
  {
    /* Peripheral clock disable */
    __ADC3_CLK_DISABLE();
  }
  if(ADC_pin == ADC_IN0 || ADC_pin == ADC_IN1 || ADC_pin == ADC_IN2 || ADC_pin == ADC_IN3 || ADC_pin == ADC_IN4
          || ADC_pin == ADC_IN5 || ADC_pin == ADC_IN6 || ADC_pin == ADC_IN7)
  {
    /* GPIO Ports Clock Disable */
    __GPIOA_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_Pin[ADC_pin]);
  }
  else if (ADC_pin == ADC_IN8 || ADC_pin == ADC_IN9)
  {
    /* GPIO Ports Clock Disable */
    __GPIOB_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_Pin[ADC_pin]);
  }
  else if (ADC_pin == ADC_IN10 || ADC_pin == ADC_IN11 || ADC_pin == ADC_IN12 || ADC_pin == ADC_IN13
           || ADC_pin == ADC_IN14 || ADC_pin == ADC_IN15)
  {
    /* GPIO Ports Clock Disable */
    __GPIOC_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOC, GPIO_Pin[ADC_pin]);
  }
  /* Peripheral interrupt Deinit*/

  /**
  * Uncomment the line below to disable the "ADC_IRQn" interrupt
  * Be aware, disabling shared interrupt may affect other IPs
  */
  // HAL_NVIC_DisableIRQ(ADC_IRQn);

  /* Peripheral DMA DeInit*/
  HAL_DMA_DeInit(hadc->DMA_Handle);
}

/**
 * @} STM32F4_DISCOVERY_TIMER_Public_Functions End
 */

/**
 * @} STM32F4_DISCOVERY_RTC End
 */

 /* --------------------------------- End Of File ------------------------------ */
