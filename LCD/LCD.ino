#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
#include "pitches.h"
#define NOTE_C4_1 260

int buzzerPin = 40;
unsigned char const mapa1 [30][30] = {
  "########@#########@#",
  "#@@#@@@@@##@@#@@@@@#",
  "#@@#@@@@@##@@#@@@@@#",
  "#@@@@@@@@##@@@@@@@@#",
  "#@@@@@@@@##@@@@@@@@#",
  "#######@@########@@#",
  "#@@@@@@@@##@@@@@@@@#",
  "#@@@@@@@@##@@@@@@@@#",
  "#@@@@@@@@##@@@@@@@@#",
  "####@@#@@#####@@#@@#",
  "#@@@@@#@@##@@@@@#@@#",
  "#@@@@@#####@@@@@####",
  "#@@@@@@@@##@@@@@@@@#",
  "@@@#@@@@@#@@@#@@@@@#",
  "@@@#######@@@#######",
};

// notes in the melody:
int melody[] = {
   NOTE_C4_1,NOTE_C4, NOTE_D4, NOTE_C4,NOTE_F4,NOTE_E4,
   NOTE_C4_1,NOTE_C4,NOTE_D4,NOTE_C4,NOTE_G4,NOTE_F4,
   NOTE_C4_1,NOTE_C4,NOTE_C5,NOTE_A4,NOTE_F4,NOTE_F4, NOTE_E4,NOTE_D4,
   NOTE_AS4,NOTE_AS4,NOTE_A4,NOTE_F4,NOTE_G4,NOTE_F4};
   
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,4,4,2,1, 
  4, 4, 2, 2,2,1};


int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

void setup() {
 pinMode(buzzerPin,OUTPUT);
SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);
  FillRect(0, 0, 319, 239, 0x07FF);


  /***************************************************************************************************************************************
 INTRODUCCION
****************************************************************************************************************************************/
    String text1 = "HEEEEEYYY";
    String text2 = "WELCOME broooou";
    String text3 = "U are about to play ";
    String text4 = "THIS GAME!!!";
    String text5 = "WELCOME TO...";
    String text6 = "BEING ";
    String text7 = "f**kn ";
    String text8 = "LOST";
    String text9 = "BEING f**kn LOST";
    
    
    LCD_Print(text1, 100, 100,2, 0x0000, 0x07FF);
   delay(3000);
   
    LCD_Print(text2, 50, 100,2, 0x0000, 0x07FF);
     delay(2000);

    LCD_Print(text3, 0, 100,2, 0x0000, 0x07FF);
    LCD_Print(text4, 30, 120,2,  0xFFDF, 0x0000);
    
    delay(3000);
    LCD_Clear(0x07FF);
    LCD_Print(text5, 30, 120,2,  0xFFDF, 0x0000);
    delay(2000);
    LCD_Clear(0x07FF);
    
     LCD_Print(text6, 20, 100,2,  0xFFDF,   0xC0C3);
     delay(1000);
     LCD_Print(text7,116, 100,2,  0xFFDF,   0xC0C3);
       delay(1000);
     LCD_Print(text8,212, 100,2,  0xFFDF,   0xC0C3);
     delay(1000);
     LCD_Print(text9, 20, 100,2,  0xFFDF,   0xC0C3);
     delay(200);
     LCD_Print(text9, 20, 100,2,  0xFFDF,  0x0405);
      delay(200);
     LCD_Print(text9, 20, 100,2,  0xFFDF,   0xFCA5);
           delay(200);
     LCD_Print(text9, 20, 100,2,  0xFFDF,     0x7A68);
     delay(200);
     LCD_Print(text9, 20, 100,2,  0xFFDF,  0x5262);
     delay(200);
     LCD_Print(text9, 20, 100,2,  0xFFDF,  0xC0C3);
     delay(2000);
    LCD_Clear(    0x95DD);





    
     /***************************************************************************************************************************************
 Seteo de mapa
****************************************************************************************************************************************/
//for(int Y = 0; Y <239; Y++){
//H_line(157, Y, 2,     0x0343);
//}

//extremos
for(int Y = 0; Y <239-24-16; Y++){
  LCD_Bitmap(0 , Y, 16, 16, PARED);
  LCD_Bitmap(160, Y, 16, 16, PARED);

   Y += 15;
}
for(int Y = 0; Y <239; Y++){
    LCD_Bitmap(160-16, Y, 16, 16, PARED);
    LCD_Bitmap(320-16, Y, 16, 16, PARED);
   Y += 15;
  }
for(int X = 0; X <160-16-30; X++){
    LCD_Bitmap(X, 0, 16, 16, PARED);
    X += 15;       
  }
  for(int X = 160; X <320-16-30; X++){
    LCD_Bitmap(X, 0, 16, 16, PARED);
    X += 15;       
  }
