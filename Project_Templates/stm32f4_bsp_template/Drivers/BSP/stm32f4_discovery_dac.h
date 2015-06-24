/******************************************************************//**
* @file   stm32f4_discovery_dac.h
* @brief  This file contains declaration for stm32f4_discovery_dac.c
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_DAC_H
#define __STM32F4_DISCOVERY_DAC_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery_uart.h"

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_DAC
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_DAC_Public_Types
 * @{
 */

extern DAC_HandleTypeDef hdac_bsp;
#define PI 3.1415

/**
 * @} STM32F4_DISCOVERY_DAC_Public_Types End
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_DAC_Public_Functions
 * @{
 */
/* Initialization functions */
void BSP_DAC_Init(uint8_t DAC_Channel, uint8_t Alignment,uint16_t Data,uint8_t DAC_Trigger);
void BSP_DAC_MspDeInit(DAC_HandleTypeDef* hdac,uint8_t DAC_Channel);

/* Application specific function */
void BSP_DAC_SquareWave(uint8_t DAC_Channel);
void BSP_DAC_SawtoothWave(uint8_t DAC_Channel);
void BSP_DAC_TriangularWave(uint8_t DAC_Channel);
void BSP_DAC_SineWave(uint8_t DAC_Channel);
/**
 * @} STM32F4_DISCOVERY_DAC_Public_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_DAC_H_ */

/**
 * @} STM32F4_DISCOVERY_DAC End
 */

/* --------------------------------- End Of File ------------------------------ */
