#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\EngineUI.h"
#pragma once
#include "Controler.h"
#include "Utils.h"

enum EngineUiOptions
{
  MINUTES,
  SECONDS
};

class EngineUI : public InterfaceControler
{
public:
  ///////////////// Singelton///////////////////
  static EngineUI &getInstance()
  {
    static EngineUI instance;
    return instance;
  }
  void operator=(const EngineUI &) = delete;
  EngineUI(EngineUI const &) = delete;
  ///////////////////////////////////////////////////

  void pushBtn() override;

  void turnRight() override;

  void turnLeft() override;

  void render() override;

  void renderPeriodOn();

  void renderPeriodOff();

  void setEngiePeriod(Utils::AlarmTime *period);

private:
  EngineUI();
  ~EngineUI();
  unsigned int lastPageChange;
  Utils::AlarmTime *enginePeriod;
  const int engineOnLine;
  const int engineOffLine;
  EngineUiOptions currentOption;
};
