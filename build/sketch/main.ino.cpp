#include <Arduino.h>
#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
//#define OLED
#include "network.h"
#include "Machine.h"
#include "Encoder.h"
#include "Oled.h"

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void (*update)()=NULL;


#line 19 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void test(void *arg);
#line 39 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void setup();
#line 58 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void loop();
#line 12 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void IRAM_ATTR timerTick(){
  portENTER_CRITICAL(&timerMux);
  Machine::getInstance().machineTimerTick();
  portEXIT_CRITICAL(&timerMux);
}

TaskHandle_t HardwereTasks;
 void test(void  *arg){
  Serial.printf("Starting on core : %d",xPortGetCoreID());
  Oled::getInstance();
  DS18B20::getInstance();
  Heater::getInstance();
  Engine::getInstance();
  Encoder::setUp();
  timerAttachInterrupt(Machine::getInstance().machineTimer,&timerTick,true);
  

    for(;;){
    Oled::getInstance().oledControler->render(); 
    DS18B20::getInstance().updateTemperature();
    if(Network::getInstance().update!=NULL){
        ( Network::getInstance().*(Network::getInstance().update) )(); 
        Network::getInstance().update=NULL;
    } 
    }
 }
   
void setup() {
  Serial.begin(115200);
// Create filesystemon esp32
  if( !SPIFFS.begin(true)){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }


  // network stuff is runing on default core 1  
  Network::getInstance();

  
  // run the application on core 0
  xTaskCreatePinnedToCore(test,"hardwere",2000,NULL,2,&HardwereTasks,0);
}



void loop() {  
  // Look for and handle WebSocket data
  
  Network::getInstance().getWebsocket().loop();
}

