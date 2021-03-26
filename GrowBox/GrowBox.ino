#include "Pins.h"
#include "Joystick.h"
#include "UI.h"
#include "Hardware.h"
#include <Arduino.h>



UI* p_userInterface = nullptr;


void setup()
{
    Hardware::setDefaultPinModesAndValues();    
    Serial.begin(115200);  
    Joystick::constructor(DEFAULT_ENCODER_SENSITIVITY);
    while (!Serial) {};
    p_userInterface = new UI();
    Serial.println("Setup done...");     	
}

void loop()
{
    p_userInterface->processUserInterface();
}
