#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\OledUI.h"
#pragma once
#include "Utils.h"
#include "Controler.h"

class OledUI : public InterfaceControler {
      public:

  ///////////////// Singelton///////////////////
  static OledUI& getInstance(){
          static  OledUI instance;
          return instance;
      }
   void operator=(const OledUI&)=delete;    
   OledUI(OledUI const&)=delete;
///////////////////////////////////////////////////

  Utils::OledInterface currentOption; 
  
  void pushBtn()override ;
  void turnRight() override;
  void turnLeft() override;
  void render() override;

    private:
    OledUI(){
      currentOption= Utils::START_STOP_MACHINE;
      lastPageChange=millis();
    }
    unsigned int lastPageChange;
};