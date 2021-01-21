#include "Beeper.h"
#include "Pins.h"
#include <Arduino.h>
#include "PersistentData.h"

void Beeper::beep(int freq, int dur)
{
    analogWrite(BEEPER, freq);
    delay(dur);
    analogWrite(BEEPER, 0);
}

void Beeper::beepOk()
{
    int dur = 0;
    PersistentData::getVolume(dur);
    if (dur) {
        beep(50,dur);
        beep(200, dur);
    }
}

void Beeper::beepNOk()
{
    int dur = 0;
    PersistentData::getVolume(dur);
    if (dur) {
        beep(200, dur);
        beep(50, dur);
        delay(30);
        beep(20, dur);
        delay(30);
        beep(20, dur);
    }
}

void Beeper::beepNav()
{
    int dur = 0;
    PersistentData::getVolume(dur);
    if (dur) 
        beep(10, dur/2);
}
