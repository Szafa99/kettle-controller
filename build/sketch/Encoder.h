#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\Encoder.h"
#pragma once
#include "LCD.h"


namespace Encoder{
#define LEFT 16
#define RIGHT 17
#define ENCODER_BTN 5


void IRAM_ATTR B_FALL();
void IRAM_ATTR B_RISE();
void IRAM_ATTR A_RISE();
void IRAM_ATTR A_FALL();


volatile int pulsesRight,pulsesLeft, A_SIG=0, B_SIG=1;
volatile int lastPushEncoder = 0,lastTurn=0;
int checkTime ;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;



void IRAM_ATTR A_RISE(){
portENTER_CRITICAL(&mux);
 lastTurn=millis();
 detachInterrupt(LEFT);
 A_SIG=1;
 
 if(B_SIG==0)
     pulsesRight++;
 if(B_SIG==1)
     pulsesLeft++;
 attachInterrupt(digitalPinToInterrupt(LEFT), A_FALL, FALLING);

 portEXIT_CRITICAL(&mux);  
}

void IRAM_ATTR A_FALL(){
portENTER_CRITICAL(&mux);
 lastTurn=millis();
 detachInterrupt(LEFT);
 A_SIG=0;
 
 if(B_SIG==1)
     pulsesRight++;
 if(B_SIG==0)
     pulsesLeft++;
 attachInterrupt(LEFT, A_RISE, RISING);  
 
 portEXIT_CRITICAL(&mux);  
}

void IRAM_ATTR B_RISE(){
portENTER_CRITICAL(&mux);
 lastTurn=millis();
 detachInterrupt((RIGHT));
 B_SIG=1;
 
 if(A_SIG==1)
    pulsesRight++;
 
 if(A_SIG==0)
     pulsesLeft++;
     
 attachInterrupt(RIGHT, B_FALL, FALLING);

portEXIT_CRITICAL(&mux);  
}

void IRAM_ATTR B_FALL(){
portENTER_CRITICAL(&mux);
 lastTurn=millis();
 detachInterrupt( RIGHT );
 B_SIG=0;
 
 if(A_SIG==0)
    pulsesRight++;

 if(A_SIG==1)
     pulsesLeft++;
 attachInterrupt(RIGHT, B_RISE, RISING);

 portEXIT_CRITICAL(&mux);  
}



 void IRAM_ATTR onEncoderBtn(){
  portENTER_CRITICAL(&mux);

   if( millis()-lastPushEncoder>500 &&  millis()-lastTurn>1000){
    LCD::getInstance().lcdControler->pushBtn();
    lastPushEncoder=millis();
    lastTurn=millis();
  }
  portEXIT_CRITICAL(&mux);
  }
  

void onEncoderTurn(){
  // if(checkTime==0)return;

   if(millis()-checkTime>50){
            if(pulsesRight-pulsesLeft>=2){
            LCD::getInstance().lcdControler->turnRight();
            lastTurn=millis();
            }
            else if(pulsesLeft-pulsesRight>=2){
            LCD::getInstance().lcdControler->turnLeft();
            lastTurn=millis();
            }
             pulsesRight=0;
             pulsesLeft=0;
             checkTime=millis();
        }
}


  void setUp(){
  digitalWrite(LEFT,LOW);
  digitalWrite(RIGHT,LOW);
  digitalWrite(ENCODER_BTN,HIGH);
      
  pinMode(LEFT,INPUT);
  pinMode(RIGHT,INPUT);
  pinMode(ENCODER_BTN,INPUT_PULLUP);

  attachInterrupt(LEFT, A_RISE, RISING);
  attachInterrupt(RIGHT, B_RISE, RISING);
  attachInterrupt(ENCODER_BTN,onEncoderBtn,FALLING);
  
  pulsesRight=0;
  pulsesLeft=0;
  checkTime=0;
  lastTurn=0;
  }

}
