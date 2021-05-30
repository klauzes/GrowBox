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
#include "DateTime.h"
#include "Pins.h"


class Automation
{
public:
	Automation();
	Automation(bool getPersistent);
	Automation(const Automation* cpy);
	~Automation();

	void setSunriseHour(const int val);
	int  getSunriseHour() { return m_suriseHour;};

	void setSunlightHours(const int val);
	int  getSunlightHours() { return m_sunlightHours; };
	
	void setTemperature(const int val);
	int  getTemperature() { return m_Temperature; };

	void setMaximuAirHumidity(const int val);
	int  getMaximuAirHumidity() { return m_maxAirHumidity; };

	void setIdealSoilHumidity(const int val);
	int  getIdealSoilHumidity() { return m_idealSoilHumidity; };

	void loadPersistent();
	void deletePersistent();
	void saveToPersistent();

	bool isValid();

	void doRoutine();

private:
	int m_suriseHour;
	int m_sunlightHours;
	int m_Temperature;
	int m_maxAirHumidity;
	int m_idealSoilHumidity;
	unsigned long m_lastSoilHumidityCheck;
	unsigned long m_nextLogWrite;
	unsigned long m_waterPumpOffTime;
	void writeLog();
	DateTime* nextHeaderDateTime;

	bool determineLightsState();
	bool determineAirHumidityState();
	bool determineAirTemperature();
	bool determineSoilHumidity();
};
