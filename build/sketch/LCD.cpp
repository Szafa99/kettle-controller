#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\LCD.cpp"
#pragma once

#include "LCD.h"
#include "MainUi.h"

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

void LCD::writeToLine(int line, char *text, uint16_t color, uint16_t bg)
{
    lcd.setTextColor(color, bg);

    if (line == 0)
        lcd.setCursor(0, line * FONT_Y_SIZE);
    else
        lcd.setCursor(0, line * FONT_Y_SIZE + PADING * line);

    lcd.println(text);
    lcd.display();
}

//6 chars for one line
void LCD::writeToRec(int x, int y, char *text, int line)
{

    lcd.setTextSize(1);
    char buff[6];
    sprintf(buff, "%.*s", 6, text);

    if (line == 1)
        lcd.setCursor(x + PADING * 2, y + PADING);
    else if (line == 2)
        lcd.setCursor(x + PADING * 2, y + PADING + FONT_Y_SIZE);

    lcd.println(buff);
    lcd.display();
}

void LCD::drawRect(int x, int y, uint16_t color)
{
    lcd.drawRect(x, y, REC_SIZE, color);
    lcd.display();
}

void LCD::drawSelectLineInOptionWindow(int x1, int y1, uint16_t color,int length)
{   
    lcd.drawFastHLine(OPTION_OFFSET + (x1*FONT_X_SIZE), y1,length,color);
    lcd.display();
}

void LCD::writeOption(char *text, int line)
{
    int length = 12;
    char buff[length + 1];
    sprintf(buff, "%.*s", length, text);
    lcd.setTextSize(1, 2);

    if (line == 1)
        lcd.setCursor(OPTION_OFFSET, FONT_Y_SIZE * 2 + PADING * 3);
    else if (line == 2)
        lcd.setCursor(OPTION_OFFSET, FONT_Y_SIZE * 4 + PADING * 2);

    lcd.println(buff);
    lcd.display();
}

void LCD::clear()
{
    lcd.clearDisplay();
}

void LCD::drawBitMap(int x, int y, const unsigned char *map, int w,int h, uint16_t color)
{
    lcd.drawBitmap(x, y, map, w,h, color);
    lcd.display();
}