#pragma once 
#include "HeaterUI.h"
#include "network.h"
#include "Machine.h"
#include "LCD.h"
#include "Oled.h"
#include "OledUI.h"
#include "Utils.h"

#define OLED
#ifdef LCD
void HeaterUI::pushBtn() {
          switch(currentOption){
          case Utils::CHANGE_TIME:
              currentOption=Utils::MINUTES;
              break;
          case Utils::START_TIMER:{
              Machine::getInstance().turnON();
              Network::getInstance().update = &Network::updateMachine;
          }
              break;
          case Utils::STOP_TIMER:{
              Machine::getInstance().turnOFF();
              Network::getInstance().update = &Network::updateMachine;
          }
                break;
          case Utils::MINUTES:
            currentOption=Utils::SECONDS;
            break;
          case Utils::SECONDS:{
            currentOption=Utils::START_TIMER;
              Network::getInstance().update = &Network::updateMachine;
          }
            break;
          case Utils::HeaterInterface::NEXT_PAGE:
               LCD::getInstance().lcdControler=&EngineUI::getInstance();
          default:break;
        }
    }


      void HeaterUI::turnRight() {
    switch(currentOption){
          case Utils::CHANGE_TIME:{
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::START_TIMER;
          }
              break;
          case Utils::START_TIMER:{
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::STOP_TIMER;
          }
              break;
          case Utils::STOP_TIMER:{
               if(millis()-lastPageChange<1000)break;
               lastPageChange=millis();
               currentOption=Utils::HeaterInterface::NEXT_PAGE;
          }
               break;
          case Utils::MINUTES:
             Machine::getInstance().workingTime.increaseMinutes();
            break;
          case Utils::SECONDS:
              Machine::getInstance().workingTime++;
            break;
          case Utils::HeaterInterface::NEXT_PAGE:
               currentOption=Utils::CHANGE_TIME;  
               break;
          default:break;
        }
    }

  void HeaterUI::turnLeft() {
      switch(currentOption){
          case Utils::CHANGE_TIME:{
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::HeaterInterface::NEXT_PAGE;
          }
              break;
          case Utils::START_TIMER:{
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::CHANGE_TIME;
          }
              break;
          case Utils::STOP_TIMER:{
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::START_TIMER;
          }
              break;
          case Utils::HeaterInterface::NEXT_PAGE:{

               if(millis()-lastPageChange<1000)break;
               lastPageChange=millis();
               currentOption=Utils::STOP_TIMER;  
          }
               break;    
          case Utils::MINUTES:
               Machine::getInstance().workingTime.decreaseMinutes();
               break;
          case Utils::SECONDS:
               Machine::getInstance().workingTime--;
               break;
          default:break;
        }
    }  

  void HeaterUI::render() {
    char text[16];
    if(currentOption!=Utils::HeaterInterface::NEXT_PAGE){
    sprintf(text,"Czas: %s min",Machine::getInstance().workingTime.toString() );
    LCD::getInstance().writeToLine(0,text);
    }
    
    switch(currentOption){
          case Utils::CHANGE_TIME:
              LCD::getInstance().writeToLine(1,"   USTAW CZAS   ");
              break;
          case Utils::START_TIMER:
              LCD::getInstance().writeToLine(1,"      START     ");
              break;
          case Utils::STOP_TIMER:
              LCD::getInstance().writeToLine(1,"      STOP      ");
              break;
          case Utils::HeaterInterface::NEXT_PAGE:{
              LCD::getInstance().writeToLine(0,"  USTAW CYKLE  ");
              LCD::getInstance().writeToLine(1,"PRACY MIESZADLA");
          }
              break;
          case Utils::MINUTES:          
            LCD::getInstance().underlineChar(0,7);          
            break;
          case Utils::SECONDS:{
            LCD::getInstance().hideCursor();
            LCD::getInstance().underlineChar(0,10);          
          }
            break;
          default:break;
        }
    }
#endif
#ifdef OLED
void HeaterUI::pushBtn() {
          switch(currentOption){
          case Utils::MINUTES:
            currentOption=Utils::SECONDS;
            break;
          case Utils::SECONDS:{
            Oled::getInstance().oledControler = &OledUI::getInstance();
            Network::getInstance().update = &Network::updateMachine;
          }
            break;
          default:break;
        }
    }


      void HeaterUI::turnRight() {
    switch(currentOption){
          case Utils::MINUTES:
             Machine::getInstance().workingTime.increaseMinutes();
             Machine::getInstance().timeChanged = true;
            break;
          case Utils::SECONDS:
              Machine::getInstance().workingTime++;
              Machine::getInstance().timeChanged = true;
            break;
          default:break;
        }
    }

  void HeaterUI::turnLeft() {
      switch(currentOption){
          case Utils::MINUTES:
               Machine::getInstance().workingTime.decreaseMinutes();
               Machine::getInstance().timeChanged = true;
               break;
          case Utils::SECONDS:
               Machine::getInstance().workingTime--;
               Machine::getInstance().timeChanged = true;
               break;
          default:break;
        }
    }  

  void HeaterUI::render() {
        char text[16];
    if(Machine::getInstance().timeChanged ){
        sprintf(text,"      %sm",Machine::getInstance().workingTime.toString());    
        Oled::getInstance().writeText2Rec(MIDLE_DOWN,text);
        Machine::getInstance().timeChanged=false;
    }
    else if(Engine::getInstance().stateChanged){
        Machine::getInstance().getState()==true ? 
                    Oled::getInstance().writeText2Rec(MIDLE_UPPER,"WYLACZKOCIOL") :
                    Oled::getInstance().writeText2Rec(MIDLE_UPPER,"WLACZ KOCIOL") ;
        
        Machine::getInstance().stateChanged=false;
      }
    }

#endif