/******************************************************************//**
* @file   stm32f4_discovery_iwdg.h
* @brief  This file contains declaration for stm32f4_discovery_iwdg.c
* @version  V1.0
* @date   May 29, 2015
* @author Bhavin Darji.Edutech learning solutions
**********************************************************************/

#ifndef __STM32F4_DISCOVERY_IWDG_H
#define __STM32F4_DISCOVERY_IWDG_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"


/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_IWDG
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_IWDG_Public_Types
 * @{
 */

extern IWDG_HandleTypeDef hiwdg_bsp;

/**
 * @} STM32F4_DISCOVERY_IWDG_Public_Types End
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_IWDG_Public_Functions
 * @{
 */

/* IWDG initialization and configuration function */
void BSP_IWDG_Init(uint16_t IWDG_Prescaler, uint16_t ReloadCount);
/**
 * @} STM32F4_DISCOVERY_IWDG_Public_Functions End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_IWDG_H */

/**
 * @} STM32F4_DISCOVERY_IWDG End
 */

/* --------------------------------- End Of File ------------------------------ */
