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
#include "Menu.h"

class NavigationMenu final:public Menu
{
public:	
	NavigationMenu(U8GLIB_ST7920_128X64_4X* dsp);
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

