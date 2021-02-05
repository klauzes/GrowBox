#include "UI.h"
static UI* p_staticUI;
#include <Arduino.h>
#include "PersistentData.h"
#include "Log.h"
#define DEFAULT_SCREEN_TIMEOUT 30

UI::UI(Joystick* joy) : UI()
{
	p_joy = joy;
}

UI::UI()
{	
	p_currentMenu = nullptr;
	p_clockSetter = nullptr;
	p_display = new U8GLIB_ST7920_128X64_1X(LCD_EN, LCD_RW, LCD_RS, 0);
	p_staticUI = this;
	m_menuTimeOut = DateTime();
	createMainMenu();
}

UI::~UI()
{
	if (p_currentMenu)
		delete p_currentMenu;
	if (p_display)
		delete p_display;
}

void UI::createMainMenu()
{
	Log logAction("Create main menu screen");
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new MainMenu(p_staticUI->p_joy, p_staticUI->p_display);
	p_staticUI->p_currentMenu->addMenuItem(mainScreenMenuItem, "x");
}

#pragma region MainMenu
void UI::mainScreenMenuItem(int x)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Clock settings");
	p_staticUI->p_currentMenu->addMenuItem(setSoundSettingsMenuItem, "Sound settings");
	p_staticUI->p_currentMenu->addMenuItem(setManualControlMenuItem, "Manual control");
	p_staticUI->p_currentMenu->addMenuItem(backToMainMenu, "Back");
}

void UI::backToMainMenu(int val)
{
	Log logAction("Back to main menu");
	if (p_staticUI->p_clockSetter) {
		delete p_staticUI->p_clockSetter;
		p_staticUI->p_clockSetter = nullptr;
	}
	p_staticUI->createMainMenu();
}
#pragma endregion

#pragma region ClockSettings
void UI::setClockMenuItem(int val)
{
	Log logAction("Access Clock Settings");
	prepareNewMenuEntry();
	if (!p_staticUI->p_clockSetter)
		p_staticUI->p_clockSetter = new DateTime(true);
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	p_staticUI->p_currentMenu->addMenuItem(setHourMenuItem, "Set hour");
	p_staticUI->p_currentMenu->addMenuItem(setMinuteMenuItem, "Set minute");
	p_staticUI->p_currentMenu->addMenuItem(setSecondMenuItem, "Set seconds");
	p_staticUI->p_currentMenu->addMenuItem(setYearMenuItem, "Set year");
	p_staticUI->p_currentMenu->addMenuItem(setMonthMenuItem, "Set month");
	p_staticUI->p_currentMenu->addMenuItem(setDateMenuItem, "Set date");
	p_staticUI->p_currentMenu->addMenuItem(saveClockMenuItem, "Save settings");
	p_staticUI->p_currentMenu->addMenuItem(mainScreenMenuItem, "Back");	
}

void UI::setHourMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	for (int i = 0; i < 24; i++) {
		char buf[10];
		if (i < 13)
			sprintf(buf,i<10?"0%i AM":"%i AM", i);
		else
			sprintf(buf, "%i PM", i);
		p_staticUI->p_currentMenu->addMenuItem(setHour, buf);
	}
}

void UI::setHour(int val)
{
	Log logAction("Hour was set");
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setHour(val);
	}
	setClockMenuItem(NULL);
}


void UI::setMinuteMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	for (int i = 0; i < 60; i++) {
		char buf[14];	
		if(i<10)
			sprintf(buf, i==1?"0%i minute": "0%i minutes", i);
		else
			sprintf(buf, "%i minutes", i);
		p_staticUI->p_currentMenu->addMenuItem(setMinute, buf);
	}
}

void UI::setMinute(int val)
{
	Log logAction("Minute was set");
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setMinute(val);
	}
	setClockMenuItem(NULL);
}


void UI::setSecondMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	for (int i = 0; i < 60; i++) {
		char buf[14];
		if(i<10)
			sprintf(buf, i==1?"0%i second": "0%i seconds", i);
		else
			sprintf(buf, "%i seconds", i);

		p_staticUI->p_currentMenu->addMenuItem(setSecond, buf);
	}
}

void UI::setSecond(int val)
{
	Log logAction("Second was set");
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setSecond(val);
	}
	setClockMenuItem(NULL);
}


void UI::setYearMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	for (int i = 2020; i < 2100; i++) {
		char buf[20];		
		sprintf(buf, "%i", i);
		p_staticUI->p_currentMenu->addMenuItem(setYear, buf);
	}
}

