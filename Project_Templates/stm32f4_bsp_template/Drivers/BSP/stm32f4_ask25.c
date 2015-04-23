/**
  ******************************************************************************
  * @file    stm32f4_ask25.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    06-April-2015
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on STM32F4-ASK25 Kit from Edutech.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_ask25.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_ASK25
  * @{
  */

/** @defgroup STM32F4_ASK25_LOW_LEVEL
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32F4-ASK25 Kit from Edutech.
  * @{
  */
/* Private Variable */
uint8_t temp = 0 ;

/* Private Functions */
uint8_t ASK25_MatKey_Find_Column(void);
uint8_t ASK25_MatKey_Find_Row(void);
void ASK25_MatKey_Row_Low (void);
void ASK25_MatKey_Row_High (void);
void ASK25_MatKey_Col_High (void);
void ASK25_SM_Send_Sequence(StMotorDirection_Typedef StMotorDirection, uint8_t Delay);

/** @defgroup STM32F4_ASK25_LOW_LEVEL_LED_Functions
  * @{
  */

/**
 * @brief Configures ASK25's LED GPIO.
 * @param LED: Specifies the Led to be configured.
 *   This parameter can be one of following parameters:
 *     @arg ALED1 to ALED8
 *     @arg ALEDALL
 * @retval None
 */
void ASK25_LED_Init(ALED_TypeDef LED)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_ALED Clock */

  __GPIOE_CLK_ENABLE();

  /* Configure the GPIO_ALED pin */
  GPIO_InitStruct.Pin = LED << 8 ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOE, LED<<8, GPIO_PIN_SET);
}


/**
 * @brief Turns selected LED On.
 * @param LED: Specifies the LED to be set on.
 *   This parameter can be one of following parameters:
 *     @arg ALED1 to ALED8
 *     @arg ALEDALL
 * @retval None
 */
void ASK25_LED_On(ALED_TypeDef LED)
{
  HAL_GPIO_WritePin(GPIOE,LED<<8, GPIO_PIN_RESET);
}


/**
 * @brief Turns selected LED Off.
 * @param LED: Specifies the Led to be set off.
 *   This parameter can be one of following parameters:
 *     @arg ALED1 to ALED8
 *     @arg ALEDALL
 * @retval None
 */
void ASK25_LED_Off(ALED_TypeDef LED)
{
  HAL_GPIO_WritePin(GPIOE,LED<<8, GPIO_PIN_SET);
}


/**
  * @brief  Toggles the selected LED.
  * @param  LED: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
 *     @arg ALED1 to ALED8
 *     @arg ALEDALL
  * @retval None
  */
void ASK25_LED_Toggle(ALED_TypeDef LED)
{
  HAL_GPIO_TogglePin(GPIOE, LED<<8);
}


/**
  * @}
  */

/** @defgroup STM32F4_ASK25_LOW_LEVEL_BUTTON_Functions
  * @{
  */

/**
  * @brief  Configures Button GPIO and EXTI Line of ASK25.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: ABUTTON_KEY1 to ABUTTON_KEY4 or ABUTTON_ALL
  * @param  Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg ABUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg ABUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                             generation capability (Make sure to ENABLE USE_BUTTON_INT
  *                             in stm32f4_ask25.h file)
  *                             Note: Include "stm32f4_ask25.h in stm32f4xx_it.c file"
  * @retval None
  */
void ASK25_PB_Init(AButton_TypeDef Button, AButtonMode_TypeDef Mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the BUTTON Clock */
  __GPIOE_CLK_ENABLE();

  if (Mode == ABUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = Button;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  }

  if (Mode == ABUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = Button;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  }
}


/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: ABUTTON_KEY1 to ABUTTON_KEY4 or ABUTTON_ALL
  * @retval The Button GPIO pin value.
  */
uint32_t ASK25_PB_GetState(AButton_TypeDef Button)
{
  return HAL_GPIO_ReadPin(GPIOE, Button);
}

/**
  * @}
  */


/** @defgroup STM32F4_ASK25_LOW_LEVEL_RELAY_Functions
  * @{
  */

/**
 * @brief Configures RELAY of ASK25
 * @retval None
 */
void ASK25_Relay_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the GPIO_Relay Clock */

  __GPIOB_CLK_ENABLE();

  /* Configure the GPIO_Relay pin */
  GPIO_InitStruct.Pin = RELAY_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(RELAY_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_SET);
}

/**
  * @}
  */

/** @defgroup STM32F4_ASK25_LOW_LEVEL_DCMOTOR_Functions
  * @{
  */

/**
 * @brief Initialize DC Motor on ASK25
 * @retval None
 */
void ASK25_DCMotor_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the DCMotor Clock */

  __GPIOB_CLK_ENABLE();

  /* Configure the GPIO_DCMoter pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}
/**
 * @brief DC Motor Clockwise Rotation Function
 */
