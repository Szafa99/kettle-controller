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
  
  void pushBtn()override {
          switch(currentOption){
          case Utils::CHANGE_TIME:
              currentOption=Utils::MINUTES;
              break;
          case Utils::START_TIMER:
              timerAlarmEnable(Machine::getInstance().machineTimer);
              break;
          case Utils::STOP_TIMER:
              timerAlarmDisable(Machine::getInstance().machineTimer);
                break;
          case Utils::MINUTES:
            currentOption=Utils::SECONDS;
            break;
          case Utils::SECONDS:
            currentOption=Utils::START_TIMER;
            break;
          case Utils::HeaterInterface::NEXT_PAGE:
               LCD::getInstance().lcdControler=&EngineUI::getInstance();
          default:break;
        }
    }
  
  void turnRight() override{
    switch(currentOption){
          case Utils::CHANGE_TIME:
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::START_TIMER;
              break;
          case Utils::START_TIMER:
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::STOP_TIMER;
              break;
          case Utils::STOP_TIMER:
               if(millis()-lastPageChange<1000)break;
               lastPageChange=millis();
               currentOption=Utils::HeaterInterface::NEXT_PAGE;
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

  void turnLeft() override{
      switch(currentOption){
          case Utils::CHANGE_TIME:
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::HeaterInterface::NEXT_PAGE;
              break;
          case Utils::START_TIMER:
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::CHANGE_TIME;
              break;
          case Utils::STOP_TIMER:
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              currentOption=Utils::START_TIMER;
              break;
          case Utils::HeaterInterface::NEXT_PAGE:
               if(millis()-lastPageChange<1000)break;
               lastPageChange=millis();
               currentOption=Utils::STOP_TIMER;  
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

  void render() override{
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
          case Utils::HeaterInterface::NEXT_PAGE:
              LCD::getInstance().writeToLine(0,"  USTAW CYKLE  ");
              LCD::getInstance().writeToLine(1,"PRACY MIESZADLA");
              break;
          case Utils::MINUTES:          
            LCD::getInstance().underlineChar(0,7);          
            break;
          case Utils::SECONDS:
            LCD::getInstance().hideCursor();
            LCD::getInstance().underlineChar(0,10);          
            break;
          default:break;
        }
    }

    private:
    HeaterUI(){
      currentOption= Utils::CHANGE_TIME;
      lastPageChange=millis();
    }
    unsigned int lastPageChange;

  
  };
