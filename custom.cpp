//---------------------------------------------------------------------------

#include <math.h>
#include <iostream>
#include "custom.h"

#include "tvector.h"
#include "tmatrix.h"


//---------------------------------------------------------------------------

long double ArtificialSatellite::atmosParam[8][4] =
{
    { 0.0L,       1.225L,     -0.2639E-8L, 0.7825E-4L  },
    { 20000.0L,   0.891E-1L,  0.4407E-9L,  0.16375E-3L },
    { 60000.0L,   2.578E-4L,  -0.2560E-8L, 0.5905E-4L  },
    { 100000.0L,  4.061E-7L,  0.1469E-8L,  0.1787E-3L  },
    { 150000.0L,  2.130E-9L,  0.8004E-10L, 0.3734E-4L  },
    { 300000.0L,  4.764E-11L, 0.7111E-11L, 0.9280E-5L  },
    { 600000.0L,  8.762E-13L, 0.1831E-11L, 0.9280E-5L  },
    { 900000.0L,  6.367E-14L, 0.0L,        0.9540E-5L  }
};

ArtificialSatellite::ArtificialSatellite() : TModel ()
{
    generator = new NormalGaussGenerator();
    generator->setOmega(50);

    A.resize(3, 3); startRotation.resize(3, 3);
    A[0][0] = cos(u)*cos(pi/2)-sin(u)*sin(pi/2.0L)*cos(i); A[0][1] = -sin(u)*cos(pi/2.0L)-cos(u)*sin(pi/2.0L)*cos(i); A[0][2] = sin(i)*sin(pi/2.0L);
    A[1][0] = cos(u)*sin(pi/2)+sin(u)*cos(pi/2.0L)*cos(i); A[1][1] = -sin(u)*sin(pi/2.0L)+cos(u)*cos(pi/2.0L)*cos(i); A[1][2] = sin(i)*cos(pi/2.0L);
    A[2][0] = sin(u)*sin(i);                               A[2][1] = cos(u)*sin(i);                                   A[2][2] = cos(i);

    startRotation[0][0] = cos(i);  startRotation[0][1] = 0; startRotation[0][2] = sin(i);
    startRotation[1][0] = 0;       startRotation[1][1] = 1; startRotation[1][2] = 0;
    startRotation[2][0] = -sin(i); startRotation[2][1] = 0; startRotation[2][2] = cos(i);

    X0.resize(8); temp.resize(3); Ve.resize(3); EarthRotation.resize(3);
    temp[0] = Re+Hp; temp[1] = 0; temp[2] = 0; temp = startRotation*temp;

    Ve[0] = sqrt(mu/p)*e*sin(0.0L); Ve[1] = sqrt(mu/p)*(1+e*cos(pi/2)); Ve[2] = 0; Ve = A*Ve;

    EarthRotation[0] = 0; EarthRotation[1] = 0; EarthRotation[2] = omega;

    X0[0] = temp[0];
    X0[1] = temp[1];
    X0[2] = temp[2];
    X0[3] = Ve[0];//*0.95L;
    X0[4] = Ve[1];//*0.95L;
    X0[5] = Ve[2];//*0.95L;
    X0[6] = 0;
    X0[7] = 0;

    t0 = 0; t1 = 25000.0L/*86400.L*15L*/; SamplingIncrement = 1;
    //std::cout << "Main created" << std::endl;
}

ArtificialSatellite::ArtificialSatellite( const TVector& X, bool with_eps, int num, long double eps) : ArtificialSatellite()
{
    main_trajectory = false; calc_eps = with_eps;
    eps_num = num;

    for (int i = 0; i < 6; i++)
        X0[i] = X[i];

    if (with_eps) { X0[num] += eps; /*std::cout << "derivative created" << std::endl; */}
    else std::cout << "MNK created" << std::endl;
}

long double ArtificialSatellite::ro(long double distance, long double rand)
{
    int i = 0;
    for (i = 8-1; i >= 0; i--) if (distance >= atmosParam[i][0]) break;
    long double thickness = atmosParam[i][1]*(1 + rand)*exp(atmosParam[i][2]*pow((distance - atmosParam[i][0]), 2)
                                                          - atmosParam[i][3]*pow((distance - atmosParam[i][0]), 1));
    return thickness;//*pow(1000.0L, 3);

}

void ArtificialSatellite::getRight( const TVector &X, long double t, TVector &Y )
{
    if (n*generator->getSamplingTime() < t) { noise = generator->generate(); n++; }
    Y.resize(X.size());
    dist = sqrt(X[0]*X[0]+X[1]*X[1]+X[2]*X[2]); h = dist - Re;
    //std::cout << "T = " << t << std::endl;
    if ( dropped == false )
    {
        atmosRoDeviation = k*T*X[7] + k*X[6];
        temp.resize(3); V.resize(3); for(int i = 0; i < 3; i++) { temp[i] = X[i]; V[i] = X[i+3]; }
        temp = EarthRotation^temp;
        Va.resize(3); Va = (V - temp);
        Y[0] = X[3];
        Y[1] = X[4];
        Y[2] = X[5];
        //км/с^2 = км^3/c^2*км/км^3  - кг^(-1)*(км^2)*(кг/км^3)*км/с*км/с
                                       //кг^(-1)*(м^2)*(кг/м^3)*(м/с)*(м/с)
        Y[3] = -mu*X[0]/pow(dist, 3); //- (1.0L/m)*CxS*(ro(h, atmosRoDeviation)*Va.length()/2.0L)*Va[0];
        Y[4] = -mu*X[1]/pow(dist, 3); //- (1.0L/m)*CxS*(ro(h, atmosRoDeviation)*Va.length()/2.0L)*Va[1];
        Y[5] = -mu*X[2]/pow(dist, 3); //- (1.0L/m)*CxS*(ro(h, atmosRoDeviation)*Va.length()/2.0L)*Va[2];
        Y[6] = X[7];
        Y[7] = 1/pow(T, 2) - 2*ksi/T*X[7] - 1/pow(T, 2)*X[6] + 1/pow(T, 2)*noise;
    } else {
        std::cout << "AES dropped" << std::endl;
        for (int i = 3; i < 6; i++) Y[i] = 0;
    }
}

bool ArtificialSatellite::run( const TVector& X, long double t )
{
    h = sqrt((pow(X[0], 2) + pow(X[1], 2) + pow(X[2], 2))) - Re;
    if (h >= 0) return true;
    else { dropped = true; return false;  }
}

void ArtificialSatellite::do_thing( const TVector &X, long double t )
{
}

void ArtificialSatellite::finalize()
{
    Result.resize(Result.row_count()-1, Result.col_count());
}
