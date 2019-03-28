//---------------------------------------------------------------------------

#include <math.h>
#include <iostream>
#include "custom.h"

#include "LA.h"

//---------------------------------------------------------------------------

EarthSolarRotation::EarthSolarRotation() : TModel()
{
    X0.resize(6);
    X0[0] = -2.566123740124270e+7L; //km
    X0[1] = 1.339350231544666e+8L;
    X0[2] = 5.805149372446711e+7L;
    X0[3] = -2.983549561177192e+1L; //km/c
    X0[4] = -4.846747552523134L;
    X0[5] = -2.100585886567924L;
}

EarthSolarRotation::EarthSolarRotation( double t0, double tk, TVector& V ) : EarthSolarRotation()
{
    this->t0 = t0;
    this->t1 = tk;
    this->X0 = TVector(V);
}

EarthSolarRotation::EarthSolarRotation( Date d0, Date dk, Date d, TVector& V ) :
    EarthSolarRotation(toJulianDate(d0), toJulianDate(dk), V)
{
    this->t0 = toJulianDate(d0)*86400;
    this->t1 = toJulianDate(dk)*86400;
    start = d0; finish = dk;
    checkDay = d;
};

double EarthSolarRotation::toJulianDate(Date date)
{
    int a = (14 - date.month)/12,
        M = date.month+12*a - 3,
        Y = date.year+4800 - a,
        JDN = date.day + ((153*M+2)/5) + 365*Y
            +(Y/4) - (Y/100) - (Y/400)-32045;
    return JDN + (date.hour - 12)/24. + (date.minute)/1440. + date.seconds/86400.;
}

void EarthSolarRotation::getRight( const TVector& X, long double t, TVector& Y )
{
    Y.resize(6);
    Y[0] = X[3];
    Y[1] = X[4];
    Y[2] = X[5];
    ro = sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
    Y[3] = -mu*X[0]/pow(ro, 3.);
    Y[4] = -mu*X[1]/pow(ro, 3.);
    Y[5] = -mu*X[2]/pow(ro, 3.);
    if (toJulianDate(checkDay)*86400 >= t) std::cout << "time to test" << std::endl;
}
