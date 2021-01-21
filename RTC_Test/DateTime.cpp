#include "DateTime.h"
#include <Arduino.h>

DateTime::DateTime()
{
	if (!rtcInstance.isRunning())
		rtcInstance.control(0, DS1307_ON);//also check if 12h day and set to 24h day
	h = m = s = 0;
	d = mm = yyyy = dow = 1;
}

DateTime::DateTime(int _h, int _m, int _s, int _d, int _mm, int _yyyy, int _dow):DateTime()
{
	h = _h;
	m = _m;
	s = _s;
	d = _d;
	mm = _mm;
	yyyy = _yyyy;
	
	if(_dow>0)
		dow = _dow;
	dow = rtcInstance.calcDoW(yyyy, mm, d);
}

DateTime DateTime::operator-(const DateTime take)
{
	DateTime reply = DateTime(take.h-h,take.m-m,take.s-s,take.d-d,take.mm-mm,take.yyyy-yyyy);	
	return reply;
}

const char* DateTime::cTime()
{
	char ret[33];
	sprintf(ret, "%.2s:%.2s:%.2s", formatedHour(), formatedMinute(), formatedSecond());
	return ret;
}

const char* DateTime::cDate()
{
	char ret[40];
	sprintf(ret, "%.2s-%.3s-%i", formatedDate(), month(), yyyy);
	return ret;
}

void DateTime::save2RTC()
{
	rtcInstance.h = h;
	rtcInstance.m = m;
	rtcInstance.s = s;
	rtcInstance.dow = dow;
	rtcInstance.dd = d;
	rtcInstance.mm = mm;
	rtcInstance.yyyy = yyyy;
	rtcInstance.writeTime();
}

void DateTime::now()
{
	rtcInstance.readTime();
	h = rtcInstance.h;
	m = rtcInstance.m;
	s = rtcInstance.s;
	d = rtcInstance.dd;
	mm = rtcInstance.mm;
	yyyy = rtcInstance.yyyy;
	dow = rtcInstance.dow;
}

void DateTime::addHours(const int _h)
{
	int i=_h;
	do
	{
		if (i > 0)
		{
			i--;
			if (h < 23)
			{
				h++;
			}
			else
			{
				h = 0;
				addDay(1);
			}
		}
		else if (i < 0)
		{
			i++;
			if (h > 0)
			{
				h--;
			}
			else
			{
				h = 23;
				addDay(-1);
			}
		}

		
		
	} while (i != 0);
}

void DateTime::addMinutes(const int _m)
{
	int i = _m;
	do
	{
		if (i > 0)
		{
			i--;
			if (m < 59)
			{
				m++;
			}
			else
			{
				m = 0;
				addHours(1);
			}
		}
		else if (i < 0)
		{
			i++;
			if (m > 0)
			{
				m--;
			}
			else
			{
				m = 59;
				addHours(-1);
			}
		}

		

	} while (i != 0);	
}

void DateTime::addSeconds(const int _s)
{
	int i = _s;
	do
	{
		if (i > 0)
		{
			i--;

			if (s < 59)
			{
				s++;
			}
			else
			{
				s = 0;
				addMinutes(1);
			}
		}
		else if (i < 0)
		{
			i++;

			if (s > 0)
			{
				s--;
			}
			else
			{
				s = 59;
				addMinutes(-1);
			}
		}

	} while (i != 0);	
}

void DateTime::addDay(const int _d)
{
	int i = _d;
	do
	{
		if (i > 0)
		{
			i--;
			if (d < daysInMonth(mm, yyyy))
			{
				d++;
			}
			else
			{
				if (mm < 12)
				{
					mm++;
					d = 1;
				}
				else
				{
					mm = d = 1;
					yyyy++;
				}
			}
		}
		else if (i < 0)
		{
			i++;
			if (d > 1)
			{
				d--;
			}
			else
			{
				if (mm > 1)
				{
					mm--;
					d = daysInMonth(mm, yyyy);
				}
				else
				{
					mm = 12;
					d = 31;
					yyyy--;
				}
			}
		}		

	} while (i != 0);	
}

const char* DateTime::formatTime(int hms)
{
	char ret[3];	
	sprintf(ret, hms<10?"0%i":"%i", hms);
	return ret;
}

const char* DateTime::formatDate(int dd, int mm)
{	
	const static char* weekdays[] = { "---", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	const static char* months[] = { "---", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul","Aug","Sep","Oct","Nov","Dec" };
	if (dd)
		return weekdays[dd];
	else
		return months[mm];
}

bool DateTime::isLeapYear(int yyyy)
{
	if ((yyyy % 4 == 0) && (yyyy % 100 != 0))
	{
		return true;
	}
	else if ((yyyy % 100 == 0) && (yyyy % 400 == 0))
	{
		return true;
	}
	else if (yyyy % 400 == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int DateTime::daysInMonth(int month,int year)
{
	if (month == 2)
	{
		if (isLeapYear(year))
			return 29;
		else
			return 28;
	}	
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		return 31;
	else
		return 30;
}
