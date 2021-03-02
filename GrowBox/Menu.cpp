#include "Menu.h"

Menu::Menu()
{	
	m_menuItemsIndex = 0;	
	p_menuItems = nullptr;
	p_display = nullptr;
	p_joy = nullptr;	
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
	MenuItem newMenuItem;
	newMenuItem.menuFunctionCall = menuFunction;	
	strcpy(newMenuItem.menuFunctionName, menuName);
	addMenuItem(newMenuItem);
}

void Menu::setFont(const u8g_fntpgm_uint8_t* font)
{
	if (p_display)
		p_display->setFont(font);
}

void Menu::addMenuItem(MenuItem mItem)
{
	MenuItem* newItemList = new MenuItem[m_menuItemsIndex + 1];
	newItemList[m_menuItemsIndex] = mItem;

	if (m_menuItemsIndex)
		memcpy(newItemList, p_menuItems, sizeof(MenuItem) * m_menuItemsIndex);

	if (p_menuItems)
		delete p_menuItems;

	p_menuItems = newItemList;
	m_menuItemsIndex++;
}
