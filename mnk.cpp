#include "mnk.h"

void write_matrix( const mat& result, std::string filename){

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

MNK::MNK()
{

}

mat MNK::get_H(vec& X0, long double t0, long double tk)
{
    ArtificialSatellite models[12];
    mat measures[12];
    mat H;

    TDormandPrinceIntegrator Integrator = TDormandPrinceIntegrator();
    Integrator.setPrecision(1E-5);

    long double delta = 0;
    for (int i = 0; i < 6; i++)
    {
        i < 3 ? delta = H_delta_x : delta = H_delta_v;
        models[2*i] = ArtificialSatellite(X0, true, i, delta, t0, tk);
        models[2*i+1] = ArtificialSatellite(X0, true, i, -delta, t0, tk);
    }
    for (int i = 0; i < 12; i++)
    {
        Integrator.Run(&models[i]);
        write_matrix(models[i].getResult(), "h_"+std::to_string(i) + ".txt");
        proc_traj.get_all_measures(models[i].getResult(), measures[i]);
    }

    H.resize(measures[0].n_rows*meas_num, 6);
    for (int i = 0; i < measures[0].n_rows; i++)
        for (int j = 0; j < 6; j++)
        {
            j < 3 ? delta = H_delta_x : delta = H_delta_v;
            for (int k = 0; k < meas_num; k++)
            {
                H(meas_num*i+k, j) = (measures[2*j](i, k) - measures[2*j+1](i, k))/(2*delta);
            }
        }
    write_matrix(measures[0], "measures_0.txt");
    write_matrix(H, "h_matrix.txt");
    //H  *= 1e5;
    return H;

}
