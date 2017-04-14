/**
  ******************************************************************************
  * @file    stm32f4_discovery_lcd.c
  * @author  Dwijay.Edutech learning solutions
  * @version V1.0.0
  * @date    25-April-2015
  * @brief   This file includes the LCD driver for (LCD_SSD2119)
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_LCD
  * @brief This file includes the LCD driver for (LCD_SSD2119)
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_LCD_Private_TypesDef
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_LCD_Private Define
  * @{
  */
#define LCD_RST_PIN                  (GPIO_PIN_3)
#define LCD_RST_PORT                 (GPIOD)

#define LCD_PWM_PIN                  (GPIO_PIN_13)
#define LCD_PWM_PORT                 (GPIOD)

#define MAX_POLY_CORNERS             200
#define POLY_Y(Z)                    ((int32_t)((Points + Z)->Y))
#define POLY_X(Z)                    ((int32_t)((Points + Z)->X))
//For EmWin
//#define POLY_Y(Z)                    ((int32_t)((Points + Z)->X))
//#define POLY_X(Z)                    ((int32_t)((Points + Z)->Y))

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LCD_Private_Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LCD_Private_Variables
  * @{
  */

LCD_DrawPropTypeDef DrawProp;
SRAM_HandleTypeDef hsram1_ssd2119;

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LCD_Private_FunctionPrototypes
  * @{
  */
static void LCD_CtrlLinesConfig(void);
static void LCD_FSMCConfig(void);

static void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
static uint16_t LCD_ReadReg(uint8_t LCD_Reg);
static void LCD_WriteRAM_Prepare(void);
static void LCD_WriteRAM(uint16_t RGB_Code);
static uint16_t LCD_ReadRAM(void);
static void LCD_DisplayOff(void);
static void LCD_DisplayOn(void);


static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
static void SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);
static void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
static void LCD_WriteBMP(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
static void LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);
/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_LCD_Private_Functions
  * @{
  */

/**
  * @brief  Configures LCD Control lines (FSMC Pins) in alternate function mode.
  * @param  None
  * @retval None
  */
static void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOB, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  __GPIOH_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

/*-- GPIO Configuration ------------------------------------------------------*/
  /** FSMC GPIO Configuration
  PE3   ------> FSMC_A19
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  /* SRAM Data lines,  NOE and NWE configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* LCD RST configuration */
  GPIO_InitStruct.Pin = LCD_RST_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(LCD_RST_PORT, &GPIO_InitStruct);

  /* LCD PWM configuration */
  GPIO_InitStruct.Pin = LCD_PWM_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(LCD_PWM_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(LCD_PWM_PORT, LCD_PWM_PIN, GPIO_PIN_SET);
}


/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
static void LCD_FSMCConfig(void)
{
    FSMC_NORSRAM_TimingTypeDef Timing;

  /* Enable FSMC clock */
    __FSMC_CLK_ENABLE();

  /*-- FSMC Configuration ------------------------------------------------------*/
  /*----------------------- SRAM Bank 1 ----------------------------------------*/
  /* FSMC_NORSRAM_BANK1 configuration */
  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Memory Type = SRAM
        - NE1 chip select/ BANK1 (NOR type)
        - LCD Register Select A19
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1_ssd2119.Instance = FSMC_NORSRAM_DEVICE;
  hsram1_ssd2119.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1_ssd2119.Init */
  hsram1_ssd2119.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1_ssd2119.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1_ssd2119.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1_ssd2119.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1_ssd2119.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1_ssd2119.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1_ssd2119.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1_ssd2119.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1_ssd2119.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1_ssd2119.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1_ssd2119.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1_ssd2119.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1_ssd2119.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 1;
  Timing.AddressHoldTime = 1;
  Timing.DataSetupTime = 9;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 0;
  Timing.DataLatency = 0;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  HAL_SRAM_Init(&hsram1_ssd2119, &Timing, NULL);
}


/**
  * @brief  Writes to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
static void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD_CMD = LCD_Reg;
  /* Write 16-bit Reg */
  LCD_DATA = LCD_RegValue;
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD Register Value.
  */
static uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_CMD = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD_DATA);
}

