#include "Pins.h"
#include "Joystick.h"
#include "UI.h"
#include "Hardware.h"
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
    Hardware::setDefaultPinModesAndValues();    
    Serial.begin(115200);  
    p_static_joystickHook = new Joystick(DEFAULT_ENCODER_SENSITIVITY); 
    while (!Serial) {
        ; // wait for serial port to connect
    }    
    attachInterrupt(0, joystickIRQ, CHANGE);
    attachInterrupt(1, joystickButtonIRQ, LOW);
    delay(500);
    Serial.println("Setup done...");
    p_userInterface = new UI(p_static_joystickHook);   
 
}

void loop()
{
    /*  digitalWrite(PARTICLE_LED, LOW);
      delayMicroseconds(280);

      float voMeasured = analogRead(PARTICLE);

      delayMicroseconds(40);
      digitalWrite(PARTICLE_LED, HIGH);
      delayMicroseconds(9680);

      float calcVoltage = voMeasured * (5.0 / 1024);
      float dustDensity = 0.17 * calcVoltage - 0.1;

      if (dustDensity < 0)
      {
          dustDensity = 0.00;
      }

      Serial.println("Raw Signal Value (0-1023):");
      Serial.println(voMeasured);

      Serial.println("Voltage:");
      Serial.println(calcVoltage);

      Serial.println("Dust Density:");
      Serial.println(dustDensity);
      delay(1000);*/
    p_userInterface->processUserInterface();
}
