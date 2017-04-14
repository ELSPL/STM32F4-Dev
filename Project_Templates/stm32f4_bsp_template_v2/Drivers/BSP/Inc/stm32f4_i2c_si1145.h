/******************************************************************//**
* @file		stm32f4_i2c_si1145.h
* @brief	Contains all I2C based SI1145 sunlight sensor declarations
* @version	1.0
* @date		18. JUL. 2016
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/
#ifndef __STM32F4_I2C_SI1145_H_
#define __STM32F4_I2C_SI1145_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Peripheral group ----------------------------------------------------------- */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_SI1145
  * @{
  */

/** @defgroup STM32F4_SI1145_Exported_Constants
  * @{
  */



/* Includes ------------------------------------------------------------------- */
#include "stm32f4_global.h"
#include "stm32f4xx_hal.h"

/* Public Macros -------------------------------------------------------------- */
/** @defgroup STM32F4_I2C_SI1145_Public_Macros
 * @{
 */

extern I2C_HandleTypeDef hi2c1_si1145;
#define SI1145_ADDRESS 				(0x60 << 1)

// SI1145 Register address definition
#define SI1145_PART_ID  			0X00
#define SI1145_REV_ID  				0X01
#define SI1145_SEQ_ID  				0X02
#define SI1145_INT_CFG  			0X03
#define SI1145_IRQ_ENABLE  			0X04
#define SI1145_IRQ_MODE1 			0x05
#define SI1145_IRQ_MODE2 			0x06
#define SI1145_HW_KEY  				0X07
#define SI1145_MEAS_RATE0 			0X08
#define SI1145_MEAS_RATE1  			0X09
#define SI1145_PS_RATE  			0X0A
#define SI1145_PS_LED21  			0X0F
#define SI1145_PS_LED3  			0X10
#define SI1145_UCOEFF0  			0X13
#define SI1145_UCOEFF1  			0X14
#define SI1145_UCOEFF2  			0X15
#define SI1145_UCOEFF3  			0X16
#define SI1145_WR  					0X17
#define SI1145_COMMAND  			0X18
#define SI1145_RESPONSE  			0X20
#define SI1145_IRQ_STATUS  			0X21
#define SI1145_ALS_VIS_DATA0 		0X22
#define SI1145_ALS_VIS_DATA1 		0X23
#define SI1145_ALS_IR_DATA0 		0X24
#define SI1145_ALS_IR_DATA1 		0X25
#define SI1145_PS1_DATA0 			0X26
#define SI1145_PS1_DATA1 			0X27
#define SI1145_PS2_DATA0 			0X28
#define SI1145_PS2_DATA1 			0X29
#define SI1145_PS3_DATA0 			0X2A
#define SI1145_PS3_DATA1 			0X2B
#define SI1145_AUX_DATA0_UVINDEX0 	0X2C
#define SI1145_AUX_DATA1_UVINDEX1 	0X2D
#define SI1145_RD 					0X2E
#define SI1145_CHIP_STAT 			0X30

//Parameter RAM offset definition
#define SI1145_CHLIST   			0X01
#define SI1145_CHLIST_ENUV 			0x80
#define SI1145_CHLIST_ENAUX 		0x40
#define SI1145_CHLIST_ENALSIR 		0x20
#define SI1145_CHLIST_ENALSVIS 		0x10
#define SI1145_CHLIST_ENPS1 		0x01
#define SI1145_CHLIST_ENPS2 		0x02
#define SI1145_CHLIST_ENPS3 		0x04

#define SI1145_PSLED12_SELECT   	0X02
#define SI1145_PSLED3_SELECT   		0X03

#define SI1145_PS_ENCODE   			0X05
#define SI1145_ALS_ENCODE  			0X06

#define SI1145_PS1_ADCMUX   		0X07
#define SI1145_PS2_ADCMUX   		0X08
#define SI1145_PS3_ADCMUX   		0X09

#define SI1145_PS_ADC_COUNTER   	0X0A
#define SI1145_PS_ADC_GAIN 			0X0B
#define SI1145_PS_ADC_MISC 			0X0C

#define SI1145_ALS_IR_ADC_MUX   	0X0E
#define SI1145_AUX_ADC_MUX   		0X0F

#define SI1145_ALS_VIS_ADC_COUNTER   0X10
#define SI1145_ALS_VIS_ADC_GAIN 	0X11
#define SI1145_ALS_VIS_ADC_MISC 	0X12

#define SI1145_LED_REC 				0X1C

#define SI1145_ALS_IR_ADC_COUNTER   0X1D
#define SI1145_ALS_IR_ADC_GAIN 		0X1E
#define SI1145_ALS_IR_ADC_MISC 		0X1F


//command register definition
#define SI1145_QUERY 				0X80
#define SI1145_SET 					0XA0
#define SI1145_NOP 					0X00
#define SI1145_RESET    			0X01
#define SI1145_BUSADDR    			0X02
#define SI1145_PS_FORCE   			0X05
#define SI1145_GET_CAL    			0X12
#define SI1145_ALS_FORCE    		0X06
#define SI1145_PSALS_FORCE    		0X07
#define SI1145_PS_PAUSE    			0X09
#define SI1145_ALS_PAUSE    		0X0A
#define SI1145_PSALS_PAUSE    		0X0B
#define SI1145_PS_AUTO    			0X0D
#define SI1145_ALS_AUTO   			0X0E
#define SI1145_PSALS_AUTO 			0X0F

//USER Setting definition
//ADCMUX
#define SI1145_ADCMUX_SMALL_IR  0x00
#define SI1145_ADCMUX_VISIABLE 0x02
#define SI1145_ADCMUX_LARGE_IR  0x03
#define SI1145_ADCMUX_NO  0x06
#define SI1145_ADCMUX_GND  0x25
#define SI1145_ADCMUX_TEMPERATURE  0x65
#define SI1145_ADCMUX_VDD  0x75
//LED SEL
#define SI1145_PSLED12_SELECT_PS1_NONE 0x00
#define SI1145_PSLED12_SELECT_PS1_LED1 0x01
#define SI1145_PSLED12_SELECT_PS1_LED2 0x02
#define SI1145_PSLED12_SELECT_PS1_LED3 0x04
#define SI1145_PSLED12_SELECT_PS2_NONE 0x00
#define SI1145_PSLED12_SELECT_PS2_LED1 0x10
#define SI1145_PSLED12_SELECT_PS2_LED2 0x20
#define SI1145_PSLED12_SELECT_PS2_LED3 0x40
#define SI1145_PSLED3_SELECT_PS2_NONE 0x00
#define SI1145_PSLED3_SELECT_PS2_LED1 0x10
#define SI1145_PSLED3_SELECT_PS2_LED2 0x20
#define SI1145_PSLED3_SELECT_PS2_LED3 0x40
//ADC GAIN DIV
#define SI1145_ADC_GAIN_DIV1 0X00
#define SI1145_ADC_GAIN_DIV2 0X01
#define SI1145_ADC_GAIN_DIV4 0X02
#define SI1145_ADC_GAIN_DIV8 0X03
#define SI1145_ADC_GAIN_DIV16 0X04
#define SI1145_ADC_GAIN_DIV32 0X05
//LED CURRENT
#define SI1145_LED_CURRENT_5MA 0X01
#define SI1145_LED_CURRENT_11MA 0X02
#define SI1145_LED_CURRENT_22MA 0X03
#define SI1145_LED_CURRENT_45MA 0X04
//Recovery period the  ADC takes before making a PS measurement
#define SI1145_ADC_COUNTER_1ADCCLK 0X00
#define SI1145_ADC_COUNTER_7ADCCLK 0X01
#define SI1145_ADC_COUNTER_15ADCCLK 0X02
#define SI1145_ADC_COUNTER_31ADCCLK 0X03
#define SI1145_ADC_COUNTER_63ADCCLK 0X04
#define SI1145_ADC_COUNTER_127ADCCLK 0X05
#define SI1145_ADC_COUNTER_255ADCCLK 0X06
#define SI1145_ADC_COUNTER_511ADCCLK 0X07
//ADC MISC
#define SI1145_ADC_MISC_LOWRANGE 0X00
#define SI1145_ADC_MISC_HIGHRANGE 0X20
#define SI1145_ADC_MISC_ADC_NORMALPROXIMITY 0X00
#define SI1145_ADC_MISC_ADC_RAWADC 0X04
//INT OE
#define SI1145_INT_CFG_INTOE 0X01
//IRQ ENABLE
#define SI1145_IRQEN_ALS 0x01
#define SI1145_IRQEN_PS1 0x04
#define SI1145_IRQEN_PS2 0x08
#define SI1145_IRQEN_PS3 0x10

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup STM32F4_I2C_SI1145_Public_Functions
 * @{
 */

void BSP_Si1145_I2C1_Init(void);
void BSP_Si1145_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c);


void SI1145_ReInit(void);
void SI1145_Reset(void);
uint8_t SI1145_Init(void);
uint16_t SI1145_GetUV(void);
uint16_t SI1145_GetVIS(void);
uint16_t SI1145_GetIR(void);
uint16_t SI1145_GetPROX(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /*__STM32F4_I2C_SI1145_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
