#include <SPI.h>
#include"demo.h"
//IO settings
int BUSY_Pin = D5; 
int RES_Pin = D0; 
int DC_Pin = D3; 
int CS_Pin = D1; 
//SCLK--GPIO23
//MOSI---GPIO18

#define EPD_W21_CS_0 digitalWrite(CS_Pin,LOW)
#define EPD_W21_CS_1 digitalWrite(CS_Pin,HIGH)
#define EPD_W21_DC_0  digitalWrite(DC_Pin,LOW)
#define EPD_W21_DC_1  digitalWrite(DC_Pin,HIGH)
#define EPD_W21_RST_0 digitalWrite(RES_Pin,LOW)
#define EPD_W21_RST_1 digitalWrite(RES_Pin,HIGH)
#define isEPD_W21_BUSY digitalRead(BUSY_Pin)

//8bit
#define Black   0x00  /// 000
#define White   0x11  /// 001
#define Green   0x22  /// 010
#define Blue    0x33  /// 011
#define Red     0x44  /// 100
#define Yellow  0x55  /// 101
#define Orange  0x66  /// 110
#define Clean   0x77  /// 111

//4bit
#define black   0x00  /// 000
#define white   0x01  /// 001
#define green   0x02  /// 010
#define blue    0x03  /// 011
#define red     0x04  /// 100
#define yellow  0x05  /// 101
#define orange  0x06  /// 110
#define clean   0x07  /// 111
////////FUNCTION//////
void driver_delay_us(unsigned int xus);
void driver_delay(unsigned long xms);
void DELAY_S(unsigned int delaytime);     
void SPI_Delay(unsigned char xrate);
void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char command);
void EPD_W21_WriteCMD(unsigned char command);
//EPD
void EPD_W21_Init(void);
void EPD_init(void);
void PIC_display(const unsigned char* picData);
void EPD_sleep(void);
void EPD_refresh(void);
void lcd_chkstatus(void);
void PIC_display_Clear(void);
void EPD_horizontal(void);
void EPD_vertical(void);
void Acep_color(unsigned char color);

void setup() {
   pinMode(BUSY_Pin, INPUT); 
   pinMode(RES_Pin, OUTPUT);  
   pinMode(DC_Pin, OUTPUT);    
   pinMode(CS_Pin, OUTPUT);   
   //SPI
   SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); 
   SPI.begin ();    
}


//Tips//
/*When the electronic paper is refreshed in full screen, the picture flicker is a normal phenomenon, and the main function is to clear the display afterimage in the previous picture.
  When the local refresh is performed, the screen does not flash.*/
/*When you need to transplant the driver, you only need to change the corresponding IO. The BUSY pin is the input mode and the others are the output mode. */


void loop() {

    EPD_init(); //EPD init
    PIC_display(gImage_demo);//EPD_picture1
    EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    delay(5000); //5s  
    //EPD_horizontal
    EPD_init(); //EPD init
    EPD_horizontal();//EPD  horizontal 7 color
    EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    delay(5000); //5s  
    
    EPD_init(); //EPD init
    EPD_vertical();//EPD  vertical 7 color
    EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    delay(5000); //5s  
    
    //Clear
    EPD_init(); //EPD init
    PIC_display_Clear();//EPD Clear
    EPD_sleep();//EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    delay(5000); //5s

    delay(300000);  // The program stops here   
}




///////////////////EXTERNAL FUNCTION////////////////////////////////////////////////////////////////////////
/////////////////////delay//////////////////////////////////////
void driver_delay_us(unsigned int xus)  //1us
{
  for(;xus>1;xus--);
}
void driver_delay(unsigned long xms) //1ms
{  
    unsigned long i = 0 , j=0;

    for(j=0;j<xms;j++)
  {
        for(i=0; i<256; i++);
    }
}
void DELAY_S(unsigned int delaytime)     
{
  int i,j,k;
  for(i=0;i<delaytime;i++)
  {
    for(j=0;j<4000;j++)           
    {
      for(k=0;k<222;k++);
                
    }
  }
}
//////////////////////SPI///////////////////////////////////
void SPI_Delay(unsigned char xrate)
{
  unsigned char i;
  while(xrate)
  {
    for(i=0;i<2;i++);
    xrate--;
  }
}


