/****************************************************************************
*  PROIECT LICENTA:     -SISTEM ELECTRONIC PENTRU AUTOMATIZAREA UNEI SERE-  *
*  LIMBAJ:              C++                                                 *
*  MICROPROCESOR        ATmega2560                                          *
*  STUDENT:             ISPAS N.O. CLAUDIU-OCTAVIAN                         *
*  GRUPA:               307                                                 *
*  PROMOTIA             2018-2021                                           *
*  FACULTATEA DE INFORMATICA                                                *
*  UNIVERSITATEA TITU MAIORESCU                                             *
* ***************************************************************************/
#pragma once
class Hardware final
{
public:	
	static double getTemperature();
	static double getHumidity();
	static double getWaterLevel(); 
	static double getSoilHumidity();
	static double getParticleCount();

	static void setWaterPump(bool state);
	static void setIntakeFan(bool state);	
	static void setHeater(bool state);
	static void setLights(bool state);
	static void setManualControl(bool state);

	static bool getWaterPumpState();
	static bool getIntakeFanState();
	static bool getHeaterState();
	static bool getLightsState();
	static bool getManualControl();

	static void setDefaultPinModesAndValues();

};