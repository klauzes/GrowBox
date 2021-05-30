/****************************************************************************
*  PROIECT LICENTA:     -SISTEM ELECTRONIC PENTRU AUTOMATIZAREA UNEI SERE-  *
*  LIMBAJ:              C++                                                 *
*  MICROPROCESOR        ATmega2560                                          *
*  STUDENT:             ISPAS N.O. CLAUDIU-OCTAVIAN                         *
*  GRUPA:               307                                                 *
*  PROMOTIA             2018-2021                                           *
*  FACULTATEA DE INFORMATICA                                                *
*  UNIVERSITATEA TITU MAIORESCU                                             *
* ***************************************************************************/
#pragma once
#include <U8glib.h>
#include "Beeper.h"
#include "Joystick.h"
#include "Pins.h"
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