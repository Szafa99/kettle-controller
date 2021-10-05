#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\EngineUI.cpp"
#include "EngineUI.h"
#include "OledUI.h"
#include "Utils.h"
#include "network.h"
#include "LCD.h"
#include "Oled.h"
#include "Engine.h"

#ifdef LCD

  void EngineUI::pushBtn() {
          switch(currentOption){
          case Utils::CHANGE_ON_TIME:
              currentOption=Utils::PERIOD_MINUTES;
              break;
          case Utils::CHANGE_OFF_TIME:{
            currentOption=Utils::PERIOD_MINUTES;
            enginePeriod = &Engine::getInstance().getEngineOffPeriod();
          }break;
          case Utils::PERIOD_MINUTES:
            currentOption=Utils::PERIOD_SECONDS;
            break;
          case Utils::PERIOD_SECONDS:{
            if(enginePeriod==&Engine::getInstance().getEngineOnPeriod()){
              currentOption=Utils::CHANGE_ON_TIME;
              Network::getInstance().update = &Network::updateEngineTimeOn;
            }
            else{
               currentOption=Utils::CHANGE_OFF_TIME;
              Network::getInstance().update=&Network::updateEngineTimeOff;
            }
          }break;
          case Utils::EngineInterface::NEXT:{
               LCD::getInstance().lcdControler=&DS18B20_UI::getInstance();
          } break;
          default:break;
        }
    }

    void EngineUI::turnRight() {
          switch(currentOption){
          case Utils::CHANGE_ON_TIME:{

              if(millis()-lastPageChange<1000)break;
              lastPageChange=millis();
              enginePeriod = &Engine::getInstance().getEngineOffPeriod();
              currentOption=Utils::CHANGE_OFF_TIME;
          }
              break;
          case Utils::CHANGE_OFF_TIME:{
            if(millis()-lastPageChange<1000)break;
            lastPageChange=millis();
            currentOption=Utils::EngineInterface::NEXT;
            enginePeriod=NULL;
           }
            break;
          case Utils::PERIOD_MINUTES:
            enginePeriod->increaseMinutes();
            break;
          case Utils::PERIOD_SECONDS:
            (*enginePeriod)++;
            break;
          case Utils::EngineInterface::NEXT:{
            if(millis()-lastPageChange<1000)break;
            lastPageChange=millis();
            enginePeriod = &Engine::getInstance().getEngineOnPeriod();
            currentOption=Utils::CHANGE_ON_TIME;
          }
            break;    
          default:break;
        }
    }

  void EngineUI::turnLeft() {
          switch(currentOption){
          case Utils::CHANGE_ON_TIME:{
              if(millis()-lastPageChange<1000)break;
              currentOption=Utils::EngineInterface::NEXT;
              enginePeriod=NULL;
              lastPageChange=millis();
          }
              break;
          case Utils::EngineInterface::NEXT:{
            if(millis()-lastPageChange<1000)break;
            currentOption=Utils::CHANGE_OFF_TIME;
            enginePeriod = &Engine::getInstance().getEngineOffPeriod();
            lastPageChange=millis();
          }
            break;
          case Utils::CHANGE_OFF_TIME:{

            if(millis()-lastPageChange<1000)break;
            currentOption=Utils::CHANGE_ON_TIME;
            enginePeriod = &Engine::getInstance().getEngineOnPeriod();
            lastPageChange=millis();
          }
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

  void EngineUI::render() {
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
          case Utils::CHANGE_ON_TIME:{
            sprintf(text,"Zmien cykl ON  ");
            LCD::getInstance().writeToLine(1,text);
          }
              break;
          case Utils::CHANGE_OFF_TIME:{
            sprintf(text,"Zmien cykl OFF ");
            LCD::getInstance().writeToLine(1,text);
          }
            break;
          case Utils::PERIOD_MINUTES:
            LCD::getInstance().underlineChar(0,10);          
            break;
          case Utils::PERIOD_SECONDS:{
            LCD::getInstance().hideCursor();
            LCD::getInstance().underlineChar(0,13);   
          }
            break;
          case Utils::EngineInterface::NEXT:{
            sprintf(text,"     USTAW     ");
            LCD::getInstance().writeToLine(0,text);
            sprintf(text,"  TEMPERATURA  ");
            LCD::getInstance().writeToLine(1,text);
          }
            break;
          default:break;
        }
    }

#endif
#ifdef OLED
void EngineUI::pushBtn() {
          switch(currentOption){
          case Utils::PERIOD_MINUTES:
            currentOption=Utils::PERIOD_SECONDS;
            break;
          case Utils::PERIOD_SECONDS:{
            if(enginePeriod==&Engine::getInstance().getEngineOnPeriod())
              Network::getInstance().update = &Network::updateEngineTimeOn;
            else
              Network::getInstance().update=&Network::updateEngineTimeOff;
            
          currentOption=Utils::CHANGE_OFF_TIME;
          Oled::getInstance().oledControler = &OledUI::getInstance(); 

          }
          break;
          default:break;
        }
    }

    void EngineUI::turnRight() {
          switch(currentOption){
          case Utils::PERIOD_MINUTES:
            enginePeriod->increaseMinutes();
            Engine::getInstance().timeChanged=true;
            break;
          case Utils::PERIOD_SECONDS:
            (*enginePeriod)++;
            Engine::getInstance().timeChanged=true;
            break;
          default:break;
        }
    }

  void EngineUI::turnLeft() {
          switch(currentOption){
            case Utils::PERIOD_MINUTES:
            enginePeriod->decreaseMinutes();
            Engine::getInstance().timeChanged=true;
            break;
          case Utils::PERIOD_SECONDS:
            (*enginePeriod)--;
            Engine::getInstance().timeChanged=true;
            break;
          default:break;
        }
    }  

  void EngineUI::render() {
      if(Engine::getInstance().timeChanged) {
        char *text;
        if(enginePeriod==&Engine::getInstance().getEngineOnPeriod() ){
          sprintf(text,"      %.2fm",enginePeriod->toString());    
          Oled::getInstance().writeText2Rec(RIGHT_UPPER,text);
          Engine::getInstance().timeChanged=false;
        }
        else if(enginePeriod==&Engine::getInstance().getEngineOffPeriod() ){
          sprintf(text,"      %.2fm",enginePeriod->toString());    
          Oled::getInstance().writeText2Rec(RIGHT_DOWN,text);
          Engine::getInstance().timeChanged=false;
        }
      }
     
      
    }


#endif