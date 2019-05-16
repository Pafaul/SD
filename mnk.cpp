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
    Integrator.setPrecision(1E-10);

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

    H.resize(measures[0].n_rows*2, 6);
    long double res = 0.0, res1 = 0.0, res2 = 0.0;
    for (int i = 0; i < measures[0].n_rows; i++)
        for (int j = 0; j < 6; j++)
        {
            j < 3 ? delta = H_delta_x : delta = H_delta_v;
            for (int k = 0; k < 2; k++)
            {
                res1 = measures[2*j](i,k);
                res2 = measures[2*j+1](i,k);
                res = (res1 - res2)/(2*delta);
                H(2*i+k, j) = res;//(measures[2*j](i, k) - measures[2*j+1](i, k))/(2*delta);
            }
        }

    return H;

}
