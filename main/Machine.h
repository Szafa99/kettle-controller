#ifndef TS_H
#define TS_H

#include "heater.h"
#include "Engine.h"


class Machine{
public:


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
      if(workingTime.timerEnded() ){
          timerAlarmDisable(machineTimer);
          Heater::getInstance().turnOFF();
          Engine::getInstance().turnOFF();
         }

  }



  private:
   Machine(){
    machineTimer=timerBegin(0,80,true);
    timerAlarmWrite(machineTimer,1*Utils::MILLIS_CONVERT::SECOND,true);   
    workingTime=Utils::AlarmTime(2,22);
  }

};







#endif
