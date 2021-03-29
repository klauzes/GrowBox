#include "log.h"
#include <SPI.h>
#include "DateTime.h"
#include <SD.h>
static bool hasInit = false;

Log::Log(const char* info, bool isDailyState , bool logsHoursState )
{
	isDaily = isDailyState;
	logHour = logsHoursState;
	if (!hasInit)		
		hasInit = initSD();
	printLn(info);	
}


void Log::setDaily(bool state)
{
	isDaily = state;
}

void Log::setHour(bool state)
{
	logHour = state;
}

void Log::printLn(const char* info)
{
	if (hasInit)
	{
		char fName[30];
		char lineTime[30];
		if (isDaily)
		{
			DateTime curTime(true);
			char buf[9]; curTime.cTime(buf);
			strcpy(lineTime, buf);
			char fd[2]; curTime.formatedDate(fd);
			char fm[2]; curTime.formatedMonth(fm);
			sprintf(fName, "%s%s.log", fd, fm);
		}
		else
		{
			strcpy(fName, logName);
		}		
		File logFile = SD.open(fName, FILE_WRITE);
		if (logFile)
		{
			logFile.seek(EOF);
			if (logHour) {
				logFile.print(lineTime);
				logFile.print(" -> ");
			}
			logFile.println(info);
			logFile.close();
		}			
	}	
}

bool Log::initSD()
{
	return SD.begin(SD_CS);
}
