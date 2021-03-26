#pragma once
class Joystick
{
public:
	static void constructor(int ticks);
	static void setTicks(int ticks);
	static void getTicks(int& val);
	static bool wasButtonPressed();
	static void setButtonPress();
	static void scan();
	//static void moveLeft();
	//static void moveRight();
	static void getPosition(int& val);
	static void setMinMax(int min, int max);
	static void reset();

};

