#include "trajectory_processing.h"

Trajectory_Processing::Trajectory_Processing()
{
}

void Trajectory_Processing::process_trajectory(const mat& trajectory, mat& meas)
{
    measure_model = new Measure_Rework();
    long double t = 0.0L;
    vec X; X.resize(6);
    int count = 0;
    time.resize(trajectory.n_rows);
    meas.resize(trajectory.n_rows, meas_num+1);
    for (int i = 0; i < trajectory.n_rows; i++)
    {
        for (int j = 1; j < 7; j++)
            X[j-1] = trajectory(i,j);
        t = trajectory(i,0);
        if (measure_model->measurable(X, t))
        {
            measure_model->measure(X, t, true);
            for (int k = 0; k < meas_num+1; k++)
                meas(count, k) =
                        measure_model->cur_result[k];
            count++;
        }
    }
    meas.resize(count, meas_num+1);
    delete measure_model;
}

void Trajectory_Processing::process_trajectory(const mat &trajectory, bool main, mat& der_measures)
{
    measure_model = new Measure_Rework();
    int count = 0;
    vec X; X.resize(6);
    der_measures.resize(measures.n_rows, 3);
    for (int i = 0; i < trajectory.n_rows; i++)
    {
        if (trajectory(i,0) == measures(count,0))
        {
            for (int j = 0; j < 6; j++)
                X[j] = trajectory(i,j+1);
            measure_model->measure(X, measures(count,0), false);
            for (int j = 0; j < 3; j++)
                der_measures(count,j) = measure_model->cur_result[j];
            count++;
        }
    }

    delete measure_model;
}

void Trajectory_Processing::process_trajectory(const mat& trajectory, long double delta_x, long double delta_v, mat& derivatives, const TModel* models)
{
    int count = 0;
    vec X; X.resize(6);
    long double der = 0.0L;
    for (int i = 0; i < trajectory.n_rows; i++)
    {
        if (trajectory(i,0) == measures(count,0))
        {

            for (int j = 0; j < 6; j++)
            {
                if (j < 3) der = delta_x;
                else der = delta_v;
                if (derivatives.n_rows < count + 1)
                    derivatives.resize(derivatives.n_rows+1, 6);
                derivatives(count,j) = (models[2*j].getResult()(i,j+1) - models[2*j+1].getResult()(i,j+1))/der;
            }
            count ++;
        }
    }
}

vec Trajectory_Processing::get_new_time_for_models(const mat &meas)
{
    bool start = false;
    bool finish = false;
    vec temp;
    int i = 0;
    double temp1 = 0.0, temp2 = 0.0;
    for (i = 0; i < meas.n_rows-2; i++)
    {
        temp1 = meas(i, 0); temp2 = meas(i+1,0);
        if (abs(meas(i, 0) - meas(i+1, 0)) == 1)
        {
            if (start == false)
            {
                start = true;
                temp.resize(temp.size()+2);
                temp[temp.size()-2] = i;
                finish = false;
            }

        } else {
            if (finish == false)
            {
                start = false;
                temp[temp.size()-1] = i;
                finish = true;
            }
        }
    }
    if ((finish == false) && (temp.size() != 0))
        temp[temp.size()-1] = i;
    return temp;
}

int Trajectory_Processing::detect_fall(const mat &traj)
{
    int i = 0;
    long double dist = 0.0;
    for ( i = traj.n_rows-1; i > 0; i++)
    {
        dist = 0.0;
        for (int j = 1; j < 4; j++)
            dist += traj(i, j)*traj(i, j);
        dist = sqrt(dist);
        if (dist <= Re)
            break;
    }
    return i;
}

void Trajectory_Processing::get_all_measures(const mat& trajectory, mat& measures)
{
    measure_model = new Measure_Rework();
    measures.resize(trajectory.n_rows, meas_num);
    vec X(6);
    double res1, res2;
    for (int i = 0; i < trajectory.n_rows; i++)
    {
        for (int j = 0; j < 6; j++) X[j] = trajectory(i, j+1);
        measure_model->measure(X, trajectory(i, 0), false);
        for (int k = 1; k < meas_num+1; k++)
        {
            measures(i, k-1) = measure_model->cur_result[k];
            res1 = measure_model->cur_result[k];
        }
    }
    delete measure_model;
}

vec Trajectory_Processing::from_matrix_measures_to_vector(const mat& meas, int dimension)
{
    vec temp (meas.n_rows*dimension);
    for (int i = 0; i < meas.n_rows; i++)
        if (meas.n_cols != dimension)
        {
            for (int j = 0; j < dimension; j++)
                temp(i*dimension+j) = meas(i*dimension,j+1);
        }
        else
        {
            for (int j = 0; j < dimension; j++)
                temp(i*dimension+j) = meas(i,j);
        }

    return temp;
}

vec Trajectory_Processing::from_matrix_measures_to_vector(const mat& meas, int dimension, int start, int finish)
{
    vec temp ((finish - start + 1)*dimension);
    int count = 0;

    for (int i = start; i < finish; i++)
    {
        if (meas.n_cols != dimension)
        {
            for (int j = 0; j < dimension; j++)
                temp((i-start)*dimension+j) = meas(i*dimension,j+1);
        }
        else
        {
            for (int j = 0; j < dimension; j++)
                temp((i-start)*dimension+j) = meas(i*dimension,j);
        }
    }

    return temp;
}

vec Trajectory_Processing::get_vec_from_main_measures(mat& meas, int start, int finish)
{
    vec temp((finish - start + 1)*meas_num);
    for (int i = start; i < finish+1; i++)
    {
        for (int j = 0; j < meas_num; j++)
            temp[(i-start)*meas_num+j] = meas(i, j+1);
    }
    return temp;
}

vec Trajectory_Processing::get_vec_from_measures_for_mnk(mat& meas)
{
    vec temp(meas.n_rows*meas_num);
    for (int i = 0; i < meas.n_rows; i++)
        for (int j = 0; j < meas_num; j++)
            temp[i*meas_num+j] = meas(i, j);

    return temp;
}
