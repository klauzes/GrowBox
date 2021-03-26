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