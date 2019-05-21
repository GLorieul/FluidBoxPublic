#ifndef TIME_INTEGRATOR___HPP
#define TIME_INTEGRATOR___HPP

#include <vector>
#include "field.hpp"

namespace fb
{

// Framework for time-integration.
// To use, create a class inheriting from it that implements integrand()
// It is also recommended to create an interface to methods
// integrateOverATimeStep_() and integrand_() to improve readability.
// Indeed, for instance:
//
//     integrateOverATimeStep(timeStep, Velocity, MassDensity, Viscosity)
//
// can benefit from type safety + is more readable than
//
//     integrateOverATimeStep(timeStep, {&Velocity}, {&MassDensity, &Viscosity}, UNUSED(timePoint))
//
class TimeIntegrator
{
public:
    TimeIntegrator() = default;
    TimeIntegrator(std::vector<const Field*> variables);
    ~TimeIntegrator();

protected:
    void integrateOverATimeStep_(const double timeStep,
                                std::vector<Field*> variables,
                                std::vector<const Field*> parameters,
                                const double timePoint);

    int nbVariables_;
    Domain domain_;
//  std::vector<std::unique_ptr<Field>> buffer_;
    std::vector<Field*> buffer_;

    // integrand() computes Rhs() in
    //     ∂q                             ⌠∂q    
    //     —— = Rhs(q,t)  =>  qⁿ⁺¹ = qⁿ + ⎮——.dt 
    //     ∂t                             ⌡∂t    
    // I.e. from q and t, compute ∂q/∂t
    virtual void integrand_(std::vector<Field*> timeDerivatives,
                            std::vector<Field*> variables,
                            std::vector<const Field*> parameters,
                            const double timePoint) = 0;
};

}

#endif

