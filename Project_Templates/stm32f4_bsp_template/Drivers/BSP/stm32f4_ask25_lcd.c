/**
  ******************************************************************************
  * @file    stm32f4_ask25_lcd.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file provides set of firmware functions to manage LCD
  *          available on STM32F4-ASK25 Kit from Edutech.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_ask25_lcd.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25_LCD
  * @{
  */

/** @defgroup STM32F4_ASK25_LCD
  * @brief This file provides set of firmware functions to manage LCD
  *        available on STM32F4-ASK25 Kit from Edutech.
  * @{
  */


/** @defgroup STM32F4_ASK25_LCD_Private_FunctionPrototypes
  * @{
  */
void ASK25_LCD_Config (void);
void ASK25_LCD_Enable (void);
void ASK25_LCD_Display_Shift (ShiftDir_e Direction, CursorType_e CursorType);

/**
  * @}
  */

/** @defgroup STM32F4_ASK25_LCD_Private_Functions
  * @{
  */

/**
 * @brief Configure Pins of LCD
 * @retval None
 */
void ASK25_LCD_Config (void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_ALED Clock */

  __GPIOE_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  #ifdef LCD_4BIT
  {
    GPIO_InitStruct.Pin = _SBF(8,0xF0);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  }
  #endif
  #ifdef LCD_8BIT
  {
    /* Configure the GPIO_ALED pin */
    GPIO_InitStruct.Pin = _SBF(8,0xFF);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  }
  #endif
  GPIO_InitStruct.Pin = 0x32;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/**
 * @brief This function sends enable signal to LCD
 * @retval None
 */
void ASK25_LCD_Enable (void)
{
  uint16_t i;

  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);    // Set   EN
  #ifdef LCD_4BIT
    for(i=0;i<199;i++);         // 1 us
  #endif
  #ifdef LCD_8BIT
    for(i=0;i<504;i++);        // 3 us
  #endif
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);  // Clear   EN
  #ifdef LCD_4BIT
    for(i=0;i<199;i++);         // 1 us
  #endif
}


/**
 * @brief This function shifts whole display in desired direction
 * @param Direction   select direction LEFT/RIGHT
 * @param CursorType  select cursor ON/OFF/BLINK
 *    @arg Off   : 0
 *    @arg On    : 1
 *    @arg Blink : 2
 * @retval None
 */
void ASK25_LCD_Display_Shift (ShiftDir_e Direction, CursorType_e CursorType)
{
  if(CursorType == Off)
  {
    ASK25_LCD_Write_Command(CURSOR_OFF);
  }
  else if(CursorType == On)
  {
    ASK25_LCD_Write_Command(CURSOR_ON);
  }
  else if(CursorType == Blink)
  {
    ASK25_LCD_Write_Command(CURSOR_BLINK);
  }

  if(Direction == Left)
  {
    ASK25_LCD_Write_Command(LCD_SHIFT_LEFT);
  }
  else if(Direction == Right)
  {
    ASK25_LCD_Write_Command(LCD_SHIFT_RIGHT);
  }
}


/**
  * @}
  */

/** @defgroup STM32F4_ASK25_LCD_Functions
  * @{
  */

/**
 * @brief This function initializes LCD
 * @param AddrCount  select increment or decrement address counter
 *    @arg Dec : 0
 *    @arg Inc : 1
 * @param DispShift  select display shift on or off
 *    @arg DispShiftOff: 0
 *    @arg DispShiftOn : 1
 * @retval None
 */
