#include "Automation.h"
#include "PersistentData.h"
#include "Hardware.h"
#include "Log.h"

Automation::Automation():
m_suriseHour {	0},
m_sunlightHours{ 0 },
m_Temperature{ 0 },
m_maxAirHumidity{ 0 },
m_idealSoilHumidity{ 0 },
m_lastSoilHumidityCheck { 0 },
m_nextLogWrite { 0 },
m_waterPumpOffTime { 0 }
{
	nextHeaderDateTime = new DateTime(true);
	nextHeaderDateTime.setHour(0);
	nextHeaderDateTime.setMinute(0);
	nextHeaderDateTime.setSecond(0);
}

Automation::Automation(bool getPersistent) :Automation()
{
	if (getPersistent)
		loadPersistent();	
}

Automation::Automation(const Automation& cpy):Automation()
{
	m_suriseHour = cpy.m_suriseHour;
	m_sunlightHours = cpy.m_sunlightHours;
	m_Temperature = cpy.m_Temperature;
	m_maxAirHumidity = cpy.m_maxAirHumidity;
	m_idealSoilHumidity = cpy.m_idealSoilHumidity;
}

Automation::~Automation()
{
}

void Automation::setSunriseHour(int val)
{
	m_suriseHour = val;
}

void Automation::setSunlightHours(int val)
{
	m_sunlightHours = val;
}

void Automation::setTemperature(int val)
{
	m_Temperature = val;
}

void Automation::setMaximuAirHumidity(int val)
{
	m_maxAirHumidity = val;
}

void Automation::setIdealSoilHumidity(int val)
{
	m_idealSoilHumidity = val;
}

void Automation::loadPersistent()
{
	PersistentData::getSunriseHour(m_suriseHour);
	PersistentData::getSunlightHours(m_sunlightHours);
	PersistentData::getTemperature(m_Temperature);
	PersistentData::getMaximumAirHumidity(m_maxAirHumidity);
	PersistentData::getIdealSoilHumidity(m_idealSoilHumidity);
}

void Automation::deletePersistent()
{
	PersistentData::setSunriseHour(0);
	PersistentData::setSunlightHours(0);
	PersistentData::setTemperature(0);
	PersistentData::setMaximumAirHumidity(0);
	PersistentData::setIdealSoilHumidity(0);
	
}

void Automation::saveToPersistent()
{
	PersistentData::setSunriseHour(m_suriseHour);
	PersistentData::setSunlightHours(m_sunlightHours);
	PersistentData::setTemperature(m_Temperature);
	PersistentData::setMaximumAirHumidity(m_maxAirHumidity);
	PersistentData::setIdealSoilHumidity(m_idealSoilHumidity);
}

