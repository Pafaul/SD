#pragma once//---------------------------------------------------------------------------

#include "model.h"
#include "normalgaussgenerator.h"
#include "measure_rework.h"
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
    long double mu = 398600.436L*pow(1000.L, 3), m = 50.0L, omega = 7.292115E-5L, CxS = 1.4L, Re = 6371.0L*1000.0L, Hp = 140.0L*1000.0L, Ha = 970.0L*1000.0L,
                //наклонение орбиты, ..., эксцентриситет, ...
                i = 42.0L/360.0L*2*pi, u = pi/2.0L, e = sqrt(1.0L - pow(Re+Hp, 2)/pow(Re+Ha, 2)), p = (Re+Ha)*(1-e*e), noise = 0.0;
    //atmosphere parameters
    static long double atmosParam[8][4]; long double atmosRoDeviation;
    //Earth rotation Vector, Speed, AeroSpeed, start speed for ellipsoid trajectory, temporary vector
    TVector EarthRotation, V, Va, Ve, temp;
    //rotation matrix
    TMatrix A, startRotation;
    //distance to center of Earth, distance to Earth surface
    long double dist = 0.0L, h = 0.0L;
    //генератор случайных чисел
    NormalGaussGenerator* generator;
    //упал ли ИСЗ
    bool dropped = false;
    int n = 0;

    //проведение измерений
    Measure_Rework* measure;
public:
    ArtificialSatellite();
    void getRight( const TVector& X, long double t, TVector& Y );
    long double ro(long double distance, long double rand );
    bool run ( const TVector& X, long double t );
    void do_thing( const TVector& X, long double t);
    void finalize();
};
