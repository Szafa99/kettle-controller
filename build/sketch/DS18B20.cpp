#line 1 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\DS18B20.cpp"
#pragma once
#include "heater.h"
#include "DS18B20.h"
#include "Machine.h"


void DS18B20::connectDS18B20()
{
    dsb1820Wrapper.begin();
    dsb1820Wrapper.setWaitForConversion(true);
    dsb1820Wrapper.requestTemperatures();
    temperature = dsb1820Wrapper.getTempCByIndex(0);
    dsb1820Wrapper.setWaitForConversion(false);
}

bool DS18B20::reachedTemperature()
{
    bool reached = (aimedTemperature-0.5f < temperature);
    return reached;
}

void DS18B20::updateTemperature()
{
    
    if (seconds() - lastUpdate > 10 && dsb1820Wrapper.isConversionComplete())
    {
        setTemperature(dsb1820Wrapper.getTempCByIndex(0));
        dsb1820Wrapper.requestTemperatures();
        lastUpdate = seconds();
    }
}

void DS18B20::setTemperature(double temperature)
{
    this->temperature = temperature;
}

double DS18B20::getTemperature()
{
    return temperature;
}

volatile double DS18B20::getAimedTemperature()
{
    return aimedTemperature;
}

void DS18B20::setAimedTemperature(volatile double temp)
{
    this->aimedTemperature = temp;
}

 DS18B20 &DS18B20::getInstance()
{
    static DS18B20 instance;
    return instance;
}

DS18B20::DS18B20()
{
    dsb1820Wrapper = DallasTemperature(new OneWire(DS18B20_PIN));
    aimedTemperature = 29.0f;
    temperature = 0.0f;
    connectDS18B20();
    lastUpdate = seconds();
}
