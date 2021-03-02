#include "MessageBox.h"


MessageBox::MessageBox(Joystick* joy, U8GLIB_ST7920_128X64_4X* disp, const char* title, const char* message1, const char* message2)
{
	p_display = disp;
	p_joy = joy;
	p_joy->setMinMax(0, 0);
	p_joy->reset();
	m_title = title;
	m_message1 = message1;
	m_message2 = message2;
	doesNotTimeOut = true;
}


void MessageBox::doMenu()
{
	p_display->firstPage();
	
	do {
		p_display->setFont(u8g_font_9x18Br);
		p_display->drawBox(0, 0, MAXWIDTH, p_display->getFontAscent()+2);
		p_display->drawBox(0, MAXHEIGHT - p_display->getFontAscent()-2, MAXWIDTH, p_display->getFontAscent()+2);
		p_display->setColorIndex(0);
		p_display->drawStr(0, p_display->getFontAscent(), m_title);
		p_display->drawStr((MAXWIDTH/2)-14, MAXHEIGHT, "OK");
		p_display->setFont(u8g_font_5x7);
		p_display->setColorIndex(1);
		p_display->drawStr(0, MAXHEIGHT / 2, m_message1);
		p_display->drawStr(0, (MAXHEIGHT / 2) + p_display->getFontAscent()+2, m_message2);
	} while (p_display->nextPage());

	if (p_joy->wasButtonPressed() && p_menuItems != nullptr)
		(*p_menuItems[0].menuFunctionCall)(2);
}
