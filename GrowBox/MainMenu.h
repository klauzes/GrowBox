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

class MainMenu final: public Menu
{
public:
	MainMenu(U8GLIB_ST7920_128X64_4X* disp);
	void doMenu() override;	

private:
	int getHorizontalAllignement(int fontSpacing,int numChars);
	unsigned long m_nextScreenTime = 0;
	bool m_screenOneOrTwo;
};