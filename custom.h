#pragma once//---------------------------------------------------------------------------

#include "model.h"
//#include "normalgaussgenerator.h"
#include <math.h>
#include <tuple>

//---------------------------------------------------------------------------

class ArtificialSatellite : public TModel
{
protected:
    //consts
    long double k = 0.07116714149L, T = 15.91347897L, ksi = 0.01591348171L, pi = 3.14159265359L, time = 0.0L, rand = 0.0L;
    //Normal Random number generator
    //NormalGaussGenerator generator;
    // Grav const, satellite mass, rotation speed, aerofriction const, Earth radius, perigee height, apogee height
    long double mu = 398600.436L, m = 50.0L, omega = 7.292115E-5L, CxS = 1.4L, Re = 6371.0L, Hp = 140.0L, Ha = 970.0L,
                //наклонение орбиты, ..., эксцентриситет, ...
                i = 42.0L/360.0L*2*pi, u = pi/2.0L, e = sqrt(1.0L - pow(Re+Hp, 2)/pow(Re+Ha, 2)), p = (Re+Ha)*(1-e*e);
    //atmosphere parameters
    static long double atmosParam[8][4];
    //Earth rotation Vector, Speed, AeroSpeed, Speed in ellips, temporary vector
    TVector EarthRotation, V, Va, Ve, temp;
    //rotation matrix
    TMatrix A, startRotation;
    //distance to center of Earth, distance to Earth surface
    long double dist = 0.0L, h = 0.0L;

    bool dropped = false;
public:
    ArtificialSatellite();
    void getRight( TVector& X, long double t, TVector& Y );
    long double ro(long double distance, long double rand);
};
