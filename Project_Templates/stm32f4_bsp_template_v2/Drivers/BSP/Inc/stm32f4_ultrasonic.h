/******************************************************************//**
* @file		stm32f4_ultrasonic.h
* @brief  This is the declaration file for UltraSonic ranger
* @version V1.0
* @date		Jan 24, 2017
* @author	Bhavin Darji
**********************************************************************/

#ifndef __STM32F4_ULTRASONIC_H_
#define __STM32F4_ULTRASONIC_H_

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "stm32f4_global.h"

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup GPIO
 * @ingroup BSP
 * @{
 */


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup STM32F4_UltraSonic_Public_Types
  * @{
  */

/**
 * @brief GPIO direction
 */
 typedef enum{
    Input=0,
    Output
 }GPIODirection_TypeDef;

 /** @defgroup STM32F4_GSM_Exported_Functions
   * @{
   */
 uint16_t UltraSonic_Measure_In_CM(void);

/**
 * @}
 */



 /**
  * @}
  */






#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_ULTRASONIC_H_ */

/**
 * @} GPIO End
 */

/* --------------------------------- End Of File ------------------------------ */
