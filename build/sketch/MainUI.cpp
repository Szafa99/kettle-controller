#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\MainUI.cpp"
#include "LCD.h"
#include "Machine.h"
#include "MainUi.h"
#include "Utils.h"
#include "DS18B20.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

void MainUI::firstRender()
{
    char buff[7];
    sprintf(buff," %s",Machine::getInstance().workingTime.toString()); 
    LCD::getInstance().drawRect(LEFT_REC);
    LCD::getInstance().drawRect(RIGHT_REC);
    LCD::getInstance().writeToRec(LEFT_REC, "TEMP  ", 1);
    LCD::getInstance().writeToRec(RIGHT_REC, "CZAS  ", 1);
    LCD::getInstance().writeToRec(RIGHT_REC, buff , 2);
    LCD::getInstance().drawBitMap(LEFT_ARROW_POS, leftArrow, ARROW_SIZE, BLACK);
    LCD::getInstance().drawBitMap(RIGHT_ARROW_POS, rightArrow, ARROW_SIZE, BLACK);
    LCD::getInstance().drawBitMap(WIDTH-14,FONT_Y_SIZE/2,minMap,minSize,BLACK);
    LCD::getInstance().drawBitMap(WIDTH*0.44f,FONT_Y_SIZE*0.4f,celsMap,CELS_SIZE,BLACK);
    renderTemperature();
}

void MainUI::pushBtn()
{
    renderEnable = true;

    if (editMode)
    {
        switch (editOption)
        {
        case AIMED_TEMP:
            editMode = false;
            break;
        case MINUTES:
            editOption = SECONDS;
            break;
        case SECONDS:
            timerEditor = NULL;
            editMode = false;
            break;
        }
        return;
    }

    switch (currentOption)
    {
    case AIMED_TEMP:
        editMode = true;
        editOption = AIMED_TEMP;
        break;
    case TOGGLE_MACHINE:
        Machine::getInstance().togleMachine();
        break;
    case MACHINE_ON_TIME:
        timerEditor = &Machine::getInstance().workingTime;
        editOption = MINUTES;
        editMode = true;
        break;
    case ENGINE_ON_CYCLE:
        timerEditor = &Engine::getInstance().getEngineOnPeriod();
        editOption = MINUTES;
        editMode = true;
        break;
    case ENGINE_OFF_CYCLE:
        timerEditor = &Engine::getInstance().getEngineOffPeriod();
        editOption = MINUTES;
        editMode = true;
        break;
    default:
        break;
    }
}

void MainUI::turnRight()
{
    renderEnable = true;
    if (editMode)
    {
        switch (editOption)
        {
        case AIMED_TEMP:
            DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature() + 0.5f);
            break;
        case MINUTES:
            timerEditor->increaseMinutes();
            break;
        case SECONDS:
            timerEditor->increaseSeconds();
            break;
        }
        return;
    }

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
}

void MainUI::turnLeft()
{

    renderEnable = true;

    if (editMode)
    {
        switch (editOption)
        {
        case AIMED_TEMP:
            DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature() - 0.5f);
            break;
        case MINUTES:
            timerEditor->decreaseMinutes();
            break;
        case SECONDS:
            timerEditor->decreaseSeconds();
            break;
        }
        return;
    }

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
}

void MainUI::render()
{
    if (!renderEnable)
        return;



    switch (currentOption)
    {
    case TEMP:
        renderTemperature();
        break;
    case AIMED_TEMP:
        renderAimedTemperature();
        break;
    case TOGGLE_MACHINE:
        renderMachineState();
        break;
    case MACHINE_ON_TIME:
        renderMachineTime();
        break;
    case ENGINE_ON_CYCLE:
        renderEngineCycleOn();
        break;
    case ENGINE_OFF_CYCLE:
        renderEngineCycleOff();
        break;

    }

    if (editMode)
    {
        switch (editOption)
        {
        case AIMED_TEMP:{

            int lineLength = ( (abs(DS18B20::getInstance().getAimedTemperature()) -10.f)  > EPSILON ) ? 5 : 4 ;
            LCD::getInstance().
             drawSelectLineInOptionWindow( findCharPos(buff, '.') - (lineLength-3) , UNDERLINE_YPOS, BLACK,FONT_X_SIZE*lineLength);
        }
            break;
        case MINUTES:
            LCD::getInstance().drawSelectLineInOptionWindow((findCharPos(buff, ':') - 2), UNDERLINE_YPOS, BLACK);
            break;
        case SECONDS:
            LCD::getInstance().drawSelectLineInOptionWindow((findCharPos(buff, ':') + 1), UNDERLINE_YPOS, BLACK);
            break;
 
        }
    }

    renderEnable = false;
}

