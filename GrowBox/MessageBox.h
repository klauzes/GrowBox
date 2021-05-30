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

class MessageBox : public Menu
{
public:	
	MessageBox(U8GLIB_ST7920_128X64_4X* disp,const char* title, const char* message1,const char* message2);
	void doMenu() override;
private:
	const char* m_title;
	const char* m_message1;
	const char* m_message2;
};