#pragma once
#include "LCD.h"
#include "DS18B20.h"
#include "HeaterUI.h"


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
  
  void turnRight() override{
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature()+0.25f);
  }

  void turnLeft() override{
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature()-0.25f);       
    }  

  void render() override{
      char text[16];
      char text2[16];
      sprintf(text,"Mleko: %.2f C \0",DS18B20::getInstance().getTemperature() );
      sprintf(text2,"Cel: %.2f C  \0",DS18B20::getInstance().getAimedTemperature() );

      LCD::getInstance().writeToLine(0,text);
      LCD::getInstance().writeToLine(1,text2);    
    }
    private:
      DS18B20_UI(){}
  
  };



