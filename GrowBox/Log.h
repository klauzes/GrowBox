#pragma once
class Log 
{
public: 

	Log(const char* info,bool isDailyState = true, bool logsHoursState= true);
	void setDaily(bool state);
	void setHour(bool state);
	void printLn(const char* info);

private:
	const char* logName = "Sera.log";
	bool isDaily;
	bool logHour; 
	bool initSD();

};