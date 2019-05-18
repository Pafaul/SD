#include "integrator.h"

#define max(a, b) ( ( (a) > (b) ) ? (a) : (b) )
#define min(a, b) ( ( (a) < (b) ) ? (a) : (b) )


//===========================================================================
// class TDormandPrinceIntegrator

const long double TDormandPrinceIntegrator::c[7] = { 0, 1./5, 3./10, 4./5, 8./9, 1., 1. };
const long double TDormandPrinceIntegrator::a[7][6] = {
    { 0. },
    { 1./5 },
    { 3./40, 9./40 },
    { 44./45, -56./15, 32./9 },
    { 19372./6561, -25360./2187, 64448./6561, -212./729 },
    { 9017./3168, -355./33, 46732./5247, 49./176, -5103./18656 },
    { 35./384, 0., 500./1113, 125./192, -2187./6784, 11./84 }
};
const long double TDormandPrinceIntegrator::b1[7] = { 35./384, 0., 500./1113, 125./192, -2187./6784, 11./84, 0 };
const long double TDormandPrinceIntegrator::b2[7] = { 5179./57600, 0., 7571./16695, 393./640, -92097./339200, 187./2100, 1./40 };

//---------------------------------------------------------------------------

TDormandPrinceIntegrator::TDormandPrinceIntegrator()
    : TIntegrator() 
{
	// ����������� ��������� ����
    double v = 1.;
    while (1.+v > 1.) {
        u = v;
        v = v/2.;
    }
}

//---------------------------------------------------------------------------

long double TDormandPrinceIntegrator::Run(TModel* Model)
{

//printf ("\n1.1");
    
    long double // ��� ����� ��� �������������� (������������� �� �������� ���� ��������������)
				t = Model->getT0(),
				// ��� ����� ��� ������ (������������� ��������� �� �������� ���������)
				t_out = t,
				// ��� �������� �����
				t1 = Model->getT1(),
				// ��� ��� ��������������
				h,
				// ��� ��� ����� ��������� (���������������� ���������� ������ ����������)
				h_new = Model->getSamplingIncrement(),
				// ��� ������ �� ���� ��������������
			    e = 0;

    vec // ��� ������ ��������� �� ������ ����
            X = Model->getInitialConditions(),
            // ��� ������ ��������� �� ����� ���� (������� 4-�� �������)
            X1( X.size(), fill::zeros ),
            // ��� ������ ��������� �� ����� ���� ��� ��������� �������� ���� (������� 5-�� �������)
            X2( X.size(), fill::zeros ),
            // ��� ������ ��� ������ ����������
            Xout ( X.size(), fill::zeros ),
            // ��� ����� ��� ���������� ������������� �
            Y( X.size(), fill::zeros );
	
	// ���������� ��������� ����������� � ������ ��� ��������� ������������� ��������� ������
	Model->prepareResult();
	
    // �������������� ����������� ������������� K[j]
    for ( int j = 7; j > 0; --j, K[j].resize( X.size() ) );
		
    //printf ("\n1.2");
	// ������� ���������� ��������� �����
	int N = 0;
	
    //printf ("\n&&&&&&K\n");
	
    // ������� ����
    while ( t < t1 )
    {
        // ������������� ��� �� ��������
        h = h_new;
		  // ��������� ������������ �
            for ( int j = 0; j < 7; j++ )
            {
                for ( int k = X.size()-1; k >= 0; k-- )
                {
                    Y[k] = X[k];
                    for ( int s = 0; s < j; s++ )
                    {
                        Y[k] += K[s][k] * a[j][s] * h;
                    }
                }
               Model->getRight( Y, t + c[j] * h, K[j] );
            }
     // ��������� ����� �������� ����������� � ��������� ������ (4 � 5 �������)
            e = 0;
            for ( int k = X.size()-1; k >= 0; k-- )
            {
                X1[k] = X2[k] = X[k];
                for ( int j = 0; j < 7; j++ )
                {
                    X1[k] += K[j][k] * b1[j] * h;
                    X2[k] += K[j][k] * b2[j] * h;
                }
                e += pow( h * (X1[k] - X2[k]) / max( max( fabsl(X[k]), fabsl(X1[k]) ), max((long double)1e-5, 2*u/Eps) ) , 2 );
            }
            e = sqrtl( e / X.size() );

            // ��������� ����
            h_new = h / max( 0.1, min( 5., pow(e / Eps, 0.2)/0.9 ) );
           // ���� ��������� ������ ��������� ������������� ��������, �������� ������� ��� ������

        if (e != e)
            std::cout << 'broken' << std::endl;
            if ( e > Eps )
                continue;
        // ������������ ����������� ��� ������ ��������� ������� ������
		while ( (t_out < t + h) && (t_out <= t1) )
        {
            long double l_ldTheta = (t_out - t)/h,
                        b[6];

			 // ������������ ������������ ������� ������
            b[0] = l_ldTheta * ( 1 + l_ldTheta*(-1337./480. + l_ldTheta*(1039./360. + l_ldTheta*(-1163./1152.))));
            b[1] = 0;
            b[2] = 100. * pow(l_ldTheta, 2) * (1054./9275. + l_ldTheta*(-4682./27825. + l_ldTheta*(379./5565.)))/3.;
            b[3] = -5. * pow(l_ldTheta, 2) * (27./40. + l_ldTheta*(-9./5. + l_ldTheta*(83./96.)))/2.;
            b[4] = 18225. * pow(l_ldTheta, 2) * (-3./250. + l_ldTheta*(22./375. + l_ldTheta*(-37./600.)))/848.;
            b[5] = -22. * pow(l_ldTheta, 2) * (-3./10. + l_ldTheta*(29./30. + l_ldTheta*(-17./24.)))/7.;

			// �������� ��������� ��� ������
            for ( int k = X.size()-1; k >= 0; k-- )
            {
                long double l_ldSum  = 0;
                for ( int j = 5; j >= 0; j-- )
                    l_ldSum += b[j] * K[j][k];
                if (Model->run(X, t))
                    Xout[k] = X[k] + h * l_ldSum;
                else
                    Xout[k] = X[k];
            }

            // �������� ���������� � ������
            Model->addResult( Xout, t_out );
            // ���������� ����� ������
            t_out += Model->getSamplingIncrement();
        }

//        if ((int)t_out % 10000 == 0) std::cout /*<< "H = " << h_new << "; eps = " << e << ";*/ << "time = " << t << std::endl;

        // ��������� X � ���������� ����� �� �������� ���������� ����
        X = X1;
        t += h;
        //Model->do_thing(X, t);
//        if (Model->run(X, t) == false) for(int i = 3; i < 6; i++) X[i] = 0.0L;

        // ������� ���������� �������� ��� ���������� ���������� �����������
        N++;
    }
	
    //printf ("\n1.4");
   // printf ("\n%i", N);

	// ���������� �������� ���������� �����������
    return Eps / pow( N, 1.5 );
}


