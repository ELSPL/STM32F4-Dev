/**
  ******************************************************************************
  * @file    ov9655.c
  * @author  Dwijay.Edutech Learning Solutions
  * @version V1.0
  * @date    16-May-2015
  * @brief   This file provides the OV9655 camera driver
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ov9655.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup OV9655
  * @brief     This file provides a set of functions needed to drive the
  *            OV9655 Camera module.
  * @{
  */

/** @defgroup OV9655_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup OV9655_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup OV9655_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup OV9655_Private_FunctionPrototypes
  * @{
  */
//static uint32_t ov9655_ConvertValue(uint32_t feature, uint32_t value);
/**
  * @}
  */

/** @defgroup OV9655_Private_Variables
  * @{
  */

CAMERA_DrvTypeDef   ov9655_drv =
{
  ov9655_Init,
  ov9655_ReadID,
  ov9655_Config,
};


/* Initialization sequence for QVGA resolution (320x240) */
const uint8_t OV9655_QVGA[][2]=
{
  {OV9655_GAIN, 0x00},  //Default
  {OV9655_BLUE, 0x80},  //Default
  {OV9655_RED, 0x80},   //Default
  {OV9655_VREF, 0x02},  //VREF Start 0x10
  {OV9655_COM1, 0x03},  //AEC low 2 LSBs 0x11
  {OV9655_COM2, 0x01},  //Output drive capability 2x
  {OV9655_VER, 0x57},   //VER 0x57 Read-Only
  {OV9655_COM5, 0x61},  //Exposure Step can be set longer than VSYNC time
  {OV9655_COM6, 0x40},  //Use Optical Black Line as BLC signal
  {OV9655_CLKRC, 0x01}, //Internal Clock Prescaler
  {OV9655_COM7, 0x62},  //30fps VGA with VarioPixel and YUV format
  {OV9655_COM8, 0xC7},  //Fast AEC/AGC algorithm and AEC unlimited step size and Enable AGC,AEC,AWB
  {OV9655_COM9, 0x3A},  //Max AGC value 16x and Enable drop frame
  {OV9655_REG16, 0x24}, //Add dummy frame when Gain is greater than 8x
  {OV9655_HSTART, 0x18},//HREF Column start High 8bits value
  {OV9655_HSTOP, 0x04}, //HREF Column end High 8bits value
  {OV9655_VSTART, 0x01},//Vertical Frame Row start High 8bits value
  {OV9655_VSTOP, 0x81}, //Vertical Frame Row end High 8bits value
  {OV9655_MVFP, 0x00},  //Mirror and Vertical Flip is disabled
  {OV9655_AEW, 0x3C},   //AGC/AEC stable operating Region (Upper Limit value)
  {OV9655_AEB, 0x36},   //AGC/AEC stable operating Region (Lower Limit value)
  {OV9655_VPT, 0x72},   //AGC/AEC Fast Mode operating Region High Nibble of Upper and Lower Limit
  {OV9655_BBIAS, 0x08}, //B Channel signal output Bias of subtracting type
  {OV9655_GbBIAS, 0x08},//Gb Channel signal output Bias of subtracting type
  {OV9655_PREGAIN,0x15},//Pre-Gain of R,G and B channel is 1 (1 of 3)
  {OV9655_EXHCH, 0x00}, //Dummy Pixel insert MSB
  {OV9655_EXHCL, 0x00}, //Dummy Pixel insert LSB
  {OV9655_RBIAS, 0x08}, //R Channel signal output Bias of subtracting type
  {OV9655_HREF, 0x12},  //HREF start and End is 2 Low LSB
  {OV9655_CHLF, 0x00},  //Array Current Control [00 to FF]
  {OV9655_AREF1, 0x3F}, //Array Reference Control Default value
  {OV9655_AREF2, 0x00}, //Array Reference Control Default value
  {OV9655_AREF3, 0x3A}, //Array Reference Control value (Default = F9)
  {OV9655_ADC2, 0x72},  //ADC Reference Control value
  {OV9655_AREF4, 0x57}, //Analog Reference Control value
  {OV9655_TSLB, 0xCC},  //PCLK output delay=6ns and YUV output sequence UYVY
  {OV9655_COM11, 0x04}, //Night Mode Disabled
  {OV9655_COM13, 0x99}, //Gamma used for raw data before interpolation and Enable Y channel delay=1 (Default)
  {OV9655_COM14, 0x02}, //White and Black pixel correction is disabled and Zoom function enabled
  {OV9655_EDGE, 0xC1},  //Edge enhancement factor=1
  {OV9655_COM15, 0xC0}, //Data format=Full range and Normal RGB output (Default)
  {OV9655_COM16, 0x41}, //Color option coefficient double=False and scaling Down=ON (Default = 40)
  {OV9655_COM17, 0xC0}, //De-noise strength auto-adjust and Edge enhancement strength auto-adjust
  {0x43, 0x0A},         //Reserved
  {0x44, 0xF0},         //Reserved
  {0x45, 0x46},         //Reserved
  {0x46, 0x62},         //Reserved
  {0x47, 0x2A},         //Reserved
  {0x48, 0x3C},         //Reserved
  {0x4A, 0xFC},         //Reserved
  {0x4B, 0xFC},         //Reserved
  {0x4C, 0x7F},         //Reserved
  {0x4D, 0x7F},         //Reserved
  {0x4E, 0x7F},         //Reserved
  {OV9655_MTX1, 0x98},  //Matrix Coefficient 1 value (Default 40)
  {OV9655_MTX2, 0x98},  //Matrix Coefficient 2 value (Default 34)
  {OV9655_MTX3, 0x00},  //Matrix Coefficient 3 value (Default 0C)
  {OV9655_MTX4, 0x28},  //Matrix Coefficient 4 value (Default 17)
  {OV9655_MTX5, 0x70},  //Matrix Coefficient 5 value (Default 29)
  {OV9655_MTX6, 0x98},  //Matrix Coefficient 6 value (Default 40)
  {OV9655_MTXS, 0x1A},  //Matrix Coefficient sign |1    -2   3| Default: |1  -2  -3| 0x1E
                        //                        |-4   -5   6|          |-4  5  -6|
  {OV9655_AWBOP1, 0x85},//AWB Control Option 1 (Default 91)
  {OV9655_AWBOP2, 0xA9},//AWB Control Option 2 (Default 94)
  {OV9655_AWBOP3, 0x64},//AWB Control Option 3 (Default AA)
  {OV9655_AWBOP4, 0x84},//AWB Control Option 4 (Default 71)
  {OV9655_AWBOP5, 0x53},//AWB Control Option 5 (Default 8D)
  {OV9655_AWBOP6, 0x0E},//AWB Control Option 6 (Default 0F)
  {OV9655_BLMT, 0xF0},  //AWB Blue Component Gain Limit Default
  {OV9655_RLMT, 0xF0},  //AWB Red Component Gain Limit Default
  {OV9655_GLMT, 0xF0},  //AWB Green Component Gain Limit Default
  {OV9655_LCC1, 0x00},  //Lens Correction Option 1 Default
  {OV9655_LCC2, 0x00},  //Lens Correction Option 2 Default
  {OV9655_LCC3, 0x02},  //Lens Correction Option 3 (Default 10)
  {OV9655_LCC4, 0x20},  //Lens Correction Option 4 (Default 80)
  {OV9655_MANU, 0x00},  //Manual U value (Default 80)
  {OV9655_VARO, 0x0A},  //Reserved
  {OV9655_DBLV, 0x5A},  //PLL freq sel=4x and Bypass internal Regulator for DVDD
                        //Also Bandgap Reference adjustment = A (Default 0A)
  {0x6C, 0x04},         //Reserved
  {0x6D, 0x55},         //Reserved
  {0x6E, 0x00},         //Reserved
  {0x6F, 0x9D},         //Reserved
  {OV9655_DNSTH, 0x21}, //De-Noise Function threshold Adjustment value (Default 02)
  {0x71, 0x78},         //Reserved
  {OV9655_POIDX, 0x11}, //Pixel output index: Vertical Pixel:output 1 line for every 2 lines (Default 00)
                        //                    Horizontal Pixel:output 1 line for every 2 pixels
  {OV9655_PCKDV, 0x01}, //Pixel clock output adjustment Default
  {OV9655_XINDX, 0x10}, //Horizontal Scaling down coefficients (Default 3A)
  {OV9655_YINDX, 0x10}, //Vertical Scaling down coefficients (Default 35)
  {0x76, 0x01},         //Reserved
  {0x77, 0x02},         //Reserved
  {OV9655_SLOP, 0x12},  //Gamma Curve Highest slope (Default 24)
  {OV9655_GAM1, 0x08},  //Gamma Curve 1st segment End point 0x10 output value (Default 04)
  {OV9655_GAM2, 0x16},  //Gamma Curve 2nd segment End point 0x20 output value (Default 07)
  {OV9655_GAM3, 0x30},  //Gamma Curve 3rd segment End point 0x40 output value (Default 10)
  {OV9655_GAM4, 0x5E},  //Gamma Curve 4th segment End point 0x80 output value (Default 28)
  {OV9655_GAM5, 0x72},  //Gamma Curve 5th segment End point 0xA0 output value (Default 36)
  {OV9655_GAM6, 0x82},  //Gamma Curve 6th segment End point 0xC0 output value (Default 44)
  {OV9655_GAM7, 0x8E},  //Gamma Curve 7th segment End point 0xE0 output value (Default 52)
  {OV9655_GAM8, 0x9A},  //Gamma Curve 8th segment End point 0x100 output value (Default 60)
  {OV9655_GAM9, 0xA4},  //Gamma Curve 9th segment End point 0x120 output value (Default 6C)
  {OV9655_GAM10, 0xAC}, //Gamma Curve 10th segment End point 0x140 output value (Default 78)
  {OV9655_GAM11, 0xB8}, //Gamma Curve 11th segment End point 0x180 output value (Default 8C)
  {OV9655_GAM12, 0xC3}, //Gamma Curve 12th segment End point 0x1C0 output value (Default 9E)
  {OV9655_GAM13, 0xD6}, //Gamma Curve 13th segment End point 0x240 output value (Default BB)
  {OV9655_GAM14, 0xE6}, //Gamma Curve 14th segment End point 0x2C0 output value (Default D2)
  {OV9655_GAM15, 0xF2}, //Gamma Curve 15th segment End point 0x340 output value (Default E5)
  {0x8A, 0x24},         //Reserved
  {OV9655_COM19, 0x80}, //UV adjust option and averaging (Default 0C)
  {0x90, 0x7D},         //Reserved
  {0x91, 0x7B},         //Reserved
  {OV9655_LCC6, 0x02},  //Lens Correction Option 6 (Default 00)
  {OV9655_LCC7, 0x02},  //Lens Correction Option 7 (Default 00)
  {0x9F, 0x7A},         //Reserved
  {0xA0, 0x79},         //Reserved
  {OV9655_AECH, 0x40},  //Exposure Value AEC MSB 5bits Default
  {OV9655_COM21, 0x50}, //Digital gain value none (Default 78)
  {0xA5, 0x68},         //Reserved
  {OV9655_GREEN, 0x4A}, //AWB Green component Gain setting (Default 86)
  {OV9655_REFA8, 0xC1}, //Analog Reference Control (Default 01)
  {OV9655_REFA9, 0xEF}, //Analog Reference Control (Default 00)
  {0xAA, 0x92},         //Reserved
  {0xAB, 0x04},         //Reserved
  {OV9655_BLC1, 0x80},  //Black Level Control 1 (Default 84)
  {OV9655_BLC2, 0x80},  //Black Level Control 2 (Default 84)
  {OV9655_BLC3, 0x80},  //Black Level Control 3 (Default 84)
  {OV9655_BLC4, 0x80},  //Black Level Control 4 (Default 84)
  {OV9655_BLC7, 0xF2},  //Black Level Control 7 (Default 01)
  {OV9655_BLC8, 0x20},  //Black Level Control 8 (Default 0F)
  {OV9655_CTRLB4, 0x20},//UV adjust slope selection
  {0xB5, 0x00},         //Reserved
  {0xB6, 0xAF},         //Reserved
  {0xB6, 0xAF},         //Reserved
  {0xBB, 0xAE},         //Reserved
  {OV9655_ADBOFF, 0x7F},//ADC B channel offset setting (Default 40)
  {OV9655_ADROFF, 0x7F},//ADC R channel offset setting (Default 40)
  {OV9655_ADGbOFF,0x7F},//ADC Gb channel offset setting (Default 40)
  {OV9655_ADGrOFF,0x7F},//ADC Gr channel offset setting (Default 40)
  {OV9655_ADGrOFF,0x7F},//ADC Gr channel offset setting (Default 40)
  {0xC0, 0xAA},         //Reserved
  {0xC1, 0xC0},         //Reserved
  {0xC2, 0x01},         //Reserved
  {0xC3, 0x4E},         //Reserved
  {0xC6, 0x05},         //Reserved
  {OV9655_COM24, 0x81}, //Pixel clock frequency selection (Default 80)
  {0xC9, 0xE0},         //Reserved
  {0xCA, 0xE8},         //Reserved
  {0xCB, 0xF0},         //Reserved
  {0xCC, 0xD8},         //Reserved
  {0xCD, 0x93},         //Reserved
};

