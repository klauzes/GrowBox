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
#pragma once
class Joystick
{
public:
	static void constructor(int ticks);
	static void setTicks(int ticks);
	static void getTicks(int& val);
	static bool wasButtonPressed();
	static void setButtonPress();
	static void scan();
	static void getPosition(int& val);
	static void setMinMax(int min, int max);
	static void reset();

};

