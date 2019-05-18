#ifndef MNK_H
#define MNK_H

#include <trajectory_processing.h>
#include <custom.h>
#include <integrator.h>
class MNK
{
protected:
    long double H_delta_x = 1, H_delta_v = 1e-1;
    Trajectory_Processing proc_traj;
    int meas_num = 2;
    int count = 0;

public:
    MNK();
    mat get_H(vec& X0, long double t0, long double tk);
};

#endif // MNK_H
