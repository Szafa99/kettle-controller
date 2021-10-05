#pragma once
#include "DS18B20_UI.h"
#include "HeaterUI.h"
#include "Oled.h"


#ifdef LCD

void DS18B20_UI::pushBtn() {
   LCD::getInstance().lcdControler=&HeaterUI::getInstance();
 };

  void DS18B20_UI::turnRight() {
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature()+0.25f);
  }

  void DS18B20_UI::turnLeft() {
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature()-0.25f);       
    }  

  void DS18B20_UI::render(){
      char text[16];
      char text2[16];
      sprintf(text,"Mleko: %.2f C \0",DS18B20::getInstance().getTemperature() );
      sprintf(text2,"Cel: %.2f C  \0",DS18B20::getInstance().getAimedTemperature() );

      LCD::getInstance().writeToLine(0,text);
      LCD::getInstance().writeToLine(1,text2);    
    }

#endif
#ifdef OLED
void DS18B20_UI::pushBtn() {
   LCD::getInstance().lcdControler=&HeaterUI::getInstance();
 };

  void DS18B20_UI::turnRight() {
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature()+0.25f);
  }

  void DS18B20_UI::turnLeft() {
    DS18B20::getInstance().setAimedTemperature(DS18B20::getInstance().getAimedTemperature()-0.25f);       
    }  

  void DS18B20_UI::render(){
      char text[16];
      if( DS18B20::getInstance().changedTemp) {
      sprintf(text,"Mleko:%.2fC\0",DS18B20::getInstance().getTemperature() );
      Oled::getInstance().writeText2Rec( LEFT_UPPER ,text);
      DS18B20::getInstance().changedTemp=false;
      }
      if( DS18B20::getInstance().changedTemp) {
      sprintf(text," Cel: %.2fC\0",DS18B20::getInstance().getAimedTemperature() );
      Oled::getInstance().writeText2Rec( LEFT_DOWN ,text);
      DS18B20::getInstance().changedAimedTemp=false; 
      }
    }

#endif