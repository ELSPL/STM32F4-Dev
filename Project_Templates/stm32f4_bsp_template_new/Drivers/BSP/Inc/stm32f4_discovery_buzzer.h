/**
  ******************************************************************************
  * @file    stm32f4_discovery_buzzer.h
  * @author  Bhavin.Edutech Learning Solutions
  * @version V1.0
  * @date    30-May-2015
  * @brief   This file contains macro definitions and declarations for its
  *          source file.
  ******************************************************************************
  */

#ifndef __STM32F4_DISCOVERY_BUZZER_H
#define __STM32F4_DISCOVERY_BUZZER_H

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery_timer.h"

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_BUZZER
 * @ingroup BSP
 * @{
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Variables ----------------------------------------------------------- */
/**
 * @defgroup STM32F4_DISCOVERY_BUZZER_Variable
 */
extern uint16_t timeout;     /* timeout for note */
extern uint16_t on_time;
extern uint16_t off_time;
extern uint16_t on_timer;
extern uint16_t off_timer;
extern uint8_t play_note;

/**
 * @} STM32F4_DISCOVERY_BUZZER_Variable End
 */


/* Public Macros ----------------------------------------------------------- */
/**
 * @defgroup STM32F4_DISCOVERY_BUZZER_Macros
 */
#define MELODY_LENGTH 95

/* Key at 20kHz interrupt service routine Timer2 */
#define SILENT_NOTE  255   //   00  Hz

/* Middle 4th C 1-line Octave */
#define C4           76    //  262  Hz
#define C_SHARP4     72    //  278  Hz  Db4
#define D4           68    //  294  Hz
#define D_SHARP4     64    //  312  Hz  Eb4
#define E4           61    //  330  Hz
#define F4           57    //  350  Hz
#define F_SHARP4     54    //  370  Hz
#define G4           51    //  392  Hz
#define G_SHARP4     48    //  416  Hz
#define A4           45    //  440  Hz
#define A_SHARP4     43    //  467  Hz
#define B4           41    //  494  Hz

/* C 2-line Octave */
#define C5           38    //  524  Hz
#define C_SHARP5     36    //  555  Hz
#define D5           34    //  588  Hz
#define D_SHARP5     32    //  623  Hz
#define E5           30    //  660  Hz
#define F5           29    //  699  Hz
#define F_SHARP5     27    //  740  Hz
#define G5           26    //  784  Hz
#define G_SHARP5     24    //  831  Hz
#define A5           23    //  880  Hz
#define A_SHARP5     22    //  933  Hz
#define B5           20    //  988  Hz

/* C 3-line Octave */
#define C6           19    // 1047  Hz
#define C_SHARP6     18    // 1109  Hz
#define D6           17    // 1175  Hz
#define D_SHARP6     16    // 1245  Hz
#define E6           15    // 1317  Hz
#define F6           14    // 1397  Hz
#define F_SHARP6     13    // 1480  Hz
#define G6           13    // 1568  Hz
#define G_SHARP6     12    // 1662  Hz
#define A6           11    // 1760  Hz
#define A_SHARP6     11    // 1865  Hz
#define B6           10    // 1976  Hz

/* C 4-line Octave */
#define C7           9     // 2093  Hz
#define D_SHARP7     8     // 2500  Hz
#define F7           7     // 2857  Hz
#define G_SHARP7     6     // 3333  Hz
#define B7           5     // 4000  Hz

#define D_SHARP8     4     // 5000  Hz
/**
 * @} STM32F4_DISCOVERY_BUZZER_Macros End
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_DISCOVERY_BUZZER_Public_Functions
 * @{
 */
void BSP_Buzzer_Init(GPIO_TypeDef *Buzzer_port,uint32_t Buzzer_pin);
void BSP_Play_Frequency(uint16_t freq, uint16_t dur);
void BSP_Play_Melody(void);

/**
 * @} STM32F4_DISCOVERY_BUZZER_Public_Functiond End
 */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_BUZZER_H */

/**
 * @} STM32F4_DISCOVERY_BUZZER End
 */

/* --------------------------------- End Of File ------------------------------ */
