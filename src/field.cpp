
#include <iostream>
#include "field.hpp"
#include "util/all.hpp"
#include "util/display.hpp"
#include "wrapper_hdf5.hpp"

namespace fb
{

// IMPLEMENTATION OF ANCHOR STRUCT

Distance Anchor::get_nodeOffset(const Distance& gridSpacing) const
{
    const Distance& h = gridSpacing;
    // DOWNWARD PLANE
    if     (anchor=="DSW") { return Distance(0.0    , 0.0    , 0.5*h.z); }
    else if(anchor=="DS" ) { return Distance(0.5*h.x, 0.0    , 0.5*h.z); }
    else if(anchor=="DSE") { return Distance(    h.x, 0.0    , 0.5*h.z); }
    else if(anchor=="DW" ) { return Distance(0.0    , 0.5*h.y, 0.5*h.z); }
    else if(anchor=="DC" ) { return Distance(0.5*h.x, 0.5*h.y, 0.5*h.z); }
    else if(anchor=="DE" ) { return Distance(    h.x, 0.5*h.y, 0.5*h.z); }
    else if(anchor=="DNW") { return Distance(0.0    ,     h.y, 0.5*h.z); }
    else if(anchor=="DN" ) { return Distance(0.5*h.x,     h.y, 0.5*h.z); }
    else if(anchor=="DNE") { return Distance(    h.x,     h.y, 0.5*h.z); }
    // CENTER PLANE
    else if(anchor=="SW") { return Distance(0.0    , 0.0    , 0.5*h.z); }
    else if(anchor=="S" ) { return Distance(0.5*h.x, 0.0    , 0.5*h.z); }
    else if(anchor=="SE") { return Distance(    h.x, 0.0    , 0.5*h.z); }
    else if(anchor=="W" ) { return Distance(0.0    , 0.5*h.y, 0.5*h.z); }
    else if(anchor=="C" ) { return Distance(0.5*h.x, 0.5*h.y, 0.5*h.z); }
    else if(anchor=="E" ) { return Distance(    h.x, 0.5*h.y, 0.5*h.z); }
    else if(anchor=="NW") { return Distance(0.0    ,     h.y, 0.5*h.z); }
    else if(anchor=="N" ) { return Distance(0.5*h.x,     h.y, 0.5*h.z); }
    else if(anchor=="NE") { return Distance(    h.x,     h.y, 0.5*h.z); }
    // UPWARD PLANE
    else if(anchor=="USW") { return Distance(0.0    , 0.0    , 0.5*h.z); }
    else if(anchor=="US" ) { return Distance(0.5*h.x, 0.0    , 0.5*h.z); }
    else if(anchor=="USE") { return Distance(    h.x, 0.0    , 0.5*h.z); }
    else if(anchor=="UW" ) { return Distance(0.0    , 0.5*h.y, 0.5*h.z); }
    else if(anchor=="UC" ) { return Distance(0.5*h.x, 0.5*h.y, 0.5*h.z); }
    else if(anchor=="UE" ) { return Distance(    h.x, 0.5*h.y, 0.5*h.z); }
    else if(anchor=="UNW") { return Distance(0.0    ,     h.y, 0.5*h.z); }
    else if(anchor=="UN" ) { return Distance(0.5*h.x,     h.y, 0.5*h.z); }
    else if(anchor=="UNE") { return Distance(    h.x,     h.y, 0.5*h.z); }
    else { raiseError(__FILE__, __LINE__, __FUNCTION__, "Illegal anchor value, anchor = " + anchor); }
}


// IMPLEMENTATION OF FIELD CLASS

Field::Field(Domain& domain, Anchor anchor,
             GhostCells ghostCells)
    :domain_(domain), anchor_(anchor), ghostCells_(ghostCells),
     data_ (new double[this->get_nbOfCells()])
{
    const GhostCells& gh = ghostCells;
    Resolution domRes = domain.get_resolution();
    Resolution fieldRes(domRes.x + gh.lft + gh.rgh,
                        domRes.y + gh.bot + gh.top,
                        domRes.z + gh.dwn + gh.upw);
    dataOffX_ = fieldRes.y*fieldRes.z;
    dataOffY_ = fieldRes.z;
    dataOffIni_ = dataOffX_*gh.lft + dataOffY_*gh.bot + dataOffZ_*gh.dwn;
    this->setValues(0.0);
}

Field::Field(const Field& other)
    :Field(other.domain_, other.anchor_, other.ghostCells_)
{ /*Do nothing*/ }

int Field::get_nbOfCells() const
{
    const Resolution& domRes = domain_.get_resolution();
    const GhostCells& gh = ghostCells_;
    const int resX = domRes.x + gh.lft + gh.rgh;
    const int resY = domRes.y + gh.bot + gh.top;
    const int resZ = domRes.z + gh.dwn + gh.upw;
    return resX*resY*resZ;
}


void Field::setValues(double fieldValues)
{
    for(int iCell=0; iCell < this->get_nbOfCells(); iCell++)
        { data_[iCell] = 0.0; }
}
void Field::setValues(std::initializer_list<double> fieldValues)
{
    int iData = 0;
    for(double val : fieldValues)
    {
        data_[iData] = val;
        iData++;
    }
}
void Field::setValues(
        std::initializer_list<
            std::initializer_list<double>> fieldValues)
{
    const GhostCells& gh = ghostCells_;
    const Resolution& res = domain_.get_resolution();
    const int iFirst = -gh.lft;
    const int jLast = res.y-1 + gh.top;
    int i = iFirst;
    int j = jLast;
    for(auto lineVals : fieldValues) {
        for(double val : lineVals)
        {
            (*this)(i,j) = val;
            i++;
        }
        i=iFirst;
        j--;
    }
}
void Field::setValues(
        std::initializer_list<
            std::initializer_list<
                std::initializer_list<double>>> fieldValues)
{
    const GhostCells& gh = ghostCells_;
    const Resolution& res = domain_.get_resolution();
    const int iFirst = -gh.lft;
    const int jLast = res.y-1 + gh.top;
    const int kLast = res.z-1 + gh.upw;
    int i = iFirst;
    int j = jLast;
    int k = kLast;
    for(auto planeVals : fieldValues) {
        for(auto lineVals : planeVals) {
            for(double val : lineVals)
            {
//              std::cout << "(" + std::to_string(i) + "," + std::to_string(j) + ") = " + std::to_string(val) << std::endl;
                (*this)(i,j,k) = val;
                i++;
            }
            i = iFirst;
            j--;
        }
        j = jLast;
        k--;
    }
}
void Field::copyValuesFrom(const Field& other)
{
    // [check] create macro that injects FILE, LINE and FUNCTION directly
    if(this->anchor_ != other.anchor_)
    { raiseError(__FILE__, __LINE__, __FUNCTION__, "Both arrays must have the same anchor in order to copy from one another"); }

    if(this->domain_ != other.domain_)
    { raiseError(__FILE__, __LINE__, __FUNCTION__, "Both arrays must have the same domain in order to copy from one another"); }

    Resolution res = domain_.get_resolution();
    for(int iCell=0 ; iCell < get_nbOfCells() ; iCell++)
        { (*this)[iCell] = other[iCell]; }
}

Point Field::get_positionOfNode(const int i, const int j, const int k) const
{
    Distance h = domain_.get_gridSpacing();
    Distance anchOff = anchor_.get_nodeOffset(h);
    Point org = domain_.get_origin();
    return Point(org.x + i*h.x + anchOff.x,
                 org.y + j*h.y + anchOff.y,
                 org.z + k*h.z + anchOff.z);
}

bool Field::equals(const Field& other, const double tol) const
{
    const bool isMetadataEqual =   (this->domain_ == other.domain_)
                               and (this->anchor_ == other.anchor_);
    if(not isMetadataEqual)
        { return false; }

    Resolution res = domain_.get_resolution();
    //For each node in the physical domain
    for(int i=0; i < res.x ; i++) {
        for(int j=0; j < res.y ; j++) {
            for(int k=0; k < res.z ; k++)
            {
                if(not areFloatsEqual((*this)(i,j,k), other(i,j,k), tol) )
                    { return false; }
            }
        }
    }

    return true;
}

void Field::saveAs(const std::string fileName, const std::string quantityName,
                   const std::string units) const //[ToDo] Add std::list<Property> object as argument, where Property object is an object that can hold string, int or double. + idea: merge quantityName + units into list<Property>
{
    Hdf5File file(fileName);
    Hdf5Group root = file.getRoot();

    Hdf5Group grp_domain = root.createGroup("/Domain");
    grp_domain.createAttribute("Origin", domain_.get_origin(), "m");
    grp_domain.createAttribute("Length", domain_.get_length(), "m");
    grp_domain.createAttribute("Resolution", domain_.get_resolution());

//  [ToDo] Requires Property class to be implemented because time -> double, but other attributes might use different types. Wait for libconfig++ implementation before doing that.
    Hdf5Group grp_field = root.createGroup("/Field");
//  grp_context.createAttribute("TimePoint", 1.34, "s");
    grp_field.createAttribute("QuantityName", quantityName);
    grp_field.createAttribute("Units", units);

    const Resolution& domRes = domain_.get_resolution();
    const GhostCells& gh = ghostCells_;
    Resolution fieldRes(gh.lft + domRes.x + gh.rgh,
                        gh.bot + domRes.y + gh.top,
                        gh.dwn + domRes.z + gh.upw);
    Hdf5DataSet dataSet = grp_field.createDatasetFromCArray("ScalarData", data_.get(), fieldRes);
    dataSet.createAttribute("Anchor", anchor_.toString());
    dataSet.createAttribute("Ghost nodes", ghostCells_);
}




#include <cstdio>

//std::string horisontalLine_solid()
//    { return "─────────"; }

void drawTopDomainBoundary(const int resX, const int nbGhLft, const int nbGhRgh)
{
    // Left ghost nodes
    for(int iGhLft=0; iGhLft < nbGhLft; iGhLft++)
        { std::cout << "---------"; }
    // Top-left domain corner
    std::cout << " ┌";
    // Top domain boundary
    for(int i=0 ; i<resX ; i++)
        { std::cout << "─────────"; }
    // Top-right domain corner
    std::cout << "─┐ ";
    // Right ghost nodes
    for(int iGhRgh=0; iGhRgh < nbGhRgh; iGhRgh++)
        { std::cout << "---------"; }
    std::cout << std::endl;
}

void drawLftDomainBoundary(const int j, const int resY)
{
    if ((0 <= j) and (j < resY))
        { std::cout << " │"; }
    else
        { std::cout << " ┆"; }
}

void drawRghDomainBoundary(const int j, const int resY)
    { drawLftDomainBoundary(j, resY); }


void drawBottomDomainBoundary(const int resX, const int nbGhLft,
                              const int nbGhRgh)
{
    // Left ghost nodes
    for(int iGhLft=0; iGhLft < nbGhLft; iGhLft++)
        { std::cout << "---------"; }
    // Bottom-left domain corner
    std::cout << " └";
    // Bottom domain boundary
    for(int i=0 ; i<resX ; i++)
        { std::cout << "─────────"; }
    // Bottom-right domain corner
    std::cout << "─┘ ";
    // Right ghost nodes
    for(int iGhRgh=0; iGhRgh < nbGhRgh; iGhRgh++)
        { std::cout << "---------"; }
    std::cout << std::endl;
}

void drawUpwardDomainBoundary(const int iFirst, const int iLast)
{
    std::cout << "══";
    for(int i=iFirst ; i<=iLast ; i++)
        { std::cout << "═════════"; }
    std::cout << "═══";
    std::cout << "\n" << std::endl;
}

void drawDownwardDomainBoundary(const int iFirst, const int iLast)
    { drawUpwardDomainBoundary(iFirst, iLast); }


void Field::display() const //[check] write unit tests (can't access internet to install google-test now)
{
    Resolution res = domain_.get_resolution();
    const GhostCells& gh = ghostCells_;
    const int iFirst = -gh.lft;
    const int jFirst = -gh.bot;
    const int kFirst = -gh.dwn;
    const int iLast = res.x-1 + gh.rgh;
    const int jLast = res.y-1 + gh.top;
    const int kLast = res.z-1 + gh.upw;
    const bool isThreeDimensional = domain_.isThreeDimensional();
//  std::cout << std::scientific;
//  std::cout << std::float;
//  std::cout << std::precision(3);

    for(int k=kLast ; k>=kFirst ; k--)
    {
        const bool areUpwGhostCellsPast = (k==res.z-1);
        if(areUpwGhostCellsPast and isThreeDimensional)
            { drawUpwardDomainBoundary(iFirst, iLast); }

        for(int j=jLast ; j>=jFirst ; j--)
        {
            const bool areTopGhostCellsPast = (j==res.y-1);
            if(areTopGhostCellsPast)
                { drawTopDomainBoundary(res.x, gh.lft, gh.rgh); }

            for(int i=iFirst ; i<=iLast ; i++)
            {
                const bool areLftGhostCellsPast = (i==0);
                if(areLftGhostCellsPast)
                    { drawLftDomainBoundary(j, res.y); }

//              std::cout << (*this)(i,j,k) << " "; //[check] write formats in the C++ style + use a function to adapt format to number value : decimal or exponent format depending on the greatness/smallness of the number.
//              printf("%+.4f ", (*this)(i,j,k));
                std::cout << " " << formatToFit((*this)(i,j,k));

                const bool areRghGhostCellsPast = (i==res.x-1);
                if(areRghGhostCellsPast)
                    { drawRghDomainBoundary(j, res.y); }
            }
            std::cout << std::endl;

            const bool areBotGhostCellsPast = (j==0);
            if(areBotGhostCellsPast)
                { drawBottomDomainBoundary(res.x, gh.lft, gh.rgh); }

        }

        std::cout << std::endl;
        const bool areDwnGhostCellsPast = (k==0);
        if(areDwnGhostCellsPast and isThreeDimensional)
            { drawDownwardDomainBoundary(iFirst, iLast); }
    }
}

#if 0
class ColorCli
{

private:
    color = ""
}
#endif

// How portable is this function?
// Should work in Linux, but what about OSX?
void Field::displayAsImage(ColorMap cm) const
{
    assume_isNotThreeDimensional(__FILE__, __LINE__, __FUNCTION__, *this);

    Resolution res = domain_.get_resolution();
    for(int j=0 ; j < res.y ; j++)
    {
        for(int i=res.x-1 ; i >= 0 ; i--)
        {
            std::string pxl = cm.writePixel((*this)(i,j));
            std::cout << pxl + pxl;
        }
        std::cout << useDefaultStyle() << std::endl;
    }
    std::cout << std::endl;

    std::cout << cm.getColorBar(24) << std::endl;
}

// In the case of a 3D Field, only a slice will be displayed
void Field::displayAsImage(ColorMap cm, char sliceDirection, int sliceIndex) const
{
    //[ToDo] To implement
}

}

