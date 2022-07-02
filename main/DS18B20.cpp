#pragma once
#include "heater.h"
#include "DS18B20.h"


void DS18B20::connectDS18B20()
{
    dsb1820Wrapper.begin();
    dsb1820Wrapper.setWaitForConversion(true);
    dsb1820Wrapper.requestTemperatures();
    temperature = dsb1820Wrapper.getTempCByIndex(0);
    dsb1820Wrapper.setWaitForConversion(false);
    Serial.printf("temp %.2f\n",temperature);
}

bool DS18B20::reachedTemperature()
{
    return abs(aimedTemperature - temperature) < 1.5f;
}

void DS18B20::updateTemperature()
{
    if (seconds() - lastUpdate > 20 && dsb1820Wrapper.isConversionComplete())
    {
        setTemperature(dsb1820Wrapper.getTempCByIndex(0));
        dsb1820Wrapper.requestTemperatures();
        lastUpdate = seconds();
    }
}

void DS18B20::setTemperature(double temperature)
{
    this->temperature = temperature;
    Serial.printf("T-%d",temperature);
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
    aimedTemperature = temp;
    // MainUI::getInstance().renderAimedTemperature();
    // Network::getInstance().updateAimedTemp();
    Serial.printf("ST-%d",temperature);

}

 DS18B20 &DS18B20::getInstance()
{
    static DS18B20 instance;
    return instance;
}

DS18B20::DS18B20()
{
    dsb1820Wrapper = DallasTemperature(new OneWire(DS18B20_PIN));
    aimedTemperature = 37.0f;
    temperature = 0.0f;
    connectDS18B20();
    lastUpdate = seconds();
}
