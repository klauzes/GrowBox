#include "Hardware.h"
#include <DHT_U.h>
#include "Pins.h"
#include <Arduino.h>

static DHT m_dht(DHT_21, DHT21);
static bool m_waterPumpState = false;
static bool m_intakeFanState = false;
static bool m_heaterState = false;
static bool m_lightsState = false;
static bool m_manualControl = false;
 
static const double INITIAL_VALUE = -1;
static const unsigned long DEFAULT_RELAY_DEBOUNCE = 30000;
static const unsigned long DEFAULT_DHT_READ_INTERVAL = 2000;


static double m_temp = INITIAL_VALUE;
static double m_humidity = INITIAL_VALUE;
static double m_waterLevel = INITIAL_VALUE;
static double m_soilHumidity = INITIAL_VALUE;
static double m_particleCount = INITIAL_VALUE;

static unsigned long m_waterPumpLastChange = 0;
static unsigned long m_intakeFanLastChange = 0;
static unsigned long m_heaterLastChange = 0;
static unsigned long m_lightsLastChange = 0;


double Hardware::getTemperature()
{
	float readT = m_dht.readTemperature();
	if(!isnan(readT))
		m_temp = readT;
	return m_temp;
}

double Hardware::getHumidity()
{
	float readH = m_dht.readHumidity();
	if (!isnan(readH))
		m_humidity = readH;
	return m_humidity;
}

double Hardware::getWaterLevel()
{
	int adcValue = analogRead(WATER_LVL);
	debounceReadings(m_waterLevel, adcValue);
	return m_waterLevel;
}

double Hardware::getSoilHumidity()
{
	int adcValue = 1023 - analogRead(SOIL_HUMIDITY);
	debounceReadings(m_soilHumidity, adcValue);
	return m_soilHumidity;
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
	//debounceReadings(m_particleCount, dustDensity);
	return dustDensity;
}

void Hardware::setWaterPump(bool state)
{
	if (millis() > m_waterPumpLastChange)
	{
		if (state == m_waterPumpState)
			return;
		digitalWrite(WATER_PUMP, state);
		m_waterPumpState = state;
		if (!getManualControl())
			m_waterPumpLastChange = millis() + DEFAULT_RELAY_DEBOUNCE;
	}	
}

void Hardware::setIntakeFan(bool state)
{
	if (millis() > m_intakeFanLastChange)
	{
		if (state == m_intakeFanState)
			return;
		if (!state && m_heaterState)
			return;//can't turn off fan if heater is on
		digitalWrite(RELAY_FAN, state);
		m_intakeFanState = state;
		if (!getManualControl())
			m_intakeFanLastChange = millis() + DEFAULT_RELAY_DEBOUNCE;
	}
}

void Hardware::setHeater(bool state)
{
	/*Serial.print("setHeater"); Serial.println(state);
	Serial.print("m_heaterLastChange"); Serial.println(m_heaterLastChange);
	Serial.print("getManualControl()"); Serial.println(getManualControl());
	Serial.print("DEFAULT_RELAY_DEBOUNCE"); Serial.println(DEFAULT_RELAY_DEBOUNCE);*/
	if (millis() > m_heaterLastChange)
	{		
		if (state == m_heaterState)
			return;
		digitalWrite(RELAY_HEATER, state);
		m_heaterState = state;
		if (state) //heater always turns fan On
		{			
			setIntakeFan(true);
		}
		if(!getManualControl())
			m_heaterLastChange = millis() + DEFAULT_RELAY_DEBOUNCE;
	}
}

void Hardware::setLights(bool state)
{
	if (millis()> m_lightsLastChange)
	{
		if (state == m_lightsState)
			return;
		digitalWrite(RELAY_LIGHT, state); 
		m_lightsState = state;
		if (!getManualControl())
			m_lightsLastChange = millis() + DEFAULT_RELAY_DEBOUNCE;
	}
}

void Hardware::setManualControl(bool state)
{
	m_manualControl = state;
}

bool Hardware::getWaterPumpState()
{
	return m_waterPumpState;
}

bool Hardware::getIntakeFanState()
{
	return m_intakeFanState;
}

bool Hardware::getHeaterState()
{
	return m_heaterState;
}

bool Hardware::getLightsState()
{
	return m_lightsState;
}

bool Hardware::getManualControl()
{
	return m_manualControl;
}

void Hardware::setDefaultPinModesAndValues()
{
	m_dht.begin();
	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, LOW);

	pinMode(WATER_PUMP, OUTPUT);
	digitalWrite(WATER_PUMP, LOW);

	pinMode(RELAY_FAN, OUTPUT);
	digitalWrite(RELAY_FAN, LOW);

	pinMode(RELAY_HEATER, OUTPUT);
	digitalWrite(RELAY_HEATER, LOW);

	pinMode(RELAY_LIGHT, OUTPUT);
	digitalWrite(RELAY_LIGHT, LOW);

	pinMode(PARTICLE_LED, OUTPUT);//ACTIVE LOW
	//digitalWrite(PARTICLE_LED, HIGH);
	analogWrite(PARTICLE_LED, 230);//10 ms

	pinMode(JOY_BT, INPUT_PULLUP);
	pinMode(JOY_LH, INPUT_PULLUP);
	pinMode(JOY_RH, INPUT_PULLUP);

	pinMode(WATER_LVL, INPUT);
	pinMode(SOIL_HUMIDITY, INPUT);
	pinMode(PARTICLE, INPUT);
}

void Hardware::debounceReadings(double& variable, const double reading)
{
	if (reading > INITIAL_VALUE  && reading < 1024)
	{
		variable = reading;
	}
}