/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
static void LCD_WriteRAM_Prepare(void)
{
  LCD_CMD = SSD2119_RAM_DATA_REG;
}

/**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
static void LCD_WriteRAM(uint16_t RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  LCD_DATA = RGB_Code;
}

/**
  * @brief  Reads the LCD RAM.
  * @param  None
  * @retval LCD RAM Value.
  */
static uint16_t LCD_ReadRAM(void)
{
  /* Read 16-bit Reg */
  return LCD_DATA;
}


/**
LCD_DisplayOff
  */
static void LCD_DisplayOff(void)
{
  LCD_WriteReg(SSD2119_PWR_CTRL_1_REG, 0x0000);
  LCD_WriteReg(SSD2119_PWR_CTRL_2_REG, 0x0000);
  LCD_WriteReg(SSD2119_PWR_CTRL_3_REG, 0x0000);
  LCD_WriteReg(SSD2119_PWR_CTRL_4_REG, 0x0000);
  LCD_WriteReg(SSD2119_PWR_CTRL_5_REG, 0x0000);

  /* Disable the display */
  LCD_WriteReg(SSD2119_DISPLAY_CTRL_REG, 0x0);
}


/**
LCD_DisplayOn
  */
static void LCD_DisplayOn(void)
{
  LCD_WriteReg(SSD2119_PWR_CTRL_1_REG, 0x0000);
  LCD_WriteReg(SSD2119_PWR_CTRL_2_REG, 0x0000);
  LCD_WriteReg(SSD2119_PWR_CTRL_3_REG, 0x0000);
  LCD_WriteReg(SSD2119_PWR_CTRL_4_REG, 0x0000);

  LCD_WriteReg(SSD2119_PWR_CTRL_1_REG, 0x6A38);
  /* Set VCIX2 voltage to 6.1V.*/
  LCD_WriteReg(SSD2119_PWR_CTRL_2_REG, 0x0005);

  /* Configure Vlcd63 and VCOMl */
  LCD_WriteReg(SSD2119_PWR_CTRL_3_REG, 0x000A);
  LCD_WriteReg(SSD2119_PWR_CTRL_4_REG, 0x2E00);

  LCD_WriteReg(SSD2119_PWR_CTRL_5_REG, 0x00B2);

  /* Enable the display */
  LCD_WriteReg(SSD2119_DISPLAY_CTRL_REG, 0x0033);
}


/**
  * @brief  Disables LCD Window mode.
  * @param  None
  * @retval None
  */
static void LCD_WindowModeDisable(void)
{
#if 0
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  LCD_WriteReg(LCD_REG_3, 0x1018);
#endif
}


/**
  * @brief  Sets display window.
  * @param  LayerIndex: layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
static void SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint32_t value = 0;

  LCD_WriteReg(SSD2119_H_RAM_START_REG, Xpos);

  if ((Xpos+Width) >= LCD_PIXEL_WIDTH) {
    LCD_WriteReg(SSD2119_H_RAM_END_REG, LCD_PIXEL_WIDTH-1);
  } else {
    LCD_WriteReg(SSD2119_H_RAM_END_REG, Xpos+Width);
  }

  if ((Ypos+Height) >= LCD_PIXEL_HEIGHT) {
    value = (LCD_PIXEL_HEIGHT-1) << 8;
  } else {
    value = (Ypos+Height) << 8;
  }
  value |= Xpos;
  LCD_WriteReg(SSD2119_V_RAM_POS_REG, value);
  LCD_SetCursor(Xpos, Ypos);

//  /********************************************************************//**
//  * @brief for stm324fxG Eval EmWin
//  **********************************************************************/
//  /* Horizontal GRAM Start Address */
//  LCD_WriteReg(SSD2119_H_RAM_START_REG, (Ypos));
//  /* Horizontal GRAM End Address */
//  LCD_WriteReg(SSD2119_H_RAM_END_REG, (Ypos + Height - 1));
//
//  value = (LCD_PIXEL_WIDTH - Xpos - 1) << 8;    // End addr
//  value |= (LCD_PIXEL_WIDTH - Xpos - Width);  // Begin Addr
//  /* Vertical GRAM Start and End Address */
//  LCD_WriteReg(SSD2119_V_RAM_POS_REG, value);
}


