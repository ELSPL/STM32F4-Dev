/**
  ******************************************************************************
  * @file    stm32f4_discovery_hid.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    02-June-2015
  * @brief   This file provides set of firmware functions to manage
  *          HID (Human Interface Devices)
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_hid.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_HID
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_HID
  * @brief This file provides set of firmware functions to manage HIDs
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_HID_Exported_Handle
 * @{
 */
/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceHID;

/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_HID_Private_Functions
  * @{
  */

/**
  * @}
  */

#ifdef USE_STM32F4_HID
/** @defgroup STM32F4_DISCOVERY_HID_Functions
  * @{
  */
void BSP_HID_Init (void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceHID, &HID_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceHID, &USBD_HID);
  USBD_Start(&hUsbDeviceHID);
}

#endif /* USE_STM32F4_HID */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/*****************************************END OF FILE******************************************/
