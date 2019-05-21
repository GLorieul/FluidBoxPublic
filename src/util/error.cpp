
#include <iostream>
#include "util/struct.hpp"
#include "field.hpp"

namespace fb
{

[[noreturn]] void
raiseError(const std::string file, const int lineNb,
           const std::string functionName, const std::string message)
{
    const std::string lineNb_str = std::to_string(lineNb);
    std::cerr << "ERROR: " + file + ":" + lineNb_str + ", " + functionName + "()\n\t" + message << std::endl;
    abort();
}

void assume(const std::string file, const int lineNb,
            const std::string functionName, bool proposition,
            const std::string messageIfFalse)
{
    if(!proposition)
        { raiseError(file, lineNb, functionName, messageIfFalse); }
}

void assume_isNotThreeDimensional(const std::string file, const int lineNb,
                             const std::string functionName,
                             const Field& field)
{
    std::string nbDimensions = "";
    if     (field.isOneDimensional())   { nbDimensions = "one"; }
    else if(field.isTwoDimensional())   { nbDimensions = "two"; }
    else if(field.isThreeDimensional()) { nbDimensions = "three"; }
    else { raiseError(__FILE__, __LINE__, __FUNCTION__, "Field is not one-, two-, nor three-dimensional?!"); }

    const std::string errorMsg = std::string("")
            + "Field must not be three-dimensional, but it is.\n"
            + "\tDomain: " + field.get_domain().toString();
    assume(file, lineNb, functionName, not field.isThreeDimensional(), errorMsg);
}
void assume_shareTheSameDomain(const std::string file, const int lineNb,
        const std::string functionName, const Field& A, const Field& B)
{
    const Domain& domA = A.get_domain();
    const Domain& domB = B.get_domain();
    const bool doShareTheSameDomain = (domA == domB);
    const std::string errorMsg = std::string("")
                               + "Both fields must share the same domain\n"
                               + "\t" + domA.toString() + "\n"
                               + "\t" + domB.toString();
    assume(file, lineNb, functionName, doShareTheSameDomain, errorMsg);
}
void assume_shareTheSameAnchor(const std::string file, const int lineNb,
        const std::string functionName, const Field& A, const Field& B)
{
    const Anchor& anchA = A.get_anchor();
    const Anchor& anchB = B.get_anchor();
    const bool doShareTheSameAnchor = (anchA == anchB);
    const std::string errorMsg = std::string("")
            + "Both fields must share the same anchor\n"
            + "\t" + anchA.toString() + "!=" + anchB.toString();
    assume(file, lineNb, functionName, doShareTheSameAnchor, errorMsg);
}

void assume_gridIsIsotropic(const std::string file, const int lineNb,
        const std::string functionName, const Distance& gridSpacing)
{
    //FIND OUT IF IT IS ISOTROPIC
    bool isIsotropic;
    const bool isTwoDimensional = areFloatsEqual(gridSpacing.z, 0.0);
    const bool isOneDimensional = areFloatsEqual(gridSpacing.y, 0.0);
    if(isOneDimensional)
        {isIsotropic = true;}
    else if(isTwoDimensional)
        {isIsotropic = areFloatsEqual(gridSpacing.x,gridSpacing.y);}
    else //isThreeDimensional
    {
        isIsotropic = areFloatsEqual(gridSpacing.x,gridSpacing.y)
                  and areFloatsEqual(gridSpacing.x,gridSpacing.z);
    }

    //MANAGE ERROR MESSAGES IF NECESSARY
    const std::string errorMsg = std::string("")
                               + "The grid must be isotropic.\n"
                               + "\tgridSpacing = " + gridSpacing.toString();
    assume(file, lineNb, functionName, isIsotropic, errorMsg);
}

}

