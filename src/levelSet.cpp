
#include "levelSet.hpp"

namespace fb
{

void LevelSetReinitializer::
reinitialize(Field& levelSet, const double distanceToReinit)
{
    const double cfl = 0.1;
    const Distance h = levelSet.get_domain().get_gridSpacing();
    assume_gridIsIsotropic(__FILE__,__LINE__,__FUNCTION__,h);
    //Advection velocity is normal in the case of reinitialization equation
    //Hence advection speed always equal to one.
    const double c = 1.0; //Advection speed
    const double timeStep = cfl * h.x/c;
    //Same argument: since advection velocity is always equal to one,
    //reinitialization duration is equal to distance to reinitialize
    const double timeEnd = distanceToReinit;

    for(double timePoint=0.0; timePoint < timeEnd; timePoint += timeStep)
        { integrateOverATimeStep_(timeStep, levelSet); }
}


void LevelSetReinitializer::
integrand_(Field& dLs_dT, const Field& levelSet)
{
    const Domain& domain = levelSet.get_domain();
    const Distance h = domain.get_gridSpacing();
    assume_gridIsIsotropic(__FILE__,__LINE__,__FUNCTION__,h);
    const Resolution res = domain.get_resolution();
    const double inv_h  = 1.0 / h.x;
    const double inv_2h = 0.5 / h.x;
    const Field& ls = levelSet;
    for(int i=0 ; i < res.x ; i++) {
        for(int j=0 ; j < res.y ; j++) {
            //COMPUTING SIGN
            double sign;
            {
                const double dLs_dX = (ls(i+1,j) - ls(i-1,j)) * inv_2h;
                const double dLs_dY = (ls(i,j+1) - ls(i,j-1)) * inv_2h;
                const double dLs_dN = std::sqrt(dLs_dX*dLs_dX + dLs_dY*dLs_dY);
                sign = ls(i,j) / sqrt(ls(i,j)*ls(i,j) + h.x);
            }

            //ROUY-TOURIN SCHEME
            double gradLsError;
            {
                const double dLs_dX_dwn = (ls(i,j) - ls(i-1,j)) * inv_h;
                const double dLs_dX_upw = (ls(i+1,j) - ls(i,j)) * inv_h;
                const double dLs_dY_dwn = (ls(i,j) - ls(i,j-1)) * inv_h;
                const double dLs_dY_upw = (ls(i,j+1) - ls(i,j)) * inv_h;
                double dLs_dX, dLs_dY;
                if(ls(i,j) > 0.0)
                {
                    dLs_dX = max(+dLs_dX_dwn, -dLs_dX_upw, 0.0);
                    dLs_dY = max(+dLs_dY_dwn, -dLs_dY_upw, 0.0);
                }
                else
                {
                    dLs_dX = max(-dLs_dX_dwn, +dLs_dX_upw, 0.0);
                    dLs_dY = max(-dLs_dY_dwn, +dLs_dY_upw, 0.0);
                }
                const double dLs_dN = sqrt(dLs_dX*dLs_dX + dLs_dY*dLs_dY);
                gradLsError = 1.0 - dLs_dN;
            }

            //SAVING RESULT
            dLs_dT(i,j) = sign*gradLsError;
        }
    }
}

}

