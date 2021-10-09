#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\network.h"

//https://shawnhymel.com/1882/how-to-create-a-web-server-with-websockets-using-an-esp32-in-arduino/
#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#include "DS18B20.h"
#include "Engine.h"


namespace Network{

//=====================FUNCTION DECLARATIONS=============
 void             updateClients(char*);
 void             createAccsesPoint();
 void             onWebSocketEvent(uint8_t client_num,WStype_t type,uint8_t * payload,size_t length);
 void             onIndexRequest(AsyncWebServerRequest *request);
 void             onPageNotFound(AsyncWebServerRequest *request) ;
//=======================================================
//===================Variabels==========================
AsyncWebServer server(80);
WebSocketsServer webSocket(1337);

char * ssid="SeryGosi";
char * password="12345678";

double timeSendTemp = millis();

portMUX_TYPE timerMUX= portMUX_INITIALIZER_UNLOCKED;
 
//=====================================================
    
// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,WStype_t type,
                      uint8_t * payload,size_t length) {
  
  char msg_buf[20];
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
        sprintf(msg_buf, "ds18b20Temp%.2f", DS18B20::getInstance().getTemperature() );
        webSocket.sendTXT(client_num, msg_buf);
        sprintf(msg_buf, "aimedTemp%.2f", DS18B20::getInstance().getAimedTemperature() );
        webSocket.sendTXT(client_num, msg_buf);
        sprintf(msg_buf, "engineState%d", Engine::getInstance().getState() );
        webSocket.sendTXT(client_num, msg_buf);

      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Received text: %s\n", client_num, payload);
      // Chnge aimed temperature 
      if ( strstr((char *)payload, "setTemp") != NULL ) {
        DS18B20::getInstance().setAimedTemperature( strtod( (char*)payload,NULL) );
        Serial.printf("Change temperature to %.2f",DS18B20::getInstance().getAimedTemperature());
        sprintf(msg_buf, "aimedTemp%.2f", DS18B20::getInstance().getAimedTemperature() );
        updateClients(msg_buf);
      } 
      // Send the temperature fetched from dsb1820 to the user
      else if ( strcmp((char *)payload, "getTemp") == 0 ) {
        sprintf(msg_buf, "ds18b20Temp%.2f", DS18B20::getInstance().getTemperature() );
        Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);
      } 
      // Send the aimed for temperature 
      else if ( strcmp((char *)payload, "getAimedTemp") == 0 ) {
        sprintf(msg_buf, "aimedTemp%.2f", DS18B20::getInstance().getAimedTemperature() );
        Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
        webSocket.sendTXT(client_num, msg_buf);
      }
       else if ( strcmp((char *)payload, "getEngineSate") == 0 ) {
          sprintf(msg_buf, "engineState%d", Engine::getInstance().getState());
          webSocket.sendTXT(client_num, msg_buf);
      }
      else if ( strstr((char *)payload, "setEngine") != NULL ) {
          int state = atoi( (char*)payload );
          if(state)
            Engine::getInstance().turnON();
          else
            Engine::getInstance().turnOFF();
          sprintf(msg_buf, "engineState%d", Engine::getInstance().getState() );
          updateClients(msg_buf);    
      } 
        else {
        Serial.println("[%u] Message not recognized");
      }
      break;
    }
}

  void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("sending index.html[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

 void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

void createAccsesPoint() {
        // Start access point
          WiFi.softAP(ssid, password);
          Serial.println("\nAP running\nMy IP address: ");
          Serial.println(WiFi.softAPIP());
        
          server.on("/", HTTP_GET, onIndexRequest);
          server.onNotFound(onPageNotFound);
          server.begin();
          webSocket.begin();
          webSocket.onEvent(onWebSocketEvent);       
        }


void updateClients(char *msg){
     for(int client_num=webSocket.connectedClients();client_num>=0;client_num--)
        webSocket.sendTXT(client_num, msg);
}




}


#endif
