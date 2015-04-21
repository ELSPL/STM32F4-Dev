/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the GPIO functions \n
*         -   Study the programs related to the LCD Display.
*
* @section section2 Example6 :
*    Objective: Program to display Empty Triangle for one character.
*
* @section section3 Program Description:
*    This program demonstrates interfacing of LCD display and display empty triangle on it
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref stm32f4_ask25_lcd.h  | @ref stm32f4_ask25_lcd.c  |
*   |                           | @ref main.c               |
*
* \n
*
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device(ASK-25-LCD) |
*   | :------------------:| :----------------: |
*   | P1.21 GPIOB.1       | RS                 |
*   | P2.25 GPIOB.4       | R/W                |
*   | P2.26 GPIOB.5       | EN                 |
*   | P1.26 GPIOE.8       | D0                 |
*   | P1.27 GPIOE.9       | D1                 |
*   | P1.28 GPIOE.10      | D2                 |
*   | P1.29 GPIOE.11      | D3                 |
*   | P1.30 GPIOE.12      | D4                 |
*   | P1.31 GPIOE.13      | D5                 |
*   | P1.32 GPIOE.14      | D6                 |
*   | P1.33 GPIOE.15      | D7                 |
*
* @section section6 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | J6                  | ASK-25 (PL3)    |
*
* @section section7 Program Folder Location
*       <Eg6>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - Flat cable \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - ASK-25 Rev2.0 \n

*
* @section section9 Hardware Configuration
*   - Connect ASK 25 to educational practice board using flat cable.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Observe the Output.

*
* @section section10 Output:
*    On LCD display you will see a empty triangle for only one character on starting location
*\n
*\n
*******************************************************************************/
