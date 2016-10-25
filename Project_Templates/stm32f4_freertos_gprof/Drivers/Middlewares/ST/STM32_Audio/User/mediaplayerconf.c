/**
  ******************************************************************************
  * @file    mediaplayerconf.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    31-July-2015
  * @brief   This is custom implementation of Audio and Playback routines.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mediaplayerconf.h"

/** @addtogroup STM32F4-Discovery_Media_Player_Config
* @{
*/
#ifdef USE_STM32F4_AUDIO
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t  USBH_USR_ApplicationState = USBH_USR_FS_INIT;

__IO uint32_t CmdIndex = CMD_PLAY;
__IO uint32_t PbPressCheck = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main routine for Mass storage application
  * @param  None
  * @retval None
  */
void MSC_Audio_Application(void)
{
  switch (USBH_USR_ApplicationState)
  {
  case USBH_USR_AUDIO:
    /* Go to Audio menu */
    COMMAND_AudioExecuteApplication();

    /* Set user initialization flag */
    USBH_USR_ApplicationState = USBH_USR_FS_INIT;
    break;

  case USBH_USR_FS_INIT:
    /* Initializes the File System */
    while(f_mount(&USBH_fatfs, USBH_Path, 0) != FR_OK)
    {
      uprintf("ERROR : Cannot Initialize FatFs! \n");
    }

    /* Go to menu */
    USBH_USR_ApplicationState = USBH_USR_AUDIO;
    break;

  default:
    break;
  }
}

/**
  * @brief  COMMAND_AudioExecuteApplication.
  * @param  None
  * @retval None
  */
void COMMAND_AudioExecuteApplication(void)
{
  /* Execute the command switch the command index */
  switch (CmdIndex)
  {
    /* Start Playing from USB Flash memory */
  case CMD_PLAY:
    if (RepeatState == REPEAT_ON)
      WavePlayerStart();
    break;

    /* Start Recording in USB Flash memory */
  case CMD_RECORD:
    RepeatState = REPEAT_ON;
    WaveRecorderProcess();
    break;

  default:
    break;
  }
}

/**
 * @brief  EXTI line detection callbacks.
 * @param  GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if(GPIO_Pin == GPIO_PIN_0)
 {
   if (PbPressCheck == 0)
   {
     HAL_Delay(10);
     /* Test on the command: Recording */
     if (CmdIndex == CMD_RECORD)
     {
       RepeatState = REPEAT_ON;

       /* Switch to Play command */
       CmdIndex = CMD_PLAY;
     }
     /* Test on the command: Playing */
     else if (CmdIndex == CMD_PLAY)
     {
       /* Switch to Record command */
       CmdIndex = CMD_RECORD;
     }
     else
     {
       RepeatState = REPEAT_ON;
       /* Default Command Index: Play command */
       CmdIndex = CMD_PLAY;
     }
     PbPressCheck = 1;
   }
   else
   {
     PbPressCheck = 0;
   }
 }

 if(GPIO_Pin == GPIO_PIN_1)
 {
   if (PressCount == 1)
   {
     /* Resume playing Wave status */
     PauseResumeStatus = RESUME_STATUS;
     PressCount = 0;
   }
   else
   {
     /* Pause playing Wave status */
     PauseResumeStatus = PAUSE_STATUS;
     PressCount = 1;
   }
 }
}

#endif
/**
  * @}
  */

/**************END OF FILE**************/
