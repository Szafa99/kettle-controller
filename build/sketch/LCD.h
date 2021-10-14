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
#define FONT_Y_SIZE 8
#define FONT_X_SIZE 6
#define PADING 1
#define OPTION_OFFSET 6

#define LEFT_REC 0, 0
#define RIGHT_REC WIDTH / 2 - PADING, 0
#define REC_SIZE WIDTH / 2, FONT_Y_SIZE * 2 + PADING * 2

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
   void writeToLine(int, char *, uint16_t = BLACK, uint16_t = WHITE);
   // void writeToLine(int,char *);
   void drawRect(int x, int y, uint16_t color = BLACK);
   //6 chars for one line. 1-FIRST LINE ,2-SECONDS LINE
   void writeToRec(int x, int y, char *text, int line = 2);
   // MAX 12 chars for one line
   void writeOption(char *text, int line = 1);
   // drawLine Wrapper
   void drawSelectLineInOptionWindow(int x1, int y1, uint16_t color,int length=FONT_X_SIZE*2);

   void clear();
   void drawBitMap(int x, int y, const unsigned char *map, int w,int h,  uint16_t c);

private:
   LCD();
   Adafruit_PCD8544 lcd;
   bool isOn = true;
   int previousLine;
};
