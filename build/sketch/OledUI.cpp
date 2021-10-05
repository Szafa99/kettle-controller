#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\OledUI.cpp"
#pragma once

#include "OledUI.h"
#include "network.h"
#include "Machine.h"
#include "LCD.h"
#include "Oled.h" 
#include "Utils.h"
#include "EngineUI.h"
#include "HeaterUI.h"


void OledUI::pushBtn() {
          switch(currentOption){
          case Utils::READ_TEMP:
              break;
          case Utils::START_STOP_MACHINE:
              Oled::getInstance().oledControler = &HeaterUI::getInstance();
              HeaterUI::getInstance().currentOption=Utils::MINUTES;
              break;
          case Utils::ENGINE_ON:
              Oled::getInstance().oledControler = &EngineUI::getInstance();
              EngineUI::getInstance().enginePeriod=&Engine::getInstance().getEngineOnPeriod();

              break;
          case Utils::ENGINE_OFF:
              Oled::getInstance().oledControler = &HeaterUI::getInstance();
              break;
          case Utils::AIMED_TEMP:
              Oled::getInstance().oledControler = &HeaterUI::getInstance();
              break;

        }
    }


      void OledUI::turnRight() {
       if(millis()-lastPageChange<1000)return;

    switch(currentOption){
          case Utils::START_STOP_MACHINE:
              currentOption=Utils::ENGINE_ON;
              Oled::getInstance().drawRect(RIGHT_UPPER);
              break;
          case Utils::ENGINE_ON:
              currentOption=Utils::AIMED_TEMP;
              Oled::getInstance().drawRect(LEFT_DOWN);
              break;
          case Utils::AIMED_TEMP:
               currentOption=Utils::MACHINE_TIME;
              Oled::getInstance().drawRect(MIDLE_DOWN);
               break;
          case Utils::MACHINE_TIME:
               currentOption=Utils::ENGINE_OFF;
              Oled::getInstance().drawRect(RIGHT_DOWN);
            break;
          case Utils::ENGINE_OFF:           
               currentOption=Utils::START_STOP_MACHINE;
              Oled::getInstance().drawRect(MIDLE_UPPER);
            break;
          default:break;
        }
              lastPageChange=millis();

    }

  void OledUI::turnLeft() {
       if(millis()-lastPageChange<1000)return;

    switch(currentOption){
          case Utils::START_STOP_MACHINE:
              currentOption=Utils::ENGINE_OFF;
              Oled::getInstance().drawRect(RIGHT_DOWN);
              break;
          case Utils::ENGINE_OFF:
              currentOption=Utils::MACHINE_TIME;
              Oled::getInstance().drawRect(MIDLE_DOWN);
              break;
          case Utils::MACHINE_TIME:
               currentOption=Utils::AIMED_TEMP;
              Oled::getInstance().drawRect(LEFT_DOWN);
               break;
          case Utils::AIMED_TEMP:
               currentOption=Utils::ENGINE_ON;
              Oled::getInstance().drawRect(RIGHT_UPPER);
            break;
          case Utils::ENGINE_ON:           
               currentOption=Utils::START_STOP_MACHINE;
              Oled::getInstance().drawRect(MIDLE_UPPER);
            break;
          default:break;
        }
              lastPageChange=millis();

    }  

  void OledUI::render() {
      DS18B20_UI::getInstance().render();
      HeaterUI::getInstance().render();
      EngineUI::getInstance().render();
    }
