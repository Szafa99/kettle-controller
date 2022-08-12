#pragma once

#define ENGINE_PIN 25

#include "Utils.h"


class Engine
{
private:
  unsigned int state;

  //variabeles to set by user
  Utils::AlarmTime engineOnPeriod;
  Utils::AlarmTime engineOffPeriod;
  // Variabels holding actual time
  Utils::AlarmTime isOnTime;
  Utils::AlarmTime isOffTime;

  Engine();

public:

  Utils::AlarmTime &getEngineOnPeriod();
  void setEngineOnPeriod(Utils::AlarmTime period);
  Utils::AlarmTime &getEngineOffPeriod();
  void setEngineOffPeriod(Utils::AlarmTime period);
  unsigned int getState();
  void setState(unsigned int state);
  void turnON();
  void turnOFF();
  void handleEngineCycles();
  Utils::AlarmTime &getEngineOnPeriodLeft();
  Utils::AlarmTime &getEngineOffPeriodLeft();


  static Engine &getInstance()
  {
    static Engine instance;
    return instance;
  }


  void operator=(const Engine &) = delete;
  Engine(Engine const &) = delete;

};
