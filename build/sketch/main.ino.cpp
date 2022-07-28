#include <Arduino.h>
#line 1 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
#define DEBUG_BAUD_RATE 1200 
#define ONBOARD_LED  2

#include <OneWire.h>

#include <DallasTemperature.h>


#include "Machine.h"
#include "DS18B20.h"
#include "heater.h"
#include "Engine.h"
#include "SerialControllerImpl.h"



SerialControllerInterface *serialController;
// SerialControllerImpl *serialController;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#line 30 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
void hardwareTask(void *arg);
#line 43 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
void setup();
#line 56 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
void loop();
#line 21 "c:\\Users\\asz\\Desktop\\projekty\\Cheese-Kettle\\main\\main.ino"
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
  pinMode(ONBOARD_LED,OUTPUT);

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

