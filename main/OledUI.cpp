#pragma once

#include "OledUI.h"
#include "network.h"
#include "Machine.h"
#include "Oled.h" 
#include "Utils.h"
#include "EngineUI.h"
#include "HeaterUI.h"


void OledUI::pushBtn() {
    Serial.println("push");

          switch(currentOption){
          case Utils::READ_TEMP:
              break;
          case Utils::START_STOP_MACHINE:
              Heater::getInstance().toggle();
              break;
          case Utils::ENGINE_ON:
              Oled::getInstance().oledControler = &EngineUI::getInstance();
              EngineUI::getInstance().enginePeriod=&Engine::getInstance().getEngineOnPeriod();

              break;
          case Utils::ENGINE_OFF:
              Oled::getInstance().oledControler = &EngineUI::getInstance();
              EngineUI::getInstance().enginePeriod=&Engine::getInstance().getEngineOffPeriod();
              break;
          case Utils::AIMED_TEMP:
              Oled::getInstance().oledControler = &HeaterUI::getInstance();
              break;
          case Utils::MACHINE_TIME:
              Oled::getInstance().oledControler = &HeaterUI::getInstance();
              HeaterUI::getInstance().currentOption=Utils::MINUTES;
          break;

          default:
          break;
        }
    }


      void OledUI::turnRight() {
    Serial.println("Turning right");

       if(millis()-lastPageChange<1000)return;

    switch(currentOption){
          case Utils::START_STOP_MACHINE:
              currentOption=Utils::ENGINE_ON;
              Oled::getInstance().drawRect(RIGHT_UPPER);
              Oled::getInstance().clearRect(MIDLE_UPPER);
              break;
          case Utils::ENGINE_ON:
              currentOption=Utils::AIMED_TEMP;
              Oled::getInstance().clearRect(RIGHT_UPPER);
              Oled::getInstance().drawRect(LEFT_DOWN);
              break;
          case Utils::AIMED_TEMP:
               currentOption=Utils::MACHINE_TIME;
              Oled::getInstance().clearRect(LEFT_DOWN);
              Oled::getInstance().drawRect(MIDLE_DOWN);
               break;
          case Utils::MACHINE_TIME:
               currentOption=Utils::ENGINE_OFF;
              Oled::getInstance().clearRect(MIDLE_DOWN);
              Oled::getInstance().drawRect(RIGHT_DOWN);
            break;
          case Utils::ENGINE_OFF:           
               currentOption=Utils::START_STOP_MACHINE;
              Oled::getInstance().clearRect(RIGHT_DOWN);
              Oled::getInstance().drawRect(MIDLE_UPPER);
            break;
          default:break;
        }
              lastPageChange=millis();

    }

  void OledUI::turnLeft() {
    Serial.println("Turning left");
       if(millis()-lastPageChange<1000)return;

    switch(currentOption){
          case Utils::START_STOP_MACHINE:
              currentOption=Utils::ENGINE_OFF;
              Oled::getInstance().drawRect(RIGHT_DOWN);
              Oled::getInstance().clearRect(MIDLE_UPPER);
              break;
          case Utils::ENGINE_OFF:
              currentOption=Utils::MACHINE_TIME;
              Oled::getInstance().drawRect(MIDLE_DOWN);
              Oled::getInstance().clearRect(RIGHT_DOWN);
              break;
          case Utils::MACHINE_TIME:
               currentOption=Utils::AIMED_TEMP;
              Oled::getInstance().drawRect(LEFT_DOWN);
              Oled::getInstance().clearRect(MIDLE_DOWN);
               break;
          case Utils::AIMED_TEMP:
               currentOption=Utils::ENGINE_ON;
              Oled::getInstance().drawRect(RIGHT_UPPER);
              Oled::getInstance().clearRect(LEFT_DOWN);
            break;
          case Utils::ENGINE_ON:           
               currentOption=Utils::START_STOP_MACHINE;
              Oled::getInstance().drawRect(MIDLE_UPPER);
              Oled::getInstance().clearRect(RIGHT_UPPER);

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
