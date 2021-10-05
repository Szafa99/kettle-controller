//https://github.com/johnrickman/LiquidCrystal_I2C
//SDA-P21
//SCL-P22
#pragma once

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Controler.h"


class LCD{

    
    public:
     
     InterfaceControler * lcdControler;
 
        
      static LCD& getInstance(){
          static  LCD instance;
          return instance;
      }

      ~LCD(){
        delete lcdControler;
        }
     
       LiquidCrystal_I2C getLcd(){
        return lcd;
        }

     void operator=(LCD const&)=delete;
     LCD(LCD const&)=delete;

    void onPassive();

     void writeToLine(int line,char *text){
        hideCursor();
        lcd.setCursor(0,line);
        lcd.print(text);
      }


    void underlineChar(int row,int column){
        lcd.setCursor(column,row);
        lcd.cursor();
     }  

     void hideCursor(){
        lcd.noCursor();
      }


     private:      
     LCD();
     LiquidCrystal_I2C lcd;  
     bool isOn=true; 


  };

 
