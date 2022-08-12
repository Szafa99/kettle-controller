# 1 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
#define DEBUG_BAUD_RATE 1200
#define ONBOARD_LED 2
#define DEBUG 1

# 6 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2

# 8 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2


# 11 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2
# 12 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2


# 15 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 2


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
  pinMode(2,0x02);

  Serial.begin(115200);
  serialController =new SerialControllerImpl();
  serialController->observe(&Machine::getInstance());
  xTaskCreatePinnedToCore(&hardwareTask, "hardware", 2108, 
# 49 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino" 3 4
                                                          __null
# 49 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
                                                              , 1, &HardwereTasks, 1);

}

void loop()
{
  serialController->listen();
}
