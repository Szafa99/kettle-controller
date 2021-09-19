#pragma once
#include "LCD.h"
#include "DS18B20_UI.h"
#include "Engine.h"
#include "Controler.h"
#include "Utils.h"

using namespace Utils;
class EngineUI:public InterfaceControler{
  public:

  ///////////////// Singelton///////////////////
  static EngineUI& getInstance(){
          static  EngineUI instance;
          return instance;
      }
   void operator=(const EngineUI&)=delete;    
   EngineUI(EngineUI const&)=delete;
///////////////////////////////////////////////////

  
  void pushBtn()override {
          switch(currentOption){
          case Utils::CHANGE_ON_TIME:
              currentOption=Utils::PERIOD_MINUTES;
              break;
          case Utils::CHANGE_OFF_TIME:
            currentOption=Utils::PERIOD_MINUTES;
            enginePeriod = &Engine::getInstance().getEngineOffPeriod();
            break;
          case Utils::PERIOD_MINUTES:
            currentOption=Utils::PERIOD_SECONDS;
            break;
          case Utils::PERIOD_SECONDS:
            if(enginePeriod==&Engine::getInstance().getEngineOnPeriod())
              currentOption=Utils::CHANGE_ON_TIME;
            else
              currentOption=Utils::CHANGE_OFF_TIME;
            break;
          case Utils::EngineInterface::NEXT:
               LCD::getInstance().lcdControler=&DS18B20_UI::getInstance();
               break;
          default:break;
        }
    }
  
  void turnRight() override{
          switch(currentOption){
          case Utils::CHANGE_ON_TIME:
              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              enginePeriod = &Engine::getInstance().getEngineOffPeriod();
              currentOption=Utils::CHANGE_OFF_TIME;
              break;
          case Utils::CHANGE_OFF_TIME:
            if(millis()-lastPageChange<1000)break;
            lastPageChange=millis();
            currentOption=Utils::EngineInterface::NEXT;
            enginePeriod=NULL;
            break;
          case Utils::PERIOD_MINUTES:
            enginePeriod->increaseMinutes();
            break;
          case Utils::PERIOD_SECONDS:
            (*enginePeriod)++;
            break;
          case Utils::EngineInterface::NEXT:
            if(millis()-lastPageChange<1000)break;
            lastPageChange=millis();
            enginePeriod = &Engine::getInstance().getEngineOnPeriod();
            currentOption=Utils::CHANGE_ON_TIME;
            break;    
          default:break;
        }
    }

  void turnLeft() override{
          switch(currentOption){
          case Utils::CHANGE_ON_TIME:
              if(millis()-lastPageChange<1000)break;
              currentOption=Utils::EngineInterface::NEXT;
              enginePeriod=NULL;
              lastPageChange=millis();
              break;
          case Utils::EngineInterface::NEXT:
            if(millis()-lastPageChange<1000)break;
            currentOption=Utils::CHANGE_OFF_TIME;
            enginePeriod = &Engine::getInstance().getEngineOffPeriod();
            lastPageChange=millis();
            break;
          case Utils::CHANGE_OFF_TIME:
            if(millis()-lastPageChange<1000)break;
            currentOption=Utils::CHANGE_ON_TIME;
            enginePeriod = &Engine::getInstance().getEngineOnPeriod();
            lastPageChange=millis();
            break;
          case Utils::PERIOD_MINUTES:
            enginePeriod->decreaseMinutes();
            break;
          case Utils::PERIOD_SECONDS:
            (*enginePeriod)--;
            break;
          default:break;
        }
    }  

  void render() override{
    char text[16];

          if(enginePeriod==&Engine::getInstance().getEngineOnPeriod() ){
            sprintf(text,"Cykl ON: %s ",Engine::getInstance().getEngineOnPeriod().toString() );
            LCD::getInstance().writeToLine(0,text);
          }
          else if( enginePeriod==&Engine::getInstance().getEngineOffPeriod() ){
            sprintf(text,"Cykl OFF: %s",Engine::getInstance().getEngineOffPeriod().toString() );
            LCD::getInstance().writeToLine(0,text);
          }


          switch(currentOption){
          case Utils::CHANGE_ON_TIME:
            sprintf(text,"Zmien cykl ON  ");
            LCD::getInstance().writeToLine(1,text);
              break;
          case Utils::CHANGE_OFF_TIME:
            sprintf(text,"Zmien cykl OFF ");
            LCD::getInstance().writeToLine(1,text);
            break;
          case Utils::PERIOD_MINUTES:
            LCD::getInstance().underlineChar(0,10);          
            break;
          case Utils::PERIOD_SECONDS:
            LCD::getInstance().hideCursor();
            LCD::getInstance().underlineChar(0,13);   
            break;
          case Utils::EngineInterface::NEXT:
            sprintf(text,"     USTAW     ");
            LCD::getInstance().writeToLine(0,text);
            sprintf(text,"  TEMPERATURA  ");
            LCD::getInstance().writeToLine(1,text);
            break;
          default:break;
        }
    }

    private:
    EngineUI(){
      enginePeriod = &Engine::getInstance().getEngineOnPeriod();
      currentOption = Utils::CHANGE_ON_TIME;
      lastPageChange = millis();
    }
    ~EngineUI(){     
       delete enginePeriod;
      }
  unsigned int lastPageChange;
  Utils::AlarmTime *enginePeriod; 
  Utils::EngineInterface currentOption; 

  
  };
