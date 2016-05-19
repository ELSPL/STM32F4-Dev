/**
  ******************************************************************************
  * @file    mediaplayerconf.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    31-July-2015
  * @brief   Header for mediaplayerconf.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEDIAPLAYERCONF_H
#define __MEDIAPLAYERCONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_audio.h"
#include "waveplayer.h"
#include "waverecorder.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MSC_Audio_Application(void);
void COMMAND_AudioExecuteApplication(void);

#endif /* __MEDIAPLAYERCONF_H */

/***********END OF FILE************/
