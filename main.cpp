/* Standard library	*/


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "LA.h"

#define max(a, b) ( ( (a) > (b) ) ? (a) : (b) )
#define min(a, b) ( ( (a) < (b) ) ? (a) : (b) )


#include "integrator.h"
//#include "custom.h"
#include "custom.h"


void writeRes(TModel* model);

int main()
{
    /*TVector X0(6);
    X0.resize(6);
    X0[0] = -2.566123740124270e+7L; //km
    X0[1] = 1.339350231544666e+8L;
    X0[2] = 5.805149372446711e+7L;
    X0[3] = -2.983549561177192e+1L; //km/c
    X0[4] = -4.846747552523134L;
    X0[5] = -2.100585886567924L;
    //Date start, finish, checkDay;
    //start.year=2019; start.month=1; start.day=1;
    //finish.year=2020; finish.month=3; finish.day=17;
    //checkDay.year=2019; checkDay.month=3; checkDay.day=16;*/
    TModel* model = new ArtificialSatellite();
    TIntegrator* Integrator = new TDormandPrinceIntegrator();
    Integrator->setPrecision(1E-16);
    Integrator->Run( model );
    writeRes(model);
    delete model;
    delete Integrator;
	return 0;
}


void writeRes(TModel* model){

    std::ofstream file("test.txt");

        TMatrix Result = model->getResult();

        for (int i=0; i<Result.rowCount(); i++)
        {
            for (int j=0; j<Result.colCount(); j++)
                file << Result(i, j) << " ";

            file << std::endl;
        }

        file.close();
}
