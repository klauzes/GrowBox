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
#include <MD_DS1307.h>
class DateTime
{

public:
	DateTime(bool _now = false);
	DateTime(const DateTime& cpy);
	DateTime(int _h, int _m, int _s, int _d, int _mm, int _yyyy, int _dow = -1);	
	friend bool operator==(const DateTime d1, const DateTime d2);
	friend bool operator>(const DateTime d1, const DateTime d2);
	friend bool operator>=(const DateTime d1, const DateTime d2);
	friend bool operator<(const DateTime d1,const DateTime d2);
	friend bool operator<=(const DateTime d1,const DateTime d2);


	int h;
	int m;
	int s;	
	int d;
	int mm;
	int yyyy;
	int dow;
	void formatedHour(char* val) { return formatDecimal(h,val); };
	void formatedMinute(char* val) { return formatDecimal(m,val); };
	void formatedSecond(char* val) { return formatDecimal(s,val); };
	void getdayOfWeek(char* val) { return formatDate(dow, 0,val); };
	void formatedDate(char* val) { return formatDecimal(d,val); };
	void formatedMonth(char* val) { return formatDecimal(mm,val); };
	void month(char* val) { return formatDate(0, mm,val); };
	void cTime(char* val);
	void cDate(char* val);
	void setHour(int _h) { h = _h; };
	void setMinute(int _m) { m = _m; };
	void setSecond(int _s) { s = _s; };
	void setDOW(int _dow) { dow = _dow; };
	void setDate(int _d);
	void setMonth(int _mm) { mm = _mm; };
	void setYear(int _yyyy) { yyyy = _yyyy; };
	void save2RTC();
	void now();
	void addHours(const int _h);
	void addMinutes(const int _m);	
	void addSeconds(const int _s);
	void addDay(const int _d);
	void getMonthName(const int _m,char* val);
	void getWeekdayName(const int _dow,char* val);
	int daysInMonth(int month, int year);

private:
	MD_DS1307 rtcInstance;
	void formatDecimal(int num,char* val);
	void formatDate(int dd,int mm,char* val);
	bool isLeapYear(int yyyy);
	
};