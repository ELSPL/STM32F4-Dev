/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Inc/usbd_conf.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    13-March-2015
  * @brief   General low level driver configuration
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/** @addtogroup USBD_OTG_DRIVER
 * @{
 */

/** @defgroup USBD_CONF
 * @brief usb otg low level driver configuration file
 * @{
 */

/** @defgroup USBD_CONF_Exported_Defines
 * @{
 */
/* Common Config */
#define USBD_MAX_NUM_INTERFACES               1
#define USBD_MAX_NUM_CONFIGURATION            1

#ifdef USE_STM32F4_VCP
  #define USBD_MAX_STR_DESC_SIZ               0x100
#endif
#ifdef USE_STM32F4_HID
  #define USBD_MAX_STR_DESC_SIZ               512
  #define USBD_LPM_ENABLED                    0
#endif

#define USBD_SUPPORT_USER_STRING              0
#define USBD_SELF_POWERED                     1
#define USBD_DEBUG_LEVEL                      0

/****************************************/
#ifdef USE_STM32F4_HID
/* #define for FS and HS identification */
#define DEVICE_FS     0
#define DEVICE_HS     1
#endif

/** @defgroup USBD_Exported_Macros
  * @{
  */
/* Memory management macros */
#define USBD_malloc               malloc
#define USBD_free                 free
#define USBD_memset               memset
#define USBD_memcpy               memcpy

#define USBD_Delay   HAL_Delay

/* DEBUG macros */
#if (USBD_DEBUG_LEVEL > 0)
#define  USBD_UsrLog(...)   printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_UsrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 1)

#define  USBD_ErrLog(...)   printf("ERROR: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_ErrLog(...)
#endif

#if (USBD_DEBUG_LEVEL > 2)
#define  USBD_DbgLog(...)   printf("DEBUG : ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_DbgLog(...)
#endif
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup USBD_CONF_Exported_Types
  * @{
  */
#ifdef USE_STM32F4_VCP
extern PCD_HandleTypeDef hpcd_USB_OTG_VCP;
#endif

#ifdef USE_STM32F4_HID
extern PCD_HandleTypeDef hpcd_USB_OTG_HID;
#endif

/**
  * @}
  */

/** @defgroup USBD_CONF_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_CONF_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_CONF_Exported_FunctionsPrototype
  * @{
  */
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif //__USBD_CONF_H

/**
  * @}
  */

/**
  * @}
  */


/******END OF FILE****/
