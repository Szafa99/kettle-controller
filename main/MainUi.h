#pragma once
#include "Controler.h"
#include "Utils.h"


#define EPSILON    (1.0E-8)
#define ARROW_SIZE 3,5
#define CELS_SIZE  5,5
#define minSize  11,3
#define UNDERLINE_YPOS FONT_Y_SIZE*4
#define LEFT_ARROW_POS 0 ,FONT_Y_SIZE*4-1
#define RIGHT_ARROW_POS WIDTH-3 , FONT_Y_SIZE*4-1


enum RenderOptions
{
  TEMP = 0,
  AIMED_TEMP,
  TOGGLE_MACHINE,
  MACHINE_ON_TIME,
  ENGINE_ON_CYCLE,
  ENGINE_OFF_CYCLE,
  MINUTES,
  SECONDS
};

class MainUI : public InterfaceControler
{
public:
  ///////////////// Singelton///////////////////
  static MainUI &getInstance(){
    static MainUI instance;
    return instance;
  }
  void operator=(const MainUI &) = delete;
  MainUI(MainUI const &) = delete;
  ///////////////////////////////////////////////////


  void pushBtn() override;
  void turnRight() override;
  void turnLeft() override;
  void render()override;
  
  void renderTemperature();
  void renderAimedTemperature();
  void renderMachineState();
  void renderMachineTime();
  void renderEngineCycleOn();
  void renderEngineCycleOff();
  void firstRender();


private:

  MainUI();
 
  int findCharPos(char * buff,char c); 

  char* optionsToRender[6]= {
        "ZMIEN",
        "fd",
        "g",
        "g",
        "g",
        "g"
    };

  const unsigned char rightArrow[5] PROGMEM = {
  B10000000,
  B11000000,
  B11100000,
  B11000000,
  B10000000
};


const unsigned char leftArrow[5] PROGMEM = {
  B00100000,
  B01100000,
  B11100000,
  B01100000,
  B00100000
};



const unsigned char minMap[6] PROGMEM = {
  B11111010,B11100000,
  B10101010,B10100000,
  B10101010,B10100000
};

  


const unsigned char celsMap[5] PROGMEM = {
  B00001000,
  B11100000,
  B10000000,
  B10000000,
  B11100000
};



  volatile bool renderEnable;  
  volatile bool editMode;
  Utils::AlarmTime * timerEditor;
  int curentLine = 0;
  char buff[14];
  RenderOptions currentOption;
  RenderOptions editOption;
};
