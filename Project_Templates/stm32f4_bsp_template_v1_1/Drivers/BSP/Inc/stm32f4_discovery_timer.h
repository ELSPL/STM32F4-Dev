/******************************************************************//**
* @file		stm32f4_discovery_timer.h
* @brief	This file contains declaration for stm32f4_discovery_timer.c
* @version	V1.0
* @date		May 20, 2015
* @author	Dwijay.Edutech Learning solutions
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_TIMER_H
#define __STM32F4_DISCOVERY_TIMER_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_TIMER
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif


/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_TIMER_Public_Types
 * @{
 */

extern __IO uint32_t TimCapPsc;
extern __IO uint32_t TimCapPeriod;

/* Capture Callback Freq Calc global variables */
extern __IO uint32_t uwIC2Value1;
extern __IO uint32_t uwIC2Value2;
extern __IO uint32_t uwDiffCapture;
extern __IO uint16_t uhCaptureIndex;
extern __IO uint32_t uwFrequency;

extern __IO uint8_t tim9_12_flag;
extern __IO uint32_t uwFrequency1;
extern __IO uint32_t uwIC2Value;
extern __IO uint32_t uwDutyCycle;

extern TIM_HandleTypeDef htim1_mcpwm;
extern TIM_HandleTypeDef htim8_mcpwm;

extern TIM_HandleTypeDef htim2_gen;
extern TIM_HandleTypeDef htim3_gen;
extern TIM_HandleTypeDef htim4_gen;
extern TIM_HandleTypeDef htim5_gen;

extern TIM_HandleTypeDef htim6_basic;
extern TIM_HandleTypeDef htim7_basic;

extern TIM_HandleTypeDef htim10_cap;
extern TIM_HandleTypeDef htim11_cap;
extern TIM_HandleTypeDef htim13_cap;
extern TIM_HandleTypeDef htim14_cap;

extern TIM_HandleTypeDef htim9_pcap;
extern TIM_HandleTypeDef htim12_pcap;


/** @brief channel type */
typedef enum
{
  None    =0, /*!< No Channel Configuration */
  TIM_CH1 =1, /*!< Config for channel 0     */
  TIM_CH2 =2, /*!< Config for channel 1     */
  TIM_CH3 =3, /*!< Config for channel 2     */
  TIM_CH4 =4  /*!< Config for channel 3     */
}TIM_CH_Type;

/** @brief Timer/Counter external match option */
typedef enum
{
  TIM_OC_MODE_NOTHING = 0, /*!< Do nothing for external output pin if match */
  TIM_OC_MODE_LOW,         /*!< Force external output pin to low if match */
  TIM_OC_MODE_HIGH,        /*!< Force external output pin to high if match */
  TIM_OC_MODE_TOGGLE       /*!< Toggle external output pin if match */
}TIM_OC_MODE_OPT;

/** @brief PWM align modes */
typedef enum
{
  PWM_EDGE_ALIGN_MODE = 0, /*!< Do nothing for external output pin if match */
  PWM_CENTER_ALIGN_MODE1,  /*!< Force external output pin to low if match */
  PWM_CENTER_ALIGN_MODE2,  /*!< Force external output pin to high if match */
  PWM_CENTER_ALIGN_MODE3   /*!< Toggle external output pin if match */
}PWM_ALIGN_MODE;

/** @brief Capture Polarity Type */
typedef enum
{
  TIM_CAP_RISING_EDGE = 0, /*!< Capture at Rising Edge */
  TIM_CAP_FALLING_EDGE,    /*!< Capture at Falling Edge */
  TIM_CAP_BOTH_EDGE        /*!< Capture at both Rising and Falling Edge */
}TIM_CAP_POLARITY_Type;

/** @brief Capture Frequency rate */
typedef enum
{
  LOW_FREQ = 0,   /*!< Capture at low frequency */
  MEDIUM_FREQ,    /*!< Capture at medium frequency */
  FAST_FREQ       /*!< Capture at fast frequency */
}TIM_CAP_FREQ_RATE;

/**
 * @} STM32F4_DISCOVERY_TIMER_Public_Types End
 */



/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_TIMER_Public_Functions
 * @{
 */
/* Basic and General Timer functions */
void BSP_TIM_Basic_Config(TIM_HandleTypeDef* htim_base, float period_us);
void BSP_TIM_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, uint32_t freq_hz, TIM_OC_MODE_OPT hw_toggle);

/* PWM and MCPWM Timer functions */
void BSP_PWM_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, uint32_t freq_hz, float pulsewidth_us, PWM_ALIGN_MODE pwm_mode);
void BSP_MCPWM_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, uint32_t freq_hz, float pulsewidth_us, uint8_t deadtime, PWM_ALIGN_MODE pwm_mode);

/* Timer and PWM capture functions */
void BSP_TIM_Capture_Config(TIM_HandleTypeDef* htim_base, TIM_CAP_FREQ_RATE freqSel, TIM_CAP_POLARITY_Type capEdge);
void BSP_PWM_Capture_Config(TIM_HandleTypeDef* htim_base, TIM_CH_Type tim_ch, TIM_CAP_FREQ_RATE freqSel);

/* Timer De-Initialization function */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);

/**
 * @} STM32F4_DISCOVERY_TIMER_Public_Functions End
 */






#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_TIMER_H */

/**
 * @} STM32F4_DISCOVERY_TIMER End
 */

/* --------------------------------- End Of File ------------------------------ */
