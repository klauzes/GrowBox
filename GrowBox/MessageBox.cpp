#include "MessageBox.h"
#include  "Joystick.h"


MessageBox::MessageBox(U8GLIB_ST7920_128X64_4X* disp, const char* title, const char* message1, const char* message2)
{
	p_display = disp;
	Joystick::setMinMax(0, 0);
	Joystick::reset();
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
		p_display->drawBox(0, 0, MAX_SCREEN_WIDTH, p_display->getFontAscent()+2);
		p_display->drawBox(0, MAX_SCREEN_HEIGHT - p_display->getFontAscent()-2, MAX_SCREEN_WIDTH, p_display->getFontAscent()+2);
		p_display->setColorIndex(0);
		p_display->drawStr(0, p_display->getFontAscent(), m_title);
		p_display->drawStr((MAX_SCREEN_WIDTH/2)-14, MAX_SCREEN_HEIGHT, "OK");
		p_display->setFont(u8g_font_5x7);
		p_display->setColorIndex(1);
		p_display->drawStr(0, MAX_SCREEN_HEIGHT / 2, m_message1);
		p_display->drawStr(0, (MAX_SCREEN_HEIGHT / 2) + p_display->getFontAscent()+2, m_message2);
	} while (p_display->nextPage());

	if (Joystick::wasButtonPressed() && p_menuItems != nullptr)
		(*p_menuItems[0].menuFunctionCall)(2);
}
