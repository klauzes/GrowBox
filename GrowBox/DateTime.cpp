#include "DateTime.h"


/*
* Aceasta clasa este utilizata pentru a efectua calcule ce tin de durate de timp.
* Are si overload pe operatorii logici <,>,<=,>= si = , ceea ce permite determinarea relatie dintre doua instante create la momente diferite in timp.
* Oferi si metode helper si pot intoarce sub forma unui sir C ora si data in diferite tipuri de formatare.
*/

// Constante pentru stringurile formatate
const static char* months[] = { "---", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul","Aug","Sep","Oct","Nov","Dec" };
const static char* weekdays[] = { "---", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

// Constructori, distructori si supraincarcare de operatori
DateTime::DateTime(bool _now = false)
{
	if (!rtcInstance.isRunning())
		rtcInstance.control(0, DS1307_ON);
	h = m = s = mm = yyyy= 0;
	d  = dow = 1;
	if (_now)
		now();
}

DateTime::DateTime(const DateTime& cpy)
{
	h = cpy.h;
	m = cpy.m;
	s = cpy.s;
	d = cpy.d;
	mm = cpy.mm;
	yyyy = cpy.yyyy;
	dow = cpy.dow;
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

bool operator==(const DateTime d1, const DateTime d2)
{
	if (d1.yyyy == d2.yyyy &&
		d1.mm == d2.mm &&
		d1.d == d2.d &&
		d1.h == d2.h &&
		d1.m == d2.m &&
		d1.s == d2.s)
		return true;
	return false;
}

bool operator>(const DateTime d1, const DateTime d2)
{
	if (d1.yyyy > d2.yyyy)
		return true;
	if (d1.yyyy == d2.yyyy && d1.mm > d2.mm)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d > d2.d)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d == d2.d && d1.h > d2.h)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d == d2.d && d1.h == d2.h && d1.m > d2.m)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d == d2.d && d1.h == d2.h && d1.m == d2.m && d1.s > d2.s)
		return true;

	return false;
}

bool operator>=(const DateTime d1, const DateTime d2)
{
	if (d1 == d2 || d1 > d2)
		return true;
	return false;
}

bool operator<(const DateTime d1, const DateTime d2)
{
	if (d1.yyyy < d2.yyyy)
		return true;
	if (d1.yyyy == d2.yyyy && d1.mm < d2.mm)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d < d2.d)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d == d2.d && d1.h < d2.h)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d == d2.d && d1.h == d2.h && d1.m < d2.m)
		return true;

	if (d1.yyyy == d2.yyyy && d1.mm == d2.mm && d1.d == d2.d && d1.h == d2.h && d1.m == d2.m && d1.s < d2.s)
		return true;

	return false;
}

bool operator<=(const DateTime d1, const DateTime d2)
{	
	if (d1 == d2 || d1 < d2)
		return true;
	return false;
}

// Ora curenta formatata in forma OO:MM:SS
void DateTime::cTime(char* val)
{
	char fh[3]; formatedHour(fh);
	char fm[3]; formatedMinute(fm);
	char fs[3]; formatedSecond(fs);
	sprintf(val, "%.2s:%.2s:%.2s", fh, fm, fs);
}

// Data curenta formata in DATA-LUNA-AN
void DateTime::cDate(char* val)
{
	char fd[3]; formatedDate(fd);
	char fmt[4]; month(fmt);
	int x = sprintf(val, "%.2s-%.3s-%.4i", fd, fmt, yyyy);
}

// Formateaza valori numerice cu 0 in fata pentru a pastra o lungime constanta a sirului
void DateTime::formatDecimal(int num, char* val)
{
	sprintf(val, num < 10 ? "0%.1i" : "%.2i", num);
}

// Salveaza valorile membrilor din instanta in dispozitivul hardware RealTimeClock atasat la Arduino
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

// Transcrie valorile membrilor clasei cu cele citite de la modulul RealTimeClock
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

// Adauga ore in instanta curenta tinand cont de posibilitate de overflow (>24h adauga o zi)
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

// Adauga minute cu overflow
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

// Adauga secunde cu overflow
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

//Adauga zile cu overflow
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

// Conversie intre valoare numerica si numele lunii asa cum a fost definita (ex. 4 = Apr)
void DateTime::getMonthName(const int _m,char* val)
{	
	strcpy(val, months[_m]);	
}

// Conversie intre valorea numerica si ziua din saptamana (Ex. 1 = Mon)
void DateTime::getWeekdayName(const int _dow,char* val)
{	
	strcpy(val, weekdays[_dow]);	
}

// Metoda combinata pentru a intoarce fie numele lunii, fie ziua din saptamana sub forma de sir formatat in variabila val
void DateTime::formatDate(int dd, int mm,char* val)
{		
	if (dd)
		return getWeekdayName(dd,val);
	else
		return getMonthName(mm,val);
}

// Determina daca anul primit ca paramantru este an bisect
// Implementra in cod a pseudocodului de aici : https://en.wikipedia.org/wiki/Leap_year#Algorithm
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

// Determina numarul de zile din luna month. Tine cont de an bisec
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

// seteaza data si implicit calculeaza ce zi din saptamana este
void DateTime::setDate(int _d)
{
	d = _d;
	//bonus track
	if (yyyy && mm)
		dow = rtcInstance.calcDoW(yyyy, mm, d);
}

