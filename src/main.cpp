
#include <iostream>
#include "field.hpp"
using namespace fb;

int main(void)
{
//  Domain domain(Resolution(9,6), Point(-1.0,-1.0), Distance(2.0,2.0));
    Domain domain(Resolution(64,64), Point(-1.0,-1.0), Distance(2.0,2.0));
    LevelSet levelSet(domain, Anchor("C"));

    LevelSet& ls = levelSet;
    for(int i=0 ; i < domain.get_resolution().x ; i++) {
        for(int j=0 ; j < domain.get_resolution().y ; j++) {
            const Point& here = ls.get_positionOfNode(i,j);
            ls(i,j) = sqrt(here.x*here.x + here.y*here.y) - 0.5;
        }
    }

    levelSet.display();
    levelSet.saveAs("/tmp/levelSet_ini.fbField");

//  FB_FOR_EACH_NODE ls as levelSet in Domain
//  FB_FOR_EACH_NODE( ls, dnst as {levelSet, massDnst} in Subdomain(Domain, {1,1,1,1}) )
//  {
//      ls = 3.0;
//  }
    return 0;
}

