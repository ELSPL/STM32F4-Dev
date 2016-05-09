Enable stm32f4 Wifi

steps:
 1. Open proprties of the Project
 2. Expand the C/C++ General option in left side window
 3. click on Paths and Symbols
 4. click on Symbols on the right side window
 5. select "GNU C" under languages
 6. click on ADD
 7. New window will open "Add Symbol"
 8. Add Name "USE_STM32F4_WIFI"
 9. click ok, Do not Rebuild the project
 10. click Apply
 11. click oK
 12. Right click on the project
 13. Go to index and click on "Freshen all files"


 Enable Uart6 in Polling Mode
 1. Open the stm32f4_discovery_uart.h file
 2. line no. 70 Enable polling mode and in line no. 71 disable interrupt mode