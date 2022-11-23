#ifndef DATE_H
#define DATE_H

#include <ctime>
using namespace std;

class Date
{
    public:

        int Year;

        int Mounth;

        int Day;

        Date(int Day, int Mounth , int Year);

        Date();

        bool operator<(Date other);

        bool operator>(Date other);

        void ShowDate();
};
Date GetCurentDate();
#endif // DATE_H
