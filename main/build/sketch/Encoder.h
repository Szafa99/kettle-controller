#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\Encoder.h"
#pragma once
// #include "LCD.h"
#include "Oled.h"

namespace Encoder{
#define INC_TEMP_PIN 39
#define DEC_TEMP_PIN 34
#define ENCODER_BTN 25

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile int lastEncoded = 0;
volatile int firstTime = 0;
volatile int lastChangeTemp=0;

enum ENCODER_STATES{LEFT,NEUTRAL,RIGHT};

ENCODER_STATES encoderState=NEUTRAL;

void IRAM_ATTR onEncoderTurn(){
  portENTER_CRITICAL(&mux);

int MSB = digitalRead(INC_TEMP_PIN); //MSB = most significant bit
int LSB = digitalRead(DEC_TEMP_PIN); //LSB = least significant bit

int encoded = (MSB << 1) |LSB; 
int sum = (lastEncoded << 2) | encoded;
      if(sum == 13 || sum == 4|| sum == 2 || sum == 11){
            if(encoderState==RIGHT) 
                Oled::getInstance().oledControler->turnRight();            
            else if(encoderState==NEUTRAL) encoderState=RIGHT;
            else encoderState=NEUTRAL;
      }
     else if(sum == 14 || sum == 7 || sum == 1 || sum == 8 ){ 
            if(encoderState==LEFT)
                 Oled::getInstance().oledControler->turnLeft();
            else if(encoderState==NEUTRAL) encoderState=LEFT;
            else encoderState=NEUTRAL;

      }
  
  lastEncoded = encoded; //store this value for next time 
  lastChangeTemp=millis();
  portEXIT_CRITICAL(&mux);  
 }



 void IRAM_ATTR onEncoderBtn(){
  portENTER_CRITICAL(&mux);

  if(millis()-firstTime>400)
    firstTime=0;
    
  if(firstTime==0 && millis()-lastChangeTemp>300){
    Oled::getInstance().oledControler->pushBtn();
    firstTime=millis();
  }
  portEXIT_CRITICAL(&mux);
  }
  


  void setUp(){
  digitalWrite(INC_TEMP_PIN,HIGH);
  digitalWrite(DEC_TEMP_PIN,HIGH);
  digitalWrite(ENCODER_BTN,HIGH);
      
  pinMode(INC_TEMP_PIN,INPUT_PULLUP);
  pinMode(DEC_TEMP_PIN,INPUT_PULLUP);
  pinMode(ENCODER_BTN,INPUT_PULLUP);

  attachInterrupt(INC_TEMP_PIN,onEncoderTurn,FALLING);
  attachInterrupt(DEC_TEMP_PIN,onEncoderTurn,FALLING);
  attachInterrupt(ENCODER_BTN,onEncoderBtn,FALLING);
  }

}
