#include <Arduino.h>
#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"

#include "network.h"
#include "Machine.h"
#include "Encoder.h"
#include "MainUI.h"
#include "DS18B20.h"
#include "heater.h"
#include "Engine.h"


portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#line 22 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void hardwareTask(void *arg);
#line 42 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void setup();
#line 60 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void loop();
#line 13 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void IRAM_ATTR timerTick()
{
  portENTER_CRITICAL(&timerMux);
  Machine::getInstance().machineTimerTick();
  portEXIT_CRITICAL(&timerMux);
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

  Serial.begin(19200);
  // Create filesystemon esp32
  if (!SPIFFS.begin(true))
  {
    Serial.println("Error mounting SPIFFS");
    while (1);
  }

  // network stuff is runing on default core 1
   Network::getInstance();

  // run the application on core 1
    xTaskCreatePinnedToCore(&hardwareTask, "hardwere", 2108, NULL, 1, &HardwereTasks, 1);
}

void loop()
{
  // Look for and handle WebSocket data
Network::getInstance().getWebsocket().loop();
}

