#pragma once//---------------------------------------------------------------------------

#include "model.h"
#include <tuple>

//---------------------------------------------------------------------------

struct Date
{
    int year, month, day;
    int hour, minute;
    double seconds;
    bool operator <(const Date& x)
    {
        return std::tie(year, month, day, hour, minute, seconds) < std::tie(x.year, x.month, x.day, x.hour, x.minute, x.seconds);
    }
};

//---------------------------------------------------------------------------
class EarthSolarRotation : public TModel
//Earth rotation around Solar, rough calculation
{
protected:
    long double mu = 132712.43994e+6; //km^3/c^2
    long double ro = 1.0;
    Date start, finish, checkDay;
public:
    EarthSolarRotation();
    EarthSolarRotation( double t0, double tk, TVector& V );
    EarthSolarRotation( Date d0, Date dk, Date d, TVector& V);
    double toJulianDate(Date date);
    void getRight( const TVector& X, long double t, TVector& Y );
};
