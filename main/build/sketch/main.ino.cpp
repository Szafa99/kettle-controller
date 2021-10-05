#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
//#define OLED
#include "network.h"
#include "Machine.h"
#include "Encoder.h"
#include "Oled.h"
#include "Arduino.h"

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// void (*update)()=NULL;


#line 21 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void test(void *arg);
#line 43 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void setup();
#line 69 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void loop();
#line 13 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
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
    vTaskDelay(11);
    }

 }
   
void setup() {
  Serial.begin(115200);
// Create filesystemon esp32
  // if( !SPIFFS.begin(true)){
  //   Serial.println("Error mounting SPIFFS");
  //   while(1);
  // }


  // network stuff is runing on default core 1  
  // Network::getInstance();

  
  // run hardwere stuff on core 0
  // xTaskCreatePinnedToCore(test,"hardware",2000,NULL,2,&HardwereTasks,0);
 Oled::getInstance();
  DS18B20::getInstance();
  Heater::getInstance();
  Engine::getInstance();
  Encoder::setUp();
  timerAttachInterrupt(Machine::getInstance().machineTimer,&timerTick,true);
 
}



void loop() {  
  // Look for and handle WebSocket data
  
   Oled::getInstance().oledControler->render(); 
    DS18B20::getInstance().updateTemperature();
 
  // Network::getInstance().getWebsocket().loop();
  // vTaskDelay(1);
}

