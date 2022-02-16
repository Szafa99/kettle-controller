#pragma once

#include "LCD.h"
#include "MainUi.h"

LCD::LCD() : lcd(Adafruit_PCD8544(
    18,  // SCLK PIN
    23,  // DIN pin 
    19,   // DC PIN
    21,  // CS PIN
    3))  //RESET PIN-RX0
{


    lcd.begin();
    lcd.clearDisplay();
    lcd.setContrast(60);
    lcd.setTextSize(1);
    lcd.setTextColor(BLACK, WHITE);
    lcd.display();
    delay(100);

    timeSincelastDisplay=0;
    pinMode(BACKLIGHT_PIN,OUTPUT);
    digitalWrite(BACKLIGHT_PIN,LOW);
    lcdControler = &MainUI::getInstance();
    previousLine = -1;

    // pinMode(4, OUTPUT);
    // pinMode(15, OUTPUT);
    // digitalWrite(4, LOW);
    // digitalWrite(15, LOW);

}

void LCD::writeToLine(int line, char *text, uint16_t color, uint16_t bg)
{
    lcd.setTextColor(color, bg);

    if (line == 0)
        lcd.setCursor(0, line * FONT_Y_SIZE);
    else
        lcd.setCursor(0, line * FONT_Y_SIZE + PADING * line);

    lcd.println(text);
    display();
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
    display();
}

void LCD::drawRect(int x, int y, uint16_t color)
{
    lcd.drawRect(x, y, REC_SIZE, color);
    display();
}

void LCD::drawSelectLineInOptionWindow(int x1, int y1, uint16_t color, int length)
{
    lcd.drawFastHLine(OPTION_OFFSET + (x1 * FONT_X_SIZE), y1, length, color);
    display();
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
    display();

    if(!isOn) {
        digitalWrite(BACKLIGHT_PIN,LOW);
            isOn=true;
    }
    timeSincelastDisplay=(millis()/1000);


}

void LCD::clear()
{
    lcd.clearDisplay();
    display();
}

void LCD::drawBitMap(int x, int y, const unsigned char *map, int w, int h, uint16_t color)
{
    lcd.drawBitmap(x, y, map, w, h, color);
    display();
}

void LCD::display()
{
    lcd.display();
}




void LCD::onPassive(){
    if ( isOn && (millis()/1000) - timeSincelastDisplay  >MAX_PASIVE_TIME ){
            digitalWrite(BACKLIGHT_PIN,HIGH);
        isOn=false;
    }
}