/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
static void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  /* Set the X address of the display cursor.*/
  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, Xpos);

  /* Set the Y address of the display cursor.*/
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, Ypos);
}


/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: Line where to display the character shape
  * @param  Ypos: Start column address
  * @param  c: Pointer to the character data
  * @retval None
  */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *pchar;
  uint32_t line;

  height = DrawProp.pFont->Height;
  width  = DrawProp.pFont->Width;

  offset =  8 *((width + 7)/8) -  width ;

  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)c + (width + 7)/8 * i);

    switch(((width + 7)/8))
    {
    case 1:
      line =  pchar[0];
      break;

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;

    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;
    }

    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1)))
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.TextColor);
      }
      else
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.BackColor);
      }
    }
    Ypos++;
  }
}


/**
  * @brief  Fills a triangle (between 3 points).
  * @param  Points: Pointer to the points array
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @param  x3: Point 3 X position
  * @param  y3: Point 3 Y position
  * @retval None
  */
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;

  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */

  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawLine(x, y, x3, y3);

    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}


/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
static void LCD_WriteBMP(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  /* Read bitmap size */
  size = *(__IO uint16_t *) (pbmp + 2);
  size |= (*(__IO uint16_t *) (pbmp + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (pbmp + 10);
  index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;
  size = (size - index)/2;
  pbmp += index;
  /* Set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : decrement, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_BMP);

  LCD_SetCursor(Xpos, Ypos);

  LCD_WriteRAM_Prepare();

  for(index = 0; index < size; index++)
  {
    LCD_WriteRAM(*(__IO uint16_t *)pbmp);
    pbmp += 2;
  }

  /* Set GRAM write direction and BGR = 1 */
  /* I/D = 01 (Horizontal : increment, Vertical : decrement) */
  /* AM = 1 (address is updated in vertical writing direction) */
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT);
}


/**
  * @brief  Displays picture.
  * @param  pdata: picture address.
  * @param  Xpos: Image X position in the LCD
  * @param  Ypos: Image Y position in the LCD
  * @param  Xsize: Image X size in the LCD
  * @param  Ysize: Image Y size in the LCD
  * @retval None
  */
static void LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  uint32_t index = 0, size = 0;

  size = (Xsize * Ysize);

  /* Set Cursor */
  LCD_SetCursor(Xpos, Ypos);

  /* Prepare to write GRAM */
  LCD_WriteRAM_Prepare();

  for(index = 0; index < size; index++)
  {
    /* Write 16-bit GRAM Reg */
    LCD_WriteRAM(*(__IO uint16_t *)pdata);
    pdata += 2;
  }
}


/**
  * @}
  */


/** @defgroup STM32F4_DISCOVERY_LCD_Functions
  * @{
  */

/**
  * @brief  LCD Init.
  * @retval None
  */
uint8_t BSP_LCD_Init(void)
{
  unsigned long ulCount;

  /* Configure the LCD Control pins */
  LCD_CtrlLinesConfig();

  /* Configure the FSMC Parallel interface */
  LCD_FSMCConfig();

  HAL_Delay(5);

  /* Reset LCD */
  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);

  /*
  SSD2119Init(void)
  */
  /* Enter sleep mode (if we are not already there).*/
  LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0001);

  /* Set initial power parameters. */
  LCD_WriteReg(SSD2119_PWR_CTRL_5_REG, 0x00B2);
  LCD_WriteReg(SSD2119_VCOM_OTP_1_REG, 0x0006);

  /* Start the oscillator.*/
  LCD_WriteReg(SSD2119_OSC_START_REG, 0x0001);

  /* Set pixel format and basic display orientation (scanning direction).*/
  LCD_WriteReg(SSD2119_OUTPUT_CTRL_REG, 0x30EF); //RGB
