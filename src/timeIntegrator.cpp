
#include "field.hpp"
#include "timeIntegrator.hpp"

namespace fb
{

TimeIntegrator::TimeIntegrator(std::vector<const Field*> variables)
    :nbVariables_(variables.size())
    ,domain_(variables[0]->get_domain())
{
    // ALLOCATE BUFFER MEMORY
    for(int iVar=0 ; iVar < nbVariables_ ; iVar++)
        { buffer_.emplace_back(variables[iVar]->clone()); } // [Note 1]
    // Note 1: By calling `clone`, `Field*` pointer `buffer_` is aware of
    // the derived class type of variables[iVar]. See:
    // https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/
}

TimeIntegrator::~TimeIntegrator()
{
    // DEALLOCATE BUFFER MEMORY
    for(int iVar=0 ; iVar < nbVariables_ ; iVar++)
        { delete buffer_[iVar]; }
}

void TimeIntegrator::
integrateOverATimeStep_(const double timeStep,
                        std::vector<Field*> variables,
                        std::vector<const Field*> parameters,
                        const double timePoint)
{
    // ALIASSING AND MEMORY MANAGEMENT
    std::vector<Field*>& dVars_dt = buffer_; //Shares same memory as varsNp1
    std::vector<Field*>& varsN    = variables;
    std::vector<Field*>& varsNp1  = buffer_; // [check] make this line work with smart pointers (shared_ptr?)

    const double tN = timePoint;
    const double ht = timeStep;

    // COMPUTE INTEGRAND
    integrand_(dVars_dt, varsN, parameters, tN);

    Resolution res = domain_.get_resolution();
    for(int iVar=0 ; iVar < nbVariables_ ; iVar++) {
        Field& varN    = *(varsN[iVar]);
        Field& varNp1  = *(varsNp1[iVar]);
        Field& dVar_dt = *(dVars_dt[iVar]);

        // APPLY INTEGRAND
        int i=0;
        int j=0;
        for(int i=0; i < res.x ; i++) {
            for(int j=0; j < res.y ; j++) {
                for(int k=0; k < res.z ; k++) 
                    { varNp1(i,j,k) = varN(i,j,k) + ht*dVar_dt(i,j,k); }
            }
        }

        // APPLY BOUNDARY CONDITIONS
        varNp1.applyBoundaryConditions();

        // COPY TO ORIGINAL MEMORY LOCATION
        variables[iVar]->copyValuesFrom(varNp1);
    }
}

}

