#include "LCD.h"
#include "Machine.h"
#include "EngineUI.h"
#include "EngineUI.h"
#include "MachineUI.h"
#include "DS18B20_UI.h"
#include "MainUi.h"
#include "Utils.h"



void MainUI::firstRender(){
    EngineUI::getInstance();
    MachineUI::getInstance();
    // DS18B20_UI::getInstance();
}

void MainUI::pushBtn() 
{
    switch (currentOption)
    {
    case AIMED_TEMP:
        // LCD::getInstance().lcdControler=&DS18B20_UI::getInstance();
        break;
    case TOGGLE_MACHINE:
        Machine::getInstance().togleMachine();
        timerAlarmEnable(Machine::getInstance().machineTimer);
        break;
    case MACHINE_ON_TIME:
        LCD::getInstance().lcdControler = &MachineUI::getInstance();
        
        break;
    case ENGINE_ON_CYCLE:
        LCD::getInstance().lcdControler = &EngineUI::getInstance();
        EngineUI::getInstance().setEngiePeriod(&Engine::getInstance().getEngineOnPeriod());
        break;
    case ENGINE_OFF_CYCLE:
        LCD::getInstance().lcdControler = &MachineUI::getInstance();
        EngineUI::getInstance().setEngiePeriod(&Engine::getInstance().getEngineOffPeriod());
        break;
    default:
        break;
    }
}

void MainUI::turnRight()
{
    switch (currentOption)
    {

    case AIMED_TEMP:
        currentOption = TOGGLE_MACHINE;
        break;
    case TOGGLE_MACHINE:
        currentOption = MACHINE_ON_TIME;
        break;
    case MACHINE_ON_TIME:
        currentOption = ENGINE_ON_CYCLE;
        break;
    case ENGINE_ON_CYCLE:
        currentOption = ENGINE_OFF_CYCLE;
        break;
    case ENGINE_OFF_CYCLE:
        currentOption = AIMED_TEMP;
        break;
    default:
        break;
    }
    LCD::getInstance().drawLine(currentOption);
}

void MainUI::turnLeft()
{

    switch (currentOption)
    {
    case AIMED_TEMP:
        currentOption = ENGINE_OFF_CYCLE;
        break;
    case TOGGLE_MACHINE:
        currentOption = AIMED_TEMP;
        break;
    case MACHINE_ON_TIME:
        currentOption = TOGGLE_MACHINE;
        break;
    case ENGINE_ON_CYCLE:
        currentOption = MACHINE_ON_TIME;
        break;
    case ENGINE_OFF_CYCLE:
        currentOption = ENGINE_ON_CYCLE;
        break;
    default:
        break;
    }
    LCD::getInstance().drawLine(currentOption);
}

void MainUI::render()
{
    // char text[16];

    // EngineUI::getInstance().render();
}

MainUI::MainUI()
{
    currentOption = AIMED_TEMP;
    // LCD::getInstance().drawLine(AIMED_TEMP);
}
