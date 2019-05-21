#ifndef LEVEL_SET___HPP
#define LEVEL_SET___HPP

#include "field.hpp"
#include "timeIntegrator.hpp"

namespace fb
{

class LevelSetReinitializer : public TimeIntegrator
{
public:
    LevelSetReinitializer(const Field& levelSet)
        :TimeIntegrator({&levelSet})
    { /*Do nothing*/ }

    void reinitialize(Field& levelSet, const double distanceToReinit);

protected:
    void integrateOverATimeStep_(const double timeStep, Field& levelSet)
        { TimeIntegrator::integrateOverATimeStep_(timeStep, {&levelSet}, {}, 0.0); }

    void virtual integrand_(std::vector<Field*> timeDerivatives,
                    std::vector<Field*> variables,
                    std::vector<const Field*> UNUSED(parameters),
                    const double UNUSED(timePoint))
        { integrand_(*timeDerivatives[0], *variables[0]); }
    
    void integrand_(Field& dLs_dT, const Field& levelSet);
};

}

#endif

