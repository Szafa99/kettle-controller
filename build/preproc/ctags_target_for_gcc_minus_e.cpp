# 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
//#define OLED
# 3 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2

# 5 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2


portMUX_TYPE timerMux = { .owner = 0xB33FFFFF, .count = 0, };

void (*update)()=
# 9 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
                __null
# 9 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
                    ;


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
# 32 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
                                     __null
# 32 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
                                         ){
        ( Network::getInstance().*(Network::getInstance().update) )();
        Network::getInstance().update=
# 34 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
                                     __null
# 34 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
                                         ;
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
  xTaskCreatePinnedToCore(test,"hardwere",2000,
# 53 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
                                              __null
# 53 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
                                                  ,2,&HardwereTasks,0);
}



void loop() {
  // Look for and handle WebSocket data

  Network::getInstance().getWebsocket().loop();
}