MainUI::MainUI()
{
    currentOption = AIMED_TEMP;
    renderEnable = true;
    editMode = false;
    timerEditor = NULL;
}

void MainUI::renderTemperature()
{
    char buf[6];
    sprintf(buf, "%.2fC", DS18B20::getInstance().getTemperature());

    LCD::getInstance().writeToRec(LEFT_REC, buf);
}

void MainUI::renderAimedTemperature()
{
    if (currentOption != AIMED_TEMP)
        return;
    sprintf(buff, "  CEL %.2f   ", DS18B20::getInstance().getAimedTemperature());

    LCD::getInstance().writeOption(buff);
    LCD::getInstance().drawBitMap(  OPTION_OFFSET+(findCharPos(buff,'.')+3)*FONT_X_SIZE ,FONT_Y_SIZE*2.5,celsMap,CELS_SIZE,BLACK);
    LCD::getInstance().writeOption(" ZMIEN TEMP ", 2);
}

void MainUI::renderMachineState()
{
    if (currentOption != TOGGLE_MACHINE)
        return;

    if (Machine::getInstance().isOn())
        LCD::getInstance().writeOption("   WYLACZ   ", 1);
    else
        LCD::getInstance().writeOption("    WLACZ   ", 1);

    LCD::getInstance().writeOption("   MASZYNA  ", 2);
}

void MainUI::renderMachineTime()
{

    sprintf(buff, " %s", Machine::getInstance().workingTime.toString());
    LCD::getInstance().writeToRec(RIGHT_REC, buff);

    if (currentOption != MACHINE_ON_TIME)
        return;
    sprintf(buff, "  %smin  ", Machine::getInstance().workingTime.toString());

    LCD::getInstance().writeOption(buff);
    // LCD::getInstance().drawBitMap(  OPTION_OFFSET+(findCharPos(buff,':')+3)*FONT_X_SIZE ,FONT_Y_SIZE*2.5,minMap,minSize,BLACK);
    LCD::getInstance().writeOption(" ZMIEN CZAS ", 2);

}

void MainUI::renderEngineCycleOn()
{
    if (currentOption != ENGINE_ON_CYCLE)
        return;
    sprintf(buff, "ON %s min", Engine::getInstance().getEngineOnPeriod().toString());

    LCD::getInstance().writeOption(buff);
    // LCD::getInstance().drawBitMap(  OPTION_OFFSET + ( findCharPos(buff,':')+3)*FONT_X_SIZE ,FONT_Y_SIZE*2.5,minMap,minSize,BLACK);
    LCD::getInstance().writeOption(" ZMIEN CYKL ", 2);
}

void MainUI::renderEngineCycleOff()
{
    if (currentOption != ENGINE_OFF_CYCLE)
        return;

    sprintf(buff, "OFF %smin", Engine::getInstance().getEngineOffPeriod().toString());

    LCD::getInstance().writeOption(buff);
    // LCD::getInstance().drawBitMap( OPTION_OFFSET + ( findCharPos(buff,':')+3) *FONT_X_SIZE ,FONT_Y_SIZE*2.5,minMap,minSize,BLACK);
    LCD::getInstance().writeOption(" ZMIEN CYKL ", 2);
}

int MainUI::findCharPos(char *buff, char c)
{
    int i = 0;
    while (buff[i] != '\0')
    {
        if (buff[i++] == c)
        {
            return (i - 1);
        }
    }

    return -1;
}
