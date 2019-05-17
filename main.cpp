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
#include "custom.h"
#include "trajectory_processing.h"
#include <mnk.h>
#include <normalgaussgenerator.h>
#include <iomanip>
using namespace arma;

TMatrix fix_matrix( const TMatrix& matrix );
void print_vector(const TVector& vector);
void writeMatrix( const mat& result, std::string filename);
void writeVector( const vec& result, std::string filename);
long double vector_diff( const TVector& X, const TVector& Y );
void matrix_copy_result(const TModel& A, TMatrix& B);
void matrix_copy_measures(const Trajectory_Processing& A, TMatrix& B);
TMatrix E(int i);

int main()
{
    /*
    NormalGaussGenerator gen = NormalGaussGenerator();
    gen.setDispersion(9e-4);
    std::cout << std::setprecision(16) << std::endl;
    for (int i = 0; i < 100; i++)
        std::cout << gen.generate() << std::endl;
    */
    //TMatrix test = E(3000);
    //test = test*0.0052;
    //test = !test;
    long double pi = 3.14159265359L;
    long double eps = 10;
    long double delta_main_x = 100; long double delta_main_v = 5;
    long double dispersion = (0.000959931)*180/pi;
    int mnk_count = 0;
    std::string filename;
    TModel* model = new ArtificialSatellite();
    TIntegrator* Integrator = new TDormandPrinceIntegrator();

    //получение опорной траектории
    std::cout << "Main model running..." << std::endl;
    Integrator->setPrecision(1E-5);
    Integrator->Run( model );
    model->finalize();
    writeMatrix(model->getResult(), "main_trajectory.txt");

    //получение опорных измерений
    std::cout << "Getting main measures..." << std::endl;
    Trajectory_Processing* measure_model = new Trajectory_Processing();
    mat main_measures(model->getResult().n_rows, 3, fill::zeros);
    measure_model->process_trajectory(model->Result, main_measures);


    writeMatrix(main_measures, "main_measures.txt");

    //получение вектора со временем для новых моделирований
    vec times = measure_model->get_new_time_for_models(main_measures);

    MNK mnk;

    long double deltas[] = {0, 0, 0, 0, 0, 0};//{1e1, -1e1, 1e1, -1e0, 1e0, -1e0};//{0, 0, 0, 0, 0, 0};//{1e3, -1e3, 1e3, -1e2, 1e2, -1e2};

    //начало МНК
    mat H, D, temp, mnk_matrix(1, 6);
    vec Y, fi, delta_x(6, fill::zeros), X(8, fill::zeros);
    ArtificialSatellite mnk_model;
    double start_cond;
    for (int i = 2; i < 3; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (j == 0)
                start_cond = 6371e3+140e3;
            else
                if (j == 3)
                    start_cond = 8e3;
            else start_cond = 0;
            X[j] = model->getResult()(model->getResult()(times[i], 0), j+1) + deltas[j];
        }
        //МНК стартует здесь
        do
        {
            std::cout << "and another MNK step..." << std::endl;
            for (int j = 0; j < 6; j++)
                X[j] += delta_x[j];
            for (int j = 0; j < 6; j++)
                mnk_matrix(mnk_matrix.n_rows-1, j) = X[j];
            mnk_matrix.resize(mnk_matrix.n_rows+1, 6);
            writeMatrix(mnk_matrix, "mnk_res.txt");

            //получение матрицы H
            H = mnk.get_H(X, main_measures(times[2*i], 0), main_measures(times[2*i+1], 0));
            writeMatrix(H, "H_matrix.txt");

            //получение опорной траектории для последующих измерений
            mnk_model = ArtificialSatellite(X, false, 0, 0, main_measures(times[2*i]), main_measures(times[2*i+1]));
            Integrator->Run(&mnk_model);

            //составление матрицы дисперсий измерений
            D = mat(H.n_rows, H.n_rows, fill::eye);
            D = D*dispersion;

            //получение истинных измерений
            Y = measure_model->get_vec_from_main_measures(main_measures, times[2*i], times[2*i+1]);
            writeMatrix(D, "dispersion_matrix.txt");
            measure_model->get_all_measures(mnk_model.getResult(), temp);
            fi = measure_model->get_vec_from_measures_for_mnk(temp);

            delta_x = ((H.t()*(D.i())*H)).i()*H.t()*(D.i())*(Y-fi);

            writeMatrix(pinv(D), "D_matrix.txt");
            std::cout << Y-fi << std::endl; writeVector(Y-fi, "delta_y.txt"); writeVector(Y, "y.txt"); writeVector(fi, "fi.txt");
            //std::cin >> start_cond;
            std::cout << "delta_x" << std::endl << delta_x << std::endl;
            std::cout << "X" << std::endl << X << std::endl << "delta_X norm: " << norm(delta_x) << std::endl;
            std::cout << "DISPERSION matrix" << std::endl << ((H.t()*(D.i())*H)).i() << std::endl;

        } while (norm(delta_x) > 1e-3);
    }

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

void writeMatrix( const mat& result, std::string filename){

    std::ofstream file(filename);

        //TMatrix Result = model->getResult();

        for (int i=0; i<result.n_rows; i++)
        {
            for (int j=0; j<result.n_cols; j++)
                file << result(i,j) << " ";

            file << std::endl;
        }

        file.close();
}

void writeVector( const vec& result, std::string filename){

    std::ofstream file(filename);

        //TMatrix Result = model->getResult();

        for (int i=0; i<result.size(); i++)
        {
                file << result[i] << std::endl;

            //file << std::endl;
        }

        file.close();
}

long double vector_diff( const vec& X, const vec& Y )
{
    long double diff = 0.0L;
    for ( int i = 0; i < 6; i++ ) diff += std::pow(X[i] - Y[i], 2);
    diff = std::sqrt(diff);
    return diff;
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
