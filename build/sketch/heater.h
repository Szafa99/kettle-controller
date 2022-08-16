#line 1 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\heater.h"

#pragma once

#include "Utils.h"
#include "Arduino.h"

#define HEATERPIN 12


class Heater{
  
  ////////////////////////////////////////////////////////////////////
  unsigned int state;
  
  ////////////////////////////////////////////////////////////////////

    Heater(){
    state=LOW;
    pinMode(HEATERPIN,OUTPUT);
    digitalWrite(HEATERPIN,state);
    Serial2.println("STARTING HEATER");

    }

  public:


  unsigned int getState(){return state; }

    static Heater& getInstance(){
        static Heater instance;
        return instance;
      }


    void operator=(Heater const&)=delete;
    Heater(Heater const&)=delete;
     
  
  void turnON(){
    state=HIGH;
    digitalWrite(HEATERPIN,HIGH);
    // Serial.println("Heater turnOn");
    }

  void turnOFF(){
    state=LOW;
    digitalWrite(HEATERPIN,LOW);
    // Serial.println("Heater turnOff");
    }
     
};
