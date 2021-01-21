#include "Joystick.h"
#include <Arduino.h>
#include "Pins.h" 

static bool m_wasButtonPressed;

Joystick::Joystick()
{
    m_EncoderTicks = 4;   
    m_wasButtonPressed = false; 
    pinMode(JOY_BT, INPUT_PULLUP);
    pinMode(JOY_LH, INPUT_PULLUP);
    pinMode(JOY_RH, INPUT_PULLUP);
    reset();
}

Joystick::Joystick(int ticks) : Joystick()
{
    m_EncoderTicks = ticks;
}

void Joystick::setTicks(int ticks)
{
    m_EncoderTicks = ticks;
}

bool Joystick::wasButtonPressed() 
{ 
    if (m_wasButtonPressed && digitalRead(JOY_BT))//button was pressed and now is released
    {       
        m_wasButtonPressed = false;
        return true;
    }        
    return false;    
}

void Joystick::setButtonPress()
{
    if (!m_wasButtonPressed) 
        m_wasButtonPressed = true;    
}

void Joystick::scan()
{
    int aState, bState = 0;
    aState = digitalRead(JOY_LH);
    delay(1);
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

void Joystick::reset()
{
    m_LastState = m_Max = m_Min = m_LastCountedTicks = m_Ticks = 0;
}
