#include "measure.h"

Measure::Measure(long double t) : t0(t)
{
    //measures.resize(1, 3);
    rotationMatrix.resize(3, 3);
    N_Re.resize(3); N0.resize(3);
    R.resize(3); R_Re.resize(3);
    AES_Re.resize(3); AES_X.resize(3); AES_X0.resize(3);

    measure_file.open("measures.txt", std::ios_base::out);
}

void Measure::calcN0()
{
    calcR_Re(t0); temp.resize(3); temp[0] = 0; temp[1] = 0; temp[2] = 1;
    N_Re = temp*(R.length()/cos(pi/2 - longtitude)); R.
    N0 = N_Re - R;
}

void Measure::calcAES_X(const TVector& X)
{
    for (int i = 0; i < 3; i++) AES_X0[i] = X[i];
    AES_Re = AES_X0*(Re/AES_X0.length());
    AES_X = AES_X0 - AES_Re;
}

void Measure::calcR_Re(long double t)
{
    long double calc_longtitude = longtitude + omega * (t - t0);
    R_Re[0] = Re*cos(latitude)*cos(calc_longtitude);
    R_Re[1] = Re*cos(latitude)*sin(calc_longtitude);
    R_Re[2] = Re*sin(latitude);
}

void Measure::calcR(long double len) { R = R_Re*(len/R_Re.length()); }

long double Measure::calcA()
{
    temp.resize(3); temp[0] = AES_X[0]; temp[1] = AES_X[1]; temp[2] = 0;
    return acos(temp*N0/(temp.length()*N0.length()));
}

long double Measure::calcE()
{
    temp.resize(3); temp[0] = AES_X[0]; temp[1] = AES_X[1]; temp[2] = 0;
    return acos(temp*AES_X/(temp.length()*AES_X.length()));
}

bool Measure::isMeasurable(const TVector& X, long double t)
{
    calcR_Re(t);
    calcAES_X(X);
    calcR(AES_X[2]);
    //calcAES_X(X);
    //радиус от точки на поверхности
    long double AES_R = sqrt(pow(AES_X[0] - R[0], 2) + pow(AES_X[1] - R[1], 2));
    long double measurableR = AES_X[2] * tan(measureAngle/2.0L);
    if ((measurableR > 0) && (AES_R <= measurableR)) return true; else return false;

}

void Measure::measure(const TVector &X, long double t)
{
    if (isMeasurable(X, t))
    {
        measures.resize(measures.row_count()+1, 3);
        measures[measures.col_count()-1][0] = t;
        measures[measures.col_count()-1][1] = calcE();
        measures[measures.col_count()-1][2] = calcA();
        for (int i = 0; i < 3; i++) measure_file << measures[measures.col_count()-1][i] << " ";
        measure_file << std::endl;
    }
}

void Measure::finalize()
{
    measure_file.close();
}
