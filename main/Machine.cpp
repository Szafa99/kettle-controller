#include "Machine.h"
#include "utils.h"
#include "MainUi.h"
#include "Heater.h"
#include "Engine.h"
#include "network.h"

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
    if (workingTime.timerEnded())
    {
        timerAlarmDisable(machineTimer);
        Heater::getInstance().turnOFF();
        Engine::getInstance().turnOFF();
        runing=false;
        Network::getInstance().machineStateChanged=true;
    }
    
    MainUI::getInstance().renderMachineTime();

}


  void Machine::togleMachine(bool changedByUser)
  {
    if (runing)
    {
      timerAlarmDisable(machineTimer);
      runing = false;
      Network::getInstance().machineStateChanged=true;
    }else if( workingTime.minutes >= 0 || workingTime.second > 0 ){
      
      timerAlarmEnable(machineTimer);
      runing = true;
      Network::getInstance().machineStateChanged=true;
    }
  }

  void Machine::setWorkingTime(Utils::AlarmTime workingTime){
      this->workingTime = workingTime;
      MainUI::getInstance().renderMachineTime();
      Network::getInstance().updateMachineTimeOn();
  }

  Utils::AlarmTime Machine::getWorkingTime(){
     return this->workingTime ;
  }

    Utils::AlarmTime& Machine::getWorkingTimeInstance(){
     return workingTime ;
  }