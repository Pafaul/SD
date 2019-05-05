/* Standard library	*/


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "tvector.h"
#include "tmatrix.h"

//#define max(a, b) ( ( (a) > (b) ) ? (a) : (b) )
//#define min(a, b) ( ( (a) < (b) ) ? (a) : (b) )


#include "integrator.h"
//#include "custom.h"
#include "custom.h"
#include "trajectory_processing.h"

void writeRes( const TMatrix& result, std::string filename);
long double vector_diff( const TVector& X, const TVector& Y );
void matrix_copy_result(const TModel& A, TMatrix& B);
void matrix_copy_measures(const Trajectory_Processing& A, TMatrix& B);

int main()
{
    long double delta_main_x = 1000; long double delta_main_v = 100;
    std::string filename;
    TModel* model = new ArtificialSatellite();
    TIntegrator* Integrator = new TDormandPrinceIntegrator();

    Integrator->setPrecision(1E-10);
    Integrator->Run( model );
    model->finalize();

    Trajectory_Processing* measure_model = new Trajectory_Processing();
    measure_model->process_trajectory(model->Result);

    filename = "main_trajectory.txt";
    writeRes(model->Result, filename);
    filename = "main_measures.txt";
    writeRes(measure_model->get_measures(), filename);

    /*TMatrix main_trajectory;// = model->getResult();
    matrix_copy_result(*model, main_trajectory);//= new TMatrix(model->getResult().rowCount(), model->getResult().colCount());
    TMatrix main_measures;// = measure_model->get_measures();
    matrix_copy_measures(*measure_model, main_measures);//main_measures.resize(measure_model->get_measures().rowCount(), measure_model->get_measures().colCount());
    /*main_trajectory.resize();
    for (int i = 0; i < main_trajectory->rowCount(); i++)
        for (int j = 0; j < main_trajectory->colCount(); j++)
            main_trajectory->operator()(i, j) = model->getResult()(i,j);
    for (int i = 0; i < main_measures.rowCount(); i++)
        for (int j = 0; j < main_measures.colCount(); j++)
            main_measures(i, j) = measure_model->get_measures()(i,j);
    */
    long double der_x = 10, der_v = 10;
    TVector delta_X, delta_Y, X;
    delta_X.resize(6); delta_Y.resize(6); X.resize(6);
    for (int i = 0; i < 6; i++) { delta_X[i] = 0.0; delta_Y[i] = 0.0; X[i] = model->getInitialConditions()[i]; }

    TMatrix temp;
    TMatrix* H = new TMatrix();
    TMatrix* derivatives = new TMatrix(model->Result.row_count(), 7);

        ArtificialSatellite* derivative_models = new ArtificialSatellite[12];
        for (int i = 0; i < 3; i++)
        {
            derivative_models[2*i] = ArtificialSatellite( X, true, i, der_x );
            derivative_models[2*i+1] = ArtificialSatellite( X, true, i, -der_x );
            derivative_models[2*i+6] = ArtificialSatellite( X, true, i+3, der_v );
            derivative_models[2*i+1+6] = ArtificialSatellite( X, true, i+3, -der_v );
        }
        for(int i = 0; i < 12; i++)
        {
            filename = "der_" + std::to_string(i) + ".txt";
            Integrator->Run( &derivative_models[i] );
 /*           temp.resize(derivative_models[i].getResult().rowCount(), derivative_models[i].getResult().colCount());
            for (int i = 0; i < temp.rowCount(); i++)
                for (int j = 0; j < temp.colCount(); j++)
                    temp(i,j) = derivative_models[i].getResult()(i,j);*/
            writeRes(derivative_models[i].getResult(), filename);
        }

        derivatives->resize(measure_model->get_measures().row_count(), 6);
        int count = 0;
        long double der = 0.0L;
        for (int i = 0; i < derivative_models[0].getResult().row_count(); i++)
        {
            if (derivative_models[0].getResult()[i][0] == measure_model->get_measures()[count][0])
            {

                for (int j = 0; j < 6; j++)
                {
                    if (j < 3) der = der_x;
                    else der = der_v;
                    if (derivatives->row_count() < count + 1)
                        derivatives->resize(derivatives->row_count()+1, 6);
                    derivatives[count][j] = (derivative_models[2*j].getResult()[i][j+1] - derivative_models[2*j+1].getResult()[i][j+1])/der;
                }
                count ++;
            }
        }
        /*measure_model->process_trajectory(
                    model->getResult(),
                    measure_model->get_measures(),
                    der_x,
                    der_v,
                    *derivatives,
                    derivative_models);*/
        filename = "derivatives.txt";
        writeRes(*derivatives, filename);

        delete [] derivative_models;



    /*TVector delta_X, delta_Y; delta_X.resize(6); for (int i = 0; i < 6; i++) delta_X[i] = 0;
    TMatrix H;
    TMatrix disp; disp.resize(2, 2);
    disp(0,0) = 0.000872665;
    disp(1,1) = 0.000872665;
    TVector X = model->getInitialConditions();
    for (int i = 0; i < 3; i++) { X[i] += delta_main_x; X[i+3] += delta_main_v; }
    ArtificialSatellite* derivative_models = new ArtificialSatellite[12];
    ArtificialSatellite* mnk_model;

    //while (vector_diff(X, model->getInitialConditions()) > 1E-5)
    //{
        X = X + delta_X;
        for (int i = 0; i < 6; i++)
        {
            derivative_models[i*2] = ArtificialSatellite(X, true, i, 1, model->get_measures());
            derivative_models[i*2+1] = ArtificialSatellite(X, true, i, -1, model->get_measures());
        }
        for (int i = 0; i < 12; i++)
            Integrator->Run( &derivative_models[i] );

        H.resize(model->get_measures().rowCount(), 6);
        TMatrix* derivatives = new TMatrix[12];
        for (int i = 0; i < 12; i++) derivatives[i] = derivative_models->get_derivative();
        for (int i = 0; i < H.rowCount(); i++)
        {
            //H(i, 0) = derivatives[0](i, 0);
            for (int i = 0; i < H.rowCount(); i++)
            {
                for (int j = 0; j < 3; j++)
                    H(i, j) = (derivatives[j*2](i, 1) - derivatives[j*2+1](i, 1))/delta_main_x;
                for (int j = 3; j < 6; j++)
                    H(i, j) = (derivatives[j*2](i, 1) - derivatives[j*2+1](i, 1))/delta_main_v;
            }
        }

        mnk_model = new ArtificialSatellite(X, false, 0, 0, model->get_measures());

        Integrator->Run(mnk_model);

        delta_X = (H.t()*(!disp)*H)*H.t()*(!disp)*delta_Y;

        X = X + delta_X;
    //}*/

    delete model;
    delete Integrator;
	return 0;
}


void writeRes( const TMatrix& result, std::string filename){

    std::ofstream file(filename);

        //TMatrix Result = model->getResult();

        for (int i=0; i<result.row_count(); i++)
        {
            for (int j=0; j<result.col_count(); j++)
                file << result[i][j] << " ";

            file << std::endl;
        }

        file.close();
}

long double vector_diff( TVector& X, TVector& Y )
{
    if ( X.length() != Y.length() ) return 0;
    long double diff = 0.0L;
    for ( int i = 0; i < X.length(); i++ ) diff += std::pow(X[i] - Y[i], 2);
    diff = std::sqrt(diff);
    return diff;
}

void matrix_copy_result(const TModel& A, TMatrix& B)
{
    B.resize(A.getResult().row_count(), A.getResult().col_count());
    for (int i = 0; i < B.row_count(); i++)
        for (int j = 0; j < B.col_count(); j++)
            B[i][j] = A.getResult()[i][j];
}

void matrix_copy_measures(const Trajectory_Processing& A, TMatrix& B)
{
    B.resize(A.get_measures().row_count(), A.get_measures().col_count());
    for (int i = 0; i < B.row_count(); i++)
        for (int j = 0; j < B.col_count(); j++)
            B[i][j] = A.get_measures()[i][j];
}