void SPI_Write(unsigned char value)                                    
{                                                           
    SPI.transfer(value);

}

void EPD_W21_WriteCMD(unsigned char command)
{
  SPI_Delay(1);
  EPD_W21_CS_0;                   
  EPD_W21_DC_0;   // command write
  SPI_Write(command);
  EPD_W21_CS_1;
}
void EPD_W21_WriteDATA(unsigned char command)
{
  SPI_Delay(1);
  EPD_W21_CS_0;                   
  EPD_W21_DC_1;   // command write
  SPI_Write(command);
  EPD_W21_CS_1;
}



/////////////////EPD settings Functions/////////////////////
void EPD_W21_Init(void)
{
  EPD_W21_RST_0;    // Module reset
  delay(100); //At least 10ms
  EPD_W21_RST_1;
  delay(100);  
}
void EPD_init(void)
{
    EPD_W21_Init(); //Electronic paper IC reset   
  
    EPD_W21_WriteCMD(0x01);    //POWER SETTING
    EPD_W21_WriteDATA (0x37);    
    EPD_W21_WriteDATA (0x00);
    EPD_W21_WriteDATA (0x23);
    EPD_W21_WriteDATA (0x23);
  
    EPD_W21_WriteCMD(0X00);     //PANNEL SETTING
    EPD_W21_WriteDATA(0xEF);
    EPD_W21_WriteDATA(0x08);

    EPD_W21_WriteCMD(0x03);     //PFS
    EPD_W21_WriteDATA (0x00);   

    EPD_W21_WriteCMD(0x06);         //boostÉè¶¨
    EPD_W21_WriteDATA (0xC7);     
    EPD_W21_WriteDATA (0xC7);
    EPD_W21_WriteDATA (0x1D);

  
    EPD_W21_WriteCMD(0x30);     //PLL setting
    EPD_W21_WriteDATA (0x3C);   //PLL:    0-25¡æ:0x3C,25+:0x3A   
  
    EPD_W21_WriteCMD(0X41);     //TSE
    EPD_W21_WriteDATA(0x00);
  
    EPD_W21_WriteCMD(0X50);     //VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0x37); //0x77

    EPD_W21_WriteCMD(0X60);     //TCON SETTING
    EPD_W21_WriteDATA(0x22);

    EPD_W21_WriteCMD(0X60);     //TCON SETTING
    EPD_W21_WriteDATA(0x22);

    EPD_W21_WriteCMD(0x61);         //tres      
    EPD_W21_WriteDATA (0x02);   //source 600
    EPD_W21_WriteDATA (0x58);
    EPD_W21_WriteDATA (0x01);   //gate 448
    EPD_W21_WriteDATA (0xC0);

    EPD_W21_WriteCMD(0xE3);     //PWS     
    EPD_W21_WriteDATA(0xAA);  

    EPD_W21_WriteCMD(0x04);     //PWR on  
    lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal

}
void EPD_refresh(void)
{
    EPD_W21_WriteCMD(0x12);     //DISPLAY REFRESH   
    driver_delay(100);          //!!!The delay here is necessary, 200uS at least!!!     
    lcd_chkstatus();
} 
void EPD_sleep(void)
{
    EPD_W21_WriteCMD(0X02);   //power off
    lcd_chkstatus();
    EPD_W21_WriteCMD(0X07);   //deep sleep
    EPD_W21_WriteDATA(0xA5);
}


