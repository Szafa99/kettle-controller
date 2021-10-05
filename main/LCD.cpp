#pragma once

#include "HeaterUI.h"
#include "EngineUI.h"
#include "Utils.h"
#include "network.h"


     LCD::LCD():
     lcd(LiquidCrystal_I2C(0x27,20,2)){
        lcd.clear();
        lcd.init();       
        lcd.backlight();
        lcd.print("Sander Cheese");  
        lcdControler=&EngineUI::getInstance();
 
     }


    

  






