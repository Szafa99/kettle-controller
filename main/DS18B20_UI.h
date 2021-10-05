#pragma once
#include "LCD.h"
#include "DS18B20.h"
#include "network.h"

class DS18B20_UI:public InterfaceControler{
  public:

  ///////////////// Singelton///////////////////
  static DS18B20_UI& getInstance(){
          static  DS18B20_UI instance;
          return instance;
      }
   void operator=(const DS18B20_UI&)=delete;    
   DS18B20_UI(DS18B20_UI const&)=delete;
///////////////////////////////////////////////////

  
  void pushBtn()override ;
  
  void turnRight() override;

  void turnLeft() override;

  void render() override;
    private:
      DS18B20_UI(){}
  
  };



