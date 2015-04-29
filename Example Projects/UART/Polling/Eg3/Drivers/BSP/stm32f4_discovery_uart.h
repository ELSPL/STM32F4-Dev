/**
  ******************************************************************************
  * @file    stm32f4_discovery_uart.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file contains definitions for UART implementation as alternative
  *          to standard input/output library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_UART_H
#define __STM32F4_DISCOVERY_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stdarg.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_UART
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_UART_Exported_Handle
 * @{
 */

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;

extern uint8_t EscFlag;
extern uint8_t UartReady;

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_UART_Exported_Constants
  * @{
  */

/**
 * UART VT100 Terminal Commands Macros
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

/******************************************************************************/
/*                       UART Mode                                            */
/******************************************************************************/
#define   POLLING_SEL       ENABLE      // Specify the type of interface
#define   INTERRUPT_SEL     DISABLE
#define   DMA_SEL           DISABLE

/******************************************************************************/
/*                       UART Mode validation                                 */
/******************************************************************************/
#if ((POLLING_SEL && INTERRUPT_SEL) || (POLLING_SEL && DMA_SEL) || (INTERRUPT_SEL && DMA_SEL))
  #error uart mode not correctly defined
#endif

#if POLLING_SEL
  #define POLLING_MODE
#elif INTERRUPT_SEL
  #define INTERRUPT_MODE
#elif DMA_SEL
  #define DMA_MODE
#else
  #error Uart Mode not defined
#endif

#define MAX_TIMEOUT   0xFFFFFFFF


/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_UART_Exported_Functions
  * @{
  */

/* UART Init and De-Init functions */
void BSP_UART_Init(uint32_t Baudrate);
void BSP_UART_MspDeInit(UART_HandleTypeDef* huart);

/* UART Stdio functions */
HAL_StatusTypeDef uprintf(const char *format, ...);
uint8_t ugetche(TRANSFER_BLOCK_Type mode);
uint8_t uget_line(int8_t s[], uint8_t lim);

/* UART utility functions */
void uart_clr_scr_rst_cur (void);
void uErase_Char (void);
void uErase_Char_With_UnderScore (void);
void uErase_Backslash (void);
void uErase_SemiColon (void);
void uErase_And_RingTheBell (void);



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

#endif /* __STM32F4_DISCOVERY_UART_H */

/****************************************END OF FILE******************************************/
