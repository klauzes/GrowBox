#include "UI.h"
static UI* p_staticUI;
#include <Arduino.h>
#include "PersistentData.h"
#include "Log.h"
#include "Hardware.h"
#include "Joystick.h"
#include "Pins.h"


UI::UI(Automation* existing)
{
	p_currentMenu = nullptr;
	p_clockSetter = nullptr;
	p_automationSetter = nullptr;
	p_display = new U8GLIB_ST7920_128X64_4X(LCD_EN, LCD_RW, LCD_RS, 0);
	p_staticUI = this;
	m_menuTimeOut = DateTime();
	if (existing != nullptr)
		p_automationProgram = new Automation(existing);
	else
		p_automationProgram = nullptr;
	createMainMenu();	
}

UI::~UI()
{
	//teoretic niciodata
	if (p_currentMenu)
		delete p_currentMenu;
	if (p_display)
		delete p_display;
	if (p_clockSetter)
		delete p_clockSetter;
	if (p_automationSetter)
		delete p_automationSetter;
}

Automation* UI::getProgram()
{
	return p_automationProgram;
}

void UI::createMainMenu()
{
	//Log logAction("Create main menu screen"); as demo
	prepareNewMenuEntry();	
	p_staticUI->p_currentMenu = new MainMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->addMenuItem(firstMenuItemList, "x");
}

#pragma region MainMenu
void UI::firstMenuItemList(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Clock settings");
	p_staticUI->p_currentMenu->addMenuItem(setSoundSettingsMenuItem, "Sound settings");
	p_staticUI->p_currentMenu->addMenuItem(setPrepareManualControl, "Manual control");
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "Automatic mode");
	p_staticUI->p_currentMenu->addMenuItem(backToMainMenu, "Back");
}

void UI::backToMainMenu(int val)
{
	Hardware::setManualControl(false);
	if (p_staticUI->p_clockSetter != nullptr) 
	{
		delete p_staticUI->p_clockSetter;
		p_staticUI->p_clockSetter = nullptr;
	}
	if (p_staticUI->p_automationSetter != nullptr)
	{
		delete p_staticUI->p_automationSetter;
		p_staticUI->p_automationSetter = nullptr;
	}	
	p_staticUI->createMainMenu();
}
#pragma endregion

#pragma region ClockSettings
void UI::setClockMenuItem(int val)
{
	prepareNewMenuEntry();
	if (p_staticUI->p_clockSetter == nullptr)
		p_staticUI->p_clockSetter = new DateTime();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->addMenuItem(setHourMenuItem, "Set hour");
	p_staticUI->p_currentMenu->addMenuItem(setMinuteMenuItem, "Set minute");
	p_staticUI->p_currentMenu->addMenuItem(setSecondMenuItem, "Set seconds");
	p_staticUI->p_currentMenu->addMenuItem(setYearMenuItem, "Set year");
	p_staticUI->p_currentMenu->addMenuItem(setMonthMenuItem, "Set month");
	p_staticUI->p_currentMenu->addMenuItem(setDateMenuItem, "Set date");
	p_staticUI->p_currentMenu->addMenuItem(setLoadCurrentDateTime, "Load current");
	p_staticUI->p_currentMenu->addMenuItem(saveClockMenuItem, "Save settings");
	p_staticUI->p_currentMenu->addMenuItem(firstMenuItemList, "Back");
}

void UI::setLoadCurrentDateTime(int val)
{
	p_staticUI->p_clockSetter->now();
	Beeper::beepOk();
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "LOADED", "Current values read", "adjust and save!");
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "x");
}

void UI::setHourMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	for (int i = 0; i < 24; i++) {
		char buf[10];
		if (i < 13)
			sprintf(buf,i<10?"0%i AM":"%i AM", i);
		else
			sprintf(buf, "%i PM", i);
		p_staticUI->p_currentMenu->addMenuItem(setHour, buf);
	}
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Back");
}

