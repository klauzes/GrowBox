#include "Automation.h"
#include "PersistentData.h"
#include "DateTime.h"
#include "Hardware.h"

Automation::Automation(bool getPersistent) :
m_suriseHour{ 0 },
m_sunlightHours{ 0 },
m_Temperature{ 0 },
m_maxAirHumidity{ 0 },
m_idealSoilHumidity{ 0 }
{
	if (getPersistent)
		loadPersistent();
}

Automation::Automation(const Automation& cpy)
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
	//if (((millis() / 1000) % 5) == 0)
	//	Serial.println(millis());// this repeats every odd 5 seconds
	//no automatin no bussiness
	
	if (!isValid())
		return;
	//lights according to dermination
	Hardware::setLights(determineLightsState());

	bool automationAirIntakeFanState = false;
	bool automationHeaterState = false;

	////fan on by humidity or by heater
	//if (determineAirHumidityState())
	//	automationAirIntakeFanState = true;
	//else
	//	automationAirIntakeFanState = determineAirTemperature();
	//
	////heater or off and fan also off if not needed by humidity control
	//if (determineAirTemperature())
	//	automationHeaterState = true;	
	//else 
	//{
	//	automationHeaterState = false();
	//	automationAirIntakeFanState = determineAirTemperature();
	//}

	automationHeaterState = determineAirTemperature();
	automationAirIntakeFanState = determineAirHumidityState();

	Serial.print("here  "); Serial.println(millis());
	Serial.print("setHeater  "); Serial.println(automationHeaterState);


	Hardware::setHeater(automationHeaterState);
	Hardware::setIntakeFan(automationAirIntakeFanState);
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
	if (currentAirHumidity > maxAirHumidity && !determineAirTemperature())
		return true;
	return false;
}

bool Automation::determineAirTemperature()
{
	int currentTemperature = Hardware::getTemperature();
	int cmaxIdealTemperature; PersistentData::getTemperature(cmaxIdealTemperature);	
	if (currentTemperature < cmaxIdealTemperature) // 10% over for not to debounde so much and inertia --to be considered 
			return true;
	return false;
}


