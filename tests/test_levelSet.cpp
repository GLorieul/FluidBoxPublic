
#include <gtest/gtest.h>
#include "levelSet.hpp"
using namespace fb;

TEST(levelSetReinitializer, test)
{
    Domain domain(Resolution(9,9), Point(-1.0,-1.0), Distance(2.0,2.0));
    LevelSet levelSet(domain, Anchor("C"), GhostCells(1,1,1,1));
    LevelSet answer(domain, Anchor("C"), GhostCells(0,0,0,0));

    //INITIALISE FIELDS
    for(int i=0 ; i < domain.get_resolution().x ; i++) {
        for(int j=0 ; j < domain.get_resolution().y ; j++) {
            Point here = levelSet.get_positionOfNode(i,j);
            levelSet(i,j) = 2.0*(sqrt(here.x*here.x + here.y*here.y) - 0.5);
        }
    }
//  levelSet.display();

    //REINITIALIZE LEVEL SET
    LevelSetReinitializer reinitializer(levelSet);
    reinitializer.reinitialize(levelSet, /*distanceToReinit*/ 1.0);

#if 0
    Resolution res = domain.get_resolution();
    for(int i = 0 ; i < res.x ; i++) {
        for(int j = 0 ; j < res.y ; j++) {
            for(int k = 0 ; k < res.z ; k++) {
                levelSet(i,j,k) *= 100000.0;
            }
        }
    }
#endif

    levelSet.display();

    //INITIALIZE ANSWER
    answer.setValues({{+0.1863, +0.2575, +0.2604, +0.2564, +0.2548, +0.2564, +0.2604, +0.2575, +0.1863},
                      {+0.2575, +0.4558, +0.3651, +0.2320, +0.1835, +0.2320, +0.3651, +0.4558, +0.2575},
                      {+0.2604, +0.3651, +0.1634, -0.0019, -0.0530, -0.0019, +0.1634, +0.3651, +0.2604},
                      {+0.2564, +0.2320, -0.0019, -0.1727, -0.2756, -0.1727, -0.0019, +0.2320, +0.2564},
                      {+0.2548, +0.1835, -0.0530, -0.2756, -0.4560, -0.2756, -0.0530, +0.1835, +0.2548},
                      {+0.2564, +0.2320, -0.0019, -0.1727, -0.2756, -0.1727, -0.0019, +0.2320, +0.2564},
                      {+0.2604, +0.3651, +0.1634, -0.0019, -0.0530, -0.0019, +0.1634, +0.3651, +0.2604},
                      {+0.2575, +0.4558, +0.3651, +0.2320, +0.1835, +0.2320, +0.3651, +0.4558, +0.2575},
                      {+0.1863, +0.2575, +0.2604, +0.2564, +0.2548, +0.2564, +0.2604, +0.2575, +0.1863}});
    answer.display();
    answer.displayAsImage( ColorMap(Range(-1.0,1.0),"RdBu_r") );
    // [check] improve values of answer by displaying results in scientific notation (and hence lower the tolerance)

    //PASS OR FAIL TEST
    ASSERT_TRUE(answer.equals(levelSet, 5e-2));
}


