/**
  ******************************************************************************
  * @file    stm32f4_discovery_buzzer.c
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    30-May-2015
  * @brief   This file provides set of firmware functions for Buzzer.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_discovery_buzzer.h"

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_BUZZER
 * @{
 */

/* Variables --------------------------------------------------------------- */
uint16_t timeout;     /* timeout for note */
uint16_t on_time;
uint16_t off_time;
uint16_t on_timer;
uint16_t off_timer;
uint8_t play_note;

GPIO_TypeDef *Buzzer_Port;
uint32_t Buzzer_Pin;

/*Constants ----------------------------------------------------------------- */

const uint8_t note[MELODY_LENGTH] =
{
  E5, SILENT_NOTE, E5, SILENT_NOTE, E5, SILENT_NOTE,

  C5, E5, G5, SILENT_NOTE, G4, SILENT_NOTE,

  C5, G4, SILENT_NOTE, E4, A4, B4, A_SHARP4, A4, G4,
  E5, G5, A5, F5, G5, SILENT_NOTE, E5, C5, D5, B4,

  C5, G4, SILENT_NOTE, E4, A4, B4, A_SHARP4, A4,G4,
  E5, G5, A5, F5, G5, SILENT_NOTE, E5, C5, D5, B4,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  G_SHARP4, A4, C5, SILENT_NOTE, A4, C5, D5,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  C6, SILENT_NOTE, C6, SILENT_NOTE, C6,

  SILENT_NOTE, G5, F_SHARP5, F5, D_SHARP5, E5, SILENT_NOTE,
  G_SHARP4, A4, C5, SILENT_NOTE, A4, C5, D5,

  SILENT_NOTE, D_SHARP5, SILENT_NOTE, D5, C5
};

const uint16_t duration[MELODY_LENGTH] =
{
  100, 25, 125, 125, 125, 125, 125, 250, 250, 250, 250, 250,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 125, 125, 125, 250, 125, 125, 200, 50, 100, 25, 500,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 250, 125, 375, 500
};

/* Private Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_BUZZER_Private_Functions
 * @{
 */
/**
 * @brief Initialization of Buzzer port pin
 * @param Buzzer_port specify the GPIO port
 *      @arg GPIOA to GPIOE
 * @param Buzzer_pin specify the GPIO port pin
 *      @arg GPIO_PIN_0 to GPIO_PIN_15
 */
void BSP_Buzzer_Init(GPIO_TypeDef *Buzzer_port,uint32_t Buzzer_pin)
{
  Buzzer_Port = Buzzer_port;
  Buzzer_Pin = Buzzer_pin;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();

  /*Configure GPIO pin */
  GPIO_InitStruct.Pin = Buzzer_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(Buzzer_port, &GPIO_InitStruct);

  /* initialize and start the the timer */
  BSP_TIM_Basic_Config(&htim7_basic,50);
}
/**
 * @} STM32F4_DISCOVERY_BUZZER_Private_Functions End
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup STM32F4_DISCOVERY_BUZZER_Public_Functions
 * @{
 */
/**
 * @brief Set up timer 1 to play the desired frequency (in Hz) and for the the
 *          desired duration (in ms). Allowed frequencies are from 40 Hz to 5 kHz.
 * @param freq   Frequency of the note
 * @param dur    Duration of the note
 * @return  None
 */
void BSP_Play_Frequency(uint16_t freq, uint16_t dur)
{
  if(freq == SILENT_NOTE)
  {
    on_time = 0; /* ON time of period */
    off_time = 0; /* OFF time of period */
    timeout = dur * 20; /* set timeout (duration) */
  }
  else
  {
    on_time = freq / 2; /* 50% duty cycle */
    off_time = freq - on_time;
    timeout = dur * 20; /* set timeout (duration) */
  }
  on_timer = on_time; /* Load on_time */
  off_timer = off_time; /* Load off_time */
}


/**
 * @brief This tune will be play at the end of the program
 * @param None
 * @return  None
 */
void BSP_Play_Melody(void)
{
  uint8_t i;
  uint16_t freq;
  uint16_t dur;

  play_note = SET; /* start tune */
  for ( i = 0; i < MELODY_LENGTH; i++)
  {
    freq = note[i];
    dur = duration[i];
    BSP_Play_Frequency(freq, dur);
    while(timeout){ /* wait */};
  }
  play_note = RESET; /* stop tune */
  HAL_GPIO_WritePin(GPIOC,_BIT(7),GPIO_PIN_RESET); //buzzer off
}


/**
 * @brief Call back function for timer7
 * @param htim htim7_basic
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* generate one note; freq + duration */
    if(play_note) /* Flag to play note */
    {
      if(timeout) /* Note Duration */
      {
        if(on_timer)
        {
          HAL_GPIO_WritePin(Buzzer_Port,Buzzer_Pin,GPIO_PIN_SET); //buzzer on
          --on_timer;
        }
        else if(off_timer)
        {
         HAL_GPIO_WritePin(Buzzer_Port,Buzzer_Pin,GPIO_PIN_RESET); //buzzer off
          --off_timer;
        }
        else
        {
          on_timer = on_time; /* Reload on_time */
          off_timer = off_time; /* Reload off_time */
        }
        --timeout; /*decrement timer */
      }
    }/* end of buzzer's code */
}

/**
 * @} STM32F4_DISCOVERY_BUZZER_Public_Functions End
 */



 /* --------------------------------- End Of File ------------------------------ */