void UI::setYear(int val)
{
	Log logAction("Year was set");
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setYear(2020+val);		
	}
	setClockMenuItem(NULL);
}


void UI::setMonthMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	for (int i = 1; i < 13; i++) {		
		char monthName[3]; p_staticUI->p_clockSetter->getMonthName(i, monthName);
		p_staticUI->p_currentMenu->addMenuItem(setMonth, monthName);
	}
}

void UI::setMonth(int val)
{
	Log logAction("Month was set");
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setMonth(val+1);		
	}
	setClockMenuItem(NULL);
}


void UI::setDateMenuItem(int val)
{
	prepareNewMenuEntry();
	if (!p_staticUI->p_clockSetter->yyyy || !p_staticUI->p_clockSetter->mm)
	{
		Beeper::beepNOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_joy, p_staticUI->p_display,"ERROR","Please set year","and month first.");
		p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "x");
	}
	else
	{		
		p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
		p_staticUI->p_currentMenu->setFont(u8g_font_7x13);
		int daysInMonth = p_staticUI->p_clockSetter->daysInMonth(p_staticUI->p_clockSetter->mm, p_staticUI->p_clockSetter->yyyy);
		char month[3]; p_staticUI->p_clockSetter->getMonthName(p_staticUI->p_clockSetter->mm, month);
		for (int i = 1; i <= daysInMonth; i++) {
			char date[150];			
			p_staticUI->p_clockSetter->setDate(i);
			char dow[3];  p_staticUI->p_clockSetter->getdayOfWeek(dow);
			char cdate[12];  p_staticUI->p_clockSetter->cDate(cdate);
			sprintf(date, "%s, %s", dow, cdate);
			p_staticUI->p_currentMenu->addMenuItem(setDate, date);
		}
	}
}

void UI::setDate(int val)
{
	Log logAction("Date was set");
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setDate(val+1);
	}
	setClockMenuItem(NULL);
}

void UI::saveClockMenuItem(int val)
{
	prepareNewMenuEntry();
	if (!p_staticUI->p_clockSetter->yyyy || !p_staticUI->p_clockSetter->mm ||
		!p_staticUI->p_clockSetter->d )
	{
		Beeper::beepNOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_joy, p_staticUI->p_display, "INCOMPLETE", "Please set all fields", "before saving.");
		p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "x");
	}
	else
	{
		p_staticUI->p_clockSetter->save2RTC();
		delete p_staticUI->p_clockSetter;
		p_staticUI->p_clockSetter = nullptr;
		Beeper::beepOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_joy, p_staticUI->p_display, "SUCCESS", "Real Time Clock", "settings saved.");
		p_staticUI->p_currentMenu->addMenuItem(mainScreenMenuItem, "x");
	}
	Log logAction("Clock was saved");
}
#pragma endregion

#pragma region SoundSettings
void UI::setSoundSettingsMenuItem(int val)
{
	Log logAction("Access Sound Settings");
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_joy, p_staticUI->p_display);
	for (int i = 0; i <= 10; i++) {
		char volTxt[20];
		sprintf(volTxt, "Vol %i", i);
		p_staticUI->p_currentMenu->addMenuItem(setSoundSettings, volTxt);
	}
	p_staticUI->p_currentMenu->addMenuItem(mainScreenMenuItem, "Back");
}

void UI::setSoundSettings(int val)
{
	Log logAction("Sound volume was set");
	PersistentData::setVolume(val * 10);
	mainScreenMenuItem(NULL);
}
#pragma endregion

void UI::prepareNewMenuEntry()
{
	p_staticUI->m_menuTimeOut.now();
	p_staticUI->m_menuTimeOut.addSeconds(DEFAULT_SCREEN_TIMEOUT);
	if (p_staticUI->p_currentMenu) {
		delete p_staticUI->p_currentMenu;
		p_staticUI->p_currentMenu = nullptr;
	}
}

void UI::setManualControlMenuItem(int val)
{
	Log logAction("Access Manual Controls");
}

void UI::processUserInterface()
{
	if (p_staticUI->p_currentMenu) 
	{
		if (!p_staticUI->p_currentMenu->IsMainMenu())
		{
			DateTime now = DateTime(true);
			//Serial.print("now:"); Serial.println(now.cTime());
			//Serial.print("mtu:"); Serial.println(p_staticUI->m_menuTimeOut.cTime());
			if (p_staticUI->m_menuTimeOut <= now)
				backToMainMenu(NULL);
		}
		p_joy->setMinMax(0, p_staticUI->p_currentMenu->getIndex() - 1);
		p_staticUI->p_currentMenu->doMenu();
	}
}



