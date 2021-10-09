#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\EngineUI.cpp"
#pragma once
#include "LCD.h"
#include "DS18B20_UI.h"
#include "Engine.h"
#include "Controler.h"
#include "Utils.h"
#include "EngineUI.h"
#include "MainUi.h"

void EngineUI::pushBtn()
{
    switch (currentOption)
    {
    case EngineUiOptions::MINUTES:
        currentOption = EngineUiOptions::SECONDS;
        break;
    case EngineUiOptions::SECONDS:
        LCD::getInstance().lcdControler = &MainUI::getInstance();
        currentOption = EngineUiOptions::MINUTES;
        break;
    default:
        break;
    }
}

void EngineUI::turnRight()
{
    switch (currentOption)
    {
    case EngineUiOptions::MINUTES:
        enginePeriod->increaseMinutes();
        break;
    case EngineUiOptions::SECONDS:
        (*enginePeriod)++;
        break;
    default:
        break;
    }
}

void EngineUI::turnLeft()
{
    switch (currentOption)
    {
    case EngineUiOptions::MINUTES:
        enginePeriod->decreaseMinutes();
        break;
    case EngineUiOptions::SECONDS:
        (*enginePeriod)--;
        break;
    default:
        break;
    }
    if (enginePeriod = &Engine::getInstance().getEngineOnPeriod())
        renderPeriodOn();
    else
        renderPeriodOff();
}

void EngineUI::renderPeriodOff()
{
    char text[16];
    sprintf(text, "Cykl OFF:%s", Engine::getInstance().getEngineOffPeriod().toString());
    LCD::getInstance().writeToLine(engineOffLine, text);
}

void EngineUI::renderPeriodOn()
{
    char text[16];
    sprintf(text, "Cykl ON:%s ", Engine::getInstance().getEngineOnPeriod().toString());
    LCD::getInstance().writeToLine(engineOnLine, text);
}

void EngineUI::render()
{
    renderPeriodOn();
    renderPeriodOff();
}

void EngineUI::setEngiePeriod(Utils::AlarmTime *period)
{
    enginePeriod = period;
}

EngineUI::EngineUI():
engineOnLine(RenderOptions::ENGINE_ON_CYCLE),
engineOffLine(RenderOptions::ENGINE_OFF_CYCLE)
{
    enginePeriod = &Engine::getInstance().getEngineOnPeriod();
    renderPeriodOn();
    renderPeriodOff();
    lastPageChange = millis();
    currentOption = EngineUiOptions::MINUTES;
}

EngineUI::~EngineUI()
{
    delete enginePeriod;
}
