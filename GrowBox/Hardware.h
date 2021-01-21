#pragma once
#include "Pins.h"

class Hardware final
{
public:
	Hardware();
	int getTemperature();
	int getHumidity();
	int getWaterLevel();
	void setWaterPump(int pwm);
	void setExhaustFan(int pwm);
	void setReciclingFan(int pwm);
	void setHeater(bool state);
	void setLights(bool state);

private:

};