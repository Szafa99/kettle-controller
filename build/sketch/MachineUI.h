#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\MachineUI.h"
#pragma once
#include "Controler.h"


class MachineUI:public InterfaceControler{
  public:

  ///////////////// Singelton///////////////////
  static MachineUI& getInstance(){
          static  MachineUI instance;
          return instance;
      }
   void operator=(const MachineUI&)=delete;    
   MachineUI(MachineUI const&)=delete;
///////////////////////////////////////////////////

  Utils::HeaterInterface currentOption; 
  
  void pushBtn()override;  
  void turnRight() override;
  void turnLeft() override;
  void render() override{}
  void renderTime();
  void renderState();

    private:
    MachineUI();

    const int timerLine;
    const int turnOnLine;
  };
