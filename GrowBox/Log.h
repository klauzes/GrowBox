#pragma once
#include "Pins.h"

class Log 
{
public: 

	Log(const char* info,bool isDailyState = true, bool logsHoursState= true);
	void setDaily(bool state);
	void setHour(bool state);
	void printLn(const char* info);
	static bool hasCard();

private:
	const char* logName = "Sera.csv";
	bool isDaily;
	bool logHour; 
	bool initSD();

};