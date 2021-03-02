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
    p_static_joystickHook = new Joystick(DEFAULT_ENCODER_SENSITIVITY);
    delay(500);
    while (!Serial) {
        ; // wait for serial port to connect
    }    
    attachInterrupt(0, joystickIRQ, CHANGE);
    attachInterrupt(1, joystickButtonIRQ, LOW);
    p_userInterface = new UI(p_static_joystickHook);   
    Serial.println("Setup done..."); 
   
}

void loop()
{
    p_userInterface->processUserInterface();    
}