void ASK25_LCD_Init (AddrCount_e AddrCount, DispShift_e DispShift)
{
  uint16_t i;
  ASK25_LCD_Config();                  // Configure Pins

  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);   // Clear RS
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);   // Clear RW
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);     // Set   EN
  for(i=0;i<2600;i++);           // 15 us

  ASK25_LCD_Write_Command(LCD_CLEAR);
  HAL_Delay(2);
  ASK25_LCD_Write_Command(LCD_CLEAR);
  HAL_Delay(4);

  #ifdef LCD_4BIT
  {
    ASK25_LCD_Write_Command(LCD_4BIT_2LINE);
    for(i=0;i<9000;i++);             // 50 us
  }
  #endif
  #ifdef LCD_8BIT
  {
    ASK25_LCD_Write_Command(LCD_8BIT_2LINE);
    HAL_Delay(1);
  }
  #endif

  ASK25_LCD_Write_Command(CURSOR_HOME);
  for(i=0;i<9000;i++);             // 50 us

  if(AddrCount == Dec)
  {
    if(DispShift == DispShiftOff)
    {
      ASK25_LCD_Write_Command(LCD_DEC_ADD_DISP_SHIFT_OFF);
    }
    else
    {
      ASK25_LCD_Write_Command(LCD_DEC_ADD_DISP_SHIFT_ON);
    }
  }
  else
  {
    if(DispShift == DispShiftOff)
    {
      ASK25_LCD_Write_Command(LCD_INC_ADD_DISP_SHIFT_OFF);
    }
    else
    {
      ASK25_LCD_Write_Command(LCD_INC_ADD_DISP_SHIFT_ON);
    }
  }
  ASK25_LCD_Write_Command(LCD_OFF);
  ASK25_LCD_Write_Command(LCD_ON);
  ASK25_LCD_Write_Command(CURSOR_OFF);
  HAL_Delay(50);
}


/**
 * @brief This function writes commands to the LCD
 * @param Command command to be written on LCD
 * @retval None
 */
void ASK25_LCD_Write_Command (uint8_t Command)
{
  uint16_t Mask;
  uint16_t LcdData = Command;
  LcdData = LcdData << LCD_DATA_START_PIN;
  Mask = (0x0F << (LCD_DATA_START_PIN));

  #ifdef LCD_4BIT
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);         // Clear RS
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);         // Clear RW
    HAL_GPIO_WritePin(GPIOE, _SBF(8, 0xF0), GPIO_PIN_RESET);   // Clear Old Value
    HAL_GPIO_WritePin(GPIOE, _SBF(8, ((LcdData >> 4) & Mask)), GPIO_PIN_SET);
    ASK25_LCD_Enable();

    HAL_GPIO_WritePin(GPIOE, _SBF(8, 0xF0), GPIO_PIN_RESET);   // Clear Old Value
    HAL_GPIO_WritePin(GPIOE, _SBF(8, (LcdData & Mask)), GPIO_PIN_SET);

    ASK25_LCD_Enable();
  }
  #endif
  #ifdef LCD_8BIT
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);         // Clear RS
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);         // Clear RW
    HAL_GPIO_WritePin(GPIOE, _SBF(8,0xFF), GPIO_PIN_RESET);   // Clear Old Value
    HAL_GPIO_WritePin(GPIOE, _SBF(8,Command), GPIO_PIN_SET);   // Send Command

    ASK25_LCD_Enable();
  }
  #endif
}


/**
 * @brief This function writes data to the LCD
 * @param Character data to be written on LCD
 * @retval None
 */
void ASK25_LCD_Write_Data (uint8_t Character)
{
   uint16_t Mask;
   uint16_t LcdData = Character;
   LcdData = LcdData << LCD_DATA_START_PIN;
   Mask = (0x0F << (LCD_DATA_START_PIN));

   ASK25_LCD_Write_Command(CURSOR_OFF); // Needed

   #ifdef LCD_4BIT
   {
     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);             // Set RS
     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);           // Clear RW
     HAL_GPIO_WritePin(GPIOE, _SBF(8, 0xF0), GPIO_PIN_RESET);    // Clear Old Value
     HAL_GPIO_WritePin(GPIOE, _SBF(8, ((LcdData >> 4) & Mask)), GPIO_PIN_SET);

     ASK25_LCD_Enable();

     HAL_GPIO_WritePin(GPIOE, _SBF(8, 0xF0), GPIO_PIN_RESET);    // Clear Old Value
     HAL_GPIO_WritePin(GPIOE, _SBF(8, (LcdData & Mask)), GPIO_PIN_SET);

     ASK25_LCD_Enable();
   }
   #endif
   #ifdef LCD_8BIT
   {
     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);             // Set RS
     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);           // Clear RW
     HAL_GPIO_WritePin(GPIOE, _SBF(8,0xFF), GPIO_PIN_RESET);     // Clear Old Value
     HAL_GPIO_WritePin(GPIOE, _SBF(8,Character), GPIO_PIN_SET); // Send Data

     ASK25_LCD_Enable();
   }
   #endif
}


