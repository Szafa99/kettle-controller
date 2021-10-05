# 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
//#define OLED
# 3 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2

# 5 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2

# 7 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2

portMUX_TYPE timerMux = { .owner = 0xB33FFFFF, .count = 0, };

// void (*update)()=NULL;


void __attribute__((section(".iram1" "." "18"))) timerTick(){
  vTaskEnterCritical(&timerMux);
  Machine::getInstance().machineTimerTick();
  vTaskExitCritical(&timerMux);
}

TaskHandle_t HardwereTasks;

 void test(void *arg){
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
         if(Network::getInstance().update!=
# 34 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
                                          __null
# 34 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
                                              ){
        ( Network::getInstance().*(Network::getInstance().update) )();
        Network::getInstance().update=
# 36 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
                                     __null
# 36 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
                                         ;
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
