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
#ifdef USE_STM32F4_DEVICE_HID
USBD_HandleTypeDef hUsbDeviceHID;
#endif

#ifdef USE_STM32F4_HOST_HID
USBH_HandleTypeDef hUsbHostHID;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;
#endif

/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_HID_Private_Functions
  * @{
  */
#ifdef USE_STM32F4_HOST_HID
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id);
#endif
/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_HID_Functions
  * @{
  */
#ifdef USE_STM32F4_DEVICE_HID
void BSP_DeviceHID_Init (void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceHID, &HID_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceHID, &USBD_HID);
  USBD_Start(&hUsbDeviceHID);
}

#endif /* USE_STM32F4_DEVICE_HID */


#ifdef USE_STM32F4_HOST_HID
/* init function */
void BSP_HostHID_Init(void)
{
  /* Init Host Library,Add Supported Class and Start the library*/
  USBH_Init(&hUsbHostHID, USBH_UserProcess, HOST_FS);

  USBH_RegisterClass(&hUsbHostHID, USBH_HID_CLASS);

  USBH_Start(&hUsbHostHID);
}

/*
 * Background task
*/
void BSP_USB_HOST_Process()
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostHID);
}

/*
 * user callback definition
*/
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch (id)
  {
    case HOST_USER_SELECT_CONFIGURATION:
      break;

    case HOST_USER_DISCONNECTION:
      Appli_state = APPLICATION_DISCONNECT;
      break;

    case HOST_USER_CLASS_ACTIVE:
      Appli_state = APPLICATION_READY;
      break;

    case HOST_USER_CONNECTION:
      Appli_state = APPLICATION_START;
      break;

    default:
      break;
  }
}
#endif /* USE_STM32F4_HOST_HID */
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