void UI::setHour(int val)
{
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setHour(val);
	}
	setClockMenuItem(NULL);
}


void UI::setMinuteMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	for (int i = 0; i < 60; i++) {
		char buf[12];		
		if(i<10)
			sprintf(buf, i==1?"0%d minute": "0%d minutes", i);
		else
			sprintf(buf, "%d minutes", i);
		p_staticUI->p_currentMenu->addMenuItem(setMinute, buf);
	}
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Back");
}

void UI::setMinute(int val)
{
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setMinute(val);
	}
	setClockMenuItem(NULL);
}


void UI::setSecondMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	for (int i = 0; i < 60; i++) {
		char buf[14];
		if(i<10)
			sprintf(buf, i==1?"0%i second": "0%i seconds", i);
		else
			sprintf(buf, "%i seconds", i);

		p_staticUI->p_currentMenu->addMenuItem(setSecond, buf);
	}
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Back");
}

void UI::setSecond(int val)
{
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setSecond(val);
	}
	setClockMenuItem(NULL);
}


void UI::setYearMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	for (int i = 2020; i < 2100; i++) {
		char buf[5];		
		sprintf(buf, "%.4i", i);
		p_staticUI->p_currentMenu->addMenuItem(setYear, buf);
	}
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Back");
}

void UI::setYear(int val)
{
	if (p_staticUI->p_clockSetter) {
		p_staticUI->p_clockSetter->setYear(2020+val);		
	}
	setClockMenuItem(NULL);
}


void UI::setMonthMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	for (int i = 1; i < 13; i++) {		
		char monthName[4]; p_staticUI->p_clockSetter->getMonthName(i, monthName);
		p_staticUI->p_currentMenu->addMenuItem(setMonth, monthName);
	}
	p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Back");
}

void UI::setMonth(int val)
{
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
		prepareNewMenuEntry();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display,"ERROR","Please set year","and month first.");
		p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "x");
	}
	else
	{		
		p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
		p_staticUI->p_currentMenu->setFont(u8g_font_7x13);
		int daysInMonth = p_staticUI->p_clockSetter->daysInMonth(p_staticUI->p_clockSetter->mm, p_staticUI->p_clockSetter->yyyy);
		char month[4]; p_staticUI->p_clockSetter->getMonthName(p_staticUI->p_clockSetter->mm, month);
		for (int i = 1; i <= daysInMonth; i++) {
			char date[17];			
			p_staticUI->p_clockSetter->setDate(i);
			char dow[4];  p_staticUI->p_clockSetter->getdayOfWeek(dow);
			char cdate[12];  p_staticUI->p_clockSetter->cDate(cdate);
			sprintf(date, "%.3s, %.11s", dow, cdate);
			p_staticUI->p_currentMenu->addMenuItem(setDate, date);
		}
		p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "Back");
	}
}

void UI::setDate(int val)
{
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
		prepareNewMenuEntry();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "INCOMPLETE", "Please fill all fields", "before saving.");
		p_staticUI->p_currentMenu->addMenuItem(setClockMenuItem, "x");
	}
	else
	{
		p_staticUI->p_clockSetter->save2RTC();
		delete p_staticUI->p_clockSetter;
		p_staticUI->p_clockSetter = nullptr;
		Beeper::beepOk();
		prepareNewMenuEntry();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "SUCCESS", "Real Time Clock", "settings saved.");
		p_staticUI->p_currentMenu->addMenuItem(firstMenuItemList, "x");
	}
}
#pragma endregion

#pragma region SoundSettings
void UI::setSoundSettingsMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	for (int i = 0; i <= 10; i++) {
		char volTxt[7];
		sprintf(volTxt, "Vol %i", i);
		p_staticUI->p_currentMenu->addMenuItem(setSoundSettings, volTxt);
	}
	p_staticUI->p_currentMenu->addMenuItem(firstMenuItemList, "Back");
}

