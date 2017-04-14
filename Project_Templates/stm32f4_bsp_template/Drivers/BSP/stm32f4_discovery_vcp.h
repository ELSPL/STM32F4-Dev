/**
  ******************************************************************************
  * @file    stm32f4_discovery_vcp.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file contains definitions for Virtual COM Port (USB Device CDC class)
  *          implementation as alternative to standard input/output library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_VCP_H
#define __STM32F4_DISCOVERY_VCP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "usbd_cdc_if_template.h"
#include "stdarg.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_VCP
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_VCP_Exported_Handle
 * @{
 */

extern uint8_t vEscFlag;

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_VCP_Exported_Constants
  * @{
  */

/**
 *  VT100 Terminal Commands Macros
 */
#define In_DELETE   0x7F    /* ASCII <DEL> */
#define In_ESC      0x1B    /* ASCII ESCAPE */
#define In_BACKSPACE  '\x8'   /* ASCII BACK_SPACE */
#define In_CR     0x0D    /* ASCII CARRIAGE RETURN */
#define Out_DELETE    "\x8 \x8" /* CIFER backspace and clear */
#define Out_ESC     0x1B    /* ASCII ESCAPE */
#define Out_BACKSPACE '\x8'   /* ASCII BACK_SPACE */
#define Out_SPACE     '\x20'    /* ASCII BACK_SPACE */
#define In_EOL      '\r'    /* ASCII <CR>  */
#define In_BELL         0x07        /* ASCII BELL */

#define MAX_TIMEOUT   0xFFFFFFFF


/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_VCP_Exported_Functions
  * @{
  */

/* VCP Initilization function */
void BSP_VCP_Init (void);

/* VCP Stdio functions */
HAL_StatusTypeDef vuprintf(const char *format, ...);
uint8_t vgetche(TRANSFER_BLOCK_Type mode);
uint8_t vget_line(int8_t s[], uint8_t lim);

/* VCP utility functions */
void vcp_clr_scr_rst_cur (void);
void vErase_Char (void);
void vErase_Char_With_UnderScore (void);
void vErase_Backslash (void);
void vErase_SemiColon (void);
void vErase_And_RingTheBell (void);
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

#endif /* __STM32F4_DISCOVERY_VCP_H */

/****************************************END OF FILE******************************************/
