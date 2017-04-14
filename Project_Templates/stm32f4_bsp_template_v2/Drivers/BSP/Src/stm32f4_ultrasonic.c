/******************************************************************//**
* @file		stm32f4_ultrasonic.c
* @brief  This is Ultrasonic ranger driver file to measure distance in
*         Centimeter
* @version V1.0
* @date		Jan 24, 2017
* @author	Bhavin Darji
**********************************************************************/


/* Includes ------------------------------------------------------------------- */
#include "stm32f4_ultrasonic.h"

/**
 * @brief Ultrasonic Ranger GPIO pin direction selection
 * @param Direction
 *      @arg Input
 *      @arg Output
 * @return Nothing
 */
static void Ultrasonic_GPIO_SetDir(GPIODirection_TypeDef Direction)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOC_CLK_ENABLE();

  /* Configure Button pin as input */
  GPIO_InitStruct.Pin = GPIO_PIN_6;         //PL10 Connector - PC6 pin
  if(Direction == Input)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  }
  else
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  }
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


/**
 * @brief Measure the distance in cm
 * @return distance value
 */
uint16_t UltraSonic_Measure_In_CM(void)
{
  uint32_t duration,start;
  uint16_t RangeInCentimeters;
  uint8_t set=1;

  Ultrasonic_GPIO_SetDir(Output);

  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
  Delay_Us(2);

  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
  Delay_Us(5);

  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);

  Ultrasonic_GPIO_SetDir(Input);
  while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==0);
  while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)==1)
  {
    if(set == 1)
    {
      start = DWT->CYCCNT;
      set=0;
    }
  }
  duration = DWT->CYCCNT - start;
  RangeInCentimeters = (duration*340)/(20000*168);
  set = 1;
  return RangeInCentimeters;
}





 /* --------------------------------- End Of File ------------------------------ */
