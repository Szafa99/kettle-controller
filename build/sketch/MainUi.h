#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\MainUi.h"
#pragma once
#include "Controler.h"



enum RenderOptions
{
  TEMP = 0,
  AIMED_TEMP,
  TOGGLE_MACHINE,
  MACHINE_ON_TIME,
  ENGINE_ON_CYCLE,
  ENGINE_OFF_CYCLE
};



class MainUI : public InterfaceControler
{
public:
  ///////////////// Singelton///////////////////
  static MainUI &getInstance(){
    static MainUI instance;
    return instance;
  }
  void operator=(const MainUI &) = delete;
  MainUI(MainUI const &) = delete;
  ///////////////////////////////////////////////////


  void pushBtn() override;
  void turnRight() override;
  void turnLeft() override;
  void render() override;
  void firstRender();

private:
  MainUI();

  int curentLine = 0;
  RenderOptions currentOption;
};
