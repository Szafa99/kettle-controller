#pragma once
#include <string>
#include <list>
#include <cstring>
#include "Arduino.h"

#include "SerialControllerInterface.h"

    namespace REQUEST_METHODS
    {
        const char *AIMED_TEMP = "AIMED_TEMP";
        const char *ENGINE_ON_PERIOD = "ENGINE_ON_PERIOD";
        const char *ENGINE_OFF_PERIOD = "ENGINE_OFF_PERIOD";
        const char *ENGINE_ON_PERIOD_LEFT = "ENGINE_ON_PERIOD_LEFT";
        const char *ENGINE_OFF_PERIOD_LEFT = "ENGINE_OFF_PERIOD_LEFT";
        const char *MACHINE_TIME = "MACHINE_TIME";
        const char *MACHINE_STATE = "MACHINE_STATE";
        const char *DS18B20_TEMP = "DS18B20_TEMP";
        
    }; // namespace REQUEST_METHODS

class SerialControllerImpl : public SerialControllerInterface
{

public:



    void onSerialMessage(std::string serialMsg) override
    {

        //   Serial.println(serialMsg.c_str());


        if (!serialMsg.empty() && serialMsg[serialMsg.length() - 1] == '\n')
            serialMsg.erase(serialMsg.length() - 1);

        std::list<std::string> result = splitString(serialMsg,DELIMETER);
        if (result.size()!=3) return;
        std::list<std::string>::iterator it = result.begin();
        std::string method = *(it++);
        std::string requestMapping = *(it++);
        std::string body = *(it++);


        if(method == TRANSMIT)
            onTransmitRequest(requestMapping,body);
        else if(method == RECIEVE)
            onRecieveRequest(requestMapping,body);

    }


    void update() override{
        // this->getMachineTime();
        // this->getMachineState();
        // this->getEngineOffPeriod();
        // this->getEngineOnPeriod();
        Serial.printf("%s/%s/FINISHED\n", TRANSMIT, REQUEST_METHODS::MACHINE_STATE);
    }

    ~SerialControllerImpl(){delete DELIMETER;}

private:
    const char *DELIMETER = "/";
    const char *TRANSMIT = "TX";
    const char *RECIEVE = "RX";
    

    

    void getAimedTemp()
    {
        double aimedTemp = DS18B20::getInstance().getAimedTemperature();
        Serial.printf("%s/%s/%2.2f\n",TRANSMIT,REQUEST_METHODS::AIMED_TEMP,aimedTemp);
    }

    void setAimedTemp(std::string body)
    {
        double newTemp = atof(body.c_str());

        DS18B20::getInstance().setAimedTemperature(newTemp);
        getAimedTemp();
    }

    void getEngineOnPeriod()
    {
        const char *engineOnPeriod = Engine::getInstance().getEngineOnPeriod().toString();
        Serial.printf("%s/%s/%s\n", TRANSMIT, REQUEST_METHODS::ENGINE_ON_PERIOD, engineOnPeriod);
    }

    void getEngineOnPeriodLeft()
    {
        const char *engineOnPeriod = Engine::getInstance().getEngineOnPeriodLeft().toString();
        Serial.printf("%s/%s/%s\n", TRANSMIT, REQUEST_METHODS::ENGINE_ON_PERIOD_LEFT, engineOnPeriod);
    }

    void setEngineOnPeriod(std::string body)
    {
        Utils::AlarmTime time = Utils::AlarmTime(body);
        Engine::getInstance().setEngineOnPeriod(time);
        getEngineOnPeriod();
    }

    void getEngineOffPeriod()
    {
        const char *engineOffPeriod = Engine::getInstance().getEngineOffPeriod().toString();
        Serial.printf("%s/%s/%s\n", TRANSMIT, REQUEST_METHODS::ENGINE_OFF_PERIOD, engineOffPeriod);
    }

    void getEngineOffPeriodLeft()
    {
        const char *engineOffPeriod = Engine::getInstance().getEngineOffPeriodLeft().toString();
        Serial.printf("%s/%s/%s\n", TRANSMIT, REQUEST_METHODS::ENGINE_OFF_PERIOD_LEFT, engineOffPeriod);
    }

    void setEngineOffPeriod(std::string body)
    {
        Utils::AlarmTime time = Utils::AlarmTime(body);
        Engine::getInstance().setEngineOffPeriod(time);
        getEngineOffPeriod();
    }

    void getEngineState()
    {
        const char * state =  Engine::getInstance().getState()==0 ? "OFF" : "ON";
        Serial.printf("%s/%s/%s\n", TRANSMIT, "ENGINE_STATE", state);
    }

    void setEngineState(std::string body)
    {
        if (body.compare("ON") == 0)
            Engine::getInstance().turnON();
        else if (body.compare("OFF") == 0)
            Engine::getInstance().turnOFF();

        getEngineState();
    }

