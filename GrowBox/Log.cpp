#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "DateTime.h"
#include "Log.h"

/*
* Aceasta clasa este responsabile de scrierea de date pe cardul SD
* Ofera un constructor usor de folosit prin care se poate apela direct cu stringul ce se dore a fi logat pe card
*/

// Date membre utilizate pentru monitorizarea starii de conectare
static bool m_hasInit = false;
static bool m_hasTryInit = false;

// Constructor
Log::Log(const char* info, bool m_isDailyState , bool logsHoursState )
{
	m_isDaily = m_isDailyState;
	m_logHour = logsHoursState;
	if (!m_hasInit)
		m_hasInit = initSD();
	if(m_hasInit && m_hasTryInit)
		printLn(info);	
}

//Setteri
void Log::setDaily(bool state)
{
	m_isDaily = state;
}

void Log::setHour(bool state)
{
	m_logHour = state;
}


// Scrie pe card stringul primit prin info tinand cont de parametrii de formatare (este jurnal zilnic si fiecare linie are propria ora)
void Log::printLn(const char* info)
{
	if (m_hasInit)
	{
		char fName[30];
		char lineTime[30];
		if (m_isDaily)
		{
			DateTime curTime(true);
			char buf[9]; curTime.cTime(buf);
			strcpy(lineTime, buf);
			char fd[2]; curTime.formatedDate(fd);
			char fm[2]; curTime.formatedMonth(fm);
			sprintf(fName, "%s%s.csv", fd, fm);
		}
		else
		{
			strcpy(fName, m_logName);
		}		
		File logFile = SD.open(fName, FILE_WRITE);
		if (logFile)
		{
			logFile.seek(EOF);			
			if (m_logHour)
			{
				logFile.print(lineTime);
				logFile.print(",");
			}
			logFile.println(info);
			logFile.close();
		}			
	}	
}

// Daca obiectul a fost initializat ,prin deductie, putem considera ca avem un card SD introdus
bool Log::hasCard()
{
	return m_hasInit;
}

// Incercare de initializare
bool Log::initSD()
{
	m_hasTryInit = true;
	return SD.begin(SD_CS);
}
