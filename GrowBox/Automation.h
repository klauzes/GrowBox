#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "DateTime.h"

class Automation
{
public:
	Automation();
	Automation(bool getPersistent);
	Automation(const Automation& cpy);
	~Automation();

	void setSunriseHour(int val);
	int  getSunriseHour() { return m_suriseHour;};

	void setSunlightHours(int val);
	int  getSunlightHours() { return m_sunlightHours; };
	
	void setTemperature(int val);
	int  getTemperature() { return m_Temperature; };

	void setMaximuAirHumidity(int val);
	int  getMaximuAirHumidity() { return m_maxAirHumidity; };

	void setIdealSoilHumidity(int val);
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
	DateTime nextHeaderDateTime;

	bool determineLightsState();
	bool determineAirHumidityState();
	bool determineAirTemperature();
	bool determineSoilHumidity();
};
