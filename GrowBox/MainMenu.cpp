#include "MainMenu.h"
#include "DateTime.h"
#include "Hardware.h"
#include "Joystick.h"

MainMenu::MainMenu( U8GLIB_ST7920_128X64_4X* disp)
{
	p_display = disp;	
	Joystick::setMinMax(0, 0);
	Joystick::reset();
	setFont(u8g_font_9x18);//default font
	doesNotTimeOut = true;
}

void MainMenu::doMenu()
{
	p_display->setFont(u8g_font_6x10);
	p_display->firstPage();
	DateTime curTime(true);
	char cTime[9]; curTime.cTime(cTime);
	char cDate[12]; curTime.cDate(cDate);
	double temp = Hardware::getTemperature();
	double humidity = Hardware::getHumidity();
	double sHumidity = Hardware::getSoilHumidity();
	char bufTemp[7];
	char bufHumi[7];
	char bufsHumi[7];
	dtostrf(temp, 2, 2, bufTemp);
	dtostrf(humidity, 2, 2, bufHumi);
	dtostrf(sHumidity, 2, 2, bufsHumi);

	//if (((millis() / 1000) % 5) == 0)
//	Serial.println(millis());// this repeats every odd 5 seconds



	do {
		p_display->drawStr(0, p_display->getFontLineSpacing()+1, cTime);
		p_display->drawStr(0, p_display->getFontLineSpacing()*2+1, cDate);
		p_display->drawStr(0, p_display->getFontLineSpacing()*3+1, bufTemp);
		p_display->drawStr(0, p_display->getFontLineSpacing()*4+1, bufHumi);
		p_display->drawStr(0, p_display->getFontLineSpacing()*5+1, bufsHumi);
	} while (p_display->nextPage());	
	
	if (Joystick::wasButtonPressed() && p_menuItems != nullptr) {
		Beeper::beepNav();
		(*p_menuItems[0].menuFunctionCall)(2);
	}
}


