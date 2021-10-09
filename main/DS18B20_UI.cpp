#include "DS18B20_UI.h"
#include "DS18B20.h"
#include "MainUI.h"
#include "LCD.h"

DS18B20_UI::DS18B20_UI():
tempLine(RenderOptions::TEMP),
aimedTempLine(RenderOptions::AIMED_TEMP)
{
    renderAimedTemp(DS18B20::getInstance().getAimedTemperature());
    renderTemp(DS18B20::getInstance().getTemperature());
}

void DS18B20_UI::pushBtn()
{
    LCD::getInstance().lcdControler = &MainUI::getInstance();
};

void DS18B20_UI::turnRight()
{
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature() + 0.25f);
}

void DS18B20_UI::turnLeft()
{
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature() - 0.25f);
}

void DS18B20_UI::renderTemp(double temperature)
{
    char text[16];
    sprintf(text, "Mleko: %.2f C \0", temperature);
    LCD::getInstance().writeToLine(tempLine, text);
}

void DS18B20_UI::renderAimedTemp(double temperature)
{
    char text2[16];
    sprintf(text2, "Cel: %.2f C  \0", temperature);
    LCD::getInstance().writeToLine(aimedTempLine, text2);
}