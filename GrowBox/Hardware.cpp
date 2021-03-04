#include "Hardware.h"
#include <Arduino.h>
#include <dht.h>

dht DHT;
static bool m_WaterPumpState = 0;
static bool m_IntakeFanState = 0;
static bool m_HeaterState = 0;
static bool m_LightsState = 0;

double Hardware::getTemperature()
{	
	DHT.read21(DHT_22);	
	return (double)DHT.temperature;
}

double Hardware::getHumidity()
{
	DHT.read21(DHT_22);
	return (double)DHT.humidity;
}

double Hardware::getWaterLevel()
{
	int adcValue = analogRead(WATER_LVL);
	return (double)adcValue;
}

double Hardware::getSoilHumidity()
{
	int adcValue = 1023 - analogRead(SOIL_HUMIDITY);
	return (double)adcValue;
}

double Hardware::getParticleCount()
{	//basically I stop the PWM and simulate it during reading to ensure PARTICLE_LED is ON(active low) for reading, hence the calculated pauses
	analogWrite(PARTICLE_LED, 0);//on	
	delayMicroseconds(280);
	double readVal = analogRead(PARTICLE);
	delayMicroseconds(40);
	analogWrite(PARTICLE_LED, 255);//off
	delayMicroseconds(9680); 
	double calcVoltage = readVal * (5.0 / 1024);
	double dustDensity = 0.17 * calcVoltage - 0.1;
	if (dustDensity < 0)
		dustDensity = 0;	
	analogWrite(PARTICLE_LED, 230);//pulse
	return dustDensity;
}

void Hardware::setWaterPump(bool state)
{
	digitalWrite(WATER_PUMP, state);
	m_WaterPumpState = state;
}

void Hardware::setIntakeFan(bool state)
{
	if (!state && m_HeaterState)
		return;//can't turn off fan if heater is on
	digitalWrite(RELAY_FAN, !state);
	m_IntakeFanState = state;
}

void Hardware::setHeater(bool state)
{
	digitalWrite(RELAY_HEATER, !state);
	m_HeaterState = state;
	if (state) //heater always turns fan On
		setIntakeFan(true);
}

void Hardware::setLights(bool state)
{
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

void Hardware::setDefaultPinModesAndValues()
{
	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, LOW);

	pinMode(WATER_PUMP, OUTPUT);
	digitalWrite(WATER_PUMP, LOW);

	pinMode(RELAY_FAN, OUTPUT);
	digitalWrite(RELAY_FAN, HIGH);

	pinMode(RELAY_HEATER, OUTPUT);
	digitalWrite(RELAY_HEATER, HIGH);

	pinMode(RELAY_LIGHT, OUTPUT);
	digitalWrite(RELAY_LIGHT, HIGH);

	pinMode(PARTICLE_LED, OUTPUT);//ACTIVE LOW
	//digitalWrite(PARTICLE_LED, HIGH);
	analogWrite(PARTICLE_LED, 230);//10 ms

	pinMode(JOY_BT, INPUT_PULLUP);
	pinMode(JOY_LH, INPUT_PULLUP);
	pinMode(JOY_RH, INPUT_PULLUP);

	pinMode(WATER_LVL, INPUT);
	pinMode(SOIL_HUMIDITY, INPUT);
	pinMode(PARTICLE, INPUT);

	delay(1000);
}

