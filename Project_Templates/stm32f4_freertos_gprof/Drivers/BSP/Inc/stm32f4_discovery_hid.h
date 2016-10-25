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

#ifdef USE_STM32F4_DEVICE_HID
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"
#endif

#ifdef USE_STM32F4_HOST_HID
#include "usbh_core.h"
#include "usbh_hid.h"
#endif

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_HID
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_HID_Exported_Handle
 * @{
 */
#ifdef USE_STM32F4_HOST_HID
typedef enum {
 APPLICATION_IDLE = 0,
 APPLICATION_START,
 APPLICATION_READY,
 APPLICATION_DISCONNECT,
}ApplicationTypeDef;

/* USB Host Core handle declaration */
extern USBH_HandleTypeDef hUsbHostHID;
extern ApplicationTypeDef Appli_state;
#endif

#ifdef USE_STM32F4_DEVICE_HID
/* USB Device Core handle declaration */
extern USBD_HandleTypeDef hUsbDeviceHID;
#endif



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
void BSP_DeviceHID_Init (void);

/* HID Host Initialization function */
void BSP_HostHID_Init (void);
void BSP_USB_HOST_Process(void);

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