void UI::setSoundSettings(int val)
{
	PersistentData::setVolume(val * 10);
	firstMenuItemList(NULL);
}
#pragma endregion


void UI::setPrepareManualControl(int val)
{
	Hardware::setManualControl(true);//Manual control overrides automation
	Hardware::setDefaultPinModesAndValues();
	setManualControlMenuItem(0);
}

void UI::setManualControlMenuItem(int val)
{
	prepareNewMenuEntry();	
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	char lightStateBuf[11];
	char fanStateBuf[8];
	char heatStateBuf[9];
	char waterStateBuf[10];

	strcpy(lightStateBuf, Hardware::getLightsState()? "Lights OFF":"Lights ON");
	strcpy(fanStateBuf, Hardware::getIntakeFanState()? "Fan OFF":"Fan ON");
	strcpy(heatStateBuf, Hardware::getHeaterState()? "Heat OFF":"Heat ON");
	strcpy(waterStateBuf, Hardware::getWaterPumpState()? "Water OFF":"Water ON");
	
	p_staticUI->p_currentMenu->addMenuItem(manualToggles,lightStateBuf);
	p_staticUI->p_currentMenu->addMenuItem(manualToggles, fanStateBuf);
	p_staticUI->p_currentMenu->addMenuItem(manualToggles, heatStateBuf);	
	p_staticUI->p_currentMenu->addMenuItem(manualToggles, waterStateBuf);

	p_staticUI->p_currentMenu->addMenuItem(manualReadings, "Air Particles");
	p_staticUI->p_currentMenu->addMenuItem(manualReadings, "Air Temp.");
	p_staticUI->p_currentMenu->addMenuItem(manualReadings, "Air Humidity");
	p_staticUI->p_currentMenu->addMenuItem(manualReadings, "Soil Humidity");
	p_staticUI->p_currentMenu->addMenuItem(manualReadings, "Water Level");	

	p_staticUI->p_currentMenu->addMenuItem(backToMainMenu, "Back");
}

void UI::manualToggles(int val)
{
	resetScreenTimeout();
	switch (val)
	{
	case 0://lights
		Hardware::setLights(!Hardware::getLightsState());
		break;
	case 1://fan
		Hardware::setIntakeFan(!Hardware::getIntakeFanState());
		break;
	case 2://heat
		Hardware::setHeater(!Hardware::getHeaterState());
		break;
	case 3://water
		Hardware::setWaterPump(!Hardware::getWaterPumpState());
		break;
	}
	Beeper::beepOk();
	setManualControlMenuItem(NULL);
}

void UI::manualReadings(int val)
{	
	prepareNewMenuEntry();
	Beeper::beepOk();
	switch (val)
	{
		case 4://particle count
			p_staticUI->p_currentMenu = new SensorMenu(p_staticUI->p_display, SensorMenu::Sensor::AirParticles, 0.6, 0);
			p_staticUI->p_currentMenu->addMenuItem(setManualControlMenuItem, "x");
			break;
		case 5://air temp
			p_staticUI->p_currentMenu = new SensorMenu(p_staticUI->p_display, SensorMenu::Sensor::AirTemp, 50, 0);
			p_staticUI->p_currentMenu->addMenuItem(setManualControlMenuItem, "x");
			break;
		case 6://air humidity
			p_staticUI->p_currentMenu = new SensorMenu(p_staticUI->p_display, SensorMenu::Sensor::AirHumidity, 100, 0);
			p_staticUI->p_currentMenu->addMenuItem(setManualControlMenuItem, "x");
			break;
		case 7://soil humidity
			p_staticUI->p_currentMenu = new SensorMenu(p_staticUI->p_display, SensorMenu::Sensor::SoilHumidity, 100, 0);
			p_staticUI->p_currentMenu->addMenuItem(setManualControlMenuItem, "x");
			break;
		case 8://water level
			p_staticUI->p_currentMenu = new SensorMenu(p_staticUI->p_display, SensorMenu::Sensor::WaterLevel, 100, 0);
			p_staticUI->p_currentMenu->addMenuItem(setManualControlMenuItem, "x");
			break;
	}

}

