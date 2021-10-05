#pragma once

#include "HeaterUI.h"
#include "EngineUI.h"
#include "Utils.h"


     LCD::LCD():
     lcd(LiquidCrystal_I2C(0x27,20,2)){
        lcd.clear();
        lcd.init();       
        lcd.backlight();
        lcd.print("Sander Cheese");  
        lcdControler=&EngineUI::getInstance();
 
     }


    
void Engine::handleEngineCycles(){
    if( Machine::getInstance().workingTime.timerEnded() ) return;

    if(isOffTime.toSeconds()>0 ){
        if(!isOffTime--)
            turnON();
    } 
    else if(isOnTime.toSeconds()>0){
        if(!isOnTime--) 
            turnOFF();           
    }
    else{
        isOffTime=engineOffPeriod;
        isOnTime=engineOffPeriod;
    }
  
}
  
void DS18B20_UI::pushBtn() {
   LCD::getInstance().lcdControler=&HeaterUI::getInstance();
 };