/**********************************display picture**********************************/  
void Acep_color(unsigned char color)
{
  unsigned int i,j;
  EPD_W21_WriteCMD(0x10);        
  for(i=0;i<448;i++)
  {
    for(j=0;j<300;j++)
    {
      EPD_W21_WriteDATA(color);
    }
  }
  
  //Refresh
  EPD_W21_WriteCMD(0x12);   //DISPLAY REFRESH   
  delay(1);              //!!!The delay here is necessary, 200uS at least!!!     
  lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal

}
/*
White   /// 001--0XFF
Yellow  /// 101--0XFC
Orange  /// 110--0XEC
Red     /// 100--0XE0
Green   /// 010--0X35
Blue    /// 011--0X2B
Black   /// 000--0X00

//0XFF,0XFC,0XEC,0XE0,0X35,0X2B,0X00
*/
unsigned char Color_get(unsigned char color)
{
  unsigned datas;
  switch(color)
  {
    case 0xFF:
      datas=white;
    break;
    case 0xFC:
      datas=yellow;
    break;  
    case 0xEC:
      datas=orange;
    break;      
    case 0xE0:
      datas=red;
    break;  
    case 0x35:
      datas=green;
    break;  
    case 0x2B:
      datas=blue;
    break;  
    case 0x00:
      datas=black;
    break;    
    default:
    break;      
  }
   return datas;
}
void PIC_display(const unsigned char* picData)
{
  unsigned int i,j,k;
  unsigned char temp1,temp2;
  unsigned char data_H,data_L,data;
  

  Acep_color(Clean); //Each refresh must be cleaned first 
  EPD_W21_WriteCMD(0x10);        
  for(i=0;i<448;i++)
  { 
    k=0;
    for(j=0;j<300;j++)
    {
      
      temp1=picData[i*600+k++]; 
      temp2=picData[i*600+k++];
      data_H=Color_get(temp1)<<4;
      data_L=Color_get(temp2);
      data=data_H|data_L;
      EPD_W21_WriteDATA(data);
    }
  } 
  
   //Refresh
    EPD_W21_WriteCMD(0x12);   //DISPLAY REFRESH   
    delay(1);   //!!!The delay here is necessary, 200uS at least!!!     
    lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal

}


void EPD_horizontal(void)
{
  unsigned int i,j;
  unsigned char index = 0x00;
  unsigned char const Color[8] = {Black,White,Green,Blue,Red,Yellow,Orange,Clean};

 Acep_color(Clean); //Each refresh must be cleaned first    
  EPD_W21_WriteCMD(0x10);      //start to transport picture
  for(i=0;i<448;i++){
    if((i%56 == 0) && (i != 0))
      index ++;
    for(j =0;j<600/2;j++){
      EPD_W21_WriteDATA(Color[index]);
    }
  }
   //Refresh
    EPD_W21_WriteCMD(0x12);   //DISPLAY REFRESH   
    delay(1);   //!!!The delay here is necessary, 200uS at least!!!     
    lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal

}
void EPD_vertical(void)
{
  unsigned int i,j,k;
  unsigned char const Color[8] = {Black,White,Green,Blue,Red,Yellow,Orange,Clean};
  
  Acep_color(Clean); //Each refresh must be cleaned first 
  EPD_W21_WriteCMD(0x10);      //start to transport pictu
  for(i=0;i<448;i++)
  {
    for(k = 0 ; k < 7; k ++)  //7 color
    {
      for(j = 0 ; j < 38; j ++)
      {
        EPD_W21_WriteDATA(Color[k]);
      }
    }
    for(j = 0; j <34 ; j++) 
    {
      EPD_W21_WriteDATA(0x77);
    }
  }
   //Refresh
    EPD_W21_WriteCMD(0x12);   //DISPLAY REFRESH   
    delay(1);   //!!!The delay here is necessary, 200uS at least!!!     
    lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal

}

void PIC_display_Clear(void)
{
   unsigned int i,j;
  Acep_color(Clean); //Each refresh must be cleaned first 
  EPD_W21_WriteCMD(0x10);        
  for(i=0;i<448;i++)
  {
    for(j=0;j<300;j++)
    {
      EPD_W21_WriteDATA(White);
    }
  }
  
  //Refresh
  EPD_W21_WriteCMD(0x12);   //DISPLAY REFRESH   
  delay(1);              //!!!The delay here is necessary, 200uS at least!!!     
  lcd_chkstatus();          //waiting for the electronic paper IC to release the idle signal

}

void lcd_chkstatus(void)
{
  while(!isEPD_W21_BUSY);                         
}
