#include <Arduino.h>

#include "Beeper.h"
#include "Menu.h"

/*
* Clasa de baza din care sunt derivata toate tipurile de meniu existente in aplicatie.
* Ofera acces la o serie de metode precum addMenuItem si setFont, insa are si o metoda pur virtuala 
* Sarcina implementarii acesteia ramanand pentru toate derivatele sale
* 	virtual void doMenu() = 0; -> aceasta metoda obligatorie reprezinta practiv activitatea specifica derulata de fiecare meniu
*/


// Constructor
// Aloca elemente MenuItem intr-un array de marime maximala definita de MAX_MENU_ENTRIES 
Menu::Menu()
{	
	m_menuItemsIndex = 0;	
	p_menuItems = new MenuItem[MAX_MENU_ENTRIES];
	p_display = nullptr;
	doesNotTimeOut = false;
}

// Distructor
Menu::~Menu()
{	
	if (p_menuItems!=nullptr)
	{	
		delete[] p_menuItems;
		p_menuItems = nullptr;		
	}
}

// Adauga intrari noi in meniu atat timp cat exista loc in memorie
// In caz contrat ,cu scopul de debug, produce un ton de eroare cu o cadenta de 1 secunda
void Menu::addMenuItem(void(*menuFunction)(int x),const char* menuName)
{
	if (m_menuItemsIndex < MAX_MENU_ENTRIES)
	{
		p_menuItems[m_menuItemsIndex].menuFunctionCall = menuFunction;
		strcpy(p_menuItems[m_menuItemsIndex++].menuFunctionName, menuName);
	}
	else
	{
		for (;;)
		{
			Beeper::beepNOk(); delay(1000);
		}
	}

}

void Menu::setFont(const u8g_fntpgm_uint8_t* font)
{
	if (p_display)
		p_display->setFont(font);
}


