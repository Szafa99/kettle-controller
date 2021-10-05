
//https://shawnhymel.com/1882/how-to-create-a-web-server-with-websockets-using-an-esp32-in-arduino/
#pragma once


#include <stdlib.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <string>

#include "DS18B20.h"
#include "Engine.h"
#include "Utils.h"
#include "Machine.h"



enum MESSAGES{CHANGE_AIMED_TEMP,SEND_TEMP,SEND_AIMED_TEMP,SEND_MACHINE_STATE,CHANGE_MACHINE_STATE,
            CHANGE_MACHINE_TIME,SEND_MACHINE_TIME,CHANGE_ENGINE_TIME_ON,SEND_ENGINE_TIME_ON,
            CHANGE_ENGINE_TIME_OFF,SEND_ENGINE_TIME_OFF,NONE};

class Network{

public:

static Network& getInstance(){
    static Network instance;
    return instance;
}

void (Network::*update)()=NULL;

void operator=(Network const&) = delete;
Network(Network const&) = delete;
    
void updateDS8B20(){
    char msg_buf[25];
    sprintf(msg_buf, "ds18b20Temp%.2f", DS18B20::getInstance().getTemperature() );
    updateClients(msg_buf);
}

void updateAimedTemp(){
    char msg_buf[25];
    sprintf(msg_buf, "setAimedTemp%.2f", DS18B20::getInstance().getAimedTemperature() );
    updateClients(msg_buf);
}

void updateMachine(){
    char msg_buf[25];
    sprintf(msg_buf, "setMachineState%d", Machine::getInstance().getState() );
    updateClients(msg_buf);
}

void updateMachineTimeOn(){
    char msg_buf[25];
    sprintf(msg_buf, "setMachineTime%s", Machine::getInstance().workingTime.toString() );
    updateClients(msg_buf);
}

void updateEngineTimeOn(){
    char msg_buf[25];
    sprintf(msg_buf, "setEngineTimeOn%s", Engine::getInstance().getEngineOnPeriod().toString() );
    updateClients(msg_buf);
}

void updateEngineTimeOff(){
    char msg_buf[25];
    sprintf(msg_buf, "setEngineTimeOff%s", Engine::getInstance().getEngineOffPeriod().toString() );
    updateClients(msg_buf);
}

WebSocketsServer& getWebsocket(){
    return webSocket;
}


private:

MESSAGES map(std::string text){

       if( text.find("setMachineTime")!=std::string::npos ) return CHANGE_MACHINE_TIME;
       if( text.find("getMachineTime")!=std::string::npos ) return SEND_MACHINE_TIME;
       if( text.find("setMachineState")!=std::string::npos ) return CHANGE_MACHINE_STATE;
       if( text.find("getMachineState")!=std::string::npos ) return SEND_MACHINE_STATE;
       if( text.find("getTemp")!=std::string::npos ) return SEND_TEMP;
       if( text.find("setAimedTemp")!=std::string::npos ) return CHANGE_AIMED_TEMP;
       if( text.find("getAimedTemp")!=std::string::npos ) return SEND_AIMED_TEMP;
       if( text.find("setEngineTimeOn")!=std::string::npos ) return CHANGE_ENGINE_TIME_ON;
       if( text.find("getEngineTimeOn")!=std::string::npos ) return SEND_ENGINE_TIME_ON;
       if( text.find("setEngineTimeOff")!=std::string::npos ) return CHANGE_ENGINE_TIME_OFF;
       if( text.find("getEngineTimeOff")!=std::string::npos ) return SEND_ENGINE_TIME_OFF;

      return NONE;
}


// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,WStype_t type,
                      uint8_t * payload,size_t length) {
  
  char msg_buf[22];


  if(type==WStype_DISCONNECTED)
      Serial.printf("Client with ip %u disconnected!\n", client_num);

  else if(type==WStype_CONNECTED){
      Serial.printf("Connection from cleint with ip: %u\n", webSocket.remoteIP(client_num));
      updateDS8B20();
      updateAimedTemp();
      updateMachine();
      updateMachineTimeOn();
      updateEngineTimeOn();
      updateEngineTimeOff();
      }

   else if(type==WStype_TEXT)
      switch( map( (char*)payload) ){
          case SEND_TEMP:{
            sprintf(msg_buf, "ds18b20Temp%.2f", DS18B20::getInstance().getTemperature() );
            webSocket.sendTXT(client_num, msg_buf);
            }
            break;
          case SEND_AIMED_TEMP:{  
            sprintf(msg_buf, "setAimedTemp%.2f", DS18B20::getInstance().getAimedTemperature() );
            webSocket.sendTXT(client_num, msg_buf);
            }
              break;          
          case CHANGE_AIMED_TEMP:{
            DS18B20::getInstance().setAimedTemperature( strtod( (char*)payload,NULL) );
            updateAimedTemp();
            }
            break; 
          case SEND_MACHINE_STATE:{
              sprintf(msg_buf, "setMachineState%d", Machine::getInstance().getState());
              webSocket.sendTXT(client_num, msg_buf);
              }
             break;          
          case CHANGE_MACHINE_STATE:{
              int state = atoi( (char*)payload );
              Machine::getInstance().setState(state);
              updateMachine();
              }    
              break;
          case SEND_MACHINE_TIME:{
              sprintf(msg_buf, "setMachineTime%s", Machine::getInstance().workingTime.toString() );
              webSocket.sendTXT(client_num, msg_buf);
              }
              break;
          case CHANGE_MACHINE_TIME:{
              Machine::getInstance().workingTime=Utils::AlarmTime( (char*)payload + std::string("setMachineTime").length() );
              Serial.printf("New MachineTime: %s", Machine::getInstance().workingTime.toString());
              updateMachineTimeOn();
              }    
              break;
          case SEND_ENGINE_TIME_ON:{
              sprintf(msg_buf, "setEngineTimeOn%s", Engine::getInstance().getEngineOnPeriod().toString() );
              webSocket.sendTXT(client_num, msg_buf);
              }
              break;
          case CHANGE_ENGINE_TIME_ON:{
              Engine::getInstance().setEngineOnPeriod( Utils::AlarmTime( (char*)payload + std::string("setEngineTimeOn").length() ) );
              Serial.printf("New EngineeTimeOn: %s", Engine::getInstance().getEngineOnPeriod().toString());
              updateEngineTimeOn();
              }    
              break;
          case SEND_ENGINE_TIME_OFF:{
              sprintf(msg_buf, "setEngineTimeOff%s", Engine::getInstance().getEngineOffPeriod().toString() );
              webSocket.sendTXT(client_num, msg_buf);
              }
              break;
          case CHANGE_ENGINE_TIME_OFF:{
              Engine::getInstance().setEngineOffPeriod( Utils::AlarmTime( (char*)payload + std::string("setEngineTimeOff").length() ));
              Serial.printf("New EngineeTimeOff: %s", Engine::getInstance().getEngineOffPeriod().toString());
              updateEngineTimeOff();
              }  
              break;
          default:
              break;          
    }
    
}

 static void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("sending index.html[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

 static void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}


void updateClients(char *msg){
     for(int client_num=webSocket.connectedClients();client_num>=0;client_num--)
        webSocket.sendTXT(client_num, msg);
}

static void handleWebSocket(uint8_t client_num,WStype_t type,
                      uint8_t * payload,size_t length){

    Network::getInstance().onWebSocketEvent(client_num, type, payload,length);

}

    Network():
    server(AsyncWebServer(80) ),
    webSocket(WebSocketsServer(1337) )
    {
        // Start access point
          WiFi.softAP("SeryGosi", "12345678");
          Serial.println("\nAP running\nMy IP address: ");
          Serial.println(WiFi.softAPIP());
          server.on("/", HTTP_GET, onIndexRequest);
          server.onNotFound(onPageNotFound);
          server.begin();
          webSocket.begin();
          webSocket.onEvent(handleWebSocket);
    }

AsyncWebServer server;
WebSocketsServer webSocket;

};
