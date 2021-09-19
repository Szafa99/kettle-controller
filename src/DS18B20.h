//Temperature sensor DS18B20 (Water safe) :
//Power 3.3-5 V
//Temp borders - -55 - +120 C
//GND - Black wire 
//VDD - Red wire
//DATA - Yellow wire
//p0 - potencjometr

#pragma once

#include  <OneWire.h>
#include  <DallasTemperature.h >
#include "heater.h"
#include "Controler.h"

#define DS18B20_PIN 13
#define seconds() (millis()/1000)

class DS18B20{
  
    public:  
     
  
    void connectDS18B20(){
        dsb1820Wrapper.begin();
        dsb1820Wrapper.setWaitForConversion(true);  
        dsb1820Wrapper.requestTemperatures();
        temperature = dsb1820Wrapper.getTempCByIndex(0);
        dsb1820Wrapper.setWaitForConversion(false);
      }


    bool reachedTemperature(){
        return abs(aimedTemperature - temperature) < 1.5f; 
      }  


    void updateTemperature(){
        if (seconds()-lastUpdate>20 && dsb1820Wrapper.isConversionComplete()) {
            temperature = dsb1820Wrapper.getTempCByIndex(0); 
            dsb1820Wrapper.requestTemperatures();   
            lastUpdate=seconds();
          }
      }

    void setTemperature(double temperature){
      this->temperature=temperature;
      }
    
    double getTemperature(){
        return temperature;
      }

     volatile double getAimedTemperature(){
        return aimedTemperature;
      }

    void setAimedTemperature(volatile double temp){
        aimedTemperature=temp;
      }


    static DS18B20& getInstance(){
          static DS18B20 instance;
          return instance;
      }
      
    void operator=(DS18B20 const&)=delete;
    DS18B20(DS18B20 const&)=delete;
      
    private:


    DS18B20(){
      dsb1820Wrapper = DallasTemperature( new OneWire(DS18B20_PIN) );
      aimedTemperature = 5.0f;
      temperature=0.0f;
      connectDS18B20();
      lastUpdate=seconds();
      }

    DallasTemperature       dsb1820Wrapper;
    volatile double         temperature;
    volatile double         aimedTemperature;
    double                  lastUpdate;
  };