    void getMachineState()
    {
        const char * state =  Machine::getInstance().isOn() ? "ON" : "OFF";
        Serial.printf("%s/%s/%s\n", TRANSMIT, REQUEST_METHODS::MACHINE_STATE, state);
    }

    void setMachineState(std::string body)
    {
        // Serial.printf("Got: %s\n",body.c_str());
        Serial.printf("%s/%s/%s\n", TRANSMIT, REQUEST_METHODS::MACHINE_STATE, body.c_str());
        if (body.compare("ON") == 0 )
            Machine::getInstance().turnOn();
        if (body.compare("OFF") == 0 )
            Machine::getInstance().turnOff();

        // getMachineState();
    }

    void getDS18B20Temp()
    {
        Serial.printf("%s/%s/%2.2f\n", TRANSMIT, REQUEST_METHODS::DS18B20_TEMP, 
        DS18B20::getInstance().getTemperature());
    }

    void getMachineTime(){
        const char *machineTime = Machine::getInstance().getWorkingTime().toString();
        Serial.printf("%s/%s/%s\n", TRANSMIT, REQUEST_METHODS::MACHINE_TIME,machineTime);
    }

    void setMachineTime(std::string body){
        Utils::AlarmTime time = Utils::AlarmTime(body);
        Machine::getInstance().setWorkingTime(time);
        getMachineTime();
    }

    int mapSerialCommand(std::string str)
    {

        if (str.compare(REQUEST_METHODS::AIMED_TEMP) == 0)
            return SerialCommand::AIMED_TEMP;

        if (str.compare(REQUEST_METHODS::ENGINE_ON_PERIOD) == 0)
            return SerialCommand::ENGINE_ON_PERIOD;

        if (str.compare(REQUEST_METHODS::ENGINE_OFF_PERIOD) == 0)
            return SerialCommand::ENGINE_OFF_PERIOD;

        if (str.compare(REQUEST_METHODS::MACHINE_TIME) == 0)
            return SerialCommand::MACHINE_TIME;

        if (str.compare(REQUEST_METHODS::MACHINE_STATE) == 0)
            return SerialCommand::MACHINE_STATE;

        if (str.compare(REQUEST_METHODS::DS18B20_TEMP) == 0)
            return SerialCommand::DS18B20_TEMP;
 
        if (str.compare(REQUEST_METHODS::ENGINE_ON_PERIOD_LEFT) == 0)
            return SerialCommand::ENGINE_ON_PERIOD_LEFT;

        if (str.compare(REQUEST_METHODS::ENGINE_OFF_PERIOD_LEFT) == 0)
            return SerialCommand::ENGINE_OFF_PERIOD_LEFT;


        return -1;
    }

    void onTransmitRequest(std::string requestMapping, std::string body){
        switch (mapSerialCommand(requestMapping))
        {

        case SerialCommand::AIMED_TEMP:
            setAimedTemp(body);
            break;
        case SerialCommand::ENGINE_ON_PERIOD:
            setEngineOnPeriod(body);
            break;
        case SerialCommand::ENGINE_OFF_PERIOD:
            setEngineOffPeriod(body);
            break;
        case SerialCommand::MACHINE_STATE:
            setMachineState(body);
            break;
        case SerialCommand::MACHINE_TIME:
            setMachineTime(body);
            break;
        }
    }

    void onRecieveRequest(std::string requestMapping, std::string body){
        switch (mapSerialCommand(requestMapping))
        {
        case SerialCommand::AIMED_TEMP:
            getAimedTemp();
            break;
        case SerialCommand::ENGINE_ON_PERIOD:
            getEngineOnPeriod();
            break;
        case SerialCommand::ENGINE_OFF_PERIOD:
            getEngineOffPeriod();
            break;
        case SerialCommand::MACHINE_STATE:
            getMachineState();
            break;
        case SerialCommand::MACHINE_TIME:
            getMachineTime();
            break;
        case SerialCommand::DS18B20_TEMP:
            getDS18B20Temp();
            break;
        case SerialCommand::ENGINE_ON_PERIOD_LEFT:
            getEngineOnPeriodLeft();
            break;
        case SerialCommand::ENGINE_OFF_PERIOD_LEFT:
            getEngineOffPeriodLeft();
            break;
        }

    }

    enum SerialCommand
    {
        AIMED_TEMP,
        ENGINE_ON_PERIOD,
        ENGINE_OFF_PERIOD,
        MACHINE_TIME,
        MACHINE_STATE,
        ENGINE_ON_PERIOD_LEFT,
        ENGINE_OFF_PERIOD_LEFT,
        DS18B20_TEMP //readOnly
    };

    std::list<std::string> splitString(std::string str,std::string delimiter){

        std::list<std::string> result;
        char *ptr; 
        ptr = strtok( (char *) str.c_str(),delimiter.c_str()); 
        while (ptr!=NULL){  
            result.push_back(ptr);
            ptr = strtok (NULL, delimiter.c_str());  
        }  

        return result;

    }


};