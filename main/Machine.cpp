#include "Machine.h"
#include "utils.h"
#include "MachineUI.h"
#include "Heater.h"
#include "Engine.h"


Machine::Machine()
{
    machineTimer = timerBegin(0, 80, true);
    timerAlarmWrite(machineTimer, 1 * Utils::MILLIS_CONVERT::SECOND, true);
    workingTime = Utils::AlarmTime(2, 22);
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
    }
        MachineUI::getInstance().renderTime();
}


  void Machine::togleMachine()
  {
    if (runing)
    {
      timerAlarmDisable(machineTimer);
      runing = false;
    }else{
      timerAlarmEnable(machineTimer);
      runing = true;
    }
        MachineUI::getInstance().renderState();

  }