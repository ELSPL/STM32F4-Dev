/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the GPIO functions \n
*         -   Study the programs related to the various patterns of LED flashing
*
* @section section2 Example3 :
*    Objective: Write a Program to glow Alternate LEDs on and off
*
* @section section3 Program Description:
*    This program demonstrates blinking of Alternate LEDs on ASK-25.
*
* @section section4 Included Files:
*
*   | Header Files           	| Source Files           	|
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		| @ref stm32f4xx_it.c	 	|
*   |                        	| @ref main.c            	|
*
*
*

* \n
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device (ASK25) |
*   | :------------------:| :-------------:|
*   | P1.18               | LED1           |
*   | P1.19               | LED2           |
*   | P1.20               | LED3           |
*   | P1.21               | LED4           |
*   | P1.22               | LED5           |
*   | P1.23               | LED6           |
*   | P1.24               | LED7           |
*   | P1.25               | LED8           |
*
*
* @section section6 Connection
*   | STM32F407 Reference | Device         |
*   | :------------------:| :-------------:|
*   | J6                  | ASK25 (PL8)    |
*
* @section section7 Program Folder Location
*       <Eg3>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - 9V Power supply \n
*   - Flat cable \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - ASK-25 Rev2.0 \n

*
* @section section9 Hardware Configuration
*   - Connect 9V Supply to STM32F4Discovery board.
*   - Connect ASK 25 to educational practice board using flat cable.
*   - Connect the board using COM port of PC using USB cable.
*   - Using RUN/PROGRAM mode selection switch set the board in program mode.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (Flash Magic) download the .hex file developed using available tools.
*   - Using RUN/PROGRAM mode selection switch set the board in run mode and reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    LED1, 2, 3, 5, 6, 7, 8,10 blinks alternatively on ASK25.
*\n
*\n
*******************************************************************************/
