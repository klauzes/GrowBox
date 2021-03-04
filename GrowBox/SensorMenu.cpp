#include "SensorMenu.h"
#include "Hardware.h"
#include <Arduino.h>

SensorMenu::SensorMenu(Joystick* joy, U8GLIB_ST7920_128X64_4X* disp, const Sensor senType, double maxPossible, double minPossible)
{
	p_display = disp;
	p_joy = joy;	
	doesNotTimeOut = true;
	sensorType = senType;
	maxPossibleValue = maxPossible;
	minPossibleValue = minPossible;
	readingCount = 0;
	switch (sensorType)
	{
	case SensorMenu::AirTemp:
		curValue = maxValue = minValue = Hardware::getTemperature();		
		break;
	case SensorMenu::AirHumidity:
		curValue = maxValue = minValue = Hardware::getHumidity();
		break;
	case SensorMenu::SoilHumidity:
		curValue = maxValue = minValue = Hardware::getSoilHumidity();
		break;
	case SensorMenu::WaterLevel:
		curValue = maxValue = minValue = Hardware::getWaterLevel();
		break;
	case SensorMenu::AirParticles:
		maxPossibleValue = maxPossible * 100;
		minPossibleValue = minPossible * 100;
		curValue = maxValue = minValue = Hardware::getParticleCount() * 100;//subunitary values	
		break;
	default:
		break;
	}
}

void SensorMenu::doMenu()
{
	switch (sensorType)
	{
	case SensorMenu::AirTemp:		
		curValue = Hardware::getTemperature();
		processReading();
		break;
	case SensorMenu::AirHumidity:
		curValue = Hardware::getHumidity();
		processReading();
		break;
	case SensorMenu::SoilHumidity:
		curValue = Hardware::getSoilHumidity();
		processReading();
		break;
	case SensorMenu::WaterLevel:
		curValue = Hardware::getWaterLevel();
		processReading();
		break;
	case SensorMenu::AirParticles:
		curValue = Hardware::getParticleCount() * 100;	
		processReading();
		break;
	}
}

void SensorMenu::doValuesAndGraph()
{
	p_display->firstPage();
	
	do {
		p_display->drawLine(0, MAXHEIGHT / 2, MAXWIDTH, MAXHEIGHT / 2);
		p_display->setFont(u8g_font_6x10);
		char minValStr[9];//1024.12
		char maxValStr[9];//9 bytes in case of minus sign
		char curValStr[9];
		dtostrf(minValue, 5, 2, minValStr);//5 for minus in front
		dtostrf(maxValue, 5, 2, maxValStr);
		dtostrf(curValue, 5, 2, curValStr);
		char strMin[14];//14 allow for minus sign
		char strMax[14];
		char strCur[14];
		sprintf(strMin, "Min:%s", minValStr);
		sprintf(strMax, "Max:%s", maxValStr);
		sprintf(strCur, "Cur:%s", curValStr);
		p_display->drawStr(0, p_display->getFontLineSpacing() + 1, strMin);
		p_display->drawStr(0, p_display->getFontLineSpacing() * 2 + 1, strMax);
		p_display->drawStr(0, p_display->getFontLineSpacing() * 3 + 1, strCur);
		for (int i = 0; i < readingCount; i++)
		{
			p_display->drawLine(i, MAXHEIGHT, i, barAsPercentage(readings[i]));
		}
	
	} while (p_display->nextPage());

	
	if (p_joy->wasButtonPressed() && p_menuItems != nullptr)
		(*p_menuItems[0].menuFunctionCall)(2);
}

int SensorMenu::barAsPercentage(double val)
{
	int availableSpace = MAXHEIGHT / 2;
	int asPercentage = map(val, minPossibleValue, maxPossibleValue, MAXHEIGHT, availableSpace);	
	return asPercentage <= availableSpace ? availableSpace : asPercentage; //if somehow measurements get larger than expected cap out to screen resolution
}

void SensorMenu::processReading()
{
	if (curValue < minValue)
		minValue = curValue;
	if (curValue > maxValue)
		maxValue = curValue;
	addReadingToList();
	doValuesAndGraph();
}

void SensorMenu::addReadingToList()
{
	if (readingCount < MAXWIDTH)
	{
		readings[readingCount] = curValue;
		if (readingCount < MAXWIDTH)
			readingCount++;
	}
	else
	{
		for (int i = 0; i < MAXWIDTH - 1; i++)
		{
			readings[i] = readings[i + 1];
		}
		readings[MAXWIDTH - 1] = curValue;
	}
}