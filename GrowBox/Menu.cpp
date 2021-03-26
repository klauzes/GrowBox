#include "Menu.h"
#include <Arduino.h>

Menu::Menu()
{	
	m_menuItemsIndex = 0;	
	p_menuItems = new MenuItem[50];
	p_display = nullptr;
	doesNotTimeOut = false;
}

Menu::~Menu()
{	
	if (p_menuItems)
	{	
		delete[] p_menuItems;
		p_menuItems = nullptr;		
	}
}


void Menu::addMenuItem(void(*menuFunction)(int x),const char* menuName)
{
	p_menuItems[m_menuItemsIndex].menuFunctionCall = menuFunction;
	strcpy(p_menuItems[m_menuItemsIndex++].menuFunctionName, menuName);
}

void Menu::setFont(const u8g_fntpgm_uint8_t* font)
{
	if (p_display)
		p_display->setFont(font);
}


