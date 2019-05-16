//---------------------------------------------------------------------------

#pragma once


#include "tvector.h"
#include "tmatrix.h"
#define ARMA_DONT_USE_WRAPPER
#define ARMA_USE_LAPACK
//#define ARMA_USE_BLAS
#include <armadillo>

using namespace arma;
//---------------------------------------------------------------------------

class TModel
{
    protected:
        vec X0;
        long double SamplingIncrement;
        long double t0, t1;
        //TMatrix Result;
		int N;
		
    public:

        mat Result;
        TModel() : SamplingIncrement( 60 ), t0( 0. ), t1( 15811200. ), N( 0. ) {}

        virtual void getRight( const  vec& X, long double t, vec& Y ) = 0;
		
        inline vec getInitialConditions() const { return X0; }
        inline int getOrder() const { return X0.size(); }

        inline long double getSamplingIncrement() const { return SamplingIncrement; }

        inline long double getT0() const { return t0; }
        inline long double getT1() const { return t1; }
       
        inline arma::mat getResult() const { return Result; }
        virtual void addResult( const vec& X, long double t );
		virtual void clearResult();
		virtual void prepareResult();
        virtual bool run( const vec& X, long double t ) { return true; }
        virtual void do_thing( const vec& X, long double t ) = 0;
        virtual void finalize() = 0;
//        virtual mat get_measures() = 0;
};

//---------------------------------------------------------------------------