//  LCD_WriteReg(SSD2119_OUTPUT_CTRL_REG, 0x38EF); //BGR
  LCD_WriteReg(SSD2119_LCD_DRIVE_AC_CTRL_REG, 0x0600);

  /* Exit sleep mode.*/
  LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0000);
  HAL_Delay(5);

  /* Configure pixel color format and MCU interface parameters.*/
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT);

  /* Set analog parameters */
  LCD_WriteReg(SSD2119_SLEEP_MODE_2_REG, 0x0999);
  LCD_WriteReg(SSD2119_ANALOG_SET_REG, 0x3800);

  /* Enable the display */
  LCD_WriteReg(SSD2119_DISPLAY_CTRL_REG, 0x0033);

  /* Set VCIX2 voltage to 6.1V.*/
  LCD_WriteReg(SSD2119_PWR_CTRL_2_REG, 0x0005);

  /* Configure gamma correction.*/
  LCD_WriteReg(SSD2119_GAMMA_CTRL_1_REG, 0x0000);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_2_REG, 0x0303);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_3_REG, 0x0407);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_4_REG, 0x0301);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_5_REG, 0x0301);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_6_REG, 0x0403);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_7_REG, 0x0707);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_8_REG, 0x0400);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_9_REG, 0x0a00);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_10_REG, 0x1000);

  /* Configure Vlcd63 and VCOMl */
  LCD_WriteReg(SSD2119_PWR_CTRL_3_REG, 0x000A);
  LCD_WriteReg(SSD2119_PWR_CTRL_4_REG, 0x2E00);

  /* Set the display size and ensure that the GRAM window is set to allow
     access to the full display buffer.*/
  LCD_WriteReg(SSD2119_V_RAM_POS_REG, (LCD_PIXEL_HEIGHT-1) << 8);
  LCD_WriteReg(SSD2119_H_RAM_START_REG, 0x0000);
  LCD_WriteReg(SSD2119_H_RAM_END_REG, LCD_PIXEL_WIDTH-1);

  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, 0x00);
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, 0x00);

  /* clear the lcd  */
  LCD_WriteReg(SSD2119_RAM_DATA_REG, 0x0000);
  for(ulCount = 0; ulCount < (LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); ulCount++)
  {
    LCD_WriteRAM(0x0000);
  }

  /* Default value for draw propriety */
  DrawProp.BackColor = 0xFFFF;
  DrawProp.pFont     = &Font24;
  DrawProp.TextColor = 0x0000;

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  return 0;
}


/**
  * @brief  LCD Default FSMC Init
  * @param  None
  * @retval None
  */
void BSP_LCD_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< LCD Display Off */
  LCD_DisplayOff();

  /* BANK 3 (of NOR/SRAM Bank 1~4) is disabled */
  hsram1_ssd2119.Init.NSBank = FSMC_NORSRAM_BANK3;
  __FMC_NORSRAM_ENABLE(hsram1_ssd2119.Instance, hsram1_ssd2119.Init.NSBank);

  /*!< LCD_SPI DeInit */
  HAL_SRAM_DeInit(&hsram1_ssd2119);

  /*-- GPIO Configuration ------------------------------------------------------*/
  /** FSMC GPIO Configuration
  PE3   ------> FSMC_A19
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  /* SRAM Data lines configuration */

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_7);
}


/**
  * @brief  Gets the LCD X size.
  * @param  None
  * @retval Used LCD X size
  */
uint32_t BSP_LCD_GetXSize(void)
{
  return((uint16_t)LCD_PIXEL_WIDTH);
}

/**
  * @brief  Gets the LCD Y size.
  * @param  None
  * @retval Used LCD Y size
  */
