#pragma once


#include "Engine.h"
#include "Machine.h"

void Engine::handleEngineCycles(){
    if( Machine::getInstance().workingTime.timerEnded() ) return;

    if(isOffTime.toSeconds()>0 ){
        if(!isOffTime--)
            turnON();
    } 
    else if(isOnTime.toSeconds()>0){
        if(!isOnTime--) 
            turnOFF();           
    }
    else{
        isOffTime=engineOffPeriod;
        isOnTime=engineOffPeriod;
    }
  
}