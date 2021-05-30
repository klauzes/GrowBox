#include <Arduino.h>
#include "Joystick.h"
#include "Pins.h" 

/*
* Clasa statica folosita pentru a facilita interfatarea cu encodorul aflat la nivelul interfetei cu utilizatorul.
*/

// Date membre
static bool m_wasButtonPressed;
static int m_EncoderTicks;
static int m_LastState;
static int m_LastCountedTicks;
static int m_Ticks;
static int m_Min;
static int m_Max;

// Constructor cu parametru de sensitivitate.
void Joystick::constructor(int ticks)
{
    m_EncoderTicks = ticks;
    m_wasButtonPressed = false;
    reset(); 
    //Atasare interupt. Realizam aceasta pentru a monitoriza interactiunea cu joystickul chiar si atunci cand Arduino este ocupat cu executatea altui cod.
    attachInterrupt(0, scan, CHANGE);
    attachInterrupt(1, scan, CHANGE);
    attachInterrupt(4, setButtonPress, FALLING);
}



// Determina daca de la ultimul apel butonul encodorului a fost apasat.
bool Joystick::wasButtonPressed() 
{ 
    if (m_wasButtonPressed && digitalRead(JOY_BT))
    {       
        m_wasButtonPressed = false;
        return true;
    }        
    return false;    
}

// Metoda callback pentru interupt apasare buton
void Joystick::setButtonPress()
{
    if (!digitalRead(JOY_BT))
        if (!m_wasButtonPressed) 
            m_wasButtonPressed = true;    
}

// Metoda callback pentru interupt rotire encodor
void Joystick::scan()
{ 
    int aState, bState = 0;
    aState = digitalRead(JOY_LH);   
    bState = digitalRead(JOY_RH);   
    int oldTicks = m_Ticks;
    if (aState != m_LastState)
    {      
        if (bState != aState)
            m_Ticks--;
        else
            m_Ticks++;
    }
    m_LastState = aState;   
    int tmpTicks= m_Ticks / m_EncoderTicks;
    if (tmpTicks <= m_Max && tmpTicks >= m_Min )    
        m_LastCountedTicks = tmpTicks;  
    else
        m_Ticks = oldTicks;    
}

//getteri si setteri
void Joystick::setTicks(int ticks)
{
    m_EncoderTicks = ticks;
}

void Joystick::getTicks(int& val)
{
    val = m_Ticks;
}
void Joystick::getPosition(int& val)
{
    val = m_LastCountedTicks;
}

void Joystick::setMinMax(int min, int max)
{
    m_Min = min; m_Max = max;
}

// Aduce datele membre in starea initiala
void Joystick::reset()
{
    m_LastState = m_Max = m_Min = m_LastCountedTicks = m_Ticks = 0;
}
