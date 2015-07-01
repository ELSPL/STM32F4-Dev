/******************************************************************//**
* @file   stm32f4_discovery_wdg.h
* @brief  This file contains declaration for stm32f4_discovery_wdg.c
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_WDG_H
#define __STM32F4_DISCOVERY_WDG_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"


/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_WDG
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_WDG_Public_Types
 * @{
 */

extern WWDG_HandleTypeDef hwwdg_bsp;     // window watchdog handle
extern IWDG_HandleTypeDef hiwdg_bsp;	 // independent watchdog handle	

/**
 * @} STM32F4_DISCOVERY_WDG_Public_Types End
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_WDG_Public_Functions
 * @{
 */

/* WWDG initialization and configuration function */
void BSP_WWDG_Init(uint16_t WWDG_Prescaler, uint8_t WindowSize, uint8_t CounterSize);
void BSP_WWDG_MspDeInit(WWDG_HandleTypeDef* hwwdg);

/* IWDG initialization and configuration function */
void BSP_IWDG_Init(uint16_t ResetTime_ms);

/**
 * @} STM32F4_DISCOVERY_WDG_Public_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_WDG_H */

/**
 * @} STM32F4_DISCOVERY_WDG End
 */

/* --------------------------------- End Of File ------------------------------ */
