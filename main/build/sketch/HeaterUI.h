#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\HeaterUI.h"
#pragma once
#include "LCD.h"
#include "Machine.h"
#include "Controler.h"
#include "EngineUI.h"


class HeaterUI:public InterfaceControler{
  public:

  ///////////////// Singelton///////////////////
  static HeaterUI& getInstance(){
          static  HeaterUI instance;
          return instance;
      }
   void operator=(const HeaterUI&)=delete;    
   HeaterUI(HeaterUI const&)=delete;
///////////////////////////////////////////////////

  Utils::HeaterInterface currentOption; 
  
  void pushBtn()override ;


  void turnRight() override;
  void turnLeft() override;
  void render() override;

    private:
    HeaterUI(){
      currentOption= Utils::CHANGE_TIME;
      lastPageChange=millis();
    }
    unsigned int lastPageChange;

  
  };
