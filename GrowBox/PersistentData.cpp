#include "PersistentData.h"
#include <EEPROM.h>

void PersistentData::setVolume(int vol)
{
	EEPROM.write(speakerVolumeAddress, vol);
}

void PersistentData::getVolume(int& vol)
{
	vol = EEPROM.read(speakerVolumeAddress);
}
