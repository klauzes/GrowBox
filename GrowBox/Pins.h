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

// Definirea a pinilor ce conecteaza dizpozitivele din sera cu Arduino.
#define LCD_EN				6
#define LCD_RW				5
#define LCD_RS				4
#define DHT_21				12
#define JOY_BT				19
#define JOY_LH				2	
#define JOY_RH				3	
#define BEEPER				11
#define SD_CS				53
#define SD_MOSI				51
#define SD_SCK				52
#define SD_MISO				50
#define PARTICLE			A14
#define PARTICLE_LED		9
#define WATER_LVL			A13
#define WATER_PUMP			10
#define RELAY_LIGHT			48
#define RELAY_FAN			46
#define RELAY_HEATER		44
#define SOIL_HUMIDITY		A15

// Parametrii impliciti de functionare.
#define DEFAULT_ENCODER_SENSITIVITY		2
#define DEFAULT_SCREEN_TIMEOUT			30
#define DEFAULT_LOG_WRITE_INTERVAL		5000
#define DEFAULT_MAIN_SCREEN_NEXT_PAGE	5000

#define DEFAULT_SOIL_HUMIDITY_CHECK_INTERVAL	1800000
#define DEFAULT_WATER_PUMP_ON_TIME				60000


// Valori minimale si maximale ce sunt asteptate de la senzori.
#define MAX_SCREEN_WIDTH		128
#define MAX_SCREEN_HEIGHT		64
#define MAX_ADC_SOIL_HUMIDITY	1023
#define MIN_ADC_SOIL_HUMIDITY	923
#define MIN_ADC_WATER_LEVEL		200
#define MAX_ADC_WATER_LEVEL		1023
#define MAX_SUNLIGHT_HOURS		16
#define MAX_SOIL_HUMIDITY		60
#define MIN_SOIL_HUMIDITY		15
#define MIN_AIR_TEMP			20
#define MAX_AIR_TEMP			32
#define MIN_AIR_HUMIDITY		25
#define MAX_AIR_HUMIDITY		65
#define MAX_MENU_ENTRIES		100



