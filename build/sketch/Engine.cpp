#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\Engine.cpp"
#include "Engine.h"
#include "Machine.h"
#include "EngineUI.h"

void Engine::handleEngineCycles()
{
    if (Machine::getInstance().workingTime.timerEnded())
        return;

    if (isOffTime.toSeconds() > 0)
    {
        if (!isOffTime--)
            turnON();
    }
    else if (isOnTime.toSeconds() > 0)
    {
        if (!isOnTime--)
            turnOFF();
    }
    else
    {
        isOffTime = engineOffPeriod;
        isOnTime = engineOffPeriod;
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
}

Utils::AlarmTime &Engine::getEngineOffPeriod()
{
    return engineOffPeriod;
}

void Engine::setEngineOffPeriod(Utils::AlarmTime period)
{
    engineOffPeriod = period;
}