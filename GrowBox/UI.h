#pragma once
#include "Joystick.h"
#include <U8glib.h>
#include "Pins.h"
#include "Beeper.h"
#include "MainMenu.h"
#include "NavigationMenu.h"
#include "MessageBox.h"
#include "DateTime.h"

class UI
{

public:
	UI();	
	UI(Joystick* joy);
	~UI();
	void processUserInterface();
	

private:
	Joystick* p_joy;
	U8GLIB_ST7920_128X64_1X* p_display;
	Menu* p_currentMenu;
	DateTime* p_clockSetter;
	DateTime  m_menuTimeOut;
	void createMainMenu();
	static void mainScreenMenuItem(int x);
	static void backToMainMenu(int val);

	static void setClockMenuItem(int val);
	static void setHourMenuItem(int val);
	static void setHour(int val);
	static void setMinuteMenuItem(int val);
	static void setMinute(int val);
	static void setSecondMenuItem(int val);
	static void setSecond(int val);
	static void setYearMenuItem(int val);
	static void setYear(int val);
	static void setMonthMenuItem(int val);
	static void setMonth(int val);
	static void setDateMenuItem(int val);
	static void setDate(int val);
	static void saveClockMenuItem(int val);

	static void setSoundSettingsMenuItem(int val);
	static void setSoundSettings(int val);
	static void prepareNewMenuEntry();

	static void setManualControlMenuItem(int val);
	static void fanON(int val);
	static void fanOFF(int val);

	static void HeatON(int val);
	static void HeatOFF(int val);

	static void LightsON(int val);
	static void LightsOFF(int val);
};