bool Automation::isValid()
{	
	if (m_suriseHour >= 0 && m_suriseHour <= 23)
	{
		if (m_sunlightHours >= 1 && m_sunlightHours <= MAX_SUNLIGHT_HOURS)
		{
			if (m_idealSoilHumidity >= MIN_SOIL_HUMIDITY && m_idealSoilHumidity <= MAX_SOIL_HUMIDITY)
			{
				if (m_maxAirHumidity <= MAX_AIR_HUMIDITY)
				{
					if (m_Temperature >= MIN_AIR_TEMP && m_Temperature <= MAX_AIR_TEMP)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Automation::doRoutine()
{	
	if (!isValid())
		return;

	writeLog();

	bool automationLightsState = false;
	bool automationAirIntakeFanState = false;
	bool automationHeaterState = false;
	bool automationWaterPump = false;

	automationLightsState = determineLightsState();
	automationHeaterState = determineAirTemperature();
	automationAirIntakeFanState = determineAirHumidityState();
	automationWaterPump = determineSoilHumidity();

	Hardware::setLights(automationLightsState);
	Hardware::setHeater(automationHeaterState);
	Hardware::setIntakeFan(automationAirIntakeFanState);
	Hardware::setWaterPump(automationWaterPump);

	if (automationWaterPump && m_waterPumpOffTime == 0)
	{	
		Hardware::setWaterPump(true);
		m_waterPumpOffTime = millis() + DEFAULT_WATER_PUMP_ON_TIME;
	}

	if (millis() > m_waterPumpOffTime)
	{
		Hardware::setWaterPump(false);
		m_waterPumpOffTime = 0;
	}
}

void Automation::writeLog()
{
	if (!Log::hasCard())
		Log("TryStart", true, true);
	if (millis() > m_nextLogWrite && Log::hasCard())
	{
		m_nextLogWrite += DEFAULT_LOG_WRITE_INTERVAL;
		DateTime today(true);
		today.setHour(0); today.setMinute(0); today.setSecond(0);
	
		if (nextHeaderDateTime == today)
		{
			nextHeaderDateTime.addDay(1);		
			Log("Requested Air Temperature, Max Air Humidity, Requested Soil Humidity, Actual Air Temperature, Actual Air Humidity, Actual Soil Humidity, Actual Particle Count, Lights State, Fan State, Heater State", true, true);
		}

		int reqAirTemp = 0; PersistentData::getTemperature(reqAirTemp);
		int reqAirHumidiy = 0; PersistentData::getMaximumAirHumidity(reqAirHumidiy);
		int reqSoilHumidity = 0; PersistentData::getIdealSoilHumidity(reqSoilHumidity);

		double actualAirTemp = Hardware::getTemperature();
		double actualAirHumidity = Hardware::getHumidity();
		double actualSoilHumidity = Hardware::getSoilHumidity();
		double actualParticleCount = Hardware::getParticleCount();

		bool actualLightState = Hardware::getLightsState();
		bool actualFanState = Hardware::getIntakeFanState();
		bool actualHeaterState = Hardware::getHeaterState();


		char strReqAirTemp[7];  dtostrf(reqAirTemp, 2, 2, strReqAirTemp);
		char strReqAirHumidiy[7];  dtostrf(reqAirHumidiy, 2, 2, strReqAirHumidiy);
		char strReqSoilHumidity[7];  dtostrf(reqSoilHumidity, 2, 2, strReqSoilHumidity);

		char strActualAirTemp[7];  dtostrf(actualAirTemp, 2, 2, strActualAirTemp);
		char strActualAirHumidity[7];  dtostrf(actualAirHumidity, 2, 2, strActualAirHumidity);
		char strActualSoilHumidity[7];  dtostrf(actualSoilHumidity, 2, 2, strActualSoilHumidity);
		char strActualParticleCount[7];  dtostrf(actualParticleCount, 2, 2, strActualParticleCount);
		
		char buffer[100];
		sprintf(buffer, "%s,%s,%s,%s,%s,%s,%s,%d,%d,%d", strReqAirTemp, strReqAirHumidiy, strReqSoilHumidity, strActualAirTemp,
			strActualAirHumidity, strActualSoilHumidity, strActualParticleCount, actualLightState, actualFanState, actualHeaterState);
		Log(buffer, true, true);		
	}
}

bool Automation::determineLightsState()
{
	DateTime now(true);
	now.setMinute(0); now.setSecond(0);
	DateTime sunriseHour(now);
	DateTime sunsetHour(now);

	int persistentSunriseHour, persistentSunDuration;
	PersistentData::getSunriseHour(persistentSunriseHour);
	PersistentData::getSunlightHours(persistentSunDuration);

	sunriseHour.setHour(persistentSunriseHour);
	sunsetHour.setHour(persistentSunriseHour);
	sunsetHour.addHours(persistentSunDuration);
	
	if (DateTime(true) >= sunriseHour && DateTime(true) <= sunsetHour)
		return true;
	return false;
}

bool Automation::determineAirHumidityState()
{
	int currentAirHumidity = Hardware::getHumidity();
	int maxAirHumidity; PersistentData::getMaximumAirHumidity(maxAirHumidity);
	int cmaxIdealTemperature; PersistentData::getTemperature(cmaxIdealTemperature);
	if (
		currentAirHumidity > maxAirHumidity && !determineAirTemperature() ||
		(Hardware::getTemperature() > cmaxIdealTemperature && Hardware::getLightsState()) 
		)
		return true;
	return false;
}

bool Automation::determineAirTemperature()
{
	int currentTemperature = Hardware::getTemperature();
	int cmaxIdealTemperature; PersistentData::getTemperature(cmaxIdealTemperature);	
	if (currentTemperature < cmaxIdealTemperature)
			return true;
	return false;
}

bool Automation::determineSoilHumidity()
{
	int idealSoil; PersistentData::getIdealSoilHumidity(idealSoil);
	if (Hardware::getSoilHumidity() < idealSoil && m_lastSoilHumidityCheck < millis() )
	{		
		m_lastSoilHumidityCheck = millis() + DEFAULT_SOIL_HUMIDITY_CHECK_INTERVAL;
		return true;
	}
	return false;
}


