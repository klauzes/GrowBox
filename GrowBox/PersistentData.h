#pragma once
constexpr auto speakerVolumeAddress = 0;

class PersistentData
{
public:
	static void setVolume(int vol);
	static void getVolume(int& vol);

};