/* Initialization sequence for QQVGA resolution (160x120) */
const uint8_t OV9655_QQVGA[][2]=
{
  {OV9655_GAIN, 0x00},  //Default
  {OV9655_BLUE, 0x80},  //Default
  {OV9655_RED, 0x80},   //Default
  {OV9655_VREF, 0x02},  //VREF Start 0x10
  {OV9655_COM1, 0x03},  //AEC low 2 LSBs 0x11
  {OV9655_COM2, 0x01},  //Output drive capability 2x
  {OV9655_VER, 0x57},   //VER 0x57 Read-Only
  {OV9655_COM5, 0x61},  //Exposure Step can be set longer than VSYNC time
  {OV9655_COM6, 0x40},  //Use Optical Black Line as BLC signal
  {OV9655_CLKRC, 0x01}, //Internal Clock Prescaler
  {OV9655_COM7, 0x62},  //30fps VGA with VarioPixel and YUV format
  {OV9655_COM8, 0xC7},  //Fast AEC/AGC algorithm and AEC unlimited step size and Enable AGC,AEC,AWB
  {OV9655_COM9, 0x3A},  //Max AGC value 16x and Enable drop frame
  {OV9655_REG16, 0x24}, //Add dummy frame when Gain is greater than 8x
  {OV9655_HSTART, 0x18},//HREF Column start High 8bits value
  {OV9655_HSTOP, 0x04}, //HREF Column end High 8bits value
  {OV9655_VSTART, 0x01},//Vertical Frame Row start High 8bits value
  {OV9655_VSTOP, 0x81}, //Vertical Frame Row end High 8bits value
  {OV9655_MVFP, 0x20},  /*[Mirror image enabled and Vertical Flip is disabled]*/
  {OV9655_AEW, 0x3C},   //AGC/AEC stable operating Region (Upper Limit value)
  {OV9655_AEB, 0x36},   //AGC/AEC stable operating Region (Lower Limit value)
  {OV9655_VPT, 0x72},   //AGC/AEC Fast Mode operating Region High Nibble of Upper and Lower Limit
  {OV9655_BBIAS, 0x08}, //B Channel signal output Bias of subtracting type
  {OV9655_GbBIAS, 0x08},//Gb Channel signal output Bias of subtracting type
  {OV9655_PREGAIN,0x15},//Pre-Gain of R,G and B channel is 1 (1 of 3)
  {OV9655_EXHCH, 0x00}, //Dummy Pixel insert MSB
  {OV9655_EXHCL, 0x00}, //Dummy Pixel insert LSB
  {OV9655_RBIAS, 0x08}, //R Channel signal output Bias of subtracting type
  {OV9655_HREF, 0xA4},  /*[HREF start and End is 2 Low LSB Default]*/
  {OV9655_CHLF, 0x00},  //Array Current Control [00 to FF]
  {OV9655_AREF1, 0x3F}, //Array Reference Control Default value
  {OV9655_AREF2, 0x00}, //Array Reference Control Default value
  {OV9655_AREF3, 0x3A}, //Array Reference Control value (Default = F9)
  {OV9655_ADC2, 0x72},  //ADC Reference Control value
  {OV9655_AREF4, 0x57}, //Analog Reference Control value
  {OV9655_TSLB, 0xCC},  //PCLK output delay=6ns and YUV output sequence UYVY
  {OV9655_COM11, 0x04}, //Night Mode Disabled
  {OV9655_COM13, 0x99}, //Gamma used for raw data before interpolation and Enable Y channel delay=1 (Default)
  {OV9655_COM14, 0x0E}, /*[White and Black pixel correction is enabled and Zoom function enabled]*/
  {OV9655_EDGE, 0xC1},  //Edge enhancement factor=1
  {OV9655_COM15, 0xC0}, //Data format=Full range and Normal RGB output (Default)
  {OV9655_COM16, 0x41}, //Color option coefficient double=False and scaling Down=ON (Default = 40)
  {OV9655_COM17, 0xC0}, //De-noise strength auto-adjust and Edge enhancement strength auto-adjust
  {0x43, 0x0A},         //Reserved
  {0x44, 0xF0},         //Reserved
  {0x45, 0x46},         //Reserved
  {0x46, 0x62},         //Reserved
  {0x47, 0x2A},         //Reserved
  {0x48, 0x3C},         //Reserved
  {0x4A, 0xFC},         //Reserved
  {0x4B, 0xFC},         //Reserved
  {0x4C, 0x7F},         //Reserved
  {0x4D, 0x7F},         //Reserved
  {0x4E, 0x7F},         //Reserved
  {OV9655_MTX1, 0x98},  //Matrix Coefficient 1 value (Default 40)
  {OV9655_MTX2, 0x98},  //Matrix Coefficient 2 value (Default 34)
  {OV9655_MTX3, 0x00},  //Matrix Coefficient 3 value (Default 0C)
  {OV9655_MTX4, 0x28},  //Matrix Coefficient 4 value (Default 17)
  {OV9655_MTX5, 0x70},  //Matrix Coefficient 5 value (Default 29)
  {OV9655_MTX6, 0x98},  //Matrix Coefficient 6 value (Default 40)
  {OV9655_MTXS, 0x1A},  //Matrix Coefficient sign |1    -2   3| Default: |1  -2  -3| 0x1E
                        //                        |-4   -5   6|          |-4  5  -6|
  {OV9655_AWBOP1, 0x85},//AWB Control Option 1 (Default 91)
  {OV9655_AWBOP2, 0xA9},//AWB Control Option 2 (Default 94)
  {OV9655_AWBOP3, 0x64},//AWB Control Option 3 (Default AA)
  {OV9655_AWBOP4, 0x84},//AWB Control Option 4 (Default 71)
  {OV9655_AWBOP5, 0x53},//AWB Control Option 5 (Default 8D)
  {OV9655_AWBOP6, 0x0E},//AWB Control Option 6 (Default 0F)
  {OV9655_BLMT, 0xF0},  //AWB Blue Component Gain Limit Default
  {OV9655_RLMT, 0xF0},  //AWB Red Component Gain Limit Default
  {OV9655_GLMT, 0xF0},  //AWB Green Component Gain Limit Default
  {OV9655_LCC1, 0x00},  //Lens Correction Option 1 Default
  {OV9655_LCC2, 0x00},  //Lens Correction Option 2 Default
  {OV9655_LCC3, 0x02},  //Lens Correction Option 3 (Default 10)
  {OV9655_LCC4, 0x20},  //Lens Correction Option 4 (Default 80)
  {OV9655_MANU, 0x00},  //Manual U value (Default 80)
  {OV9655_VARO, 0x0A},  //Reserved
  {OV9655_DBLV, 0x5A},  //PLL freq sel=4x and Bypass internal Regulator for DVDD
                        //Also Bandgap Reference adjustment = A (Default 0A)
  {0x6C, 0x04},         //Reserved
  {0x6D, 0x55},         //Reserved
  {0x6E, 0x00},         //Reserved
  {0x6F, 0x9D},         //Reserved
  {OV9655_DNSTH, 0x21}, //De-Noise Function threshold Adjustment value (Default 02)
  {0x71, 0x78},         //Reserved
  {OV9655_POIDX, 0x22}, /*[Pixel output index: Vertical Pixel:output 1 line for every 4 lines (Default 00)
                                               Horizontal Pixel:output 1 line for every 4 pixels]*/
  {OV9655_PCKDV, 0x02}, /*[Pixel clock output adjustment (Default 01)]*/
  {OV9655_XINDX, 0x10}, //Horizontal Scaling down coefficients (Default 3A)
  {OV9655_YINDX, 0x10}, //Vertical Scaling down coefficients (Default 35)
  {0x76, 0x01},         //Reserved
  {0x77, 0x02},         //Reserved
  {OV9655_SLOP, 0x12},  //Gamma Curve Highest slope (Default 24)
  {OV9655_GAM1, 0x08},  //Gamma Curve 1st segment End point 0x10 output value (Default 04)
  {OV9655_GAM2, 0x16},  //Gamma Curve 2nd segment End point 0x20 output value (Default 07)
  {OV9655_GAM3, 0x30},  //Gamma Curve 3rd segment End point 0x40 output value (Default 10)
  {OV9655_GAM4, 0x5E},  //Gamma Curve 4th segment End point 0x80 output value (Default 28)
  {OV9655_GAM5, 0x72},  //Gamma Curve 5th segment End point 0xA0 output value (Default 36)
  {OV9655_GAM6, 0x82},  //Gamma Curve 6th segment End point 0xC0 output value (Default 44)
  {OV9655_GAM7, 0x8E},  //Gamma Curve 7th segment End point 0xE0 output value (Default 52)
  {OV9655_GAM8, 0x9A},  //Gamma Curve 8th segment End point 0x100 output value (Default 60)
  {OV9655_GAM9, 0xA4},  //Gamma Curve 9th segment End point 0x120 output value (Default 6C)
  {OV9655_GAM10, 0xAC}, //Gamma Curve 10th segment End point 0x140 output value (Default 78)
  {OV9655_GAM11, 0xB8}, //Gamma Curve 11th segment End point 0x180 output value (Default 8C)
  {OV9655_GAM12, 0xC3}, //Gamma Curve 12th segment End point 0x1C0 output value (Default 9E)
  {OV9655_GAM13, 0xD6}, //Gamma Curve 13th segment End point 0x240 output value (Default BB)
  {OV9655_GAM14, 0xE6}, //Gamma Curve 14th segment End point 0x2C0 output value (Default D2)
  {OV9655_GAM15, 0xF2}, //Gamma Curve 15th segment End point 0x340 output value (Default E5)
  {0x8A, 0x24},         //Reserved
  {OV9655_COM19, 0x80}, //UV adjust option and averaging (Default 0C)
  {0x90, 0x7D},         //Reserved
  {0x91, 0x7B},         //Reserved
  {OV9655_LCC6, 0x02},  //Lens Correction Option 6 (Default 00)
  {OV9655_LCC7, 0x02},  //Lens Correction Option 7 (Default 00)
  {0x9F, 0x7A},         //Reserved
  {0xA0, 0x79},         //Reserved
  {OV9655_AECH, 0x40},  //Exposure Value AEC MSB 5bits Default
  {OV9655_COM21, 0x50}, //Digital gain value none (Default 78)
  {0xA5, 0x68},         //Reserved
  {OV9655_GREEN, 0x4A}, //AWB Green component Gain setting (Default 86)
  {OV9655_REFA8, 0xC1}, //Analog Reference Control (Default 01)
  {OV9655_REFA9, 0xEF}, //Analog Reference Control (Default 00)
  {0xAA, 0x92},         //Reserved
  {0xAB, 0x04},         //Reserved
  {OV9655_BLC1, 0x80},  //Black Level Control 1 (Default 84)
  {OV9655_BLC2, 0x80},  //Black Level Control 2 (Default 84)
  {OV9655_BLC3, 0x80},  //Black Level Control 3 (Default 84)
  {OV9655_BLC4, 0x80},  //Black Level Control 4 (Default 84)
  {OV9655_BLC7, 0xF2},  //Black Level Control 7 (Default 01)
  {OV9655_BLC8, 0x20},  //Black Level Control 8 (Default 0F)
  {OV9655_CTRLB4, 0x20},//UV adjust slope selection
  {0xB5, 0x00},         //Reserved
  {0xB6, 0xAF},         //Reserved
  {0xB6, 0xAF},         //Reserved
  {0xBB, 0xAE},         //Reserved
  {OV9655_ADBOFF, 0x7F},//ADC B channel offset setting (Default 40)
  {OV9655_ADROFF, 0x7F},//ADC R channel offset setting (Default 40)
  {OV9655_ADGbOFF,0x7F},//ADC Gb channel offset setting (Default 40)
  {OV9655_ADGrOFF,0x7F},//ADC Gr channel offset setting (Default 40)
  {OV9655_ADGrOFF,0x7F},//ADC Gr channel offset setting (Default 40)
  {0xC0, 0xAA},         //Reserved
  {0xC1, 0xC0},         //Reserved
  {0xC2, 0x01},         //Reserved
  {0xC3, 0x4E},         //Reserved
  {0xC6, 0x05},         //Reserved
  {OV9655_COM24, 0x82}, /*[Pixel clock frequency selection (Default 80)]*/
  {0xC9, 0xE0},         //Reserved
  {0xCA, 0xE8},         //Reserved
  {0xCB, 0xF0},         //Reserved
  {0xCC, 0xD8},         //Reserved
  {0xCD, 0x93},         //Reserved
};
/**
  * @}
  */

