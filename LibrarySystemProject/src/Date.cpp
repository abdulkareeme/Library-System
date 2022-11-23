#include "Date.h"
#include <iostream>

Date::Date(int Day, int Mounth , int Year)
{
    this->Day=Day;
    this->Mounth=Mounth;
    this->Year=Year;
}
Date::Date(){}
bool Date::operator<(Date other)
{
    if(this->Year < other.Year)return true;
    if(this->Year==other.Year && this->Mounth < other.Mounth)return true;
    if(this->Year==other.Year && this->Mounth == other.Mounth && this->Day < other.Day)return true;
    return false;
}
bool Date::operator>(Date other)
{
    if(this->Year > other.Year)return true;
    if(this->Year==other.Year && this->Mounth > other.Mounth)return true;
    if(this->Year==other.Year && this->Mounth == other.Mounth && this->Day > other.Day)return true;
    return false;
}
Date GetCurentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int Year=1900 + ltm->tm_year;
    int Mounth=1 + ltm->tm_mon;
    int Day=ltm->tm_mday;
    Date Today=Date(Day,Mounth,Year);
    return Today;
}
void Date::ShowDate()
{
    cout<<Day<<'/'<<Mounth<<'/'<<Year;
}
