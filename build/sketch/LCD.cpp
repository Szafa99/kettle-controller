#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\LCD.cpp"
#pragma once

#include "LCD.h"
#include "MainUi.h"
#include "EngineUI.h"
#include "MachineUI.h"
#include "DS18B20_UI.h"

LCD::LCD() : lcd(Adafruit_PCD8544(18, 23, 4, 15, 2))
{

    lcd.begin();
    lcd.clearDisplay();
    lcd.setContrast(60);
    lcd.setTextSize(1);
    lcd.setTextColor(BLACK, WHITE);
    lcd.display();
    delay(100);
    
    lcdControler = &MainUI::getInstance();
    previousLine = -1;
}

// void LCD::writeToLine(int line, char *text)
// {
//     lcd.setCursor(0, line*8);
//     lcd.println(text);
//     lcd.display();
// }

void LCD::writeToLine(int line, char *text, uint16_t color, uint16_t bg)
{

    lcd.setTextColor(color, bg);

    if (line == 0)
        lcd.setCursor(0, line * FONT_SIZE);
    else
        lcd.setCursor(0, line * FONT_SIZE+PADING*line);

    lcd.println(text);
    lcd.display();
}

void LCD::drawRect(int line, uint16_t color)
{
    int y = (line)*FONT_SIZE + (PADING * line);
    lcd.drawRect(0, y, WIDTH, FONT_SIZE + 1, color);

    lcd.display();
}

void LCD::drawLine(int line, uint16_t color)
{
    if (previousLine!=-1 && previousLine!=line)
        drawLine(previousLine, WHITE);
    previousLine = line;

    int y = (line + 1) * FONT_SIZE + (PADING * line);
    lcd.drawLine(0, y, WIDTH, y, color);
    lcd.display();
}


void LCD::setCursor(int x,int y){
      
      lcd.setCursor(x,y);
   }
