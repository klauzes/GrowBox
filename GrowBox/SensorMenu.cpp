#include<Arduino.h>
#include "SensorMenu.h"
#include "Hardware.h"
#include "Joystick.h"

/*
* Aceasta este o clasa derivata din Menu si face folos de un ENUM ce inglobeaza toate tipurile de senzori ce pot fi cititi
* pentru a afisa informatiile acestora pe ecran. Afiseaza valoare minima si maxima intalnica cat si cea curent cititia.
* Poate genera si cu grafic in jumatatea de jos a ecranului motiv pentru care in constructor trimitem si valorile minimale si maximale 
* Specifice acelui tip de senzor
*/

SensorMenu::SensorMenu(U8GLIB_ST7920_128X64_4X* disp, const Sensor senType, double maxPossible, double minPossible)
{
	p_display = disp;
	doesNotTimeOut = true;
	m_sensorType = senType;
	m_maxPossibleValue = maxPossible;
	m_minPossibleValue = minPossible;
	m_readingCount = 0;
	switch (m_sensorType)
	{
	case SensorMenu::AirTemp:
		m_curValue = m_maxValue = m_minValue = Hardware::getTemperature();		
		break;
	case SensorMenu::AirHumidity:
		m_curValue = m_maxValue = m_minValue = Hardware::getHumidity();
		break;
	case SensorMenu::SoilHumidity:
		m_curValue = m_maxValue = m_minValue = Hardware::getSoilHumidity();
		break;
	case SensorMenu::WaterLevel:
		m_curValue = m_maxValue = m_minValue = Hardware::getWaterLevel();
		break;
	case SensorMenu::AirParticles:
		m_maxPossibleValue = maxPossible * 100;
		m_minPossibleValue = minPossible * 100;
		m_curValue = m_maxValue = m_minValue = Hardware::getParticleCount() * 100;//subunitare
		break;
	default:
		break;
	}
}

void SensorMenu::doMenu()
{
	switch (m_sensorType)
	{
	case SensorMenu::AirTemp:		
		m_curValue = Hardware::getTemperature();
		processReading();
		break;
	case SensorMenu::AirHumidity:
		m_curValue = Hardware::getHumidity();
		processReading();
		break;
	case SensorMenu::SoilHumidity:
		m_curValue = Hardware::getSoilHumidity();
		processReading();
		break;
	case SensorMenu::WaterLevel:
		m_curValue = Hardware::getWaterLevel();
		processReading();
		break;
	case SensorMenu::AirParticles:
		m_curValue = Hardware::getParticleCount() * 100;	
		processReading();
		break;
	}
}

void SensorMenu::doValuesAndGraph()
{
	p_display->firstPage();
	
	do {
		p_display->drawLine(0, MAX_SCREEN_HEIGHT / 2, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT / 2);
		p_display->setFont(u8g_font_6x10);
		char minValStr[9];
		char maxValStr[9];//9 octeti pentru a acoperi cazul cu valori negative
		char curValStr[9];
		dtostrf(m_minValue, 5, 2, minValStr);//la fel
		dtostrf(m_maxValue, 5, 2, maxValStr);
		dtostrf(m_curValue, 5, 2, curValStr);
		char strMin[14];//tot pentru minus
		char strMax[14];
		char strCur[14];
		sprintf(strMin, "Min:%s", minValStr);
		sprintf(strMax, "Max:%s", maxValStr);
		sprintf(strCur, "Cur:%s", curValStr);
		p_display->drawStr(0, p_display->getFontLineSpacing() + 1, strMin);
		p_display->drawStr(0, p_display->getFontLineSpacing() * 2 + 1, strMax);
		p_display->drawStr(0, p_display->getFontLineSpacing() * 3 + 1, strCur);
		for (int i = 0; i < m_readingCount; i++)
		{
			p_display->drawLine(i, MAX_SCREEN_HEIGHT, i, barAsPercentage(m_readings[i]));
		}
	
	} while (p_display->nextPage());

	
	if (Joystick::wasButtonPressed() && p_menuItems != nullptr)
		(*p_menuItems[0].menuFunctionCall)(2);
}

int SensorMenu::barAsPercentage(double val)
{
	int availableSpace = MAX_SCREEN_HEIGHT / 2;
	int asPercentage = map(val, m_minPossibleValue, m_maxPossibleValue, MAX_SCREEN_HEIGHT, availableSpace);	
	return asPercentage <= availableSpace ? availableSpace : asPercentage; //dar nu mai mari decat rezolutia
}

void SensorMenu::processReading()
{
	if (m_curValue < m_minValue)
		m_minValue = m_curValue;
	if (m_curValue > m_maxValue)
		m_maxValue = m_curValue;
	addReadingToList();
	doValuesAndGraph();
}

void SensorMenu::addReadingToList()
{
	if (m_readingCount < MAX_SCREEN_WIDTH)
	{
		m_readings[m_readingCount] = m_curValue;
		if (m_readingCount < MAX_SCREEN_WIDTH)
			m_readingCount++;
	}
	else
	{
		for (int i = 0; i < MAX_SCREEN_WIDTH - 1; i++)
		{
			m_readings[i] = m_readings[i + 1];
		}
		m_readings[MAX_SCREEN_WIDTH - 1] = m_curValue;
	}
}