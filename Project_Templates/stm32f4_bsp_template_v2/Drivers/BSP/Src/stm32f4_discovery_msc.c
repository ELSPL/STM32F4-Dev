/**
  ******************************************************************************
  * @file    stm32f4_discovery_msc.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-June-2015
  * @brief   This file provides set of firmware functions to manage
  *          MSC (Mass storage).
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_msc.h"
#include "waveplayer.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_MSC
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_MSC
  * @brief This file provides set of firmware functions to manage MSCs
  * @{
  */
#ifdef USE_STM32F4_HOST_MSC

/** @defgroup STM32F4_DISCOVERY_MSC_Exported_Handle
 * @{
 */
/* USB Host Core handle declaration */
USBH_HandleTypeDef hUsbHostMSC;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_MSC_Private_Functions
  * @{
  */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id);

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_MSC_Functions
  * @{
  */
void BSP_HostMSC_Init (void)
{
  /* Init Host Library,Add Supported Class and Start the library*/
  USBH_Init(&hUsbHostMSC, USBH_UserProcess, HOST_FS);

  USBH_RegisterClass(&hUsbHostMSC, USBH_MSC_CLASS);

  USBH_Start(&hUsbHostMSC);

  MX_FATFS_Init();
}


/*
 * Background task
*/
void BSP_USB_HOST_Process()
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostMSC);
}

/*
 * user callback definition
*/
#ifndef USE_STM32F4_AUDIO
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
#endif

#ifdef USE_STM32F4_AUDIO
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch (id)
  {
    case HOST_USER_SELECT_CONFIGURATION:
      break;

    case HOST_USER_DISCONNECTION:
      WavePlayer_CallBack();
      Appli_state = APPLICATION_IDLE;
      f_mount(NULL, (TCHAR const*)"", 0);
      break;

    case HOST_USER_CLASS_ACTIVE:
      Appli_state = APPLICATION_START;
      break;

    case HOST_USER_CONNECTION:
      break;

    default:
      break;
  }
}
#endif

#endif /* USE_STM32F4_HOST_MSC */

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
