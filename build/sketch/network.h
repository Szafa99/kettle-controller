#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\network.h"

//https://shawnhymel.com/1882/how-to-create-a-web-server-with-websockets-using-an-esp32-in-arduino/
#pragma once

#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

enum MESSAGES
{
  CHANGE_AIMED_TEMP,
  SEND_TEMP,
  SEND_AIMED_TEMP,
  SEND_MACHINE_STATE,
  CHANGE_MACHINE_STATE,
  CHANGE_MACHINE_TIME,
  SEND_MACHINE_TIME,
  CHANGE_ENGINE_TIME_ON,
  SEND_ENGINE_TIME_ON,
  CHANGE_ENGINE_TIME_OFF,
  SEND_ENGINE_TIME_OFF,
  NONE
};

class Network
{

public:
  static Network &getInstance()
  {
    static Network instance;
    return instance;
  }

  void (Network::*update)() = NULL;
  void operator=(Network const &) = delete;
  Network(Network const &) = delete;


  void onMachineStateChanged();
  void updateDS8B20();
  void updateAimedTemp();
  void updateMachine();
  void updateMachineTimeOn();
  void updateEngineTimeOn();
  void updateEngineTimeOff();
  WebSocketsServer &getWebsocket();
  bool machineStateChanged;

private:
  MESSAGES map(std::string text);
  void updateClients(char *msg);
  void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length);

  static void onIndexRequest(AsyncWebServerRequest *request)
  {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("sending index.html[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/index.html", "text/html");
  }

  static void onPageNotFound(AsyncWebServerRequest *request)
  {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                   "] HTTP GET request of " + request->url());
    request->send(404, "text/plain", "Nie ma takygo czygos.Obejrzi czy plik dodoles do SPIFS");
  }

  static void handleWebSocket(uint8_t client_num, WStype_t type,
                              uint8_t *payload, size_t length)
  {

    Network::getInstance().onWebSocketEvent(client_num, type, payload, length);
  }

  Network();

  WebSocketsServer webSocket;
  AsyncWebServer server;

};