/** @defgroup OV9655_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the OV9655 CAMERA component.
  * @param  DeviceAddr: Device address on communication Bus.
  * @param  resolution: Camera resolution
  * @retval None
  */
void ov9655_Init(uint16_t DeviceAddr, uint32_t resolution)
{
  uint32_t index;

  /* Initialize I2C */
  CAMERA_IO_Init();
  /* Reset all registers and check the presence of the OV9655 camera module */
  while (CAMERA_IO_Write(DeviceAddr, OV9655_COM7, 0x80)); //Page12 OV9655
  /* Initialize OV9655 */
  switch (resolution)
  {
  case CAMERA_R160x120:
    {
      for(index=0; index<(sizeof(OV9655_QQVGA)/2); index++)
      {
        CAMERA_IO_Write(DeviceAddr, OV9655_QQVGA[index][0], OV9655_QQVGA[index][1]);
        CAMERA_Delay(2);
      }
      break;
    }
  case CAMERA_R320x240:
    {
      for(index=0; index<(sizeof(OV9655_QVGA)/2); index++)
      {
        CAMERA_IO_Write(DeviceAddr, OV9655_QVGA[index][0], OV9655_QVGA[index][1]);
        CAMERA_Delay(2);
      }
      break;
    }
  default:
    {
      break;
    }
  }

  /* Set the RGB565 mode */
  CAMERA_IO_Write(DeviceAddr, OV9655_COM7, 0x63);  // 30fps VarioPixel and RGB
  CAMERA_IO_Write(DeviceAddr, OV9655_COM15, 0x10); // RGB565 mode
  /* Invert the HRef signal*/
  CAMERA_IO_Write(DeviceAddr, OV9655_COM10, 0x08); // HREF Reverse
}

