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


extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart4_rx;

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;

extern uint8_t EscFlag;
extern uint8_t Uart4Ready;
extern uint8_t Uart6Ready;

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
/*                       UART4 Mode                                            */
/******************************************************************************/
#define   POLLING_SEL_UART4       DISABLE      // Specify the type of interface
#define   INTERRUPT_SEL_UART4     ENABLE
#define   DMA_SEL_UART4           DISABLE

/******************************************************************************/
/*                       UART6 Mode                                            */
/******************************************************************************/
#define   POLLING_SEL_UART6       DISABLE      // Specify the type of interface
#define   INTERRUPT_SEL_UART6     ENABLE
#define   DMA_SEL_UART6           DISABLE


/******************************************************************************/
/*                       UART6 Mode validation                                 */
/******************************************************************************/
#if ((POLLING_SEL__UART6 && INTERRUPT_SEL_UART6) || (POLLING_SEL_UART6 && DMA_SEL_UART6) || (INTERRUPT_SEL_UART6 && DMA_SEL_UART6))
  #error uart mode not correctly defined
#endif

#if POLLING_SEL_UART6
  #define POLLING_MODE_UART6
#elif INTERRUPT_SEL_UART6
  #define INTERRUPT_MODE_UART6
#elif DMA_SEL_UART6
  #define DMA_MODE_UART6
#else
  #error Uart Mode not defined
#endif


/******************************************************************************/
/*                       UART4 Mode validation                                 */
/******************************************************************************/
#if ((POLLING_SEL__UART4 && INTERRUPT_SEL_UART4) || (POLLING_SEL_UART4 && DMA_SEL_UART4) || (INTERRUPT_SEL_UART4 && DMA_SEL_UART4))
  #error uart mode not correctly defined
#endif

#if POLLING_SEL_UART4
  #define POLLING_MODE_UART4
#elif INTERRUPT_SEL_UART4
  #define INTERRUPT_MODE_UART4
#elif DMA_SEL_UART4
  #define DMA_MODE_UART4
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
void BSP_UART_Init(UART_HandleTypeDef* huart,uint32_t Baudrate);
void BSP_UART_MspDeInit(UART_HandleTypeDef* huart);

/* UART General Transmit and Receive functions */
HAL_StatusTypeDef UART_Transmit(UART_HandleTypeDef* huart,uint8_t *data_buf, uint16_t size, uint32_t timeout);
HAL_StatusTypeDef UART_Receive(UART_HandleTypeDef* huart,uint8_t *data_buf, uint16_t size, uint32_t timeout);

/* UART Stdio functions */
HAL_StatusTypeDef uprintf(UART_HandleTypeDef* huart,const char *format, ...);
uint8_t ugetche(UART_HandleTypeDef* huart,TRANSFER_BLOCK_Type mode);
uint8_t uget_line(UART_HandleTypeDef* huart,int8_t s[], uint8_t lim);

/* UART utility functions */
void uart_clr_scr_rst_cur (UART_HandleTypeDef* huart);
void uErase_Char (UART_HandleTypeDef* huart);
void uErase_Char_With_UnderScore (UART_HandleTypeDef* huart);
void uErase_Backslash (UART_HandleTypeDef* huart);
void uErase_SemiColon (UART_HandleTypeDef* huart);
void uErase_And_RingTheBell (UART_HandleTypeDef* huart);



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
