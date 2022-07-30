#include "Engine.h"
#include "Machine.h"
#define DEBUG 1

void Engine::handleEngineCycles()
{
    if (Machine::getInstance().getWorkingTime().timerEnded()||
        !Machine::getInstance().isOn())
        return;

    if (isOnTime.toSeconds() > 0)
    {
        if (!isOnTime--)
            turnOFF();
        else if(state==LOW)    
            turnON();
    }
    else if (isOffTime.toSeconds() > 0)
    {
        if (!isOffTime--)
            turnON();
        else if(state==HIGH)
            turnOFF();
    }

    else
    {
        isOffTime = engineOffPeriod;
        isOnTime = engineOnPeriod;
    }
}

Engine::Engine()
{
    state = LOW;
    pinMode(ENGINE_PIN, OUTPUT);
    digitalWrite(ENGINE_PIN, state);
    engineOnPeriod = Utils::AlarmTime(0, 0);
    engineOffPeriod = Utils::AlarmTime(0, 0);
    isOnTime = Utils::AlarmTime(0, 0);
    isOffTime = Utils::AlarmTime(0, 0);
    bool engineOnPeriodRender = false;
    bool engineOffPeriodRender = false;
    Serial2.println("sTARTING ENGINE");
}

void Engine::turnON()
{
    state = HIGH;
    digitalWrite(ENGINE_PIN, HIGH);
     #ifdef DEBUG
        Serial.println("Start engine");
     #endif
}

void Engine::turnOFF()
{
    state = LOW;
    digitalWrite(ENGINE_PIN, LOW);
    #ifdef DEBUG
        Serial.println("Stop engine");
    #endif
}

unsigned int Engine::getState()
{
    return state;
}

void Engine::setState(unsigned int state)
{
    this->state = state;
}

Utils::AlarmTime &Engine::getEngineOnPeriod()
{
    return engineOnPeriod;
}

void Engine::setEngineOnPeriod(Utils::AlarmTime period)
{
    this->engineOnPeriod = period;

    if(state==LOW || engineOnPeriod < isOnTime)
        isOnTime = engineOnPeriod;


    #ifdef DEBUG
        Serial.printf("Set engineOnPer: %s\n",engineOnPeriod.toString());
    #endif
    
}

Utils::AlarmTime &Engine::getEngineOffPeriod()
{
    return engineOffPeriod;
}

void Engine::setEngineOffPeriod(Utils::AlarmTime period)
{
    this->engineOffPeriod = period;
    if(state==HIGH || engineOffPeriod < isOffTime)
        isOffTime = engineOffPeriod;

    #ifdef DEBUG
        Serial.printf("Set engineOffPer: %s\n",engineOffPeriod.toString());
    #endif

}