#include "Machine.h"
#include "utils.h"
#include "Heater.h"
#include "Engine.h"
#include "DS18B20.h"
#define DEBUG 1


Machine::Machine()
{
    machineTimer = timerBegin(0, 80, true);
    timerAlarmWrite(machineTimer, 1 * Utils::MILLIS_CONVERT::SECOND, true);
    workingTime = Utils::AlarmTime(30, 00);
    runing = false;
}


// main loop when machine is runing
void Machine::machineTimerTick()
{
    workingTime--;
    Engine::getInstance().handleEngineCycles();
    if(DS18B20::getInstance().reachedTemperature())
      Heater::getInstance().turnOFF();
    else
      Heater::getInstance().turnON();  
    

    if (workingTime.timerEnded())
    {
        timerAlarmDisable(machineTimer);

        Heater::getInstance().turnOFF();
        Engine::getInstance().turnOFF();
        runing=false;
        this->notify();
    }
    

}


  void Machine::togleMachine(bool changedByUser)
  {
    if (runing)
    {
      timerAlarmDisable(machineTimer);
      runing = false;
      // this->notify();
      #ifdef DEBUG
        Serial.println("Stoping machine");
      #endif
    }else if( workingTime.minutes >= 0 || workingTime.second > 0 ){
      
      timerAlarmEnable(machineTimer);
      runing = true;
      #ifdef DEBUG
      Serial.println("Starting machine");
      #endif
      // this->notify();
    }

  }

  void Machine::setWorkingTime(Utils::AlarmTime workingTime){
      this->workingTime = workingTime;
      #ifdef DEBUG
        Serial.printf("Change machine time: %s\n",this->workingTime.toString());
      #endif
  }

  Utils::AlarmTime Machine::getWorkingTime(){
     return this->workingTime ;
  }

    Utils::AlarmTime& Machine::getWorkingTimeInstance(){
     return workingTime ;
  }