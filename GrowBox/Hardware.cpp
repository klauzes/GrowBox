#include "Hardware.h"
#include <DHT_U.h>
#include "Pins.h"
#include <Arduino.h>

// Obiectul DHT serverserte comunicarea cu senzorul combinat temperatura-umiditate
static DHT m_dht(DHT_21, DHT21);
// Data membre folosite pentru persistarea starii componentelor activate (activabile)
static bool m_waterPumpState = false;
static bool m_intakeFanState = false;
static bool m_heaterState = false;
static bool m_lightsState = false;
static bool m_manualControl = false;

// Aceasta valoare reprezinta durata minima de timp in milisecunde sub care nu se permit schimbari de stare a releelor.
// Acestea fiind dispozitive mecanice nu pot fi actionare cu o frecventa mare
static const unsigned long DEFAULT_RELAY_DEBOUNCE = 30000; 
// Valorile de debounce pentru fiecare dintre cele 3 relee actionate
static unsigned long m_intakeFanLastChange = 0;
static unsigned long m_heaterLastChange = 0;
static unsigned long m_lightsLastChange = 0;

// Data membre ce persista valorile citite de la senzori
static const double INITIAL_VALUE = -1;
static double m_temp = INITIAL_VALUE;
static double m_humidity = INITIAL_VALUE;
static double m_waterLevel = INITIAL_VALUE;
static double m_soilHumidity = INITIAL_VALUE;
static double m_particleCount = INITIAL_VALUE;


//Getteri si Setteri
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
	int sensorValue = analogRead(WATER_LVL);
	if (sensorValue >= MAX_ADC_WATER_LEVEL)
		sensorValue = MAX_ADC_WATER_LEVEL;
	if (sensorValue <= MIN_ADC_WATER_LEVEL)
		sensorValue = MIN_ADC_WATER_LEVEL;
	
	//m_waterLevel = analogRead(WATER_LVL);
	m_waterLevel = map(sensorValue, MIN_ADC_WATER_LEVEL, MAX_ADC_WATER_LEVEL, 0, 100);
	return m_waterLevel;
}

double Hardware::getSoilHumidity()
{
	double averageTenReadings[10]{};
	for (int i = 0; i < 10; i++) {
		int sensorValue = analogRead(SOIL_HUMIDITY);
		if (sensorValue < MIN_ADC_SOIL_HUMIDITY)
			sensorValue = MIN_ADC_SOIL_HUMIDITY;
		if (sensorValue > MAX_ADC_SOIL_HUMIDITY)
			sensorValue = MAX_ADC_SOIL_HUMIDITY;
		averageTenReadings[i] = map(sensorValue, MAX_ADC_SOIL_HUMIDITY, MIN_ADC_SOIL_HUMIDITY, 0, 100);
	}
	double meanAverage = 0;
	for (int i = 0; i < 10; i++) { meanAverage += averageTenReadings[i]; }; meanAverage /= 10;
	double deviationPerSample[10]{};
	for (int i = 0; i < 10; i++)
	{
		deviationPerSample[i] = pow((averageTenReadings[i] - meanAverage), 2);
	}
	double variance = 0;
	for (int i = 0; i < 10; i++) { variance += deviationPerSample[i]; }; variance /= 10;
	double standardDeviation = sqrt(variance);

	int considered = 0;
	double meanAverageWithStandardDeviation = 0;
	for (int i = 0; i < 10; i++)
	{
		if (fabs(averageTenReadings[i] - meanAverage) <= standardDeviation)
		{
			considered++;
			meanAverageWithStandardDeviation += averageTenReadings[i];
		}
	}
	meanAverageWithStandardDeviation /= considered;
	m_soilHumidity = meanAverageWithStandardDeviation;
	return m_soilHumidity;	
}