uint32_t BSP_LCD_GetYSize(void)
{
  return((uint16_t)LCD_PIXEL_HEIGHT);
}


/**
  * @brief  Gets the LCD text color.
  * @param  None
  * @retval Used text color.
  */
uint16_t BSP_LCD_GetTextColor(void)
{
  return DrawProp.TextColor;
}

/**
  * @brief  Gets the LCD background color.
  * @param  None
  * @retval Used background color
  */
uint16_t BSP_LCD_GetBackColor(void)
{
  return DrawProp.BackColor;
}

/**
  * @brief  Gets the LCD text font.
  * @param  None
  * @retval Used font
  */
sFONT *BSP_LCD_GetFont(void)
{
  return DrawProp.pFont;
}


/**
  * @brief  Sets the LCD text color.
  * @param  Color: Text color code RGB(5-6-5)
  * @retval None
  */
void BSP_LCD_SetTextColor(uint16_t Color)
{
  DrawProp.TextColor = Color;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Color: Background color code RGB(5-6-5)
  * @retval None
  */
void BSP_LCD_SetBackColor(uint16_t Color)
{
  DrawProp.BackColor = Color;
}

/**
  * @brief  Sets the LCD text font.
  * @param  fonts: Font to be used
  * @retval None
  */
void BSP_LCD_SetFont(sFONT *fonts)
{
  DrawProp.pFont = fonts;
}


/**
  * @brief  Clears the hole LCD.
  * @param  Color: Color of the background
  * @retval None
  */
void BSP_LCD_Clear(uint16_t Color)
{
  uint32_t counter = 0;
  uint32_t color_backup = DrawProp.TextColor;
  DrawProp.TextColor = Color;

  for(counter = 0; counter < BSP_LCD_GetYSize(); counter++)
  {
    BSP_LCD_DrawHLine(0, counter, BSP_LCD_GetXSize());
  }
  DrawProp.TextColor = color_backup;
  BSP_LCD_SetTextColor(DrawProp.TextColor);
}


/**
  * @brief  Clears the selected line.
  * @param  Line: Line to be cleared
  *          This parameter can be one of the following values:
  *            @arg  0..9: if the Current fonts is Font16x24
  *            @arg  0..19: if the Current fonts is Font12x12 or Font8x12
  *            @arg  0..29: if the Current fonts is Font8x8
  * @retval None
  */
void BSP_LCD_ClearStringLine(uint16_t Line)
{
  uint32_t color_backup = DrawProp.TextColor;
  DrawProp.TextColor = DrawProp.BackColor;;

  /* Draw a rectangle with background color */
  BSP_LCD_FillRect(0, (Line * DrawProp.pFont->Height), BSP_LCD_GetXSize(), DrawProp.pFont->Height);

  DrawProp.TextColor = color_backup;
  BSP_LCD_SetTextColor(DrawProp.TextColor);
}


/**
  * @brief  Displays one character.
  * @param  Xpos: Start column address
  * @param  Ypos: Line where to display the character shape.
  * @param  Ascii: Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
  * @retval None
  */
void BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  DrawChar(Xpos, Ypos, &DrawProp.pFont->table[(Ascii-' ') *\
    DrawProp.pFont->Height * ((DrawProp.pFont->Width + 7) / 8)]);
}


/**
  * @brief  Displays characters on the LCD.
  * @param  Xpos: X position (in pixel)
  * @param  Ypos: Y position (in pixel)
  * @param  Text: Pointer to string to display on LCD
  * @param  Mode: Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE
  * @retval None
  */
