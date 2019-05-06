#include <math.h>
#include "measure_rework.h"

Measure_Rework::Measure_Rework()
{
    AES_R.resize(3); AES_Re.resize(3); AES_Se.resize(3);
    NIP_R.resize(3); NIP_Re.resize(3); NIP_Se.resize(3);
    diff_R.resize(3); diff_Re.resize(3); diff_Se.resize(3);
    North_R.resize(3); North_Re.resize(3); North_Se.resize(3);
    North_Direction.resize(3); North_E.resize(3);
    //Result.resize(1, 3);

    North_E[0] = 0; North_E[1] = 0; North_E[2] = 1;

    file.open("measures.txt");
}

void Measure_Rework::calcAES( const TVector &X, long double t )
{
    for ( int i = 0; i < 3; i++ ) AES_R[i] = X[i];
    AES_Re = AES_R*(Re/AES_R.length());
    AES_Se = AES_R - AES_Re;
}

void Measure_Rework::calcNIP( const TVector &X, long double t )
{
    long double calc_longtitude = longtitude + omega*(t - t0);
    NIP_Re[0] = Re*cos(latitude)*cos(calc_longtitude);
    NIP_Re[1] = Re*cos(latitude)*sin(calc_longtitude);
    NIP_Re[2] = Re*sin(latitude);

    NIP_Se = NIP_Re*(AES_Se.length()/Re);

    NIP_R = NIP_Re + NIP_Se;
}

void Measure_Rework::calcDiff( const TVector &X, long double t )
{
    diff_R  = AES_R  - NIP_R; //разница между векторами
    diff_Re = AES_Re - NIP_Re; //от НИПа до проекции спутника на землю
    diff_Se = AES_R - NIP_Re; //От НИПа до спутника
}

void Measure_Rework::calcNorth()
{
    long double alfa_cos = North_E*NIP_Re/(NIP_Re.length()*North_E.length());
    North_R = North_E*(NIP_Re.length()/alfa_cos);
    North_Direction = North_R - NIP_Re; North_Direction = North_Direction*(1/(North_Direction.length()));
}

bool Measure_Rework::measurable( const TVector &X, long double t )
{
    bool flag = false;
    if (t >= prevMeasureTime + 1/frequency)
    {
        //вычисление радиус-векторов
        calcAES( X, t );
        calcNIP( X, t );
        calcNorth();
        calcDiff( X, t );
        //вычисление радиуса окружности измерения НИПа на высоте полёта спутника
        long double measure_R = AES_Se.length() * tan(measureAngle/2.0);
        if (diff_R.length() < measure_R) flag = true;
    }
    return flag;

}

long double Measure_Rework::calcA()
{
    long double cos_azimut = acos(diff_Re*North_Direction/(diff_Re.length()*North_Direction.length()));
    return cos_azimut;

}

long double Measure_Rework::calcE()
{
    long double elevation = acos(diff_Se*diff_Re/(diff_Se.length()*diff_Re.length()));
    return elevation;
}

void Measure_Rework::measure( const TVector &X, long double t, bool main )
{
    if ((measurable( X, t ) && main) || !main)
    {
        //вычисление радиус-векторов
        calcAES( X, t );
        calcNIP( X, t );
        calcNorth();
        calcDiff( X, t );
        //вычисление радиуса окружности измерения НИПа на высоте полёта спутника
        long double measure_R = AES_Se.length() * tan(measureAngle/2.0);
        cur_result.resize(3);
        prevMeasureTime = t;
        calcNorth();
        cur_result[0] = t;
        cur_result[1] = calcA();
        cur_result[2] = calcE();
        /*Result.resize(Result.col_count()+1, 3);
        Result[Result.col_count()-1][0] = t;
        Result[Result.col_count()-1][1] = calcA();
        Result[Result.col_count()-1][2] = calcE();
        /*
        for (int i = 0; i < 3; i++) file << Result[Result.col_count()-1][i] << " ";
        for (int i = 0; i < 3; i++) file << NIP_Se[i] << " ";
        for (int i = 0; i < 3; i++) file << AES_Re[i] << " ";
        for (int i = 0; i < 3; i++) file << North_Direction[i] << " ";
        file << std::endl;
        */
    }
}

void Measure_Rework::finalize()
{
    file.close();
}
