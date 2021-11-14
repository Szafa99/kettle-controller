#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\DS18B20.h"
//Temperature sensor DS18B20 (Water safe) :
//Power 3.3-5 V
//Temp borders - -55 - +120 C
//GND - Black wire
//VDD - Red wire
//DATA - Yellow wire
//p0 - potencjometr

#pragma once

#include <OneWire.h>
#include <DallasTemperature.h >

#define DS18B20_PIN 27
#define seconds() (millis() / 1000)

class DS18B20
{

public:
  void connectDS18B20();
  bool reachedTemperature();
  void updateTemperature();
  void setTemperature(double temperature);
  double getTemperature();
  volatile double getAimedTemperature();
  void setAimedTemperature(volatile double temp);
  static DS18B20 &getInstance();
  void operator=(DS18B20 const &) = delete;
  DS18B20(DS18B20 const &) = delete;


private:
  DS18B20();

  DallasTemperature dsb1820Wrapper;
  volatile double temperature;
  volatile double aimedTemperature;
  double lastUpdate;
};
