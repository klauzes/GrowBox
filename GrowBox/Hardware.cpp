#include "Hardware.h"
#include <Arduino.h>
#include <dht.h>

dht DHT;
static bool m_WaterPumpState;
static bool m_IntakeFanState;
static bool m_HeaterState;
static bool m_LightsState;
static bool m_ParticleLED;

float Hardware::getTemperature()
{	
	DHT.read21(DHT_22);	
	return DHT.temperature;
}

float Hardware::getHumidity()
{
	DHT.read21(DHT_22);
	return DHT.humidity;
}

float Hardware::getWaterLevel()
{
	pinMode(WATER_LVL, INPUT);
	int adcValue = analogRead(WATER_LVL);
	return (float)adcValue;
}

float Hardware::getSoilHumidity()
{
	pinMode(SOIL_HUMIDITY, INPUT);
	int adcValue = analogRead(SOIL_HUMIDITY);
	return (float)adcValue;
}

float Hardware::getParticleCount()
{
	pinMode(PARTICLE, INPUT);
	setParticleSensorLed(false);
	delayMicroseconds(280);
	float readVal = analogRead(PARTICLE);
	delayMicroseconds(40);
	setParticleSensorLed(true);
	float calcVoltage = readVal * (5.0 / 1024);
	float dustDensity = 0.17f * calcVoltage - 0.1;
	if (dustDensity < 0)
		dustDensity = 0;	
	return dustDensity;
}

void Hardware::setWaterPump(bool state)
{
	pinMode(RELAY_LIGHT, OUTPUT);
	digitalWrite(WATER_PUMP, state);
	m_WaterPumpState = state;
}

void Hardware::setIntakeFan(bool state)
{
	if (!state && m_HeaterState)
		return;//can't turn off fan if heater is on
	pinMode(RELAY_FAN, OUTPUT);
	digitalWrite(RELAY_FAN, !state);
	m_IntakeFanState = state;
}

void Hardware::setHeater(bool state)
{
	pinMode(RELAY_HEATER, OUTPUT);
	digitalWrite(RELAY_HEATER, !state);
	m_HeaterState = state;
	if (state) //heater always turns fan On
		setIntakeFan(true);
}

void Hardware::setLights(bool state)
{
	pinMode(RELAY_LIGHT, OUTPUT);
	digitalWrite(RELAY_LIGHT, !state); //active low
	m_LightsState = state;
}

bool Hardware::getWaterPumpState()
{
	return m_WaterPumpState;
}

bool Hardware::getIntakeFanState()
{
	return m_IntakeFanState;
}

bool Hardware::getHeaterState()
{
	return m_HeaterState;
}

bool Hardware::getLightsState()
{
	return m_LightsState;
}

void Hardware::setParticleSensorLed(bool state)
{
	pinMode(RELAY_LIGHT, OUTPUT);
	digitalWrite(PARTICLE_LED, state);
	m_ParticleLED = state;
}
