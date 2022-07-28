# 1 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
#define DEBUG_BAUD_RATE 1200
#define ONBOARD_LED 2

# 5 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2

# 7 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2


# 10 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2
# 11 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2


# 14 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2



SerialControllerInterface *serialController;
// SerialControllerImpl *serialController;
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
  pinMode(2,0x02);

  Serial.begin(9600);
  Serial2.begin(1200);
  serialController =new SerialControllerImpl();
  // Machine::getInstance().addObserver( &serialController);
  serialController->observe(&Machine::getInstance());
  xTaskCreatePinnedToCore(&hardwareTask, "hardware", 2108, 
# 52 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 3 4
                                                          __null
# 52 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
                                                              , 1, &HardwereTasks, 1);

}

void loop()
{
  serialController->listen();
}
