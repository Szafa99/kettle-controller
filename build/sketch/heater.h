#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\heater.h"

#pragma once

#include "Utils.h"
#include "Controler.h"
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
    Serial.println("sTARTING HEATER");

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
    }

  void turnOFF(){
    state=LOW;
    digitalWrite(HEATERPIN,LOW);
    }
     
};
