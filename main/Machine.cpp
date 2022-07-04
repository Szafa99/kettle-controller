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
    runing = false;
}


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
        this->notify();

        Heater::getInstance().turnOFF();
        Engine::getInstance().turnOFF();
        runing=false;
    }
    

}


  void Machine::togleMachine(bool changedByUser)
  {
    if (runing)
    {
      Heater::getInstance().turnOFF();
      Engine::getInstance().turnOFF();
      timerAlarmDisable(machineTimer);
      runing = false;
      this->notify();

    }else if( workingTime.minutes >= 0 || workingTime.second > 0 ){
      
      timerAlarmEnable(machineTimer);
      runing = true;
      this->notify();
    }

  }

  void Machine::setWorkingTime(Utils::AlarmTime workingTime){
      this->workingTime = workingTime;
  }

  Utils::AlarmTime Machine::getWorkingTime(){
     return this->workingTime ;
  }

    Utils::AlarmTime& Machine::getWorkingTimeInstance(){
     return workingTime ;
  }