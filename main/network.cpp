
#pragma once

#include <stdlib.h>
#include "DS18B20.h"
#include "Engine.h"
#include "Machine.h"
#include "network.h"
#include "resources/Credentials.h"

    
void Network::updateDS8B20(){
    char msg_buf[25];
    sprintf(msg_buf, "ds18b20Temp%.2f", DS18B20::getInstance().getTemperature() );
    updateClients(msg_buf);
}

void Network::updateAimedTemp(){
    char msg_buf[25];
    sprintf(msg_buf, "setAimedTemp%.2f", DS18B20::getInstance().getAimedTemperature() );
    updateClients(msg_buf);
}

void Network::updateMachine(){
    char msg_buf[25];
    sprintf(msg_buf, "setMachineState%d", Machine::getInstance().isOn() ? 1 : 0);
    updateClients(msg_buf);
}

void Network::updateMachineTimeOn(){
    char msg_buf[25];
    sprintf(msg_buf, "setMachineTime%s", Machine::getInstance().getWorkingTime().toString() );
    updateClients(msg_buf);
}

void Network::updateEngineTimeOn(){
    char msg_buf[25];
    sprintf(msg_buf, "setEngineTimeOn%s", Engine::getInstance().getEngineOnPeriod().toString() );
    updateClients(msg_buf);
}

void Network::updateEngineTimeOff(){
    char msg_buf[25];
    sprintf(msg_buf, "setEngineTimeOff%s", Engine::getInstance().getEngineOffPeriod().toString() );
    updateClients(msg_buf);
}

WebSocketsServer& Network::getWebsocket(){
    return webSocket;
}



MESSAGES Network::map(std::string text){

       if( text.find("setMachineTime")!=std::string::npos ) return MESSAGES::CHANGE_MACHINE_TIME;
       if( text.find("getMachineTime")!=std::string::npos ) return MESSAGES::SEND_MACHINE_TIME;
       if( text.find("setMachineState")!=std::string::npos ) return MESSAGES::CHANGE_MACHINE_STATE;
       if( text.find("getMachineState")!=std::string::npos ) return MESSAGES::SEND_MACHINE_STATE;
       if( text.find("getTemp")!=std::string::npos ) return MESSAGES::SEND_TEMP;
       if( text.find("setAimedTemp")!=std::string::npos ) return MESSAGES::CHANGE_AIMED_TEMP;
       if( text.find("getAimedTemp")!=std::string::npos ) return MESSAGES::SEND_AIMED_TEMP;
       if( text.find("setEngineTimeOn")!=std::string::npos ) return MESSAGES::CHANGE_ENGINE_TIME_ON;
       if( text.find("getEngineTimeOn")!=std::string::npos ) return MESSAGES::SEND_ENGINE_TIME_ON;
       if( text.find("setEngineTimeOff")!=std::string::npos ) return MESSAGES::CHANGE_ENGINE_TIME_OFF;
       if( text.find("getEngineTimeOff")!=std::string::npos ) return MESSAGES::SEND_ENGINE_TIME_OFF;

      return MESSAGES::NONE;
}


// Callback: receiving any WebSocket message
void Network::onWebSocketEvent(uint8_t client_num,WStype_t type,
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
            // updateAimedTemp();
            }
            break; 
          case SEND_MACHINE_STATE:{
              sprintf(msg_buf, "setMachineState%d", Machine::getInstance().isOn() ? 1 : 0);
              webSocket.sendTXT(client_num, msg_buf);
              }
             break;          
          case CHANGE_MACHINE_STATE:{
              int state = atoi( (char*)payload );
              if (Machine::getInstance().isOn() != state)
                  Machine::getInstance().togleMachine();
              }    
              break;
          case SEND_MACHINE_TIME:{
              sprintf(msg_buf, "setMachineTime%s", Machine::getInstance().getWorkingTime().toString() );
              webSocket.sendTXT(client_num, msg_buf);
              }
              break;
          case CHANGE_MACHINE_TIME:{
              int offset = std::string("setMachineTime").length();
              Machine::getInstance().setWorkingTime(Utils::AlarmTime( (char*)payload + offset ));
              
              Serial.printf("New MachineTime: %s", Machine::getInstance().getWorkingTime().toString());
            //   updateMachineTimeOn();
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
            //   updateEngineTimeOn();
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
            //   updateEngineTimeOff();
              }  
              break;
          default:
              break;          
    }
    
}



void Network::updateClients(char *msg){
     for(int client_num=webSocket.connectedClients();client_num>=0;client_num--)
        webSocket.sendTXT(client_num, msg);
}

void Network::onMachineStateChanged(){
    if(machineStateChanged){
        updateMachine();
        machineStateChanged=false;
    }
}


    Network::Network():
    server(AsyncWebServer(80) ),
    webSocket(WebSocketsServer(1337) )
    {
        // Start access point
        #ifdef ACCSES_POINT_MODE
            WiFi.softAP(AP_NAME, AP_PSWD);
            Serial.println("\nAP running\nMy IP address: ");
            Serial.println(WiFi.softAPIP());
         #else
            WiFi.mode(WIFI_STA);
            WiFi.begin(WIFI_SSID,WIFI_PSWD);
            Serial.printf("\nConnecting to %s .",WIFI_SSID );
            while (WiFi.status() != WL_CONNECTED){
                Serial.print(".");
                vTaskDelay(200);
            }
            int i=10;
            while (i--)
            {
                vTaskDelay(200);
            Serial.printf("Connectd. Local Ip-addr is %s \n.",WiFi.localIP());
            }
         #endif   
          server.on("/", HTTP_GET, onIndexRequest);
          server.onNotFound(onPageNotFound);
          server.begin();
          webSocket.begin();
          webSocket.onEvent(handleWebSocket);
          machineStateChanged=false;
    }