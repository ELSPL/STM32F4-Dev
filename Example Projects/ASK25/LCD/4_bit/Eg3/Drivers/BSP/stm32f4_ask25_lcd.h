/**
  ******************************************************************************
  * @file    stm32f4_ask25_lcd.h
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file contains definitions for STM32F4-ASK25 Kit's LCD.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_ASK25_LCD_H
#define __STM32F4_ASK25_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25_LCD
  * @{
  */


/** @defgroup STM32F4_ASK25_LCD_Exported_Types
  * @{
  */

typedef enum                    // Use following notations in function where
{                               // this enum is used as a parameter
   Dec = 0,
   Inc = 1
}AddrCount_e;

typedef enum
{
   DispShiftOff = 0,
   DispShiftOn = 1
}DispShift_e;

typedef enum
{
   Off = 0,
   On  = 1,
   Blink =2
}CursorType_e;

typedef enum
{
   Left = 1,
   Right = 2
}ShiftDir_e;

/**
  * @}
  */

/** @defgroup STM32F4_ASK25_LCD_Exported_Constants
  * @{
  */

/******************************************************************************/
/*                              LCD Commands                                  */
/******************************************************************************/
#define     LCD_CLEAR           0X01
#define     CURSOR_HOME         0x02

#define     LCD_OFF             0X08
#define     LCD_ON              0X0C
#define     CURSOR_ON           0X0E
#define     CURSOR_OFF          0X0C
#define     CURSOR_BLINK        0X0F

#define     LCD_SHIFT_LEFT      0x18
#define     LCD_SHIFT_RIGHT     0x1C
#define     CURSOR_SHIFT_LEFT   0X10
#define     CURSOR_SHIFT_RIGHT  0X14

#define     LCD_DEC_ADD_DISP_SHIFT_OFF  0x04   //decrement address & display shift off
#define     LCD_DEC_ADD_DISP_SHIFT_ON   0x05   //decrement address & display shift on
#define     LCD_INC_ADD_DISP_SHIFT_OFF  0X06   //increment address & display shift off
#define     LCD_INC_ADD_DISP_SHIFT_ON   0X0    //increment address & display shift on

#define     LCD_4BIT_1LINE      0x20           //4-bit interface, 1 line,  5*7 Pixels
#define     LCD_4BIT_2LINE      0x28           //4-bit interface, 2 lines, 5*7 Pixels
#define     LCD_8BIT_1LINE      0X30           //8-bit interface, 1 line,  5*7 Pixels
#define     LCD_8BIT_2LINE      0X38           //8-bit interface, 2 lines, 5*7 Pixels


/******************************************************************************/
/*                              LCD Selection                                 */
/******************************************************************************/
#define     LCD16X1         DISABLE         // Specify the type of LCD
#define     LCD16X2         ENABLE
#define     LCD16X4         DISABLE
#define     LCD20X2         DISABLE
#define     LCD20X4         DISABLE
#define     LCD40X2         DISABLE
#define     LCD40X4         DISABLE


/******************************************************************************/
/*                       LCD Selection Validation                             */
/******************************************************************************/
#if LCD20X2|LCD20X4
    #define     ROWADDR1        0x80
    #define     ROWADDR2        0xC0
    #define     ROWADDR3        0x94
    #define     ROWADDR4        0xD4
    #define     COLUMNSIZE      20
#elif LCD40X2|LCD40X4
    #define     ROWADDR1        0x80
    #define     ROWADDR2        0xC0
    #define     ROWADDR3        0x80
    #define     ROWADDR4        0xC0
    #define     COLUMNSIZE      40
#elif (LCD16X1|LCD16X2|LCD16X4)
    #define     ROWADDR1        0x80
    #define     ROWADDR2        0xC0
    #define     ROWADDR3        0x90
    #define     ROWADDR4        0xD0
    #define     COLUMNSIZE      16
#else
    #error LCD not selected
#endif


/******************************************************************************/
/*                       LCD Interface Mode                                   */
/******************************************************************************/

#define     FOUR_BIT        ENABLE  // Specify the type of interface
#define     EIGHT_BIT       DISABLE

/******************************************************************************/
/*                       LCD Interface Mode validation                        */
/******************************************************************************/
#if (FOUR_BIT == EIGHT_BIT)
    #error Interface mode not correctly defined
#endif

#if FOUR_BIT
    #define LCD_4BIT
    #define     LCD_DATA_START_PIN  4    // Specify the starting pin number of port
    #if (LCD_DATA_START_PIN > 4)
        #error Datalines not correctly defined
    #endif
#elif EIGHT_BIT
    #define LCD_8BIT
    #define     LCD_DATA_START_PIN  0    // Specify the starting pin number of port
    #if (LCD_DATA_START_PIN > 8)
        #error Datalines not correctly defined
    #endif
#else
    #error Interface Mode not defined
#endif

/**
  * @}
  */


/** @defgroup STM32F4_ASK25_LCD_Exported_Functions
  * @{
  */
/* LCD Functions */
void ASK25_LCD_Init (AddrCount_e AddrCount, DispShift_e DispShift);
void ASK25_LCD_Write_Command (uint8_t Command);
void ASK25_LCD_Write_Data (uint8_t Character);
void ASK25_LCD_Set_Cursor (uint16_t LineNum, uint16_t Position, CursorType_e CursorType);
void ASK25_LCD_Display_Character (uint8_t Character, uint16_t LineNum, uint16_t Position, CursorType_e CursorType);
void ASK25_LCD_Display_String (uint8_t *String, uint16_t LineNum, uint16_t Position, CursorType_e CursorType);
void ASK25_LCD_CGRAM_CharGen (uint8_t loc,uint8_t *p);
//void ASK25_LCD_Display_Decimal (uint16_t VarData, uint16_t Row, uint16_t Col);
//void ASK25_LCD_Display_Hex (uint16_t VarData, uint16_t Row, uint16_t Col);


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

#endif /* __STM32F4_ASK25_LCD_H */

/****************************************END OF FILE******************************************/
