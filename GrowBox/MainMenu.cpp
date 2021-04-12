#include "MainMenu.h"
#include "DateTime.h"
#include "Hardware.h"
#include "Joystick.h"
#include "Log.h"

MainMenu::MainMenu( U8GLIB_ST7920_128X64_4X* disp)
{
	p_display = disp;	
	Joystick::setMinMax(0, 0);
	Joystick::reset();
	setFont(u8g_font_9x18);//default font
	doesNotTimeOut = true;
	nextScreenTime = millis() + DEFAULT_MAIN_SCREEN_NEXT_PAGE;
	screenOneOrTwo = true;
}

void MainMenu::doMenu()
{
	p_display->setFont(u8g_font_6x10);
	p_display->firstPage();

	DateTime curTime(true);
	char cTime[9]; curTime.cTime(cTime);
	char cDate[12]; curTime.cDate(cDate);

	double airTemp = Hardware::getTemperature();
	double airHumidity = Hardware::getHumidity();
	double soilHumidity = Hardware::getSoilHumidity();
	double waterLevel = Hardware::getWaterLevel();
	double particleCount = Hardware::getParticleCount() * 100;
	int lightsState = Hardware::getLightsState();
	int heaterState = Hardware::getHeaterState();
	int fanState = Hardware::getIntakeFanState();
	int waterPumpState = Hardware::getWaterPumpState();
	bool cardState = Log::hasCard();

	const char on[] = "ON";
	const char off[] = "OFF";
	char bufAirTemp[7];
	char bufAirHumidity[7];
	char bufSoilHumidity[7];
	char bufWaterLevel[7];

	char bugParticleCount[7];

	dtostrf(airTemp, 2, 2, bufAirTemp);
	dtostrf(airHumidity, 2, 2, bufAirHumidity);
	dtostrf(soilHumidity, 2, 2, bufSoilHumidity);
	dtostrf(waterLevel, 2, 2, bufWaterLevel);

	dtostrf(particleCount, 2, 2, bugParticleCount);


	char airTempString[30]; sprintf(airTempString,				"Air  Temp.   :%s C", bufAirTemp);
	char airHumidityString[30]; sprintf(airHumidityString,		"Air  Humidity:%s%%", bufAirHumidity);
	char soilHumidityString[30]; sprintf(soilHumidityString,	"Soil Humidity:%s%%", bufSoilHumidity);
	char waterLevelString[30]; sprintf(waterLevelString,		"Water Level  :%s%%", bufWaterLevel);

	char particleCountString[30]; sprintf(particleCountString,	"Particles :%s mg/mc", bugParticleCount);
	char lightStateString[30]; sprintf(lightStateString,		"Lights    :%s", lightsState ? on:off);
	char heaterStateString[30]; sprintf(heaterStateString,		"Heater    :%s", heaterState ? on:off);
	char fanStateString[30]; sprintf(fanStateString,			"Fan       :%s", fanState ? on:off);
	char waterPumpStateString[30]; sprintf(waterPumpStateString,"Water Pump:%s", waterPumpState ? on:off);
	char sdCardStateString[30]; sprintf(sdCardStateString,		"SD Card   :%s", cardState ? on:off);

	if (millis() > nextScreenTime)
	{
		screenOneOrTwo = !screenOneOrTwo;
		nextScreenTime = millis() + DEFAULT_MAIN_SCREEN_NEXT_PAGE;
	}

	if (screenOneOrTwo) 
	{
		do {
			p_display->setFont(u8g_font_7x13B);
			p_display->drawStr(getHorizontalAllignement(7, 8), p_display->getFontLineSpacing() + 1, cTime);
			p_display->drawStr(getHorizontalAllignement(7, 11), p_display->getFontLineSpacing() * 2 + 1, cDate);
			p_display->drawLine(0, 27, MAXWIDTH, 27);
			p_display->setFont(u8g_font_6x10r);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 4 + 1, airTempString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 5 + 1, airHumidityString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 6 + 1, soilHumidityString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 7 + 1, waterLevelString);
		} while (p_display->nextPage());
	}
	else
	{
		do {	
			p_display->setFont(u8g_font_6x10r);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 1 + 1, particleCountString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 2 + 1, lightStateString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 3 + 1, heaterStateString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 4 + 1, fanStateString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 5 + 1, waterPumpStateString);
			p_display->drawStr(0, p_display->getFontLineSpacing() * 6 + 1, sdCardStateString);


		} while (p_display->nextPage());

	}

	if (Joystick::wasButtonPressed() && p_menuItems != nullptr)
	{
		Beeper::beepNav();
		(*p_menuItems[0].menuFunctionCall)(2);
	}
	
}

int MainMenu::getHorizontalAllignement(int fontSpacing, int numChars)
{
	int lineTotalPixels = numChars * fontSpacing;
	int midPoint = lineTotalPixels / 2;
	int screenMidPoint = MAXWIDTH / 2;
	return screenMidPoint - midPoint;
}



