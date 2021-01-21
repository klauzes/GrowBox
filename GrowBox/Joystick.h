#pragma once

class Joystick
{
public:
	Joystick();
	Joystick(int ticks);
	void setTicks(int ticks);
	void getTicks(int& val)  { val = m_Ticks; }
	bool wasButtonPressed();
	void setButtonPress();
	void scan();
	void getPosition(int& val) { val = m_LastCountedTicks; }
	void setMinMax(int min, int max) { m_Min = min; m_Max = max; }
	void reset();

private:
	int m_EncoderTicks;
	int m_LastState;
	int m_LastCountedTicks;
	int m_Ticks;
	int m_Min;
	int m_Max;
};

