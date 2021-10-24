#ifndef _C_DATE_H_
#define _C_DATE_H_
#include <string>
#include <time.h>
class Date
{
public:
  explicit Date(int year, int month, int day)  //构造
  :_year(year)
  , _month(month)
  , _day(day)
  {
    if (!isInvalidDate(_year, _month, _day))
    {
      _year = 1900;
      _month = 1;
      _day = 1;
    }
  }
  explicit Date()
  {

    // 基于当前系统的当前日期/时间
    time_t now = time(0);
   
    tm *gmtm = gmtime(&now);
    this->_year = gmtm->tm_year;
    this->_month = gmtm->tm_mon;
    this->_day = gmtm->tm_mday;
  }
  std::string ToString()
  {
      char buff[16]={0};
      sprintf(buff,"%4d-%02d-%02d",_year+1900,_month+1,_day);
      return std::string(buff);
  }
  Date operator+(int count)
  { 
    Date tmp(*this);
    tmp._day += count;
    ToCorrect(tmp);
    return tmp;
  }
private:
  
  
  Date operator-(int count)
  {
    Date tmp(*this);
    tmp._day -= count;
    ToCorrect(tmp);
    return tmp;
  }
  
  Date& operator++()  //前置++
  {
    (*this)++;
    return *this;
  }
  Date operator++(int)  //后置++
  {
    Date tmp(*this);
    (*this)+=1;
    return tmp;
  }
  Date& operator--()
  {
    (*this)--;
    return *this;
  }
  Date operator--(int)
  {
    Date tmp(*this);
    (*this)--;
    return tmp;
  }
  int operator-(const Date &d)
  {
    int flag = 1;
    Date max = *this;
    Date min = d;
    if (*this<d)
    {
      max = d;
      min = *this;
      flag = -1;
    }
    int count=0;
    while (min != max)
    {
      ++min;
      count++;
    }
    return count*flag;
  }
  Date& operator+=(int day)
  {
    *this = *this + day;
    return *this;
  }
  bool operator!=(const Date &d)
  {
    return !(*this == d);
  }
  bool operator<(const Date &d)
  {
    return !((*this>d)||(*this==d));
  }
  bool operator>=(const Date &d)
  {
    return !(*this<d);
  }
  bool operator>(const Date &d)
  {
    return (_year > d._year
      || (_year == d._year && _month > d._month)
      || (_year == d._year && _month == d._month && _day > d._day));
  }
  bool operator==(const Date &d)
  {
    return ((_year == d._year) && (_month == d._month) && (_day == d._day));
  }
  
public:  
  bool IsLeapYear(int year)
  {
    if(year % 400 || (year % 4 && year % 100))
      return true;
    return false;
  }
  int YearsOfMonth(int year, int month)
  {
    int day;
    int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    day = days[month];
    if (month == 2)
      day += IsLeapYear(year);
    return day;
  }
  
  Date ToCorrect(Date &d)
  {
    if (d._day>YearsOfMonth(d._year, d._month))
    {
      while (d._day > YearsOfMonth(d._year, d._month))
      {
         d._day -= YearsOfMonth(d._year,d._month);

        if (d._month == 12)
        {
          d._year++;
          d._month = 1;
        }
        else
        {
          ++d._month;
        }
      }
    }
    else
    {
      while (d._day <= 0)
      {
        if (d._month == 1)
        {
          d._year--;
          d._month = 12;
        }
        else
        {
          --d._month;
        }
        d._day += YearsOfMonth(d._year, d._month);
      }
    }
    return d;
  }
  

  bool isInvalidDate(int year, int month, int day)
  {
    if ((year < 1) || (month<0 || month>12) || (day<0 || day>YearsOfMonth(year,month)))
      return false;
    return true;
  }
private:
  int _year;
  int _month;
  int _day;
};

#endif