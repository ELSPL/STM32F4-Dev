/*******************************************************************************
*//**
* @mainpage
* @section section1 Introduction:
*         Having studied this LAB you will able to: \n
*         -   Understand the Ethernet functions \n
*         -   Study the programs related to the web server
*
* @section section2 Example6 :
*    Objective: Write a program to create web server
*
* @section section3 Program Description:
*    This program demonstrates the LED control using web
*
* @section section4 Included Files:
*
*   | Header Files           	  | Source Files           	  |
*   | :------------------------:| :------------------------:|
*   | @ref stm32f4xx_hal_conf.h | @ref stm32f4xx_hal_msp.c  |
*   | @ref stm32f4xx_it.h 		  | @ref stm32f4xx_it.c	 	    |
*   | @ref stm32f4_discovery.h  | @ref stm32f4_discovery.c  |
*   | @ref httpd.h              | @ref httpd.c              |
*   | @ref lwip.h               | @ref lwip.c               |
*   |                           | @ref main.c               |
*
*
* @section section5 Connection
*   | STM32F407 Reference |
*   | :------------------:|
*   | J1                  |
*
* @section section6 Program Folder Location
*       <Eg6>
*
*
* @section section8 Part List
*   - STM32F4Discovery Board \n
*   - USB cable \n
*   - Eclipse IDE \n
*   - PC \n
*   - Ethernet cable \n
*
*
* @section section9 Hardware Configuration
*   - Connect Ethernet cable between target board and PC.
*   - Connect the board using USB port of PC using USB cable.
*   - Apply Reset condition by pressing the Reset switch to ensure proper communication.
*   - Using download tool (STM ST-LINK Utility) download the .hex file developed using available tools.
*   - Reset the board.
*   - Open Web Browser and type 192.168.0.10. It will load a web a page.
*   - Observe the Output.

*
* @section section10 Output:
*    Led can easily accessible using web page
*\n
*\n
*******************************************************************************/
