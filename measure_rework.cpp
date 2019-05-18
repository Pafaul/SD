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
    rand_gen = NormalGaussGenerator();
    rand_gen.setDispersion((0.000959931)*180/pi);
    //rand_gen.setDispersion(1.0L);
}

void Measure_Rework::calcAES( const vec &X, long double t )
{
    for ( int i = 0; i < 3; i++ ) AES_R[i] = X[i];
    AES_Re = AES_R*(Re/norm(AES_R));
    AES_Se = AES_R - AES_Re;
}

void Measure_Rework::calcNIP( const vec &X, long double t )
{
    long double calc_longtitude = longtitude + omega*(t - t0);
    NIP_Re[0] = Re*cos(latitude)*cos(calc_longtitude);
    NIP_Re[1] = Re*cos(latitude)*sin(calc_longtitude);
    NIP_Re[2] = Re*sin(latitude);

    NIP_Se = NIP_Re*(norm(AES_Se)/Re);

    NIP_R = NIP_Re + NIP_Se;
}

void Measure_Rework::calcDiff( const vec &X, long double t )
{
    diff_R  = AES_R  - NIP_R; //разница между векторами
    diff_Re = AES_Re - NIP_Re; //от НИПа до проекции спутника на землю
    for (int i = 0; i < 3; i++)
        diff_Se[i] = AES_R[i] - NIP_Re[i]; //От НИПа до спутника
}

void Measure_Rework::calcNorth()
{
    North_R = North_E * (norm(NIP_Re)/cos(pi/2-latitude));
    North_Direction = North_R - NIP_Re;
}

bool Measure_Rework::measurable( const vec &X, long double t )
{
    bool flag = false;
    if (t >= prevMeasureTime + 1/frequency)
    {
        //вычисление радиус-векторов
        calcAES( X, t );
        calcNIP( X, t );
        calcNorth();
        calcDiff( X, t );

        double alfa = acos(dot(diff_Se, NIP_Se)/norm(NIP_Se)/norm(diff_Se));
        if (alfa > pi)
            alfa -= pi*2;
        file << t << " " << alfa << " ";
        for (int i = 0; i < 3; i++)
            file << NIP_Re[i] << " ";
        file << std::endl;
        long double dist = sqrt(pow(X[0],2) + pow(X[1],2) + pow(X[2],2)) - Re;
        if ((abs(alfa) <= measureAngle/2) && (dist > 0))
            flag = true;
    }
    return flag;

}

long double Measure_Rework::calcA()
{
    vec third = cross(diff_Se, NIP_Se);
    long double cos_azimut = pi/2 - acos(dot(third,North_Direction)/(norm(third)*norm(North_Direction)));
    return (cos_azimut)*180/pi;
}

long double Measure_Rework::calcE()
{
    long double elevation = pi/2 - acos(dot(NIP_Se, diff_Se)/(norm(NIP_Se)*norm(diff_Se)));
    return (elevation)*180/pi;
}

long double Measure_Rework::calcR()
{
    long double dist = 0.0L;
    for (int i = 0; i < 3; i++)
        dist += pow(diff_Se[i],2);
    return sqrt(dist);
}

void Measure_Rework::measure( const vec &X, long double t, bool main )
{
    if ((measurable( X, t ) && main) || !main)
    {
        //вычисление радиус-векторов
        calcAES( X, t );
        calcNIP( X, t );
        calcNorth();
        calcDiff( X, t );
        cur_result.resize(3);
        calcNorth();
        cur_result[0] = t;
        cur_result[1] = calcA();
        cur_result[2] = calcE();
        //cur_result[3] = calcR();
        if (main)
        {
            cur_result[1] += rand_gen.generate();
            cur_result[2] += rand_gen.generate();
            //cur_result[3] += rand_gen.generate();
        }
        prevMeasureTime = t;

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
