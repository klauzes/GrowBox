#include "Pins.h"
#include "Joystick.h"
#include "UI.h"
#include "Hardware.h"
#include <Arduino.h>
#define DEFAULT_ENCODER_SENSITIVITY 2

static Joystick* p_static_joystickHook = nullptr;
UI* p_userInterface = nullptr;

void joystickButtonIRQ()
{
    if (!digitalRead(JOY_BT))
        p_static_joystickHook->setButtonPress();
}


void joystickIRQ()
{
    p_static_joystickHook->scan();    
}

void setup()
{
    Hardware::setDefaultPinModesAndValues();    
    Serial.begin(115200);
    p_static_joystickHook = new Joystick(DEFAULT_ENCODER_SENSITIVITY); 
    while (!Serial) {
        ; // wait for serial port to connect
    }    
    attachInterrupt(0, joystickIRQ, CHANGE);
    attachInterrupt(1, joystickButtonIRQ, FALLING);
    delay(500);
    Serial.println("Setup done...");
    p_userInterface = new UI(p_static_joystickHook);   	
}

void loop()
{
    p_userInterface->processUserInterface();
}