/**
  * @brief  Configures the OV9655 camera feature. (STILL in DEVELOPMENT)
  * @param  DeviceAddr: Device address on communication Bus.
  * @param  feature: Camera feature to be configured
  * @param  value: Value to be configured
  * @param  brightness_value: Brightness value to be configured
  * @retval None
  */
void ov9655_Config(uint16_t DeviceAddr, uint32_t feature, uint32_t value, uint32_t brightness_value)
{
  uint8_t value1, value2;
  uint32_t value_tmp;
  uint32_t br_value;

  /* Convert the input value into ov2640 parameters */
//  value_tmp = ov2640_ConvertValue(feature, value);
//  br_value = ov2640_ConvertValue(CAMERA_CONTRAST_BRIGHTNESS, brightness_value);

  switch(feature)
  {
  case CAMERA_BLACK_WHITE:
    {
      CAMERA_IO_Write(DeviceAddr, 0xff, 0x00);
      CAMERA_IO_Write(DeviceAddr, 0x7c, 0x00);
      CAMERA_IO_Write(DeviceAddr, 0x7d, value_tmp);
      CAMERA_IO_Write(DeviceAddr, 0x7c, 0x05);
      CAMERA_IO_Write(DeviceAddr, 0x7d, 0x80);
      CAMERA_IO_Write(DeviceAddr, 0x7d, 0x80);
      break;
    }
  case CAMERA_CONTRAST_BRIGHTNESS:
    {
      value1 = (uint8_t)(value_tmp);
      value2 = (uint8_t)(value_tmp >> 8);
      CAMERA_IO_Write(DeviceAddr, 0xff, 0x00);
      CAMERA_IO_Write(DeviceAddr, 0x7c, 0x00);
      CAMERA_IO_Write(DeviceAddr, 0x7d, 0x04);
      CAMERA_IO_Write(DeviceAddr, 0x7c, 0x07);
      CAMERA_IO_Write(DeviceAddr, 0x7d, br_value);
      CAMERA_IO_Write(DeviceAddr, 0x7d, value1);
      CAMERA_IO_Write(DeviceAddr, 0x7d, value2);
      CAMERA_IO_Write(DeviceAddr, 0x7d, 0x06);
      break;
    }
  case CAMERA_COLOR_EFFECT:
    {
      value1 = (uint8_t)(value_tmp);
      value2 = (uint8_t)(value_tmp >> 8);
      CAMERA_IO_Write(DeviceAddr, 0xff, 0x00);
      CAMERA_IO_Write(DeviceAddr, 0x7c, 0x00);
      CAMERA_IO_Write(DeviceAddr, 0x7d, 0x18);
      CAMERA_IO_Write(DeviceAddr, 0x7c, 0x05);
      CAMERA_IO_Write(DeviceAddr, 0x7d, value1);
      CAMERA_IO_Write(DeviceAddr, 0x7d, value2);
      break;
    }
  default:
    {
      break;
    }
  }
}

