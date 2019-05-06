#include "trajectory_processing.h"

Trajectory_Processing::Trajectory_Processing()
{
}

void Trajectory_Processing::process_trajectory(const TMatrix& trajectory)
{
    measure_model = new Measure_Rework();
    long double t = 0.0L;
    TVector X; X.resize(6);
    int count = 0;
    measures.resize(trajectory.row_count(), 3);
    time.resize(trajectory.row_count());
    for (int i = 0; i < trajectory.row_count(); i++)
    {
        for (int j = 1; j < 7; j++)
            X[j-1] = trajectory[i][j];
        t = trajectory[i][0];
        if (measure_model->measurable(X, t))
        {
            measures[count][0] = t;
            measures[count][1] = measure_model->calcA();
            measures[count][2] = measure_model->calcE();
            count++;
        }
    }
    measures.resize(count, 3);
    delete measure_model;
}

void Trajectory_Processing::process_trajectory(const TMatrix &trajectory, bool main, TMatrix& der_measures)
{
    measure_model = new Measure_Rework();
    int count = 0;
    TVector X; X.resize(6);
    der_measures.resize(measures.row_count(), 3);
    for (int i = 0; i < trajectory.row_count(); i++)
    {
        if (trajectory[i][0] == measures[count][0])
        {
            for (int j = 0; j < 6; j++)
                X[j] = trajectory[i][j+1];
            measure_model->measure(X, measures[count][0], false);
            for (int j = 0; j < 3; j++)
                der_measures[count][j] = measure_model->cur_result[j];
            count++;
        }
    }

    delete measure_model;
}

void Trajectory_Processing::process_trajectory(const TMatrix& trajectory, long double delta_x, long double delta_v, TMatrix& derivatives, const TModel* models)
{
    int count = 0;
    TVector X; X.resize(6);
    long double der = 0.0L;
    for (int i = 0; i < trajectory.row_count(); i++)
    {
        if (trajectory[i][0] == measures[count][0])
        {

            for (int j = 0; j < 6; j++)
            {
                if (j < 3) der = delta_x;
                else der = delta_v;
                if (derivatives.row_count() < count + 1)
                    derivatives.resize(derivatives.row_count()+1, 6);
                derivatives[count][j] = (models[2*j].getResult()[i][j+1] - models[2*j+1].getResult()[i][j+1])/der;
            }
            count ++;
        }
    }
}
