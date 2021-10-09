#line 1 "c:\\Users\\andrz\\Desktop\\Kettle\\main\\Controler.h"
#pragma once



class Renderer{
  public:
    virtual void render()=0;
  };

class EncoderControler{
    public:
    virtual void turnRight() =0;
    virtual void turnLeft()=0;
    virtual void pushBtn()=0;
};

class InterfaceControler: public virtual Renderer, public virtual EncoderControler {
 
 };
