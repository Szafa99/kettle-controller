#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\Oled.h"
//https://github.com/johnrickman/LiquidCrystal_I2C
//SDA-P21
//SCL-P22
#pragma once

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Controler.h"
#include "Utils.h"

#define CLOCK_DIAMETER 10,10

const unsigned char CLOCK[] PROGMEM = {
  B00000000,B00000000,
  B00111110,B00000000,
  B01101011,B00000000,
  B11001001,B10000000,
  B10001000,B10000000,
  B10001111,B10000000,
  B10000000,B10000000,
  B11000001,B10000000,
  B01100011,B00000000,
  B00111110,B00000000,
  B00000000,B00000000,
  B00000000,B00000000
};


#define PROPELER_DIAMETER  10,10

const unsigned char PROPELER[] PROGMEM = {
  B00001000,B00000000,
  B00001000,B00000000,
  B00001000,B00000000,
  B00001000,B00000000,
  B00001000,B00000000,
  B00011100,B00000000,
  B01111111,B00000000,
  B11100011,B10000000,
  B11000001,B10000000
};


using namespace Utils;
class Oled{

    
    public:
     
     InterfaceControler * oledControler;
 
        
      static Oled& getInstance(){
          static  Oled instance;
          return instance;
      }

      ~Oled(){
        delete oledControler;
        }
     


     void operator=(Oled const&)=delete;
     Oled(Oled const&)=delete;

    void onPassive();

void writeText2Rec(int x,int y,char * text){
  display.setCursor(x+3,y+REC_INNER_HEIGHT_PADDING);  
  char line[6];
  strncpy(line, text, 6);
  line[6] = '\0'; 
  display.println(line);
  display.setCursor(x+3,y+REC_INNER_HEIGHT_PADDING*3);
  display.println(text+=6);

}

void drawBitMap2Rec(int x,int y,int mapHeight,int mapWidth,const unsigned char* bitMap,bool alignCenter=true,uint16_t color=SSD1306_WHITE,int pading=2){
  if(alignCenter)
    display.drawBitmap(x+ (REC_WIDTH-mapWidth)/2 ,y+pading,bitMap,mapWidth,mapHeight,color);
  else
    display.drawBitmap(x+pading ,y+pading,bitMap,mapWidth,mapHeight,color);
}

void drawRect(int x,int y){
  display.drawRoundRect(x, y, REC_WIDTH, REC_HEIGHT,REC_RADIOUS, SSD1306_WHITE);
  display.display();
}

void clearRect(int x,int y){
  display.drawRoundRect(x, y, REC_WIDTH, REC_HEIGHT,REC_RADIOUS, SSD1306_BLACK);
  display.display();
}

     private:      
     Oled(){
          display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
          if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;); 
        }
        display.clearDisplay();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        writeText2Rec(LEFT_UPPER,"MLEKO:99.99C");
        writeText2Rec(LEFT_DOWN," CEL: 99.99C");
        writeText2Rec(MIDLE_UPPER,"WLACZ KOCIOL");
        drawBitMap2Rec(MIDLE_DOWN,CLOCK_DIAMETER,CLOCK,false);
        writeText2Rec(MIDLE_DOWN,"  min 99:99");
        drawBitMap2Rec(RIGHT_UPPER,PROPELER_DIAMETER,PROPELER,false);
        drawBitMap2Rec(RIGHT_DOWN,PROPELER_DIAMETER,PROPELER,false);
        writeText2Rec(RIGHT_UPPER,"  ON  99:99m");
        writeText2Rec(RIGHT_DOWN,"  OFF 99:99m");
        drawRect(RIGHT_UPPER);

        
        display.display();
     };

    bool isOn=true; 
    Adafruit_SSD1306 display;


  };

 
