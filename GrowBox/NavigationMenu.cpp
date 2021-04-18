#include "NavigationMenu.h"
#include "Arduino.h"
#include "Joystick.h"

NavigationMenu::NavigationMenu(U8GLIB_ST7920_128X64_4X* dsp )
{
	p_display = dsp;	
	Joystick::reset();

	m_menuStart = m_menuEnd = m_menuItemsIndex = 0;
	m_currentPage = -1;
	setFont(u8g_font_9x18);	
}

void NavigationMenu::doMenu()
{	
	Joystick::getPosition(m_selectedMenuItem);	

	if (m_selectedMenuItem >= m_menuEnd )	
		m_currentPage++;	
	else if (m_selectedMenuItem < m_menuStart && m_currentPage > 0)	
		m_currentPage--;	

	int currentSelection = m_selectedMenuItem - (m_maxMenuItemsInPage * m_currentPage);
	int maxMenuItemsInNextPage = m_maxMenuItemsInPage * (m_currentPage + 1);

	m_menuStart = m_maxMenuItemsInPage * m_currentPage;
	m_menuEnd = m_menuItemsIndex > maxMenuItemsInNextPage ? maxMenuItemsInNextPage : m_menuItemsIndex;

	p_display->firstPage();
	do {	
		int linePos = m_fontHeight;
		p_display->drawBox(0, currentSelection * m_fontHeight + 1, MAX_SCREEN_WIDTH, m_fontHeight);
		for (int i = m_menuStart; i < m_menuEnd; i++)
		{
			if (currentSelection == i- (m_maxMenuItemsInPage * m_currentPage))
				p_display->setColorIndex(0);						
			p_display->drawStr(0, linePos, p_menuItems[i].menuFunctionName);
			p_display->setColorIndex(1);
			linePos += m_fontHeight;
		}
	} while (p_display->nextPage());

	if (Joystick::wasButtonPressed()) {
		Beeper::beepNav();
		(*p_menuItems[m_selectedMenuItem].menuFunctionCall)(m_selectedMenuItem);
	}
	
}

void NavigationMenu::setFont(const u8g_fntpgm_uint8_t* font)
{
	if (p_display)
		p_display->setFont(font);
	m_fontHeight = p_display->getFontLineSpacing();
	m_maxMenuItemsInPage = MAX_SCREEN_HEIGHT / m_fontHeight;
}

