#include "Beeper.h"
#include "Pins.h"
#include <Arduino.h>
#include "PersistentData.h"

/*
* Aceata este o clasa statica folosita pentru a genera tonori cu ajutorul difuzorului piezo
* Ofera si trei metode cu tonuri prestabilite, al caror nume sugereaza utilizarea adecvata
*/

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
