# 1 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
# 2 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2

# 4 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2


# 7 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2
# 8 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2


# 11 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2

#define DEBUG_BAUD_RATE 1200

SerialControllerInterface *serialController;
portMUX_TYPE timerMux = { .owner = 0xB33FFFFF, .count = 0, };

void __attribute__((section(".iram1" "." "16"))) timerTick()
{
  vTaskEnterCritical(&timerMux);
  Machine::getInstance().machineTimerTick();
  vTaskExitCritical(&timerMux);
}

TaskHandle_t HardwereTasks;

void hardwareTask(void *arg)
{

  Heater::getInstance();
  Engine::getInstance();
  timerAttachInterrupt(Machine::getInstance().machineTimer, &timerTick, true);

   for (;;)
   {
     DS18B20::getInstance().updateTemperature();
   }
}

void setup()
{

  Serial.begin(9600);
  Serial2.begin(1200);
  serialController =new SerialControllerImpl();
  // Machine::getInstance().addObserver( &serialController);
  serialController->observe(Machine::getInstance());
  xTaskCreatePinnedToCore(&hardwareTask, "hardware", 2108, 
# 47 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 3 4
                                                          __null
# 47 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
                                                              , 1, &HardwereTasks, 1);
}

void loop()
{
  serialController->listen();
}