/**
 * @brief This function sets coursor to desired Position
 * @param LineNum     enter line number
 * @param Position    enter the Position ie. column number
 * @param CursorType  select cursor ON/OFF/BLINK
 *    @arg Off   : 0
 *    @arg On    : 1
 *    @arg Blink : 2
 * @retval None
 */
void ASK25_LCD_Set_Cursor (uint16_t LineNum, uint16_t Position, CursorType_e CursorType)
{
  uint16_t LcdCmd=0;
  switch(LineNum)
  {
    case 1:
        LcdCmd = ROWADDR1 + (Position - 1);
        break;
    case 2:
        LcdCmd = ROWADDR2 + (Position - 1);
        break;
    case 3:
        LcdCmd = ROWADDR3 + (Position - 1);
        break;
    case 4:
        LcdCmd = ROWADDR4 + (Position - 1);
        break;
  }
  ASK25_LCD_Write_Command(LcdCmd);

  if(CursorType == Off)
  {
    ASK25_LCD_Write_Command(CURSOR_OFF);
  }
  else if(CursorType == On)
  {
    ASK25_LCD_Write_Command(CURSOR_ON);
  }
  else if(CursorType == Blink)
  {
    ASK25_LCD_Write_Command(CURSOR_BLINK);
  }
}


/**
 * @brief This function display Character to desired Position
 * @param Character   string to be displayed
 * @param LineNum     enter line number
 * @param Position    enter the Position ie. column number
 * @param CursorType  select cursor ON/OFF/BLINK
 *    @arg Off   : 0
 *    @arg On    : 1
 *    @arg Blink : 2
 * @retval None
 */
void ASK25_LCD_Display_Character (uint8_t Character, uint16_t LineNum, uint16_t Position, CursorType_e CursorType)
{
  ASK25_LCD_Set_Cursor(LineNum, Position, CursorType);
  ASK25_LCD_Write_Data(Character);
}


/**
 * @brief This function display string to desired Position
 * @param String      string to be displayed
 * @param LineNum     enter line number
 * @param Position    enter the Position ie. column number
 * @param CursorType  select cursor ON/OFF/BLINK
 *    @arg Off   : 0
 *    @arg On    : 1
 *    @arg Blink : 2
 * @retval None
 */
void ASK25_LCD_Display_String (uint8_t *String, uint16_t LineNum, uint16_t Position, CursorType_e CursorType)
{
  while (*String)
  {
    ASK25_LCD_Display_Character(*String++,LineNum,Position++,CursorType);

    if(Position-1 == COLUMNSIZE)
    {
      LineNum++;
      Position =1;
    }
  }
}


/**
 * @brief This function builds custom character
 * @param loc position
 * @param p   Custom Character array
 * @retval  None
 */
void ASK25_LCD_CGRAM_CharGen (uint8_t loc,uint8_t *p)
{
  uint8_t i;

  if(loc<8)                             // If valid
  {
    ASK25_LCD_Write_Command(0x40+(loc*8));  // Access to CGRAM
    for(i=0;i<8;i++)
    {
      ASK25_LCD_Write_Data(p[i]);         // Write Pattern
    }
  }
  ASK25_LCD_Write_Command(0x80);              // Shift to DDRAM location 0
}



void ASK25_LCD_Display_Decimal (uint32_t VarData, uint8_t Row, uint8_t Col)
{
  uint8_t i=0,Character[10]={"0123456789"};
  uint32_t width_dec[6]={1,10,100,1000,10000,100000};
  uint32_t data = VarData;
  uint8_t BaseValue=10;
  while(data)
  {
      data /=BaseValue;
      if(data>=1)
      {
          i++;
      }
  }
  uint32_t DivValue =  width_dec[i];
  while(DivValue)
  {
    ASK25_LCD_Display_Character(Character[VarData/DivValue],Row,Col++,Off);
    VarData%=DivValue;
    DivValue/=BaseValue;
  }
}


//void ASK25_LCD_Display_Hex (uint16_t VarData, uint16_t Row, uint16_t Col)
//{
//  uint16_t Character[18]={"0123456789ABCDEF"};
//  uint16_t DivValue=10,BaseValue=16;
//  while(DivValue)
//  {
//    ASK25_LCD_Display_Character(Character[VarData/DivValue],Row,Col++,Off);
//    VarData%=DivValue;
//    DivValue/=BaseValue;
//  }
//}


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
