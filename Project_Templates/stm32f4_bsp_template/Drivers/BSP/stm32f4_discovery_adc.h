/******************************************************************//**
* @file   stm32f4_discovery_adc.h
* @brief  This file contains declaration for stm32f4_discovery_adc.c
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_ADC_H
#define __STM32F4_DISCOVERY_ADC_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_ADC
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_ADC_Public_Types
 * @{
 */

extern ADC_HandleTypeDef hadc_bsp1;
extern ADC_HandleTypeDef hadc_bsp2;
extern ADC_HandleTypeDef hadc_bsp3;

extern DMA_HandleTypeDef hdma_adcbsp1;
extern DMA_HandleTypeDef hdma_adcbsp2;
extern DMA_HandleTypeDef hdma_adcbsp3;

typedef enum
{
  ADC_IN0,
  ADC_IN1,
  ADC_IN2,
  ADC_IN3,
  ADC_IN4,
  ADC_IN5,
  ADC_IN6,
  ADC_IN7,
  ADC_IN8,
  ADC_IN9,
  ADC_IN10,
  ADC_IN11,
  ADC_IN12,
  ADC_IN13,
  ADC_IN14,
  ADC_IN15,
  ADC_IN16
}ADC_PortPin_Typedef;

typedef enum
{
  Indepenent_Mode,
  Dual_Mode_REGSIMULT,
  Dual_Mode_INTERL,
  Triple_Mode_INTERL,
  Temperature_Measure
}ADC_ModeSel_Typedef;

extern uint16_t GPIO_Pin[16];
extern uint32_t ADC_Chnl[19];
/**
 * @} STM32F4_DISCOVERY_ADC_Public_Types End
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_ADC_Public_Functions
 * @{
 */
void BSP_ADC_Init(ADC_HandleTypeDef* hadc,ADC_PortPin_Typedef ADC_channel, uint8_t rank, ADC_ModeSel_Typedef multimode_type);
void BSP_ADC_WDG_Init(ADC_HandleTypeDef* hadc, uint16_t highthreshold, uint16_t lowthreshold);
float BSP_ADC_Get_Temperature(void);
void BSP_ADC_MspDeInit(ADC_HandleTypeDef* hadc,ADC_PortPin_Typedef ADC_pin);

/**
 * @} STM32F4_DISCOVERY_ADC_Public_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_ADC_H */

/**
 * @} STM32F4_DISCOVERY_ADC End
 */

/* --------------------------------- End Of File ------------------------------ */
