#pragma once
#include "Arduino.h"
#include <string>
#include "Observable.h"

class SerialControllerInterface: public Observer{

public:
    virtual void listen() final{
        while (true)
        {
            if (Serial.available()>0)
            {   
                this->onSerialMessage( Serial.readString().c_str() );
            }
            
        }
    }

    virtual void observe(Observable *observable){
        observable->addObserver(this);
    }


    virtual void onSerialMessage(std::string serialMsg)=0;
    
};