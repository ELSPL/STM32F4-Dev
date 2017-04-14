/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Inc/usbd_desc.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    13-March-2015
  * @brief   Header for usbd_desc.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_DESC_H
#define __USBD_DESC_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @{
 */

/** @defgroup USB_DESC
 * @brief general defines for the usb device library file
 * @{
 */

/** @defgroup USB_DESC_Exported_Defines
 * @{
 */
#ifdef USE_STM32F4_VCP
#define         DEVICE_ID1          (0x1FFF7A10)
#define         DEVICE_ID2          (0x1FFF7A14)
#define         DEVICE_ID3          (0x1FFF7A18)

#define  USB_SIZ_STRING_SERIAL       0x1A
#endif


/**
 * @}
 */

/** @defgroup USBD_DESC_Exported_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_DESC_Exported_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_DESC_Exported_Variables
 * @{
 */
#ifdef USE_STM32F4_VCP
extern USBD_DescriptorsTypeDef VCP_Desc;
#endif

#ifdef USE_STM32F4_HID
extern USBD_DescriptorsTypeDef HID_Desc;
#endif

/**
 * @}
 */

/** @defgroup USBD_DESC_Exported_FunctionsPrototype
  * @{
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __USBD_DESC_H */
/**
 * @}
 */

/**
 * @}
 */
/******END OF FILE****/
