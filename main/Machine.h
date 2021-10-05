#pragma once

#include "heater.h"
#include "Engine.h"
#include "DS18B20.h"
class Machine{
public:

    void turnOFF(){
          state=0;
          Heater::getInstance().turnOFF();
          Engine::getInstance().turnOFF();
          timerAlarmDisable(machineTimer);
          stateChanged=true;
  
  }

    void turnON(){
          state=1;
          Heater::getInstance().turnON();
          Engine::getInstance().turnON();
          timerAlarmEnable(machineTimer);
          stateChanged=true;

  }


  unsigned int getState(){return state;}

  void setState(unsigned int state){
      this->state=state;
      if(state) turnON();
      else turnOFF();
  }

  static Machine& getInstance(){
    static Machine instance;
    return instance;
  }

  Machine(const Machine&) = delete;
  void operator=(const Machine &)=delete;

  hw_timer_t * machineTimer=NULL;
  Utils::AlarmTime workingTime;


void  machineTimerTick(){
      workingTime--;
      Engine::getInstance().handleEngineCycles();
      if(DS18B20::getInstance().reachedTemperature() )
        Heater::getInstance().turnOFF();
      else
        Heater::getInstance().turnON();

      if(workingTime.timerEnded() ){
          timerAlarmDisable(machineTimer);
          Heater::getInstance().turnOFF();
          Engine::getInstance().turnOFF();
         }

  }


  bool stateChanged=false;
  bool timeChanged=false;
  private:
   Machine(){
    machineTimer=timerBegin(0,80,true);
    timerAlarmWrite(machineTimer,1*Utils::MILLIS_CONVERT::SECOND,true);   
    workingTime=Utils::AlarmTime(2,22);
  }
  unsigned int state;

};