void ASK25_DCMotor_Clk(void)
{
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
}

/**
 * @brief DC Motor Anticlockwise Rotation Function
 */
void ASK25_DCMotor_AntiClk(void)
{
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
}
/**
  * @}
  */

/**@defgroup STM32F4_ASK25_7Segment_Functions
 * @{
 */

/**
 * @brief Initialize 7segment on ASK25
 * @return none
 */
void ASK25_7Segment_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable the 7Segment clock */
  __GPIOB_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();

 /* Configure 7 Segment Data pin */
  GPIO_InitStruct.Pin = 0xFF << 8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* Enable the 7 Segment selection and gate pin */
  GPIO_InitStruct.Pin =  GPIO_PIN_4 | GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);// Select first Segment
  HAL_GPIO_WritePin(GPIOE, 0xFF << 8, GPIO_PIN_SET); // Common anode so 0xFF clears display
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // gate signal high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // latch signal on falling edge

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // Select second Segment
  HAL_GPIO_WritePin(GPIOE, 0xFF << 8, GPIO_PIN_SET); // Common anode so 0xFF clears display
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // gate signal high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // latch signal on falling edge
}


/**
 * @brief To display digit on 7 Segment
 * @param Digit: specifies which digit display on 7 Segment
 *      This parameter should be any number from 0 to 9
 * @param Seg: specifies which Segment to be selected
 *      This parameter should be ASEGMENT1 or ASEGMENT2
 * @return none
 */
void ASK25_7Segment_Display_Digit (uint8_t Digit, uint8_t Seg)
{
  uint8_t Number[16]= {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};

  /* Select segment */
  if(Seg == ASEGMENT1)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);// Select first Segment
  }
  else if(Seg == ASEGMENT2)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // Select second Segment
  }

  HAL_GPIO_WritePin(GPIOE, 0xFF << 8, GPIO_PIN_SET); //  clears display
  HAL_GPIO_WritePin(GPIOE, Number[Digit] << 8, GPIO_PIN_RESET);

  /* Latch data */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // gate signal high
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // latch signal on falling edge
}

/**
 * @brief To display 2 digit data using both 7 Segment
 * @param Number: specifies which digit to display
 *        can be any digit from 00 to 99
 * @return None
 */
void ASK25_7Segment_Display_Data (uint8_t Number)
{
  uint8_t i=0;
  uint8_t j=TOTAL_SEGMENTS-1;
  uint8_t Digits[TOTAL_SEGMENTS];

  while(1)
  {
    Digits[i] = Number % 10; //Store unit value
    Display_Digit(Digits[i], TOTAL_SEGMENTS - j);
    j--;
    i++;
    Number = Number / 10; //Trim a unit value
    if(i >= TOTAL_SEGMENTS)
    {
      break;
    }
  }
}
/**
  * @}
  */


/**@defgroup STM32F4_ASK25_MAT-KEY_Functions
 * @{
 */


/**
 * @brief Matrix key pin assignment function
 * @return None
 */