void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
  uint16_t refcolumn = 1, i = 0;
  uint32_t size = 0, xsize = 0;
  uint8_t  *ptr = Text;

  /* Get the text size */
  while (*ptr++) size ++ ;

  /* Characters number per line */
  xsize = (BSP_LCD_GetXSize()/DrawProp.pFont->Width);

  switch (Mode)
  {
  case CENTER_MODE:
    {
      refcolumn = Xpos + ((xsize - size)* DrawProp.pFont->Width) / 2;
      break;
    }
  case LEFT_MODE:
    {
      refcolumn = Xpos;
      break;
    }
  case RIGHT_MODE:
    {
      refcolumn =  - Xpos + ((xsize - size)*DrawProp.pFont->Width);
      break;
    }
  default:
    {
      refcolumn = Xpos;
      break;
    }
  }

  /* Send the string character by character on lCD */
  while ((*Text != 0) & (((BSP_LCD_GetXSize() - (i*DrawProp.pFont->Width)) & 0xFFFF) >= DrawProp.pFont->Width))
  {
    /* Display one character on LCD */
    BSP_LCD_DisplayChar(refcolumn, Ypos, *Text);
    /* Decrement the column position by 16 */
    refcolumn += DrawProp.pFont->Width;
    /* Point on the next character */
    Text++;
    i++;
  }
}


/**
  * @brief  Displays a character on the LCD.
  * @param  Line: Line where to display the character shape
  *          This parameter can be one of the following values:
  *            @arg  0..9: if the Current fonts is Font16x24
  *            @arg  0..19: if the Current fonts is Font12x12 or Font8x12
  *            @arg  0..29: if the Current fonts is Font8x8
  * @param  ptr: Pointer to string to display on LCD
  * @retval None
  */
void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr)
{
  BSP_LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}


/**
  * @brief  Reads an LCD pixel.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @retval RGB pixel color
  */
uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  uint16_t ret=0;

  LCD_SetCursor(Xpos, Ypos);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  ret = LCD_ReadRAM();
  return ret;
}


/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  RGB_Code: Pixel color in RGB mode (5-6-5)
  * @retval None
  */
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code)
{
  LCD_SetCursor(Xpos, Ypos);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  LCD_WriteRAM(RGB_Code);
}


/**
  * @brief  Draws an horizontal line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t i = 0;

  LCD_SetCursor(Xpos, Ypos);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

  for(i = 0; i < Length; i++)
  {
    /* Write 16-bit GRAM Reg */
    LCD_WriteRAM(DrawProp.TextColor);
  }
}


/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t i = 0;

  /* AM=1 (address is updated in vertical writing direction) */
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_VINC);

  LCD_SetCursor(Xpos, Ypos);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

  for(i = 0; i < Length; i++)
  {
    /* Write 16-bit GRAM Reg */
    LCD_WriteRAM(DrawProp.TextColor);
  }

  /* AM=0 (address is updated in Horizontal writing direction) */
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_HINC);
}


/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;

  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */

  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawPixel(x, y, DrawProp.TextColor);  /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}


/**
  * @brief  Draws a rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width
  * @param  Height: Rectangle height
  * @retval None
  */
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Draw horizontal lines */
  BSP_LCD_DrawHLine(Xpos, Ypos, Width);
  BSP_LCD_DrawHLine(Xpos, (Ypos+ Height), Width);

  /* Draw vertical lines */
  BSP_LCD_DrawVLine(Xpos, Ypos, Height);
  BSP_LCD_DrawVLine((Xpos + Width), Ypos, Height);
}


/**
  * @brief  Draws a circle.
  * @param  Xpos: X position of center of circle
  * @param  Ypos: Y position of center of circle
  * @param  Radius: Circle radius
  * @retval None
  */
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;       /* Decision Variable */
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;

  while (CurX <= CurY)
  {
    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos - CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos - CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos - CurX), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos - CurX), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos + CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos + CurY), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos + CurX), DrawProp.TextColor);

    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos + CurX), DrawProp.TextColor);

    /* Initialize the font */
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}


/**
  * @brief  Draws an poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;

  if(PointCount < 2)
  {
    return;
  }

  //Closed Polygon
//  BSP_LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y); // always start from (0,0)

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    BSP_LCD_DrawLine(X, Y, Points->X, Points->Y);
  }
}


/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  K = (float)(rad2/rad1);

  do {
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);

    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}


/**
  * @brief  Draws a bitmap picture (16 bpp).
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pbmp: Pointer to Bmp picture address.
  * @retval None
  */
void BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t height = 0;
  uint32_t width  = 0;

  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;

  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16;

  SetDisplayWindow(Xpos, Ypos, width, height);

  LCD_WriteBMP(Xpos, Ypos, pbmp);

  SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}


/**
  * @brief  Draws RGB Image (16 bpp).
  * @param  Xpos:  X position in the LCD
  * @param  Ypos:  Y position in the LCD
  * @param  Xsize: X size in the LCD
  * @param  Ysize: Y size in the LCD
  * @param  pdata: Pointer to the RGB Image address.
  * @retval None
  */
void BSP_LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata)
{
  SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);

  LCD_DrawRGBImage(Xpos, Ypos, Xsize, Ysize, pdata);

  SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}


/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void BSP_LCD_DrawMonoPict(const uint32_t *Pict)
{
  uint32_t index = 0, i = 0;
  LCD_SetCursor(0, (LCD_PIXEL_WIDTH - 1));
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index = 0; index < 2400; index++)
  {
    for(i = 0; i < 32; i++)
    {
      if((Pict[index] & (1 << i)) == 0x00)
      {
        LCD_WriteRAM(DrawProp.BackColor);
      }
      else
      {
        LCD_WriteRAM(DrawProp.TextColor);
      }
    }
  }
}


/**
  * @brief  Draws a full rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width
  * @param  Height: Rectangle height
  * @retval None
  */
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  BSP_LCD_SetTextColor(DrawProp.TextColor);
  do
  {
    BSP_LCD_DrawHLine(Xpos, Ypos++, Width);
  }
  while(Height--);
}


/**
  * @brief  Draws a full circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;        /* Decision Variable */
  uint32_t  CurX;    /* Current X Value */
  uint32_t  CurY;    /* Current Y Value */

  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;

  BSP_LCD_SetTextColor(DrawProp.TextColor);

  while (CurX <= CurY)
  {
    if(CurY > 0)
    {
      BSP_LCD_DrawHLine(Xpos - CurY, Ypos + CurX, 2*CurY);
      BSP_LCD_DrawHLine(Xpos - CurY, Ypos - CurX, 2*CurY);
    }

    if(CurX > 0)
    {
      BSP_LCD_DrawHLine(Xpos - CurX, Ypos - CurY, 2*CurX);
      BSP_LCD_DrawHLine(Xpos - CurX, Ypos + CurY, 2*CurX);
    }
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  BSP_LCD_SetTextColor(DrawProp.TextColor);
  BSP_LCD_DrawCircle(Xpos, Ypos, Radius);
}


/**
  * @brief  Draws a full poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

  for(counter = 1; counter < PointCount; counter++)
  {
    pixelX = POLY_X(counter);
    if(pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if(pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }

    pixelY = POLY_Y(counter);
    if(pixelY < IMAGE_TOP)
    {
      IMAGE_TOP = pixelY;
    }
    if(pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }

  if(PointCount < 2)
  {
    return;
  }

  X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
  Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;

  X_first = Points->X;
  Y_first = Points->Y;

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;

    FillTriangle(X, X2, X_center, Y, Y2, Y_center);
    FillTriangle(X, X_center, X2, Y, Y_center, Y2);
    FillTriangle(X_center, X2, X, Y_center, Y2, Y);
  }

  FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
  FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
  FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}


/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  K = (float)(rad2/rad1);

  do
  {
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1));
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1));

    e2 = err;
    if (e2 <= x)
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}


/**
  * @brief  Enables the display.
  * @param  None
  * @retval None
  */
void BSP_LCD_DisplayOn(void)
{
  LCD_DisplayOn();
}


/**
  * @brief  Disables the display.
  * @param  None
  * @retval None
  */
void BSP_LCD_DisplayOff(void)
{
  LCD_DisplayOff();
}




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

/**
  * @}
  */

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
