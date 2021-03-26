#pragma once
#include "Joystick.h"
#include <U8glib.h>
#include "Pins.h"
#include "Beeper.h"
class UI; 

class Menu
{
public:

	struct MenuItem
	{
		void (*menuFunctionCall)(int x);
		char menuFunctionName[30];
	};

	Menu();
	~Menu();
	virtual void doMenu() = 0;
	virtual void addMenuItem(void (*menuFunction)(int x),const char* menuName);
	int getIndex() { return m_menuItemsIndex; }
	virtual void setFont(const u8g_fntpgm_uint8_t* font);
	bool isExpirable() { return doesNotTimeOut; }

protected:
	MenuItem* p_menuItems;
	U8GLIB_ST7920_128X64_4X* p_display;
	int m_menuItemsIndex;
	bool doesNotTimeOut;
};