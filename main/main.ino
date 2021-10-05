
#include "network.h"
#include "Machine.h"
#include "Encoder.h"

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR timerTick(){
  portENTER_CRITICAL(&timerMux);
  Machine::getInstance().machineTimerTick();
  portEXIT_CRITICAL(&timerMux);
}

TaskHandle_t HardwereTasks;
 void test(void  *arg){
  Serial.printf("Starting on core : %d",xPortGetCoreID());
  LCD::getInstance();
  DS18B20::getInstance();
  Heater::getInstance();
  Engine::getInstance();
  Encoder::setUp();
  timerAttachInterrupt(Machine::getInstance().machineTimer,&timerTick,true);
  

    for(;;){
    LCD::getInstance().lcdControler->render(); 
    DS18B20::getInstance().updateTemperature();
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
  Network::createAccsesPoint();

  
  // run the application on core 0
  xTaskCreatePinnedToCore(test,"hardwere",2000,NULL,2,&HardwereTasks,0);
}



void loop() {  
  // Look for and handle WebSocket data
  
  Network::webSocket.loop();
}