double Hardware::getParticleCount()
{	
	// Conform manualului senzorul de particule are nevoie de un puls de lumina IR la intervale de 10 milisecunde.
	// Aceasta se trduce intr-un semnal PWM cu duty cycle de 230 (ledul este activ in faza negativa de ciclu)
	// Pe durata citirii trebuie totusi oprit acest semnal PWM si simulat prin pauze cronometrate pe durata citiri canalului ADC (40-50 milisecunde)

	//Oprim PWM suprascriin valoarea 0, ledul fiind aprins cand este conectat la GND avem deci ledul IR aprins
	analogWrite(PARTICLE_LED, 0);
	delayMicroseconds(280); // pauza 0.28 ms 
	double readVal = analogRead(PARTICLE); //citire ADC
	delayMicroseconds(40); // pauza 0.04 ms reprezentand restul de timp ramas
	analogWrite(PARTICLE_LED, 255); //Stingem ledul IR


	delayMicroseconds(9680); // facem pauza 9.68 ms si efectuam calculele
	double calcVoltage = readVal * (5.0 / 1024); // facem conversia volti canal ADC 10 bit. Obtinem voltajul citit la iesirea din fotodioda
	double dustDensity = 0.17 * calcVoltage - 0.1; // iesirea analog a senzorului atinge aproximativ 85% din tensiunea de alimentare la o concentratie de 0,58 ug/m^3
	if (dustDensity < 0)	//vezi caracteristici de functionare in lucare
		dustDensity = 0;	
	analogWrite(PARTICLE_LED, 230);//revenim la PWM
	// PWM Arduino Mega 490Hz
	// 255 - 230 = 25 (ledul IR este Activ cand semnalul acesta este la zero ,de aici inversia)
	// 25% din 490Hz = 122Hz
	return dustDensity;
}

void Hardware::setWaterPump(bool state)
{
	if (state == m_waterPumpState)
		return;
	digitalWrite(WATER_PUMP, state);
	m_waterPumpState = state;
}

void Hardware::setIntakeFan(bool state)
{
	if (millis() > m_intakeFanLastChange || getManualControl())
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
	if (millis() > m_heaterLastChange || getManualControl())
	{	
		// protectie impotriva supraincalzirii rezistentei.
		// Pornirea acesteia activeaza implicit si ventilatorul.
		if (state) 
		{
			m_intakeFanState = state;
			digitalWrite(RELAY_FAN, state);
		}

		if (state == m_heaterState)
			return;
		digitalWrite(RELAY_HEATER, state);	
		m_heaterState = state;		
		if(!getManualControl())
			m_heaterLastChange = millis() + DEFAULT_RELAY_DEBOUNCE;
	}
}

void Hardware::setLights(bool state)
{
	if (millis()> m_lightsLastChange || getManualControl())
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


// Aceata metoda stabileste starea initial desirata a pinilor.
// Arduino nu are o stare implicita a pinilor, iar cei neconfigurati se numesc "FLOATING"
// Adica se afla intr-o stare nedeterminata fara nicio garantie ca aceasta nu atinge pragul negativ sau cel pozitiv.
// Aceasta ar insemna ca putem avea dispozitive actionare chiar daca nu am decretat in mod explicit
// Pentru a mitiga acest efect, aceasta metoda se poate apela ori de cate ori dorm o aducere la starea " initiala" sau de boot a configuratiei pinilor
void Hardware::setDefaultPinModesAndValues()
{
	m_dht.begin();
	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, LOW);

	pinMode(WATER_PUMP, OUTPUT);
	digitalWrite(WATER_PUMP, LOW);
	m_waterPumpState = false;

	pinMode(RELAY_FAN, OUTPUT);
	digitalWrite(RELAY_FAN, LOW);
	m_intakeFanState = false;

	pinMode(RELAY_HEATER, OUTPUT);
	digitalWrite(RELAY_HEATER, LOW);
	m_heaterState = false;

	pinMode(RELAY_LIGHT, OUTPUT);
	digitalWrite(RELAY_LIGHT, LOW);
	m_lightsState = false;

	pinMode(PARTICLE_LED, OUTPUT);//ACTIVE LOW
	analogWrite(PARTICLE_LED, 230);//10 ms

	pinMode(JOY_BT, INPUT_PULLUP);
	pinMode(JOY_LH, INPUT_PULLUP);
	pinMode(JOY_RH, INPUT_PULLUP);

	pinMode(WATER_LVL, INPUT);
	pinMode(SOIL_HUMIDITY, INPUT);
	pinMode(PARTICLE, INPUT);
}