/**
  * @brief  Read the OV9655 Camera identity.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval the OV9655 ID
  */
uint16_t ov9655_ReadID(uint16_t DeviceAddr)
{
  /* Initialize I2C */
  CAMERA_IO_Init();

  /* Get the camera ID */
  return (CAMERA_IO_Read(DeviceAddr, OV9655_PID));
}


/**
  * @brief  Resets the OV9655 camera.
  * @param  None
  * @retval None
  */
void ov9655_Reset(uint16_t DeviceAddr)
{
  CAMERA_IO_Write(DeviceAddr, OV9655_COM7, 0x80);
}


/**
  * @brief  Configures the OV9655 camera brightness.
  * @param  Brightness: Brightness value, where Brightness can be:
  *         positively (0x01 ~ 0x7F) and negatively (0x80 ~ 0xFF)
  * @retval None
  */
void ov9655_BrightnessConfig(uint16_t DeviceAddr, uint8_t Brightness)
{
    CAMERA_IO_Write(DeviceAddr, OV9655_BRTN, Brightness);
}

/******************************************************************************
                            Static Functions
*******************************************************************************/
/**
  * @brief  Convert input values into ov9655 parameters.
  * @param  feature: Camera feature to be configured
  * @param  value: Value to be configured
  * @retval The converted value
  */
