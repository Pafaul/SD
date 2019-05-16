#ifndef MNK_H
#define MNK_H

#include <trajectory_processing.h>
#include <custom.h>
#include <integrator.h>
class MNK
{
protected:
    long double H_delta_x = 1e-3, H_delta_v = 1e-3;
    Trajectory_Processing proc_traj;

public:
    MNK();
    mat get_H(vec& X0, long double t0, long double tk);
};

#endif // MNK_H
