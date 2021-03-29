#include "PersistentData.h"
#include <EEPROM.h>

void PersistentData::setVolume(int val)
{
	EEPROM.write(speakervalumeAddress, val);
}

void PersistentData::getVolume(int& val)
{
	val = EEPROM.read(speakervalumeAddress);
}

void PersistentData::setSunriseHour(int val)
{
	EEPROM.write(sunriseHourAddress, val);
}

void PersistentData::getSunriseHour(int& val)
{
	val = EEPROM.read(sunriseHourAddress);
}

void PersistentData::setSunlightHours(int val)
{
	EEPROM.write(sunlightHoursAddress, val);
}

void PersistentData::getSunlightHours(int& val)
{
	val = EEPROM.read(sunlightHoursAddress);
}

void PersistentData::setTemperature(int val)
{
	EEPROM.write(airTemperatureAddress, val);
}

void PersistentData::getTemperature(int& val)
{
	val = EEPROM.read(airTemperatureAddress);
}

void PersistentData::setMaximumAirHumidity(int val)
{
	EEPROM.write(maximumAirHumidityAddress, val);
}

void PersistentData::getMaximumAirHumidity(int& val)
{
	val = EEPROM.read(maximumAirHumidityAddress);
}

void PersistentData::setIdealSoilHumidity(int val)
{
	EEPROM.write(idealSoilHumidityAddress, val);
}

void PersistentData::getIdealSoilHumidity(int& val)
{
	val = EEPROM.read(idealSoilHumidityAddress);
}
