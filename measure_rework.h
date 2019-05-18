#ifndef MEASURE_REWORK_H
#define MEASURE_REWORK_H

#include "tvector.h"
#include "math.h"
#include "normalgaussgenerator.h"
#include <fstream>
#include <model.h>
class Measure_Rework
{
public:
    Measure_Rework();
    const long double pi = 3.14159265359L;
    // положение спутника (широта и долгота), высота над Землёй, радиус Земли
    long double latitude = 18.0L/360.0L*2*pi, longtitude = 173.0L/360.0L*2*pi, h = 0, Re = 6731*1000.0L,
                //угол обзора НИП-а, время предыдущего измерения, частота измерений, время начала работы
                measureAngle = 150.0L/180.0L*pi, prevMeasureTime = 0.0L, frequency = 1.0L, t0 = 0.0L,
                //скорость вращения Земли
                omega = 7.292115E-5L;
    //R - от центра земли, Re - до поверхности земли, Se - от поверзности земли.
    //<V>_R = <V>_Re + <V_Se>
    vec AES_R, AES_Re, AES_Se, //ИСЗ
            NIP_R, NIP_Re, NIP_Se, //НИП
            diff_R, diff_Re, diff_Se, //Разности между соответствующими векторами
            North_R, North_Re, North_Se, //Направление на Север
            North_Direction, //направление на Север из НИПа
            North_E; //единичное направление на Север
    mat Result;
    vec cur_result;
    std::ofstream file;
    NormalGaussGenerator rand_gen;
    void measure ( const vec& X, long double t, bool main );    //само измерение
    bool measurable( const vec& X, long double t );  //можно ли измерить спуник
    void calcAES( const vec& X, long double t );     //расчёт радиус-векторов ИСЗ
    void calcNIP( const vec& X, long double t );     //расчёт радиус-векторов НИПа
    void calcDiff( const vec& X, long double t );    //расчёт разностей радиус-векторов
    void calcNorth();   //расчёт направления на север
    long double calcA(); //расчёт азимута
    long double calcE(); //расчёт элевации
    long double calcR(); //расчёт расстояния
    void finalize();

    long double length(vec V) { long double res = 0.0; for (int i = 0; i < V.size(); i++) res += pow(V[i],2); return sqrt(res); }
};

#endif // MEASURE_REWORK_H
