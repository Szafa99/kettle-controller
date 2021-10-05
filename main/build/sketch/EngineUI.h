#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\EngineUI.h"
#pragma once
// #include "LCD.h"
#include "DS18B20_UI.h"
#include "Engine.h"
#include "Controler.h"
#include "Utils.h"

#define OLED

using namespace Utils;
class EngineUI:public InterfaceControler{

  #ifdef OLED
  friend class OledUI;
  #endif

  public:

  ///////////////// Singelton///////////////////
  static EngineUI& getInstance(){
          static  EngineUI instance;
          return instance;
      }
   void operator=(const EngineUI&)=delete;    
   EngineUI(EngineUI const&)=delete;
///////////////////////////////////////////////////

  
  void pushBtn()override;
  void turnRight() override;
  void turnLeft() override;
  void render() override;
    

    private:
    EngineUI(){
      enginePeriod = &Engine::getInstance().getEngineOnPeriod();
      currentOption = Utils::CHANGE_ON_TIME;
      lastPageChange = millis();
    }
    ~EngineUI(){     
       delete enginePeriod;
      }
  unsigned int lastPageChange;
  Utils::AlarmTime *enginePeriod; 
  Utils::EngineInterface currentOption; 

  
  };
