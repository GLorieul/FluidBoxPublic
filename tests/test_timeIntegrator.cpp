
#include <gtest/gtest.h>
#include "timeIntegrator.hpp"
using namespace fb;

class TestIntegrator : public TimeIntegrator
{
public:
    TestIntegrator(const Field& field)
        :TimeIntegrator({&field})
    {/*Do nothing*/}

    void integrateOverATimeStep(const double timeStep,
                               std::vector<Field*> variables,
                               std::vector<const Field*> parameters,
                               const double timePoint)
        { integrateOverATimeStep_(timeStep, variables, parameters, timePoint); }

protected:

    virtual void integrand_(std::vector<Field*> timeDerivatives,
                            std::vector<Field*> variables,
                            std::vector<const Field*> UNUSED(parameters),
                            const double timePoint)
    {
        const Domain& domain = variables[0]->get_domain();
        const Resolution res = domain.get_resolution();
        /***/ Field& dF_dT = (*timeDerivatives[0]);
        const Field& f = (*variables[0]);
        for(int i=0 ; i < res.x ; i++) {
            for(int j=0 ; j < res.y ; j++) {
                for(int k=0 ; k < res.z ; k++)
                    { dF_dT(i,j,k) = f(i,j,k) + timePoint; }
            }
        }
    }
};

TEST(timeIntegrator, test)
{
    /* We'll integrate
     *
     *     ∂q         
     *     —— = q + t 
     *     ∂t         
     *
     * whose solution is
     *
     *    q(t) = C.exp(t) - (t+1)
     *
     * with
     *
     *         q^n + (1+t^n)
     *     C = —————————————
     *          exp(t^n)
    */
    Domain domain(Resolution(4,3,2), Point(0.0,0.0,0.0), Distance(1.0,1.0,1.0));
    Field initial(domain, Anchor("C"), GhostCells(0,0, 0,0, 0,0));
    Field field(domain, Anchor("C"), GhostCells(0,0, 0,0, 0,0));
    Field answer(domain, Anchor("C"), GhostCells(0,0, 0,0, 0,0));

    initial.setValues({{{ 21.0, 22.0, 23.0, 24.0},
                      { 17.0, 18.0, 19.0, 20.0},
                      { 13.0, 14.0, 15.0, 16.0}},

                     {{  9.0, 10.0, 11.0, 12.0},
                      {  5.0,  6.0,  7.0,  8.0},
                      {  1.0,  2.0,  3.0,  4.0}}});

    TestIntegrator integrator(field);
    std::vector<double> timeSteps;
    std::vector<double> errors;

    for(double timeStep = 0.1 ; timeStep >= 0.1/intPow<4>(2.0) ; timeStep/=2.0)
    {
        field.copyValuesFrom(initial);

        //COMPUTE NUMERICAL SOLUTION
        const double timePointBefore = 20.0;
        integrator.integrateOverATimeStep(timeStep, {&field}, {}, timePointBefore);

        //COMPUTE EXACT SOLUTION
        const Resolution res = domain.get_resolution();
        const double tN = timePointBefore;
        const double tNp1 = timePointBefore + timeStep;
        for(int i=0 ; i < res.x ; i++) {
            for(int j=0 ; j < res.y ; j++) {
                for(int k=0 ; k < res.z ; k++) {
                    const double C = (initial(i,j,k) + (1+tN)) / exp(tN);
                    answer(i,j,k) = C*exp(tNp1) - (1.0 + tNp1);
                }
            }
        }

        //COMPUTE ERROR
        timeSteps.push_back(timeStep);
        errors.push_back( computeMaxError(field, answer) );
    }

    //COMPUTE CONVERGENCE RATE
    std::vector<double>& ht  = timeSteps;
    std::vector<double>& err = errors;
    std::vector<double> slopes = {};
    for(int iErr=0 ; iErr < errors.size()-1 ; iErr++ ) //[check] check that vector.size() is the right function to call.
        { slopes.push_back( log(err[iErr+1]/err[iErr]) / log(ht[iErr+1]/ht[iErr]) ); }
    
    //PASS (OR FAIL) TEST
    for(double slope : slopes)
        { ASSERT_TRUE(slope > 1.0); }

#if 0
    //BONUS: PRINT CONVERGENCE RESULTS
    slopes.push_back(0.0); // Append an extra atom in vector to prevent overflow //[check] replace 0.0 with nan
    std::cout << "timeStep | error | slope 1 iter | slope n iters" << std::endl;
    for(int iErr=0 ; iErr < errors.size() ; iErr++ ) //[check] check that vector.size() is the right function to call.
    {
        //[check] improve formating of float numbers
        //[check] add proof tthat if slope for 1 iter is 1, then slope for n iters is s-1
        std::cout << timeSteps[iErr] << ", " << errors[iErr] << ", "
                  << slopes[iErr] << ", " << slopes[iErr] - 1 << std::endl;
    }
#endif
}

