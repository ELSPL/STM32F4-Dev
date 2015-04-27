/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the GPIO functions \n
*         -   Study the programs related to the DC Motor.
*
* @section section2 Example1 :
*    Objective:  Program to interface of DC motor and demonstrate Clock-wise movement.
*
* @section section3 Program Description:
*    This program demonstrates interfacing of DC Motor and demonstrate Clock-wise movement
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref stm32f4_ask25.h      | @ref stm32f4_ask25.c      |
*   |                           | @ref main.c               |
*
* \n
*
* @section section5 Pin Assignments
*
*   | STM32F407 Reference | Device(ASK-25)     |
*   | :------------------:| :----------------: |
*   | P1.21 GPIOB.1       | DC Motor           |
*   | P1.22 GPIOB.0       | DC Motor           |
*
*
* @section section6 Connection
*   | STM32F407 Reference | Device          |
*   | :------------------:| :-------------: |
*   | J6                  | ASK-25 (PL8)    |
*
* @section section7 Program Folder Location
*       <Eg1>
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
*    You will see the DC Motor rotating in clockwise direction
*\n
*\n
*******************************************************************************/
