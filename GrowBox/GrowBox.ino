#include "Pins.h"
#include "Joystick.h"
#include "UI.h"
#include "Hardware.h"
#include <Arduino.h>
#include "Automation.h"


UI* p_userInterface = nullptr;

void setup()
{
    Automation* savedConfigIfAny = new Automation();
    savedConfigIfAny->loadPersistent();
   
    Hardware::setDefaultPinModesAndValues();    
    Serial.begin(115200);  
    Joystick::constructor(DEFAULT_ENCODER_SENSITIVITY);
    while (!Serial) {};
    if (savedConfigIfAny->isValid()) 
    {
        Serial.println("isvalidconfing");
        p_userInterface = new UI(savedConfigIfAny);
    }
    else
    {
        p_userInterface = new UI(nullptr);
        delete savedConfigIfAny;
        savedConfigIfAny = nullptr;
    }
    Serial.println("Setup done...");     	
}

void loop()
{
    p_userInterface->processUserInterface();
}
