#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\LCD.h"
//https://github.com/johnrickman/LiquidCrystal_I2C
//SDA-P21
//SCL-P22
#pragma once

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "Controler.h"

#define WIDTH 84
#define HEIGHT 48
#define FONT_SIZE 7
#define PADING 1


class LCD
{

public:
   InterfaceControler *lcdControler;

   static LCD &getInstance()
   {
      static LCD instance;
      return instance;
   }

   ~LCD()
   {
      delete lcdControler;
   }

   void operator=(LCD const &) = delete;
   LCD(LCD const &) = delete;

   void onPassive();
   void writeToLine(int,char * ,uint16_t=BLACK,uint16_t =WHITE);
   // void writeToLine(int,char *);
   void drawRect(int line, uint16_t=BLACK);
   void drawLine(int line, uint16_t=BLACK );
   void setCursor(int x,int y);
private:
   LCD();
   Adafruit_PCD8544 lcd;
   bool isOn = true;
   int previousLine;
};
