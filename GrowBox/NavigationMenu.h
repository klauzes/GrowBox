#pragma once
#include "Menu.h"

class NavigationMenu final:public Menu
{
public:	
	NavigationMenu(Joystick* joy, U8GLIB_ST7920_128X64_1X* dsp);
	void  doMenu() override;	
	void setFont(const u8g_fntpgm_uint8_t* font) override;
private:		
	int m_selectedMenuItem;
	int m_currentPage;	
	int m_maxMenuItemsInPage;
	int m_fontHeight;
	int m_menuStart;
	int m_menuEnd;
};

