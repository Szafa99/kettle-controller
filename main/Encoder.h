#pragma once
#include "LCD.h"

namespace Encoder
{
#define LEFT 16//RX2
#define RIGHT 4 //D4
#define ENCODER_BTN 17 // TX2



#define LAST_TURN_WAIT 50
#define LAST_PUSHBTN_WAIT 500

    void IRAM_ATTR encoderTurnInterrupt();

    volatile int pulsesRight = 0, pulsesLeft = 0, rightPinState, leftPinState, lastRightPinState;
    volatile int lastPushEncoder = 0, lastTurn = 0;
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

    void IRAM_ATTR encoderTurnInterrupt()
    {
        portENTER_CRITICAL(&mux);

        rightPinState = digitalRead(RIGHT);
        leftPinState = digitalRead(LEFT);
        if (rightPinState != lastRightPinState)
        {
            if (leftPinState != rightPinState)
            {
                pulsesLeft++;
                lastRightPinState = rightPinState;
            }
            else
            {
                pulsesRight++;
                lastRightPinState = rightPinState;
            }
        }
        portEXIT_CRITICAL(&mux);
    }

    void IRAM_ATTR onEncoderBtn()
    {
        portENTER_CRITICAL(&mux);
        if (millis() - lastPushEncoder > LAST_PUSHBTN_WAIT && millis() - lastTurn > LAST_TURN_WAIT)
        {
            if (millis() - lastTurn < 200){
                LCD::getInstance().lcdControler->turnLeft();
            }
            LCD::getInstance().lcdControler->pushBtn();
            lastPushEncoder = millis();
            lastTurn = millis();
        }
        portEXIT_CRITICAL(&mux);
    }

    void onEncoderTurn()
    {

        if (pulsesRight - pulsesLeft > 5)
        {
            pulsesLeft = 0;
            pulsesRight = 0;
            if (millis() - lastTurn > LAST_TURN_WAIT)
            {
                lastTurn = millis();
                LCD::getInstance().lcdControler->turnRight();
            }
        }

       else if (pulsesLeft - pulsesRight > 3)
        {
            pulsesLeft = 0;
            pulsesRight = 0;
            if (millis() - lastTurn > LAST_TURN_WAIT)
            {
                lastTurn = millis();
                LCD::getInstance().lcdControler->turnLeft();
            }
        }
    }

    void setUp()
    {
          digitalWrite(ENCODER_BTN,HIGH);


        pinMode(LEFT, INPUT_PULLUP);
        pinMode(RIGHT, INPUT_PULLUP);
        pinMode(ENCODER_BTN, INPUT_PULLUP);
        lastRightPinState = digitalRead(RIGHT);

        attachInterrupt(LEFT, encoderTurnInterrupt, CHANGE);
        attachInterrupt(RIGHT, encoderTurnInterrupt, CHANGE);
        attachInterrupt(ENCODER_BTN, onEncoderBtn, FALLING);

        pulsesRight = 0;
        pulsesLeft = 0;
        lastTurn = millis();
    }

}