void UI::setAutomaticMode(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	if(p_staticUI->p_automationSetter == nullptr)
		p_staticUI->p_automationSetter = new Automation(false);
	p_staticUI->p_currentMenu->addMenuItem(setSunriseHourMenuItem, "Sunrise Hour");
	p_staticUI->p_currentMenu->addMenuItem(setSunlightHoursMenuItem, "Sunlight Time");
	p_staticUI->p_currentMenu->addMenuItem(setAirTemperatureMenuItem, "Air Temp");
	p_staticUI->p_currentMenu->addMenuItem(setAirMaxHumidityMenuItem, "Air Max Humid");
	p_staticUI->p_currentMenu->addMenuItem(setSoilHumidityMenuItem, "Soil Humidity");
	p_staticUI->p_currentMenu->addMenuItem(setLoadConfigMenuItem, "Load config");
	p_staticUI->p_currentMenu->addMenuItem(setSaveConfigurationMenuItem, "Save config");
	p_staticUI->p_currentMenu->addMenuItem(setDeleteConfigurationMenuItem, "DELETE config");

	p_staticUI->p_currentMenu->addMenuItem(firstMenuItemList, "Back");
}

void UI::setSunriseHourMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->setFont(u8g_font_7x13);		
	for (int i = 0; i < 24; i++) {
		char fHour[6];
		sprintf(fHour, i<10?"0%d:00":"%d:00", i);
		p_staticUI->p_currentMenu->addMenuItem(setSunriseHour, fHour);
	}
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "Back");
}

void UI::setSunriseHour(int val)
{
	p_staticUI->p_automationSetter->setSunriseHour(val);
	setAutomaticMode(NULL);
}

void UI::setSunlightHoursMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->setFont(u8g_font_7x13);
	for (int i = 1; i < MAX_SUNLIGHT_HOURS; i++) {
		char fDlightHours[6];
		sprintf(fDlightHours, i < 10 ? "0%d:00" : "%d:00", i);
		p_staticUI->p_currentMenu->addMenuItem(setSunlightHours, fDlightHours);
	}
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "Back");
}

void UI::setSunlightHours(int val)
{	
	p_staticUI->p_automationSetter->setSunlightHours(val + 1);
	setAutomaticMode(NULL);
}

void UI::setAirTemperatureMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->setFont(u8g_font_7x13);
	for (int i = MIN_AIR_TEMP; i < MAX_AIR_TEMP; i++) {
		char fAirTemp[11];
		sprintf(fAirTemp, "%d Celsius", i);
		p_staticUI->p_currentMenu->addMenuItem(setAirTemperature, fAirTemp);
	}
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "Back");
}

void UI::setAirTemperature(int val)
{
	p_staticUI->p_automationSetter->setTemperature(MIN_AIR_TEMP + val);
	setAutomaticMode(NULL);
}

void UI::setAirMaxHumidityMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->setFont(u8g_font_7x13);
	for (int i = MIN_AIR_HUMIDITY; i < MAX_AIR_HUMIDITY; i++) {
		char fAirTemp[5];
		sprintf(fAirTemp, "%d %", i);
		p_staticUI->p_currentMenu->addMenuItem(setAirMax, fAirTemp);
	}
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "Back");
}

void UI::setAirMax(int val)
{
	p_staticUI->p_automationSetter->setMaximuAirHumidity(MIN_AIR_HUMIDITY + val);
	setAutomaticMode(NULL);
}

void UI::setSoilHumidityMenuItem(int val)
{
	prepareNewMenuEntry();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->setFont(u8g_font_7x13);
	for (int i = MIN_SOIL_HUMIDITY; i < MAX_SOIL_HUMIDITY; i++) {
		char fAirTemp[5];
		sprintf(fAirTemp, "%d %", i);
		p_staticUI->p_currentMenu->addMenuItem(setSoilHumidity, fAirTemp);
	}
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "Back");
}

