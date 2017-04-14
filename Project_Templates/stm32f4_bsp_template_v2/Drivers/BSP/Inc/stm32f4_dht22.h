/******************************************************************//**
* @file		stm32f4_dht22.h
* @brief    This is the declaration file for Temp and Humi sensor
* @version  V1.0
* @date		Jan 24, 2017
* @author	Bhavin Darji
**********************************************************************/

#ifndef __STM32F4_DHT22_H_
#define __STM32F4_DHT22_H_

/* Includes ------------------------------------------------------------------- */
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "stm32f4_global.h"
#include "stm32f4_discovery_uart.h"

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
/** @defgroup STM32F4_dht22_Public_Types
  * @{
  */


 /** @defgroup STM32F4_dht22_Exported_Functions
   * @{
   */

 void DHT22_Init(void);
 void DHT22_read(void);

/**
 * @}
 */

 /**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DHT22_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
