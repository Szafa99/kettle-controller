#include <Arduino.h>
#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"

#include "network.h"
#include "Machine.h"
#include "Encoder.h"
#include "MainUI.h"

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#line 18 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void hardwareTask(void *arg);
#line 33 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void setup();
#line 52 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
void loop();
#line 9 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\main.ino"
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
  Encoder::setUp();
  timerAttachInterrupt(Machine::getInstance().machineTimer, &timerTick, true);

  for (;;)
  {
    LCD::getInstance().lcdControler->render();
    Encoder::onEncoderTurn();
    // // DS18B20::getInstance().updateTemperature();
  }
}

void setup()
{
  Serial.begin(115200);
  // Create filesystemon esp32
  if (!SPIFFS.begin(true))
  {
    Serial.println("Error mounting SPIFFS");
    while (1)
      ;
  }

  // network stuff is runing on default core 1
  // Network::createAccsesPoint();
  test(NULL);

  // run the application on core 0
  // xTaskCreatePinnedToCore(&hardwareTask, "hardwere", 2000, NULL, 2, &HardwereTasks, 0);
}

void loop()
{
  // Look for and handle WebSocket data

  // Network::webSocket.loop();
}

