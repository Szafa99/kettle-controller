#include "Engine.h"
#include "Machine.h"

void Engine::handleEngineCycles()
{
    if (Machine::getInstance().getWorkingTime().timerEnded() && 
        !Machine::getInstance().isOn())
        return;

    if (isOnTime.toSeconds() > 0)
    {
        if (!isOnTime--){
            Serial.printf("TX/ENGINE_OFF_PERIOD_STATE/RUNNING\n"); 
            turnOFF();
        }
        else if(state==LOW){    
            Serial.printf("TX/ENGINE_ON_PERIOD_STATE/RUNNING\n");  
            turnON();
        }
    }
    else if (isOffTime.toSeconds() > 0)
    {
        if (!isOffTime--){
            Serial.printf("TX/ENGINE_ON_PERIOD_STATE/RUNNING\n");
            turnON();
        }
        else if(state==HIGH){
            Serial.printf("TX/ENGINE_OFF_PERIOD_STATE/RUNNING\n"); 
            turnOFF();
        }
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
}

void Engine::turnON()
{
    state = HIGH;
    digitalWrite(ENGINE_PIN, HIGH);
}

void Engine::turnOFF()
{
    state = LOW;
    digitalWrite(ENGINE_PIN, LOW);
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
}

Utils::AlarmTime &Engine::getEngineOffPeriod()
{
    return engineOffPeriod;
}

Utils::AlarmTime &Engine::getEngineOffPeriodLeft()
{
    return isOffTime;
}

Utils::AlarmTime &Engine::getEngineOnPeriodLeft()
{
    return isOnTime;
}

void Engine::setEngineOffPeriod(Utils::AlarmTime period)
{
    this->engineOffPeriod = period;
    if(state==HIGH || engineOffPeriod < isOffTime || Machine::getInstance().isOn() == false )
        isOffTime = engineOffPeriod;
}