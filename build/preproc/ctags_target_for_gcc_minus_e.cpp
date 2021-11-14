# 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"

# 3 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2
# 4 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2
# 5 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2
# 6 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2
# 7 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 2



portMUX_TYPE timerMux = { .owner = 0xB33FFFFF, .count = 0, };

void __attribute__((section(".iram1" "." "19"))) timerTick()
{
  vTaskEnterCritical(&timerMux);
  Machine::getInstance().machineTimerTick();
  vTaskExitCritical(&timerMux);
}

TaskHandle_t HardwereTasks;

void hardwareTask(void *arg)
{

  LCD::getInstance();
  MainUI::getInstance().firstRender();
  Heater::getInstance();
  Engine::getInstance();
  Encoder::setUp();
  timerAttachInterrupt(Machine::getInstance().machineTimer, &timerTick, true);

   for (;;)
   {
     LCD::getInstance().lcdControler->render();
     LCD::getInstance().onPassive();
     Encoder::onEncoderTurn();
     Network::getInstance().onMachineStateChanged();
     DS18B20::getInstance().updateTemperature();
   }
}

void setup()
{

  Serial.begin(9600);
  // Create filesystemon esp32
  if (!SPIFFS.begin(true))
  {
    Serial.println("Error mounting SPIFFS");
    while (1)
      ;
  }

  // network stuff is runing on default core 1
   Network::getInstance();

  // run the application on core 0
    xTaskCreatePinnedToCore(&hardwareTask, "hardwere", 2108, 
# 57 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino" 3 4
                                                            __null
# 57 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
                                                                , 1, &HardwereTasks, 1);
}

void loop()
{
  // Look for and handle WebSocket data
Network::getInstance().getWebsocket().loop();
}
