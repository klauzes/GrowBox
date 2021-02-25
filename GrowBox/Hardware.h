#pragma once
#include "Pins.h"

class Hardware final
{
public:	
	static float getTemperature();
	static float getHumidity();
	static float getWaterLevel(); //TODO implement Liters (as function of volume) but for now ADC
	static float getSoilHumidity(); //TODO implement as percentage but for now ADC value 
	static float getParticleCount();
	static void setWaterPump(bool state);
	static void setIntakeFan(bool state);	
	static void setHeater(bool state);
	static void setLights(bool state);
	static bool getWaterPumpState();
	static bool getIntakeFanState();
	static bool getHeaterState();
	static bool getLightsState();

private:
	static void setParticleSensorLed(bool state);
};