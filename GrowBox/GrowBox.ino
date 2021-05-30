/****************************************************************************
*  PROIECT LICENTA:     -SISTEM ELECTRONIC PENTRU AUTOMATIZAREA UNEI SERE-  *
*  LIMBAJ:              C++                                                 *
*  MICROPROCESOR        ATmega2560                                          *
*  STUDENT:             ISPAS N.O. CLAUDIU-OCTAVIAN                         *
*  GRUPA:               307                                                 *
*  PROMOTIA             2018-2021                                           *
*  FACULTATEA DE INFORMATICA                                                *
*  UNIVERSITATEA TITU MAIORESCU                                             *
* ***************************************************************************/


#include <Arduino.h>
#include <EEPROM.h>
#include <stdlib.h>

#include "Automation.h"
#include "Hardware.h"
#include "Joystick.h"
#include "Pins.h"
#include "UI.h"

//  Acest macro defineste modul de functionare.
//  Cand este definit programul ruleaza in mod obisnuit, altfel se executa doar sectiune de teste.
//  Este util exclusiv in faza de development.
#define REAL_MODE;

#ifdef REAL_MODE
// Instante gloabale ale claselor responsabile cu interfata utilizatorului si automatizarea in sine a serei.
UI* p_userInterface = nullptr;
Automation* currentConfig = nullptr;
#endif // REAL_MODE

// In platforma de dezvoltare ARDUINO aceasta metoda este apelata o singura data la boot.
void setup()
{
    // Activare serial. Folosit mai mult in debug.
    Serial.begin(115200);
    while (!Serial) {};
    // Stabilire stare initiala a pinilor.
    Hardware::setDefaultPinModesAndValues();

#ifdef REAL_MODE
    // Instantiere Joystick si Automare.
    Joystick::constructor(DEFAULT_ENCODER_SENSITIVITY);
    currentConfig = new Automation(true);      
    // Daca avem o configurare de automatizare salvata o incarcam.
    if (currentConfig->isValid())    
        p_userInterface = new UI(currentConfig);    
    else    
        p_userInterface = new UI(nullptr);  
    
    // Stergem obiectul, clasa UI isi face o copie.
    delete currentConfig;
    currentConfig = nullptr;
#endif  

    // Avertizare sonora terminare faza boot.
    for (int i = 1 ; i < 255; i++)
    {
        Beeper::beep(i, 1);
    }
}

// In platforma ARDUINO metoda loop() este chemata in bucla infinita.
void loop()
{
#ifdef REAL_MODE
    // Procesare intergata grafica.
    p_userInterface->processUserInterface();
    // Atat timp cat exista o configuratie valida de functionare automata, se apeleaza doRoutine din aceasta.
    Automation* curentAutomation = p_userInterface->getProgram();   
    if (!Hardware::getManualControl() && curentAutomation != nullptr && curentAutomation->isValid())
        curentAutomation->doRoutine();
    else if (!Hardware::getManualControl())
        Hardware::setDefaultPinModesAndValues();
#else
    tests();
#endif
}

// Avesta metoda a fost folosita pentru testare.
void tests()
{
    double soil = analogRead(SOIL_HUMIDITY);
    Serial.print("Soil: "); Serial.println(soil);
    delay(500);  
}