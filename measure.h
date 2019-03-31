#ifndef MEASURE_H
#define MEASURE_H

#include "LA.h"
#include <math.h>

class Measure
{
protected:
    const long double pi = 3.14159265359L;
    // положение спутника (широта и долгота), высота над Землёй, радиус Земли
    long double latitude = 18.0L/360.0L*2*pi, longtitude = 173.0L/360.0L*2*pi, h = 0, Re = 6731,
                //угол обзора НИП-а, время предыдущего измерения, частота измерений, время начала работы
                measureAngle = 150.0L/360.0L*2*pi, prevMeasureTime = 0.0L, frequency = 1.0L, t0 = 0.0L,
                //скорость вращения Земли
                omega = 7.292115E-5L;
    //Матрица для перезода из топографической в геоцентрическую СК; матрица измерений
    TMatrix rotationMatrix, measures;
    //вектора: направление на Север из центра Земли, направление на Север из места расположения НИП-а, радиус-вектор измерений НИП-а из точки на Земле,
    //радиус-вектор из центра Земли до НИП-а, радиус-вектор из центра Земли до поверзности Земли для ИСЗ, радиус-вектор от поверхности Земли до ИСЗ,
    //Радиус-вектор из центра Земли до ИСЗ, вектор для разных разностей.
    //ПРИМЕЧАНИЯ:
    //AES_X0 = AES_Re + AES_X
    //N0 = Ne - R
    TVector N_Re, N0, R, R_Re, AES_Re, AES_X, AES_X0, temp;

public:
    Measure(long double t0);
    //расчёт направления на север в местной горизонтальной плоскости
    void calcN0(void);
    //расчёт оси обнаружения НИП-а
    void calcR(long double len);
    //расчёт радиус-вектора из центра Земли к НИП-у
    void calcR_Re(long double t);
    //расчёт вектора положения ИСЗ в местной горизонтальной плоскости
    void calcAES_X(const TVector& X);
    //расчёт азимута ИСЗ
    long double calcA();
    //расчёт элевации ИСЗ
    long double calcE();
    //проверка на возможность провести измерения
    bool isMeasurable(const TVector& X, long double t);
    //собственно измерения
    void measure(const TVector& X, long double t);
};

#endif // MEASURE_H
