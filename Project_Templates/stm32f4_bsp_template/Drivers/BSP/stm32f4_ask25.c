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

  /* Configure the GPIO_ALED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}


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
 *      This parameter should be any number from ZERO to NINE
 * @param Seg: specifies which Segment to be selected
 *      This parameter should be Asegment1 or Asegment2
 * @return none
 */
void ASK25_7Segment_Display_Digit (uint8_t Digit, uint8_t Seg)
{
  uint8_t Number[16]= {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};

  /* Select segment */
  if(Seg == Asegment1)
  {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);// Select first Segment
  }
  else if(Seg == Asegment2)
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
 * @return none
 */
void ASK25_7Segment_Display_Data (uint16_t Number)
{
  uint16_t i=0;
  uint16_t j=TOTAL_SEGMENTS-1;
  uint16_t Digits[TOTAL_SEGMENTS];
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
