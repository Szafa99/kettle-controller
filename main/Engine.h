#pragma once


#define ENGINE_PIN 4


class Engine
{
private:
unsigned int state;
 //variabeles to set by user
 Utils::AlarmTime engineOnPeriod;
 Utils::AlarmTime engineOffPeriod;
// Variabels holding actual time
 Utils::AlarmTime isOnTime;
 Utils::AlarmTime isOffTime;


  Engine(){ 
    state=LOW;
    pinMode(ENGINE_PIN,OUTPUT);
    digitalWrite(ENGINE_PIN,state);
    engineOnPeriod = Utils::AlarmTime(0,0);
    engineOffPeriod = Utils::AlarmTime(0,0);
    isOnTime = Utils::AlarmTime(0,0);
    isOffTime = Utils::AlarmTime(0,0);
 }

 public:

   Utils::AlarmTime& getEngineOnPeriod(){return engineOnPeriod;}
   void setEngineOnPeriod(Utils::AlarmTime period){
        engineOnPeriod = period;
     }
   Utils::AlarmTime& getEngineOffPeriod(){return engineOffPeriod;}
   void setEngineOffPeriod(Utils::AlarmTime period){
        engineOffPeriod = period;
     }

  unsigned int getState(){return state;}
  void setState(unsigned int state){this->state = state;}


  static Engine& getInstance(){
          static Engine instance;
          return instance;
  }

  void operator=(const Engine&) = delete;
  Engine( Engine const&) = delete;

  void turnON(){
    state=HIGH;
    digitalWrite(ENGINE_PIN,HIGH);
    }

  void turnOFF(){
    state=LOW;
    digitalWrite(ENGINE_PIN,LOW);
    }
  
  void handleEngineCycles();


};


