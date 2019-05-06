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
    TMatrix measures, measures_MNK;
    TVector time;
public:
    Trajectory_Processing();
    void process_trajectory(const TMatrix& trajectory);
    void process_trajectory(const TMatrix &trajectory, bool main, TMatrix& der_measures);
    void process_trajectory(const TMatrix& trajectory, long double delta_x, long double delta_v, TMatrix& derivatives, const TModel* models);
    const TMatrix& get_measures() const { return measures; }
};

#endif // TRAJECTORY_PROCESSING_H
