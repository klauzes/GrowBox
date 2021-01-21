#include "DateTime.h"

void setup() {
    Serial.begin(115200);
    Serial.println("preamble.......");
}

void loop() {
  /*  x->setHour(17);
    x->setMinute(48);
    x->setSecond(20);
    x->setDate(2);
    x->setMonth(11);
    x->setYear(2020);
    x->setDOW(2);
    Serial.println("DONE");
    delay(50000);*/

    Serial.println("===================");
    DateTime first = DateTime();
    first.now();
    Serial.print("Now:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addSeconds(5);
    Serial.print("+5 sec:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addSeconds(45);
    Serial.print("+45 sec:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addSeconds(87000);
    Serial.print("+87000 sec:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addMinutes(5);
    Serial.print("+5 min:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addMinutes(180);
    Serial.print("+180 min:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addMinutes(1500);
    Serial.print("+1500 min:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addHours(1);
    Serial.print("+1 hrs:"); Serial.print(first.cTime()); Serial.println(first.cDate());


    first.addHours(7);
    Serial.print("+7 hrs:"); Serial.print(first.cTime()); Serial.println(first.cDate());


    first.addHours(48);
    Serial.print("+48 hrs:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addDay(1);
    Serial.print("+1 dys:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addDay(5);
    Serial.print("+5 dys:"); Serial.print(first.cTime()); Serial.println(first.cDate());


    first.addDay(750);
    Serial.print("+750 dys:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    first.addDay(-60);
    Serial.print("-60 dys:"); Serial.print(first.cTime()); Serial.println(first.cDate());

    DateTime scnd = DateTime();
    scnd.now();

    DateTime diff = scnd - first;
    Serial.println("Diff:");
    Serial.print("years:"); Serial.println(diff.yyyy);
    Serial.print("months:"); Serial.println(diff.mm);
    Serial.print("date:"); Serial.println(diff.d);
    Serial.print("hour:"); Serial.println(diff.h);
    Serial.print("min:"); Serial.println(diff.m);
    Serial.print("sec:"); Serial.println(diff.s);
    

    Serial.println("===================");
  
    delay(2000);
}
