#pragma once
#include "Menu.h"

class MainMenu final: public Menu
{
public:
	MainMenu(U8GLIB_ST7920_128X64_4X* disp);
	void doMenu() override;	

private:
	int getHorizontalAllignement(int fontSpacing,int numChars);
	unsigned long nextScreenTime = 0;
	bool screenOneOrTwo;
};