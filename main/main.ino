#include <OneWire.h>

#include <DallasTemperature.h>


#include "Machine.h"
#include "DS18B20.h"
#include "heater.h"
#include "Engine.h"
#include "SerialControllerImpl.h"

#define DEBUG_BAUD_RATE 1200 

SerialControllerInterface *serialController;
// SerialControllerImpl *serialController;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR timerTick()
{
  portENTER_CRITICAL(&timerMux);
  Machine::getInstance().machineTimerTick();
  portEXIT_CRITICAL(&timerMux);
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
  Serial2.begin(DEBUG_BAUD_RATE);
  serialController =new SerialControllerImpl();
  // Machine::getInstance().addObserver( &serialController);
  serialController->observe(&Machine::getInstance());
  xTaskCreatePinnedToCore(&hardwareTask, "hardware", 2108, NULL, 1, &HardwereTasks, 1);

}

void loop()
{
  serialController->listen();
}
