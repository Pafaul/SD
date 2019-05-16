#ifndef TRAJECTORY_PROCESSING_H
#define TRAJECTORY_PROCESSING_H
#include <math.h>

#include "tvector.h"
#include "tmatrix.h"
#include "measure_rework.h"
#include "model.h"

class Trajectory_Processing
{
protected:
    Measure_Rework* measure_model;
    mat measures, measures_MNK;
    vec time;
    long double Re = 6731*1000.0L;
public:
    Trajectory_Processing();
    void process_trajectory(const mat& trajectory, mat& meas);
    void process_trajectory(const mat &trajectory, bool main, mat& der_measures);
    void process_trajectory(const mat& trajectory, long double delta_x, long double delta_v, mat& derivatives, const TModel* models);
    void get_all_measures(const mat& trajectory, mat& measures);
    vec get_new_time_for_models(const mat& measures);
    const mat& get_measures() const { return measures; }
    int detect_fall(const mat& traj);
    vec from_matrix_measures_to_vector(const mat& meas, int dimension);
    vec from_matrix_measures_to_vector(const mat& meas, int dimension, int start, int finish);
};

#endif // TRAJECTORY_PROCESSING_H
