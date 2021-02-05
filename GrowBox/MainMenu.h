#pragma once
#include "Menu.h"

class MainMenu final: public Menu
{
public:
	MainMenu(Joystick* joy, U8GLIB_ST7920_128X64_1X* disp);
	void doMenu() override;	

};