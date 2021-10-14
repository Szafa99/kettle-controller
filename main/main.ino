
#include "network.h"
#include "Machine.h"
#include "Encoder.h"
#include "MainUI.h"

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
  LCD::getInstance();
  MainUI::getInstance().firstRender();
  Encoder::setUp();
  timerAttachInterrupt(Machine::getInstance().machineTimer, &timerTick, true);

  // for (;;)
  // {
  //   LCD::getInstance().lcdControler->render();
  //   Encoder::onEncoderTurn();
  //   // // DS18B20::getInstance().updateTemperature();
  // }
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
  hardwareTask(NULL);

  // run the application on core 0
  // xTaskCreatePinnedToCore(&hardwareTask, "hardwere", 2000, NULL, 2, &HardwereTasks, 0);
}

void loop()
{
  // Look for and handle WebSocket data
    LCD::getInstance().lcdControler->render();
    Encoder::onEncoderTurn();

  // Network::webSocket.loop();
}
