#include "MainMenu.h"
#include "DateTime.h"

MainMenu::MainMenu(Joystick* joy, U8GLIB_ST7920_128X64_1X* disp)
{
	p_display = disp;
	p_joy = joy;
	p_joy->setMinMax(0, 0);
	p_joy->reset();
	setFont(u8g_font_9x18);//default font
	isMainMenu = true;
}

void MainMenu::doMenu()
{
	p_display->firstPage();
	DateTime curTime(true);
	char cTime[9]; curTime.cTime(cTime);
	char cDate[12]; curTime.cDate(cDate);
	do {
		p_display->drawStr(0, p_display->getFontLineSpacing()*2, cTime);
		p_display->drawStr(0, p_display->getFontLineSpacing()*3, cDate);
	} while (p_display->nextPage());	
	
	if (p_joy->wasButtonPressed() && p_menuItems != nullptr) {
		Beeper::beepNav();
		(*p_menuItems[0].menuFunctionCall)(2);
	}
}

