#include <SPI.h>
//EPD
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "demo.h"

void setup() {
  pinMode(D5, INPUT);   //BUSY
  pinMode(D0, OUTPUT);  //RES
  pinMode(D3, OUTPUT);  //DC
  pinMode(D1, OUTPUT);  //CS
  //SPI
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
}

//Tips//
/*
1.Flickering is normal when EPD is performing a full screen update to clear ghosting from the previous image so to ensure better clarity and legibility for the new image.
2.There will be no flicker when EPD performs a partial refresh.
3.Please make sue that EPD enters sleep mode when refresh is completed and always leave the sleep mode command. Otherwise, this may result in a reduced lifespan of EPD.
4.Please refrain from inserting EPD to the FPC socket or unplugging it when the MCU is being powered to prevent potential damage.)
5.Re-initialization is required for every full screen update.
6.When porting the program, set the BUSY pin to input mode and other pins to output mode.
*/
void loop() {

  EPD_HW_Init();                     //Full screen refresh initialization.
  EPD_WhiteScreen_White();           //Clear screen function.
  EPD_DeepSleep();                   //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
  delay(2000);                       //Delay for 2s.
                                     /************Full display(2s)*******************/
  EPD_HW_Init();                     //Full screen refresh initialization.
  EPD_WhiteScreen_ALL(gImage_demo);  //To Display one image using full screen refresh.
  EPD_DeepSleep();                   //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
  delay(2000);
  ;  //Delay for 2s.

  delay(300000);  // The program stops here   
}




//////////////////////////////////END//////////////////////////////////////////////////
