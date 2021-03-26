#pragma once
#include "Pins.h"

class Hardware final
{
public:	
	static double getTemperature();
	static double getHumidity();
	static double getWaterLevel(); //TODO implement Liters (as function of volume) but for now ADC
	static double getSoilHumidity(); //TODO implement as percentage but for now ADC value 
	static double getParticleCount();
	static void setWaterPump(bool state);
	static void setIntakeFan(bool state);	
	static void setHeater(bool state);
	static void setLights(bool state);
	static bool getWaterPumpState();
	static bool getIntakeFanState();
	static bool getHeaterState();
	static bool getLightsState();
	static void setDefaultPinModesAndValues();
	static void debounceReadings(double& variable,const double reading);

	
};