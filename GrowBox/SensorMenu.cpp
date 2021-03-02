#include "SensorMenu.h"
#include "Hardware.h"
#include <Arduino.h>

SensorMenu::SensorMenu(Joystick* joy, U8GLIB_ST7920_128X64_4X* disp, const Sensor senType, int maxPossible, int minPossible)
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
	}
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
		readings[MAXWIDTH-1] = curValue;
	}	
}

void SensorMenu::doValuesAndGraph()
{
	p_display->firstPage();
	
	do {
		p_display->drawLine(0, MAXHEIGHT / 2, MAXWIDTH, MAXHEIGHT / 2);
		p_display->setFont(u8g_font_6x10);
		char minValStr[8];//1024.12
		char maxValStr[8];
		char curValStr[8];
		dtostrf(minValue, 4, 2, minValStr);
		dtostrf(maxValue, 4, 2, maxValStr);
		dtostrf(curValue, 4, 2, curValStr);
		char strMin[13];
		char strMax[13];
		char strCur[13];
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
	int asPercentage = map((int)val, minPossibleValue, maxPossibleValue, MAXHEIGHT, availableSpace);	
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
