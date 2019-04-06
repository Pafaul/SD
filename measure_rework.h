#ifndef MEASURE_REWORK_H
#define MEASURE_REWORK_H
#include "LA.h"
#include <fstream>

class Measure_Rework
{
public:
    Measure_Rework();
    const long double pi = 3.14159265359L;
    // положение спутника (широта и долгота), высота над Землёй, радиус Земли
    long double latitude = 18.0L/360.0L*2*pi, longtitude = 173.0L/360.0L*2*pi, h = 0, Re = 6731*1000.0L,
                //угол обзора НИП-а, время предыдущего измерения, частота измерений, время начала работы
                measureAngle = 150.0L/360.0L*2*pi, prevMeasureTime = 0.0L, frequency = 1.0L, t0 = 0.0L,
                //скорость вращения Земли
                omega = 7.292115E-5L;
    //R - от центра земли, Re - до поверхности земли, Se - от поверзности земли.
    //<V>_R = <V>_Re + <V_Se>
    TVector AES_R, AES_Re, AES_Se, //ИСЗ
            NIP_R, NIP_Re, NIP_Se, //НИП
            diff_R, diff_Re, diff_Se, //Разности между соответствующими векторами
            North_R, North_Re, North_Se, //Направление на Север
            North_Direction, //направление на Север из НИПа
            North_E; //единичное направление на Север
    TMatrix Result;
    std::ofstream file;
    void measure ( const TVector& X, long double t );    //само измерение
    bool measurable( const TVector& X, long double t );  //можно ли измерить спуник
    void calcAES( const TVector& X, long double t );     //расчёт радиус-векторов ИСЗ
    void calcNIP( const TVector& X, long double t );     //расчёт радиус-векторов НИПа
    void calcDiff( const TVector& X, long double t );    //расчёт разностей радиус-векторов
    void calcNorth();   //расчёт направления на север
    long double calcA(); //расчёт азимута
    long double calcE(); //расчёт элевации
    void finalize();
};

#endif // MEASURE_REWORK_H
