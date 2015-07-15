/******************************************************************//**
* @file		  LCD_Touch_Calibration.h
* @brief	  header for LCD_Touch_Calibration.c
* @version	V1.0
* @date		  May 8, 2015
* @author	  Dwijay.Edutech learning solutions
**********************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_TOUCH_CALIBRATION_H
#define __LCD_TOUCH_CALIBRATION_H

#define LCD_TOUCH_CALIBRATION_MODULE

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "stm32f4_discovery_tsc.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY_LCD
  * @{
  */

/** @defgroup STM32F4_DISCOVERY_TSC_CAL
  * @brief
  * @{
  */


/** @defgroup STM32F4_DISCOVERY_TSC_CAL_Exported_Constants
  * @{
  */
typedef struct{
   uint16_t x;
   uint16_t y;
}Point_Struct;

typedef struct{
   /*scale factor for x direction*/
   float    xScale;
   /*scale factor for x direction*/
   float    yScale;
   /*Offset in x direction*/
   int16_t  xOffset;
   /*Offset in y direction*/
   int16_t  yOffset;
}Adjust_Struct;


#ifdef LCD_TOUCH_CALIBRATION_MODULE
    #define LCD_TOUCH_CALIBRATION_EXT
#else
    #define LCD_TOUCH_CALIBRATION_EXT extern
#endif

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_TSC_CAL_Exported_Functions
  * @{
  */

void  Lcd_Touch_Calibration (void);
void TSC_Read_Cal_Value(Point_Struct* tpoint);
void  Calibration_Test_Dispose (void);


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

#endif /* __LCD_TOUCH_CALIBRATION_H */

/****************************************END OF FILE******************************************/
