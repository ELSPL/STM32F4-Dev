/**
  ******************************************************************************
  * @file    stm32f4_discovery_hid.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    02-June-2015
  * @brief   This file contains definitions and support for HID drivers
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_HID_H
#define __STM32F4_DISCOVERY_HID_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_HID
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_HID_Exported_Handle
 * @{
 */
/* USB Device Core handle declaration */
extern USBD_HandleTypeDef hUsbDeviceHID;

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_HID_Exported_Constants
  * @{
  */


/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_HID_Exported_Functions
  * @{
  */

/* HID Initialization function */
void BSP_HID_Init (void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_HID_H */

/****************************************END OF FILE******************************************/
