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
#include "Pins.h"

class Log 
{
public: 

	Log(const char* info,bool m_isDailyState = true, bool logsHoursState= true);
	void setDaily(bool state);
	void setHour(bool state);
	void printLn(const char* info);
	static bool hasCard();

private:
	const char* m_logName = "Sera.csv";
	bool m_isDaily;
	bool m_logHour; 
	bool initSD();

};