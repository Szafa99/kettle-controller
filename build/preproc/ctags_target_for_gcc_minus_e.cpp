# 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"

# 3 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2
# 4 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2
# 5 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2
# 6 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2

portMUX_TYPE timerMux = { .owner = 0xB33FFFFF, .count = 0, };

void __attribute__((section(".iram1" "." "25"))) timerTick(){
  vTaskEnterCritical(&timerMux);
  Machine::getInstance().machineTimerTick();
  vTaskExitCritical(&timerMux);
}

TaskHandle_t HardwereTasks;
 void test(void *arg){
  LCD::getInstance();
  MainUI::getInstance().firstRender();
  Encoder::setUp();
  timerAttachInterrupt(Machine::getInstance().machineTimer,&timerTick,true);



    // for(;;){
    // // LCD::getInstance().lcdControler->render(); 
    // // DS18B20::getInstance().updateTemperature();
    // }
 }

void setup() {
  Serial.begin(115200);
// Create filesystemon esp32
  if( !SPIFFS.begin(true)){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }


  // network stuff is runing on default core 1  
  // Network::createAccsesPoint();
  test(
# 41 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
      __null
# 41 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
          );

  // run the application on core 0
  // xTaskCreatePinnedToCore(test,"hardwere",2000,NULL,2,&HardwereTasks,0);
}



void loop() {
  // Look for and handle WebSocket data
  Encoder::onEncoderTurn();
  // Network::webSocket.loop();
}
