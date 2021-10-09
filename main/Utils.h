#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>
namespace Utils{

  
struct AlarmTime{

  AlarmTime():
    minutes(0),
    second(0){}

  AlarmTime(int minutes,int seconds):
                    minutes(minutes),
                    second(seconds){}

  AlarmTime(std::string time){
      std::size_t pos = time.find(":");
      if( pos==std::string::npos || time.substr(pos,time.length()).find(":")>0 ){
        minutes=0;
        second=0;
      }else{
          minutes = atoi( time.substr(0,pos).c_str() );
          second = atoi( time.substr( pos+1,time.length() ).c_str() );
          if(minutes>99 || minutes<0)  minutes=0;
          if(second>59 || second<0)  second=0;
      }
  }


  bool operator== ( AlarmTime& o1){
        return  toSeconds() == o1.toSeconds() ;
  }


  bool operator> ( AlarmTime& o1){
        return  toSeconds() > o1.toSeconds() ;
  }


bool operator>= ( AlarmTime& o1){
        return  toSeconds() >= o1.toSeconds() ;
  }

 bool operator< ( AlarmTime& o1){
        return  toSeconds() < o1.toSeconds() ;
  }

 bool operator<= ( AlarmTime& o1){
        return  toSeconds() <= o1.toSeconds() ;
  }

  bool timerEnded(){
    return !(minutes || second);
  }                  

int toSeconds(){
  return minutes*60+second;
}

//postfix
bool operator--(){
      second = second==0 ? 59 : this->second-1;
      if(second==59 )
          if(minutes==0){
            minutes=99;
            }
           else
            minutes-=1; 
      return second;
}

//prefix
bool operator--(int){
   return --*this;
}  

bool operator++(){
       
      second = second==59 ? 0 : this->second+1;
      if(second==0 )
          if(minutes==99){
            minutes=0;
            return false;
            }
           else
            minutes+=1; 
      return true;
}

//prefix
bool operator++(int){
   return ++*this;
}

   
   bool decreaseSeconds(){
         return --*this;
    }

   bool increaseSeconds(){
         return ++*this;
    } 

   bool decreaseMinutes(){
       if(minutes==0){
          minutes=99;
          second=59;
          return false;
          }
          else
            minutes--;    
       return true;     
    }



     bool increaseMinutes(){
          if(minutes==99){
            minutes=0;
            second=0;
            return false;
            }
          else
            minutes+=1; 
      return true;
    }


    char * toString(){
      char *msg_buf = new char;
        sprintf(msg_buf, "%02d:%02d",minutes,second);      
     return msg_buf;  
   }  
                       
 
  int minutes;
  int second;
  };
    

     enum LCDPage{TEMP_PAGE,HEATER_PAGE,ENGINE_PAGE};
     enum MILLIS_CONVERT{SECOND=1000000};
     enum HeaterInterface{CHANGE_TIME,START_TIMER,STOP_TIMER,MINUTES,SECONDS,NEXT_PAGE};
     enum EngineInterface{CHANGE_ON_TIME,CHANGE_OFF_TIME,PERIOD_MINUTES,PERIOD_SECONDS,NEXT};


}
#endif