//mapa
  for(int X = 16; X <160-16-16-16-16-16-16; X++){
    LCD_Bitmap(X, 192-16-16-16, 16, 16, PARED);
    X += 15;       
  }      
   for(int X = 160+16; X <320-16-16-16-16-16-16; X++){
    LCD_Bitmap(X, 192-16-16-16, 16, 16, PARED);
    X += 15;       
  }   
for(int y= 240-16-16; y <240; y++){
    LCD_Bitmap(0+24+24, y, 16, 16, PARED);
     LCD_Bitmap(160+24+24, y, 16, 16, PARED);
    y += 15;       
  }   

  for(int y= 0+16; y <0+16+16+16; y++){
    LCD_Bitmap(16+16+16, y, 16, 16, PARED);
     LCD_Bitmap(160+16+16+16, y, 16, 16, PARED);
    y += 15;       
  } 
   for(int X = 16+16+16+16; X <160-24; X++){
    LCD_Bitmap(X, 240-16, 16, 16, PARED);
    X += 15;       
  }  
  for(int X = 160+16+16+16+16; X <320-16; X++){
    LCD_Bitmap(X, 240-16, 16, 16, PARED);
    X += 15;       
  }  
  for(int X = 160+16+16+16+16+16+16; X <320-16; X++){
    LCD_Bitmap(X, 240-16-16-16-16, 16, 16, PARED);
    X += 15;       
  }  
for(int X = 0+16+16+16+16+16+16; X <160-16; X++){
    LCD_Bitmap(X, 240-16-16-16-16, 16, 16, PARED);
    X += 15;       
  }  
   for(int y= 240-16-16-16-16-16-16; y <240-16-16-16-16; y++){
    LCD_Bitmap(0+16+16+16+16+16+16, y, 16, 16, PARED);
     LCD_Bitmap(160+16+16+16+16+16+16, y, 16, 16, PARED);
    y += 15;       
  }   
  for(int X = 0+16; X <16*7; X++){
    LCD_Bitmap(X, 240-16-16-16-16-16-16-16-16-16-16, 16, 16, PARED);
    X += 15;       
  }  
    for(int X = 160+16; X <160+16*7; X++){
    LCD_Bitmap(X, 240-16-16-16-16-16-16-16-16-16-16, 16, 16, PARED);
    X += 15;       
  } 

////mario on place
 LCD_Bitmap(0+24, 240-24-24-20, 11, 18, MARIOstanded);

  LCD_Bitmap(160+24, 240-24-24-20, 11, 18, LUIGIstanded);
// for(int x = 0; x <320-32; x++){
//    delay(15);
//    int anim2 = (x/20)%4;
//    
//    LCD_Sprite(x,100,11,18,MARIO_sprite,6,anim2,0,1);
//    V_line( x -1, 100, 18, 0x95DD);  }
////  LCD_Sprite(2,100,16,24,planta,2,1,0,1);
 
       
//  for(int x = 0; x <319; x++){
//    LCD_Bitmap(x, 52, 16, 16, PARED);
//    LCD_Bitmap(x, 68, 16, 16, PARED);
//    
//    LCD_Bitmap(x, 207, 16, 16, PARED);
//    LCD_Bitmap(x, 223, 16, 16, PARED);
//    x += 15;
// }
//  
     
  //FillRect(0, 0, 319, 239, 0x95DD);
  

}

//player1:
//  if (digitalRead(PD_6) == LOW) {mov=1;}
//  if (digitalRead(PA_4) == LOW) {mov=2;}
//  if (digitalRead(PC_7) == LOW) {mov=3;}
//  if (digitalRead(PD_7) == LOW) {mov=4;}
//  switch (mov) {
//    case 1:
//    if (mapa1 [x+1][y] == '@'){
//      while (digitalRead(PD_6) == LOW) {
//        x++;
//        LCD_Sprite(int x, int y, int width, int height, MARIO_sprite,int columns, int index, char flip, char offset);
//      }
//    }
//  }

void loop() {
  // put your main code here, to run repeatedly: 
   for (int thisNote = 0; thisNote < 26; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration + 50;      //delay between pulse
    delay(pauseBetweenNotes);
    
    noTone(buzzerPin);                // stop the tone playing
  }
//  if (digitalRead(PD_6) == LOW){
// for(int x = 0; x <320-32; x++){
//  
//    delay(15);
//    int anim2 = (x/20)%4;
    
//    LCD_Sprite(x,100,11,18,MARIO_sprite,6,anim2,0,1);
//    V_line( x -1, 100, 18, 0x95DD);  }
    //}
  
}

//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}
