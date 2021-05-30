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
constexpr auto speakervalumeAddress = 0;
constexpr auto sunriseHourAddress = 1;
constexpr auto sunlightHoursAddress = 2;
constexpr auto airTemperatureAddress = 3;
constexpr auto maximumAirHumidityAddress = 4;
constexpr auto idealSoilHumidityAddress = 5;



class PersistentData
{
public:
	static void setVolume(int val);
	static void getVolume(int& val);

	static void setSunriseHour(int val);
	static void getSunriseHour(int& val);

	static void setSunlightHours(int val);
	static void getSunlightHours(int& val);

	static void setTemperature(int val);
	static void getTemperature(int& val);

	static void setMaximumAirHumidity(int val);
	static void getMaximumAirHumidity(int& val);

	static void setIdealSoilHumidity(int val);
	static void getIdealSoilHumidity(int& val);

};