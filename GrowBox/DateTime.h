#pragma once
#include <MD_DS1307.h>
class DateTime
{

public:
	DateTime(bool _now = false);
	//DateTime(bool now);
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
	const char* formatedHour() { return formatTime(h); };
	const char* formatedMinute() { return formatTime(m); };
	const char* formatedSecond() { return formatTime(s); };
	const char* getdayOfWeek() { return formatDate(dow, 0); };
	const char* formatedDate() { return formatTime(d); };
	const char* month() { return formatDate(0, mm); };
	const char* cTime();
	const char* cDate();
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
	const char* getMonthName(const int _m);
	const char* getWeekdayName(const int _dow);
	int daysInMonth(int month, int year);

private:
	MD_DS1307 rtcInstance;
	const char* formatTime(int hms);
	const char* formatDate(int dd,int mm);
	bool isLeapYear(int yyyy);
	

};