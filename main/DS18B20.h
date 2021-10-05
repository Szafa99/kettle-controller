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
#include "Controler.h"

#define DS18B20_PIN 13
#define sec() millis()/1000
#define UPDATE_PERIOD 15
class DS18B20{
  
    public:  
     
  
    void connectDS18B20(){
        dsb1820Wrapper.begin();
        dsb1820Wrapper.setWaitForConversion(true);  
        dsb1820Wrapper.requestTemperatures();
        temperature = dsb1820Wrapper.getTempCByIndex(0);
        dsb1820Wrapper.setWaitForConversion(false);
        Serial.println("Conncecting ds18");       
      }


    bool reachedTemperature(){
        return abs(aimedTemperature - temperature) < 1.5f; 
      }  


    void updateTemperature(){
        if (sec()-lastUpdate>UPDATE_PERIOD && dsb1820Wrapper.isConversionComplete()) {
            setTemperature( dsb1820Wrapper.getTempCByIndex(0) ); 
            dsb1820Wrapper.requestTemperatures();   
            lastUpdate=sec();
          }
      }

    void setTemperature(double temperature){
      this->temperature=temperature;
      changedTemp=true;
      }
    
    double getTemperature(){
        return temperature;
      }

     volatile double getAimedTemperature(){
        return aimedTemperature;
      }

    void setAimedTemperature(volatile double temp){
        aimedTemperature=temp;
        changedAimedTemp=true;
      }


    static DS18B20& getInstance(){
          static DS18B20 instance;
          return instance;
      }
      
    void operator=(DS18B20 const&)=delete;
    DS18B20(DS18B20 const&)=delete;

    bool changedTemp=false;  
    bool changedAimedTemp=false;  
    private:


    DS18B20(){
      dsb1820Wrapper = DallasTemperature( new OneWire(DS18B20_PIN) );
      aimedTemperature = 5.0f;
      temperature=0.0f;
      connectDS18B20();
      lastUpdate=sec();
      }

    DallasTemperature       dsb1820Wrapper;
    volatile double         temperature;
    volatile double         aimedTemperature;
    double                  lastUpdate;
  };