void UI::setSoilHumidity(int val)
{
	p_staticUI->p_automationSetter->setIdealSoilHumidity(MIN_SOIL_HUMIDITY + val);
	setAutomaticMode(NULL);
}

void UI::setLoadConfigMenuItem(int val)
{
	prepareNewMenuEntry();
	if (p_staticUI->p_automationProgram == nullptr)
	{
		Beeper::beepNOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "ERROR", "There is no existing", "configuration to load!");
	}
	else
	{		
		delete p_staticUI->p_automationSetter;
		p_staticUI->p_automationSetter == nullptr;	
		p_staticUI->p_automationSetter  =new Automation(p_staticUI->p_automationProgram);			
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "SUCCES", "Config loaded.Modify", "and then save!");
		Beeper::beepOk();
	}
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "x");
}

void UI::setSaveConfigurationMenuItem(int val)
{
	prepareNewMenuEntry();
	if (!p_staticUI->p_automationSetter->isValid())
	{
		Beeper::beepNOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "ERROR", "Configuration incomplete", "please set all fields!");
		p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "x");
	}
	else
	{
		p_staticUI->p_automationSetter->saveToPersistent();
		if (p_staticUI->p_automationProgram == nullptr)		
			p_staticUI->p_automationProgram = new Automation(p_staticUI->p_automationSetter);
		else			
			p_staticUI->p_automationProgram->loadPersistent();
		Beeper::beepOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "SUCCES", "Configuration saved", "and apllied!");
		p_staticUI->p_currentMenu->addMenuItem(firstMenuItemList, "x");
	}
}

void UI::setDeleteConfigurationMenuItem(int val)
{
	prepareNewMenuEntry();
	Beeper::beepNOk(); Beeper::beepNOk(); Beeper::beepNOk();
	p_staticUI->p_currentMenu = new NavigationMenu(p_staticUI->p_display);
	p_staticUI->p_currentMenu->addMenuItem(setDeleteConfiguration, "DELETE");
	p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "Back");
}

void UI::setDeleteConfiguration(int val)
{
	prepareNewMenuEntry();
	if (p_staticUI->p_automationProgram != nullptr && p_staticUI->p_automationProgram->isValid())
	{
		Beeper::beepOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "SUCCES", "Configuration deleted", "---");
		p_staticUI->p_automationProgram->deletePersistent();
		p_staticUI->p_automationProgram->loadPersistent();
		delete p_staticUI->p_automationProgram;
		p_staticUI->p_automationProgram == nullptr;
		p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "x");
	}
	else
	{
		Beeper::beepNOk();
		p_staticUI->p_currentMenu = new MessageBox(p_staticUI->p_display, "ERROR", "Nothing to delete", "!!!");
		p_staticUI->p_currentMenu->addMenuItem(setAutomaticMode, "x");
	}
}

void UI::processUserInterface()
{
	if (p_staticUI->p_currentMenu) 
	{
		if (!p_staticUI->p_currentMenu->isExpirable())
		{
			DateTime now = DateTime(true);
			if (p_staticUI->m_menuTimeOut <= now)
				backToMainMenu(NULL);
		}
		Joystick::setMinMax(0, p_staticUI->p_currentMenu->getIndex() - 1);
		p_staticUI->p_currentMenu->doMenu();
	}
}

void UI::prepareNewMenuEntry()
{
	resetScreenTimeout();
	if (p_staticUI->p_currentMenu) {
		delete p_staticUI->p_currentMenu;
		p_staticUI->p_currentMenu = nullptr;
	}
}

void UI::resetScreenTimeout()
{
	p_staticUI->m_menuTimeOut.now();
	p_staticUI->m_menuTimeOut.addSeconds(DEFAULT_SCREEN_TIMEOUT);
}

