#pragma once
#include "Menu.h"

class SensorMenu : public Menu
{

public:
	enum Sensor
	{
		AirTemp = 0,
		AirHumidity,
		SoilHumidity,
		WaterLevel,
		AirParticles
	};

	SensorMenu(U8GLIB_ST7920_128X64_4X* disp, const Sensor senType, double maxPossible, double minPossible);
	void doMenu() override;

private:
	Sensor sensorType;	
	double minValue;
	double maxValue;
	double curValue;
	int readingCount;
	double readings[MAXWIDTH+1];
	void addReadingToList();
	void doValuesAndGraph();
	int barAsPercentage(double val);
	int maxPossibleValue;
	int minPossibleValue;
	void processReading();	
};