//static uint32_t ov9655_ConvertValue(uint32_t feature, uint32_t value)
//{
//  uint32_t ret = 0;
//
//  switch(feature)
//  {
//  case CAMERA_BLACK_WHITE:
//    {
//      switch(value)
//      {
//      case CAMERA_BLACK_WHITE_BW:
//        {
//          ret =  OV9655_BLACK_WHITE_BW;
//          break;
//        }
//      case CAMERA_BLACK_WHITE_NEGATIVE:
//        {
//          ret =  OV2640_BLACK_WHITE_NEGATIVE;
//          break;
//        }
//      case CAMERA_BLACK_WHITE_BW_NEGATIVE:
//        {
//          ret =  OV2640_BLACK_WHITE_BW_NEGATIVE;
//          break;
//        }
//      case CAMERA_BLACK_WHITE_NORMAL:
//        {
//          ret =  OV2640_BLACK_WHITE_NORMAL;
//          break;
//        }
//      default:
//        {
//          ret =  OV2640_BLACK_WHITE_NORMAL;
//          break;
//        }
//      }
//      break;
//    }
//  case CAMERA_CONTRAST_BRIGHTNESS:
//    {
//      switch(value)
//      {
//      case CAMERA_BRIGHTNESS_LEVEL0:
//        {
//          ret =  OV2640_BRIGHTNESS_LEVEL0;
//          break;
//        }
//      case CAMERA_BRIGHTNESS_LEVEL1:
//        {
//          ret =  OV2640_BRIGHTNESS_LEVEL1;
//          break;
//        }
//      case CAMERA_BRIGHTNESS_LEVEL2:
//        {
//          ret =  OV2640_BRIGHTNESS_LEVEL2;
//          break;
//        }
//      case CAMERA_BRIGHTNESS_LEVEL3:
//        {
//          ret =  OV2640_BRIGHTNESS_LEVEL3;
//          break;
//        }
//      case CAMERA_BRIGHTNESS_LEVEL4:
//        {
//          ret =  OV2640_BRIGHTNESS_LEVEL4;
//          break;
//        }
//      case CAMERA_CONTRAST_LEVEL0:
//        {
//          ret =  OV2640_CONTRAST_LEVEL0;
//          break;
//        }
//      case CAMERA_CONTRAST_LEVEL1:
//        {
//          ret =  OV2640_CONTRAST_LEVEL1;
//          break;
//        }
//      case CAMERA_CONTRAST_LEVEL2:
//        {
//          ret =  OV2640_CONTRAST_LEVEL2;
//          break;
//        }
//      case CAMERA_CONTRAST_LEVEL3:
//        {
//          ret =  OV2640_CONTRAST_LEVEL3;
//          break;
//        }
//      case CAMERA_CONTRAST_LEVEL4:
//        {
//          ret =  OV2640_CONTRAST_LEVEL4;
//          break;
//        }
//      default:
//        {
//          ret =  OV2640_CONTRAST_LEVEL0;
//          break;
//        }
//      }
//      break;
//    }
//  case CAMERA_COLOR_EFFECT:
//    {
//      switch(value)
//      {
//      case CAMERA_COLOR_EFFECT_ANTIQUE:
//        {
//          ret =  OV2640_COLOR_EFFECT_ANTIQUE;
//          break;
//        }
//      case CAMERA_COLOR_EFFECT_BLUE:
//        {
//          ret =  OV2640_COLOR_EFFECT_BLUE;
//          break;
//        }
//      case CAMERA_COLOR_EFFECT_GREEN:
//        {
//          ret =  OV2640_COLOR_EFFECT_GREEN;
//          break;
//        }
//      case CAMERA_COLOR_EFFECT_RED:
//        {
//          ret =  OV2640_COLOR_EFFECT_RED;
//          break;
//        }
//      default:
//        {
//          ret =  OV2640_COLOR_EFFECT_RED;
//          break;
//        }
//      }
//      break;
//    default:
//      {
//        ret = 0;
//        break;
//      }
//    }
//  }
//
//  return ret;
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

/***************END OF FILE****************/
