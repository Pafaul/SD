//---------------------------------------------------------------------------

#include "model.h"
#include <iostream>

//---------------------------------------------------------------------------
void TModel::addResult( const TVector& X, long double t )
{
    Result.resize(N + 1, getOrder() + 1);
    //if (N == Result.rowCount())
    //    Result.resize(N + 1, getOrder() + 1);

	Result(N, 0) = t;
    for (int i = X.size(); i > 0 ; i--)
    {
		Result(N, i) = X[i-1];
        //std::cout << Result(N, i) << std::endl;
    }
    N++;
}

void TModel::clearResult() 
{
	Result.resize(0, 0); 
	N = 0; 
}


void TModel::prepareResult() 
{ 

	N = 0; 
}