void ASK25_MatKey_Config (void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the BUTTON Clock */
  __GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Pin = _SBF(4,0x03);     // Set GPIO E.4 & GPIO E.5 as ROW output
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = _SBF(6,0x03);    // Set GPIO E.6 & GPIO E.7 as COL input
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/**
 * @brief This function initializes matrix keyboard.
 *              Make ROWs as o/p & Columns as an i/p.
 * @return None
 */
void ASK25_MatKey_Init (void)
{
  ASK25_MatKey_Config();

  ASK25_MatKey_Col_High();
  ASK25_MatKey_Row_Low();
}

/**
 * @brief This function will scan whether any key is pressed or not
 * @return It will return status of key pressed
 */
uint8_t ASK25_MatKey_Scan_Key (void)
{
  uint8_t ReadValue = 0,col1,col2;

  col1 = HAL_GPIO_ReadPin(GPIOE,_BIT(6));
  col2 = HAL_GPIO_ReadPin(GPIOE,_BIT(7));

  #if MATKB_2X2
      ReadValue = (col1 ^ col2);
  #endif

  if((ReadValue != 0))  //check if both key is pressed
      return(1);
  else
      return(0);
}

/**
 * @brief This function detects whether any key is pressed or
 *        not also returns its its location
 * @return Pressed key code
 */
uint8_t ASK25_MatKey_Detect_Key(void)
{
  uint8_t Col = 0,Row = 0,Key = 0;

  Row = ASK25_MatKey_Find_Row();
  Col = ASK25_MatKey_Find_Column();
  Key = Col + (10 * Row);
  return(Key);
}

/**
 * @brief This function will find the column from where key is pressed
 * @return Return column index
 */
uint8_t ASK25_MatKey_Find_Column(void)
{
  uint8_t col1,col2;

  if (ASK25_MatKey_Scan_Key() != 1)
  {
      return(0);
  }

  #if MATKB_2X2
  col1 = HAL_GPIO_ReadPin(GPIOE,_BIT(6));
  col2 = HAL_GPIO_ReadPin(GPIOE,_BIT(7));
  if(col1 == 0)
  {
    return(1);
  }
  else if (col2 == 0)
  {
    return(2);
  }
  else
  {
    return 0;
  }
  #endif
}

/**
 * @brief This function will find the row from where key is pressed
 * @return Return row index
 */
uint8_t ASK25_MatKey_Find_Row(void)
{
   uint8_t i;

    for(i = 1; i <= ROW ;i++)
    {
      ASK25_MatKey_Row_High();
      switch (i)
      {
        case 1:
          HAL_GPIO_WritePin(GPIOE,_BIT(4),GPIO_PIN_RESET); // ROW1
          break;

        case 2:
          HAL_GPIO_WritePin(GPIOE,_BIT(5),GPIO_PIN_RESET); // ROW2
          break;
      }
      if (ASK25_MatKey_Scan_Key() == 1)
      {
        ASK25_MatKey_Row_Low();
        return(i);
      }
    }
    return 0;
}

/**
 * @brief This function will set all columns
 * @return None
 */
void ASK25_MatKey_Col_High (void)
{
  HAL_GPIO_WritePin(GPIOE,_SBF(6,0x03),GPIO_PIN_SET);  // COL1 & COL2 High
}

/**
 * @brief This function will set all Rows
 * @return None
 */
void ASK25_MatKey_Row_High (void)
{
  HAL_GPIO_WritePin(GPIOE,_SBF(4,0x03),GPIO_PIN_SET);  // ROW1 & ROW2 High
}

/**
 * @brief This function will clear all Rows
 * @return None
 */
void ASK25_MatKey_Row_Low (void)
{
  HAL_GPIO_WritePin(GPIOE,_SBF(4,0x03),GPIO_PIN_RESET);  // ROW1 & ROW2 Low
}
/**
  * @}
  */


/**
 * @defgroup STM32F4_ASK25_Stepper_Motor_Function
 * @{
 */
/**
 * @brief Stepper Motor initialization
 */
void ASK25_SM_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStruct;

   /* Enable the GPIO_SM Clock */
  __GPIOE_CLK_ENABLE();

  /* Configure the GPIO_SM pin */
  GPIO_InitStruct.Pin = _SBF(8,0xAA);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
/**
 * @brief This function rotates stepper motor in desired direction and angle
 * @param StMotorDirection specifies direction of rotation
 *        This parameter should be: StMotorClockwise
 *                                  StMotorAntiClockwise
 * @param Angle specifies rotation angle
 * @param Delay specifies delay between steps
 */
void ASK25_SM_Rotate (StMotorDirection_Typedef StMotorDirection, uint8_t Angle, uint8_t Delay)
{
  uint16_t i,Count;
  Count = ((Angle*10) / CAL_ANGLE);
  for(i=0;i < Count;i++)
  {
    ASK25_SM_Send_Sequence(StMotorDirection,Delay);
  }
}

/**
 * @brief This function will send sequence to Stepper motor
 * @param StMotorDirection specifies direction of rotation
 *        This parameter should be: StMotorClockwise
 *                                  StMotorAntiClockwise
 * @param Delay specifies delay between steps
 */
void ASK25_SM_Send_Sequence(StMotorDirection_Typedef StMotorDirection, uint8_t Delay)
{

  #ifdef General_SM
  uint8_t SmAntClk[4]= {0x80,0x08,0x20,0x02};
  uint8_t SmClk[4]= {0x02,0x20,0x08,0x80};
  #endif

  #ifdef STM_601
  uint8_t SmAntClk[4] = {0x0A,0x88,0xA0,0x22};
  uint8_t SmClk[4] = {0x22,0xA0,0x88,0x0A};
  #endif

  if(StMotorDirection == StMotorClockwise)
  {
    HAL_GPIO_WritePin(GPIOE,_SBF(8,0xAA),GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE,_SBF(8,SmClk[temp]),GPIO_PIN_SET);
  }
  if(StMotorDirection == StMotorAntiClockwise)
  {
    HAL_GPIO_WritePin(GPIOE,_SBF(8,0xAA),GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE,_SBF(8,SmAntClk[temp]),GPIO_PIN_SET);
  }
  HAL_Delay(Delay);
  temp++;
  if(temp>3) temp = 0;
}


/**
  * @}
  */

/**
  * @}
  */

/*****************************************END OF FILE******************************************/
