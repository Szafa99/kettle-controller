#pragma once

#include "Arduino.h"
#include <list>
#include "Observer.h"

class Observable{

    private:
    std::list<Observer*> observers;


    public:
    ~Observable(){
        for(auto &observer : this->observers)
            delete observer;
    }

    void notify(){
        for(auto &observer : this->observers)
            observer->update();
    };

    void addObserver(Observer *observer){
        if(observer!=NULL)
            observers.push_back(observer);
    };
    void removeObserver(Observer *observer) {
        observers.remove(observer);
    };

    std::list<Observer*> getObservers(){return this->observers;}

};