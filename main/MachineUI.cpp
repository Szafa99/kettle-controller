#pragma once
#include "LCD.h"
#include "Machine.h"
#include "MachineUI.h"
#include "Controler.h"
#include "EngineUI.h"
#include "MainUi.h"

void MachineUI::pushBtn()
{
    switch (currentOption)
    {
    case Utils::MINUTES:
        currentOption = Utils::SECONDS;
        break;
    case Utils::SECONDS:
        currentOption = Utils::MINUTES;
        LCD::getInstance().lcdControler = &MainUI::getInstance();
    default:
        break;
    }
}

void MachineUI::turnRight()
{
    switch (currentOption)
    {
    case Utils::MINUTES:
        Machine::getInstance().workingTime.increaseMinutes();
        break;
    case Utils::SECONDS:
        Machine::getInstance().workingTime++;
        break;
    default:
        break;
    }
    renderTime();
}

void MachineUI::turnLeft()
{
    switch (currentOption)
    {
    case Utils::MINUTES:
        Machine::getInstance().workingTime.decreaseMinutes();
        break;
    case Utils::SECONDS:
        Machine::getInstance().workingTime--;
        break;
    default:
        break;
    }
    renderTime();
}

void MachineUI::renderTime()
{
    char text[16];
    sprintf(text, "Czas:%s min", Machine::getInstance().workingTime.toString());
    LCD::getInstance().writeToLine(timerLine, text);
}

void MachineUI::renderState()
{
    char text[16];
    sprintf(text,Machine::getInstance().isOn() ?
                                        "WYLACZ" : "WLACZ");
    LCD::getInstance().writeToLine(turnOnLine, text);
}

MachineUI::MachineUI():
timerLine(RenderOptions::MACHINE_ON_TIME),
turnOnLine(RenderOptions::TOGGLE_MACHINE)
{
    currentOption = Utils::MINUTES;
    renderTime();
    renderState();
}
