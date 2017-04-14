/**
  ******************************************************************************
  * @file    stm32f4_discovery_lcd.h
  * @author  Dwijay.edutech leaning solutions
  * @version V1.0.0
  * @date    28-April-2015
  * @brief   This file contains all the functions prototypes for the
  *          stm32f4_discovery_lcd.c driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_LCD_H
#define __STM32F4_DISCOVERY_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "fonts.h"
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_LCD
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_LCD_Exported_Types
  * @{
  */
typedef enum
{
 LCD_OK = 0,
 LCD_ERROR = 1,
 LCD_TIMEOUT = 2
}LCD_StatusTypeDef;

typedef struct
{
 uint32_t TextColor;
 uint32_t BackColor;
 sFONT    *pFont;
}LCD_DrawPropTypeDef;

typedef enum
{
  CENTER_MODE             = 0x01,    /* center mode */
  RIGHT_MODE              = 0x02,    /* right mode  */
  LEFT_MODE               = 0x03,    /* left mode   */
}Text_AlignModeTypdef;

typedef struct
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;

extern LCD_DrawPropTypeDef DrawProp;

/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_LCD_Exported_Constants
  * @{
  */

/* Note: LCD /CS is NE1 - Bank 1 of NOR/SRAM Bank 1~4 */
#define  LCD_BASE_DATA               ((uint32_t)(0x60000000|0x00100000))
#define  LCD_BASE_ADDR               ((uint32_t)(0x60000000|0x00000000))
#define  LCD_CMD                     (*(uint16_t *)LCD_BASE_ADDR)
#define  LCD_DATA                    (*(uint16_t *)LCD_BASE_DATA)

/**
  * @brief Various internal SD2119 registers name labels
  */
#define SSD2119_DEVICE_CODE_READ_REG  0x00
#define SSD2119_OSC_START_REG         0x00
#define SSD2119_OUTPUT_CTRL_REG       0x01
#define SSD2119_LCD_DRIVE_AC_CTRL_REG 0x02
#define SSD2119_PWR_CTRL_1_REG        0x03
#define SSD2119_DISPLAY_CTRL_REG      0x07
#define SSD2119_FRAME_CYCLE_CTRL_REG  0x0B
#define SSD2119_PWR_CTRL_2_REG        0x0C
#define SSD2119_PWR_CTRL_3_REG        0x0D
#define SSD2119_PWR_CTRL_4_REG        0x0E
#define SSD2119_GATE_SCAN_START_REG   0x0F
#define SSD2119_SLEEP_MODE_1_REG      0x10
#define SSD2119_ENTRY_MODE_REG        0x11
#define SSD2119_SLEEP_MODE_2_REG      0x12
#define SSD2119_GEN_IF_CTRL_REG       0x15
#define SSD2119_PWR_CTRL_5_REG        0x1E
#define SSD2119_RAM_DATA_REG          0x22
#define SSD2119_FRAME_FREQ_REG        0x25
#define SSD2119_ANALOG_SET_REG        0x26
#define SSD2119_VCOM_OTP_1_REG        0x28
#define SSD2119_VCOM_OTP_2_REG        0x29
#define SSD2119_GAMMA_CTRL_1_REG      0x30
#define SSD2119_GAMMA_CTRL_2_REG      0x31
#define SSD2119_GAMMA_CTRL_3_REG      0x32
#define SSD2119_GAMMA_CTRL_4_REG      0x33
#define SSD2119_GAMMA_CTRL_5_REG      0x34
#define SSD2119_GAMMA_CTRL_6_REG      0x35
#define SSD2119_GAMMA_CTRL_7_REG      0x36
#define SSD2119_GAMMA_CTRL_8_REG      0x37
#define SSD2119_GAMMA_CTRL_9_REG      0x3A
#define SSD2119_GAMMA_CTRL_10_REG     0x3B
#define SSD2119_V_RAM_POS_REG         0x44
#define SSD2119_H_RAM_START_REG       0x45
#define SSD2119_H_RAM_END_REG         0x46
#define SSD2119_X_RAM_ADDR_REG        0x4E
#define SSD2119_Y_RAM_ADDR_REG        0x4F

#define ENTRY_MODE_DEFAULT 0x6830
#define ENTRY_MODE_BMP     0x6810
#define ENTRY_MODE_HINC    0x6830
#define ENTRY_MODE_VINC    0x6838

// According to STM324xG Eval EmWin
//#define ENTRY_MODE_DEFAULT 0x6818
//#define ENTRY_MODE_BMP     0x6808
//#define ENTRY_MODE_HINC    0x6818
//#define ENTRY_MODE_VINC    0x6810

#define MAKE_ENTRY_MODE(x) ((ENTRY_MODE_DEFAULT & 0xFF00) | (x))

/**
  * @brief The dimensions of the LCD panel.
  */
#define LCD_VERTICAL_MAX   240
#define LCD_HORIZONTAL_MAX 320

/**
  * @brief Various definitions controlling coordinate space mapping and drawing
           direction in the four supported orientations.
  */
#define PORTRAIT

#ifdef PORTRAIT
#define HORIZ_DIRECTION 0x28
#define VERT_DIRECTION 0x20
#define MAPPED_X(x, y) (319 - (y))
#define MAPPED_Y(x, y) (x)
#endif
#ifdef LANDSCAPE
#define HORIZ_DIRECTION 0x00
#define VERT_DIRECTION  0x08
#define MAPPED_X(x, y) (319 - (x))
#define MAPPED_Y(x, y) (239 - (y))
#endif
#ifdef PORTRAIT_FLIP
#define HORIZ_DIRECTION 0x18
#define VERT_DIRECTION 0x10
#define MAPPED_X(x, y) (y)
#define MAPPED_Y(x, y) (239 - (x))
#endif
#ifdef LANDSCAPE_FLIP
#define HORIZ_DIRECTION 0x30
#define VERT_DIRECTION  0x38
#define MAPPED_X(x, y) (x)
#define MAPPED_Y(x, y) (y)
#endif

/**
  * @brief  LCD color
  */

#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_BLUE2         0x051F
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_CYAN2         0x7FFF
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_LIGHTBLUE     0x841F
#define LCD_COLOR_LIGHTGREEN    0x87F0
#define LCD_COLOR_LIGHTRED      0xFC10
#define LCD_COLOR_LIGHTCYAN     0x87FF
#define LCD_COLOR_LIGHTMAGENTA  0xFC1F
#define LCD_COLOR_LIGHTYELLOW   0xFFF0
#define LCD_COLOR_DARKBLUE      0x0010
#define LCD_COLOR_DARKGREEN     0x0400
#define LCD_COLOR_DARKRED       0x8000
#define LCD_COLOR_DARKCYAN      0x0410
#define LCD_COLOR_DARKMAGENTA   0x8010
#define LCD_COLOR_DARKYELLOW    0x8400
#define LCD_COLOR_WHITE         0xFFFF
#define LCD_COLOR_LIGHTGRAY     0xD69A
#define LCD_COLOR_GRAY          0x8410
#define LCD_COLOR_GREY2         0xF7DE
#define LCD_COLOR_DARKGRAY      0x4208
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_BROWN         0xA145
#define LCD_COLOR_ORANGE        0xFD20

#define White		          LCD_COLOR_WHITE
#define Black		          LCD_COLOR_BLACK
#define Red		            LCD_COLOR_RED
#define Blue		          LCD_COLOR_BLUE
#define Green		          LCD_COLOR_GREEN
#define Cyan              LCD_COLOR_CYAN

/**
  * @brief  LCD Lines depending on the chosen fonts.
  */
#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19)
#define LCD_LINE_20              LINE(20)
#define LCD_LINE_21              LINE(21)
#define LCD_LINE_22              LINE(22)
#define LCD_LINE_23              LINE(23)
#define LCD_LINE_24              LINE(24)
#define LCD_LINE_25              LINE(25)
#define LCD_LINE_26              LINE(26)
#define LCD_LINE_27              LINE(27)
#define LCD_LINE_28              LINE(28)
#define LCD_LINE_29              LINE(29)

#define Line0                    LCD_LINE_0
#define Line1                    LCD_LINE_1
#define Line2                    LCD_LINE_2
#define Line3                    LCD_LINE_3
#define Line4                    LCD_LINE_4
#define Line5                    LCD_LINE_5
#define Line6                    LCD_LINE_6
#define Line7                    LCD_LINE_7
#define Line8                    LCD_LINE_8
#define Line9                    LCD_LINE_9

/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT         Font24

/**
  * @brief  LCD Direction
  */
#define LCD_DIR_HORIZONTAL       0x0000
#define LCD_DIR_VERTICAL         0x0001

/**
  * @brief  LCD Size (Width and Height)
  */
#define LCD_PIXEL_WIDTH          320
#define LCD_PIXEL_HEIGHT         240

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LCD_Exported_Macros
  * @{
  */
#define ASSEMBLE_RGB(R ,G, B)    ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3))

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LCD_Exported_Functions
  * @{
  */

void BSP_LCD_DeInit(void);

uint8_t BSP_LCD_Init(void);
uint32_t BSP_LCD_GetXSize(void);
uint32_t BSP_LCD_GetYSize(void);

uint16_t BSP_LCD_GetTextColor(void);
uint16_t BSP_LCD_GetBackColor(void);
sFONT *BSP_LCD_GetFont(void);
void BSP_LCD_SetTextColor(uint16_t Color);
void BSP_LCD_SetBackColor(uint16_t Color);
void BSP_LCD_SetFont(sFONT *fonts);

void BSP_LCD_Clear(uint16_t Color);
void BSP_LCD_ClearStringLine(uint16_t Line);
void BSP_LCD_DisplayChar(uint16_t Column, uint16_t Line, uint8_t Ascii);
void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);

uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void BSP_LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);
void BSP_LCD_DrawMonoPict(const uint32_t *Pict); //extra
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount);
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void BSP_LCD_DisplayOn(void);
void BSP_LCD_DisplayOff(void);

/**
  * @}
  */


/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_LCD_H */
/**
  * @}
  */
/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
