#include "Pins.h"
#include "Joystick.h"
#include "UI.h"
#include "Hardware.h"
#include <Arduino.h>
#include "Automation.h"
#include "Log.h"

#define REAL_MODE;

#ifdef REAL_MODE
UI* p_userInterface = nullptr;
Automation* currentConfig = nullptr;
#endif // REAL_MODE



void setup()
{
    Serial.begin(115200);
    while (!Serial) {};
    Hardware::setDefaultPinModesAndValues();

#ifdef REAL_MODE
    currentConfig = new Automation(true);  
    Joystick::constructor(DEFAULT_ENCODER_SENSITIVITY);  
    if (currentConfig->isValid())
    {
        p_userInterface = new UI(currentConfig);
    }
    else
    {
        p_userInterface = new UI(nullptr);
        delete currentConfig;
        currentConfig = nullptr;
    }
#endif  
    for (int i = 1 ; i < 255; i++)
    {
        Beeper::beep(i, 1);
    }
}

void loop()
{
#ifdef REAL_MODE
    p_userInterface->processUserInterface();
    Automation* curentAutomation = p_userInterface->getProgram();   
    if (!Hardware::getManualControl() && curentAutomation != nullptr && curentAutomation->isValid())
        curentAutomation->doRoutine();
    else if (!Hardware::getManualControl())
        Hardware::setDefaultPinModesAndValues();
#else
    tests();
#endif
}


void tests()
{
    Log("abc", true, true);
    delay(5000);
    Serial.println(Log::hasCard());
   /* Hardware::setDefaultPinModesAndValues();
    Hardware::setManualControl(true);
    Hardware::setLights(true);
    delay(5000);
    Hardware::setLights(false);
    delay(2000);*/
}