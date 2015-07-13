/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the DAC functions \n
*         -   Study the programs related to the DAC conversion
*
* @section section2 Example4 :
*    Objective: Write a program to Generate sawtooth wave.
*
* @section section3 Program Description:
*    This program demonstrates generation of sawtooth wave
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   |                           | @ref main.c               |
*
* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Oscilloscope   |
*   | :------------------:| :-------------:|
*   | P1.15               |  Channel(CH1)  |
*
*
* @section section6 Program Folder Location
*       <Eg4>
*
*
* @section section7 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Oscilloscope \n
*
*
* @section section8 Hardware Configuration
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Connect Oscilloscope to STM32F407 Reference pin.
*   - Observe the Output.

*
* @section section10 Output:
*    Sawtooth wave will be generated and displayed on Oscilloscope.
*\n
*\n
*******************************************************************************/
