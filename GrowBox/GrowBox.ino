#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Pins.h"
#include "Joystick.h"
#include "UI.h"

#define DEFAULT_ENCODER_SENSITIVITY 2

static Joystick* p_static_joystickHook = nullptr;
UI* p_userInterface = nullptr;

void joystickButtonIRQ()
{
    if(!digitalRead(JOY_BT))
        p_static_joystickHook->setButtonPress();   
}


void joystickIRQ()
{
    p_static_joystickHook->scan();    
}

void setup()
{    
    pinMode(SD_CS, OUTPUT);
    Serial.begin(115200);  
    while (!Serial) {
        ; // wait for serial port to connect
    }    
    p_static_joystickHook = new Joystick(DEFAULT_ENCODER_SENSITIVITY);
    attachInterrupt(0, joystickIRQ, CHANGE);
    attachInterrupt(1, joystickButtonIRQ, LOW);
    p_userInterface = new UI(p_static_joystickHook);   
    Serial.println("Setup done...");
}

void loop()
{/*
    DateTime test(true);
    char ctime[9]; test.cTime(ctime);
    char cdate[12]; test.cDate(cdate);
    Serial.println(ctime);
    Serial.println(cdate);
    delay(500);*/
   p_userInterface->processUserInterface();
}
