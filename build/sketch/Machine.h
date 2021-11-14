#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\Machine.h"
#pragma once

#include "heater.h"
#include "Engine.h"


class Machine
{
public:
  static Machine &getInstance()
  {
    static Machine instance;
    return instance;
  }

  Machine(const Machine &) = delete;
  void operator=(const Machine &) = delete;

  hw_timer_t *machineTimer = NULL;


  void setWorkingTime(Utils::AlarmTime workingTime);
  Utils::AlarmTime getWorkingTime();
  Utils::AlarmTime& getWorkingTimeInstance(); 
  void machineTimerTick();
  void togleMachine(bool changedByUser=true);


  bool isOn(){
    return runing;
  }



private:
  Machine();
  bool runing;
  Utils::AlarmTime workingTime;

};

