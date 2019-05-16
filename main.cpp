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

TMatrix fix_matrix( const TMatrix& matrix );
void print_vector(const TVector& vector);
void writeMatrix( const TMatrix& result, std::string filename);
void writeVector( const TVector& result, std::string filename);
long double vector_diff( const TVector& X, const TVector& Y );
void matrix_copy_result(const TModel& A, TMatrix& B);
void matrix_copy_measures(const Trajectory_Processing& A, TMatrix& B);
TMatrix E(int i);

int main()
{
    //TMatrix test = E(3000);
    //test = test*0.0052;
    //test = !test;

    long double eps = 10;
    long double delta_main_x = 100; long double delta_main_v = 5;
    long double dispersion = 1.59989e-5;
    int mnk_count = 0;
    std::string filename;
    TModel* model = new ArtificialSatellite();
    TIntegrator* Integrator = new TDormandPrinceIntegrator();

    std::cout << "Main model running..." << std::endl;
    Integrator->setPrecision(1E-10);
    Integrator->Run( model );
    model->finalize();

    std::cout << "Getting main measures..." << std::endl;
    Trajectory_Processing* measure_model = new Trajectory_Processing();
    measure_model->process_trajectory(model->Result);

    TMatrix main_measures(measure_model->get_measures());

    filename = "main_trajectory.txt";
    writeMatrix(model->Result, filename);
    filename = "main_measures.txt";
    writeMatrix(main_measures, filename);

    TVector X0 = TVector(6); for (int i = 0; i < 6; i++) X0[i] = model->getInitialConditions()[i];
    long double der_x = 1, der_v = 0.1;
    TVector /*delta_X, delta_Y,*/ X; TVector delta_X;
    /*delta_X.resize(6); delta_Y.resize(6);*/ X.resize(6);
    for (int i = 0; i < 6; i++) { /*delta_X[i] = 0.0; delta_Y[i] = 0.0;*/ X[i] = model->getInitialConditions()[i]; }
    X[0] += delta_main_x;
    X[1] += 0;
    X[2] += 0;
    X[3] += delta_main_v;
    X[4] += 0;
    X[5] += 0;
    TVector delta_measures = TVector();
    TMatrix H;
    TMatrix derivatives = TMatrix(main_measures.row_count()*2, 6);
    TMatrix der_arr [12];
    TMatrix mnk_measures = TMatrix(main_measures.row_count(), 3);
    TMatrix D;
    std::cout << "Starting MNK..." << std::endl;
    do
    {
        std::cout << "Vector diff: " << vector_diff(X, X0) << std::endl;
        for (int i = 0; i < 12; i++)
            der_arr[i] = TMatrix(main_measures.row_count(), main_measures.col_count());

       ArtificialSatellite* derivative_models[12];
       ArtificialSatellite mnk_model;
       for (int i = 0; i < 3; i++)
       {
           derivative_models[2*i] = new ArtificialSatellite( X, true, i, der_x );
           derivative_models[2*i+1] = new ArtificialSatellite( X, true, i, -der_x );
           derivative_models[2*i+6] = new ArtificialSatellite( X, true, i+3, der_v );
           derivative_models[2*i+1+6] = new ArtificialSatellite( X, true, i+3, -der_v );
       }

       for(int i = 0; i < 12; i++)
       {
           filename = "der_" + std::to_string(i) + ".txt";
           std::cout << "Model " << i << " is running." << std::endl;
           Integrator->Run( derivative_models[i] );
           writeMatrix(derivative_models[i]->getResult(), filename);
           measure_model->process_trajectory((derivative_models[i]->Result), false, der_arr[i]);
       }

       std::cout << "calc derivatives" << std::endl;
       derivatives.resize(main_measures.row_count()*2, 6);
       long double der = 0.0L;
       for (int i = 0; i < 2; i++)
       {
           for (int count = 0; count < main_measures.row_count(); count ++)
           {
               for (int j = 0; j < 6; j++)
               {
                   if (j < 3) der = der_x;
                   else der = der_v;
                   derivatives[count*2+i][j] = (
                               der_arr[j*2][count][i+1]-
                                der_arr[j*2+1][count][i+1])/(2*der);
               }
           }
       }

       filename = "derivatives.txt";
       writeMatrix(derivatives, filename);

       mnk_model = ArtificialSatellite( X, false, 0, 0);
       Integrator->Run(&mnk_model);
       measure_model->process_trajectory(mnk_model.getResult(), false, mnk_measures);
       writeMatrix(mnk_measures, "measures_" + std::to_string(mnk_count) + ".txt"); mnk_count++;

       delta_measures.resize(main_measures.row_count()*2);
       for (int i = 0; i < main_measures.row_count(); i++)
           for (int j = 0; j < 2; j++)
               delta_measures[2*i+j] = main_measures[i][j+1] - mnk_measures[i][j+1];

       std::cout << "DeltaX calculation start" << std::endl;
       D.resize(derivatives.row_count(), derivatives.row_count());
       for (int i = 0; i < D.row_count(); i++) D[i][i] = 1.0L;
       //D = (D)*dispersion;
       TMatrix temp_D = !D, temp_der = derivatives.t();
       TMatrix temp;
       //TMatrix temp = derivatives.t()*D.inv();
       //delta_X = temp*delta_measures;
       /*delta_X = derivatives.t()*
               (D.inv())*
               delta_measures;*/
       //temp = derivatives.t()*D.inv()*derivatives;
       //temp = temp.inv();
       /*delta_X = (((derivatives.t()*
                    (D.inv()))*
                    derivatives).inv())*delta_X;*/
       //delta_X = temp*delta_X;
       temp = temp_der*temp_D; fix_matrix(temp);
       temp = temp*derivatives; fix_matrix(temp);
       temp = !temp; fix_matrix(temp);
       temp = temp*temp_der; fix_matrix(temp);
       temp = temp*temp_D; fix_matrix(temp);
       delta_X = temp*delta_measures;
       /*delta_X = (((((temp_der*
                    temp_D)*
                    derivatives).inv())*temp_der)*
               temp_D)*
               delta_measures;*/


       writeVector(delta_X, "delta_x.txt");
       std::cout << "Vector delta_X: " << std::endl; print_vector(delta_X);
       std::cout << "Vector X: " << std::endl; print_vector(X);
       for (int i = 0; i < 6; i++ )
           X[i] += delta_X[i];
       std::cout << "New vector X: " << std::endl; print_vector(X);
       std::cout << "Real vector X: " << std::endl; print_vector(model->getInitialConditions());

    } while (vector_diff(X, X0) > eps);

    delete model;
    delete Integrator;
	return 0;
}

TMatrix fix_matrix( const TMatrix& matrix )
{
    TMatrix temp (matrix);
    for ( int i = 0; i < temp.row_count(); i++ )
        for ( int j = 0; j < temp.col_count(); j++ )
            if (temp[i][j] != temp[i][j])
                temp[i][j] = 1E16L;
    return temp;
}

void writeMatrix( const TMatrix& result, std::string filename){

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

void writeVector( const TVector& result, std::string filename){

    std::ofstream file(filename);

        //TMatrix Result = model->getResult();

        for (int i=0; i<result.size(); i++)
        {
                file << result[i] << std::endl;

            //file << std::endl;
        }

        file.close();
}

long double vector_diff( const TVector& X, const TVector& Y )
{
    long double diff = 0.0L;
    for ( int i = 0; i < 6; i++ ) diff += std::pow(X[i] - Y[i], 2);
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

void print_vector(const TVector& vector)
{
    std::cout << "printing vector:" << std::endl;
    for (int i = 0; i < vector.size(); i++)
        std::cout << vector[i] << " | ";
    std::cout << std::endl;
}

TMatrix E(int i)
{
    TMatrix e = TMatrix(i, i);
    for (int j = 0; j < i; j++)
        e[j][j] = 1.0;
    return e;
}
