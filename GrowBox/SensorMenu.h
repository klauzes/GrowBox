/****************************************************************************
*  PROIECT LICENTA:     -SISTEM ELECTRONIC PENTRU AUTOMATIZAREA UNEI SERE-  *
*  LIMBAJ:              C++                                                 *
*  MICROPROCESOR        ATmega2560                                          *
*  STUDENT:             ISPAS N.O. CLAUDIU-OCTAVIAN                         *
*  GRUPA:               307                                                 *
*  PROMOTIA             2018-2021                                           *
*  FACULTATEA DE INFORMATICA                                                *
*  UNIVERSITATEA TITU MAIORESCU                                             *
* ***************************************************************************/
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
	Sensor m_sensorType;	
	double m_minValue;
	double m_maxValue;
	double m_curValue;
	int m_readingCount;
	double m_readings[MAX_SCREEN_WIDTH+1];
	int m_maxPossibleValue;
	int m_minPossibleValue;

	void addReadingToList();
	void doValuesAndGraph();
	int barAsPercentage(double val);
	void processReading();	
};
