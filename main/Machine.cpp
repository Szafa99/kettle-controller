#include "Machine.h"
#include "utils.h"
#include "Heater.h"
#include "Engine.h"
#include "DS18B20.h"


Machine::Machine()
{
    machineTimer = timerBegin(0, 80, true);
    timerAlarmWrite(machineTimer, 1 * Utils::MILLIS_CONVERT::SECOND, true);
    workingTime = Utils::AlarmTime(30, 00);
    workingTimeSet = workingTime; 
    runing = false;
}


// main loop when machine is runing
void Machine::machineTimerTick()
{
    workingTime--;
    Engine::getInstance().handleEngineCycles();
    if(DS18B20::getInstance().reachedTemperature()){
      Heater::getInstance().turnOFF();
    }
    else{
      Heater::getInstance().turnON();  
    }

    if (workingTime.timerEnded())
    {
        timerAlarmDisable(machineTimer);

        Heater::getInstance().turnOFF();
        Engine::getInstance().turnOFF();
        runing=false;
        workingTime = workingTimeSet; 
        this->notify();
    }
}



  void Machine::setWorkingTime(Utils::AlarmTime workingTime){
      this->workingTime = workingTime;
      workingTimeSet = workingTime; 

  }

  void Machine::turnOff(){
      timerAlarmDisable(machineTimer);
      if(Engine::getInstance().getState()==LOW && runing)
          Serial.printf("TX/ENGINE_OFF_PERIOD_STATE/STOPPED\n"); 
      else if(Engine::getInstance().getState()==HIGH && runing)
          Serial.printf("TX/ENGINE_ON_PERIOD_STATE/STOPPED\n"); 
     
      runing = false;
      Heater::getInstance().turnOFF();
      Engine::getInstance().turnOFF();
  }

  void Machine::turnOn(){
     if( workingTime.minutes >= 0 || workingTime.second > 0 ){
      timerAlarmEnable(machineTimer);
      runing = true;
    }
  }

  Utils::AlarmTime Machine::getWorkingTime(){
     return this->workingTime ;
  }

    Utils::AlarmTime& Machine::getWorkingTimeInstance(){
     return workingTime ;
  }