#ifndef FIELD___HPP
#define FIELD___HPP

//[check] Make unit tests for ghost node values (can't do that now because google test is not installed + no access to the internet)

#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include "domain.hpp"

namespace fb
{

struct Anchor
{
public:
    Anchor() : anchor("Undefined") {/*Do nothing*/};
    Anchor(std::string anchor) : anchor(anchor) {/*Do nothing*/}

    const bool operator== (const Anchor& other) const
        { return this->anchor == other.anchor; }
    const bool operator!= (const Anchor& other) const
        { return not (*this == other); }

    Distance get_nodeOffset(const Distance& gridSpacing) const;
    std::string toString() const {return anchor;}

    std::string anchor;
};

struct GhostCells
{
public:
    GhostCells() = default;
    GhostCells(int lft, int rgh, int bot=0, int top=0, int dwn=0, int upw=0)
        :lft(lft), rgh(rgh), bot(bot), top(top), dwn(dwn), upw(upw)
        {/*Do nothing*/};
    operator std::vector<int>() const
        { return std::vector<int>({lft, rgh, bot, top, dwn, upw}); }
    std::string toString() const
    {
        return std::string("(") + std::to_string(lft) + ","
                                + std::to_string(rgh) + " , "
                                + std::to_string(bot) + ","
                                + std::to_string(top) + " , "
                                + std::to_string(dwn) + ","
                                + std::to_string(upw) + ")";
    }
    int lft, rgh, bot, top, dwn, upw;
};

class Field
{
public:
    Field() = default;
    Field(Domain& domain, Anchor anchor,
          GhostCells ghostCells=GhostCells(0,0,0,0,0,0));
    Field(const Field& other);

    /* Why the `clone()` function is useful:
     * Imagine:
     *   - A calculation module that requires a buffer copy of field `myField`
     *   - Object `myField` is of a derived class of `Field`, e.g. `LevelSet`
     *   - The calculations call virtual method `applyBoundaryConditions()`
     *
     * If one creates the buffer as
     *     Field* buffer_(new Field(myField));
     * Then `buffer_.applyBoundaryCondition()` will call
     * `Field::applyBoundaryCondition()` instead of
     * `LevelSet::applyBoundaryCondition()`
     *
     * We would want something more like
     *     Field* buffer_(new LevelSet(myField));
     * Except that if the calculation module is unaware of the derived type of
     * `myField` (as is the case of the `TimeIntegrator*` classes),
     * then it cannot know before hand that consturctor to call is `LevelSet`
     *
     * [ToDo] Use smart pointers instead, see
     * https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/
    */
    virtual Field* clone() const
        { return new Field(*this); }

    //Direct access to data
    inline double& operator[] (const int iData) {return data_[iData];}
    inline const double& operator[] (const int iData) const {return data_[iData];}

    //(i,j,k) indexed access to data
    inline double& operator() (const int i, const int j=0, const int k=0)
    {
        const int iData = dataOffIni_ + dataOffX_*i + dataOffY_*j + dataOffZ_*k;
        #ifdef BOUNDS_CHECKING
        performBoundChecking_(iData, i,j,k);
        #endif
        return data_[iData]; //[check] check that using dataOffZ_ does not slow it down (should not because it is a constexpr)
    }
    inline const double& operator() (const int i, const int j=0, const int k=0) const
    {
        const int iData = dataOffIni_ + dataOffX_*i + dataOffY_*j + dataOffZ_*k;
        #ifdef BOUNDS_CHECKING
        performBoundChecking_(iData, i,j,k);
        #endif
        return data_[iData]; //[check] check that using dataOffZ_ does not slow it down (should not because it is a constexpr)
    }
    void performBoundChecking_(const int iData, const int i, const int j,
                               const int k) const
    {
        if(not isIndexWithinBounds_(iData) )
        {
            std::cerr << std::string("Error: index is beyond field's boundaries!\n")
                       + "\t(i,j,k) = (" + std::to_string(i) + ","
                       + std::to_string(j) + "," + std::to_string(k) + ")\n"
                       + "\tResolution = " + domain_.get_resolution().toString() + "\n"
                       + "\tGhost nodes = " + ghostCells_.toString() << std::endl;
            abort();
        }
    }
    bool isIndexWithinBounds_(const int iData) const
        { return (iData >= 0) and (iData < this->get_nbOfCells()); }

    Point get_positionOfNode(const int i, const int j=0, const int k=0) const;

    virtual void applyBoundaryConditions() { /*Do nothing*/ }

    bool equals(const Field& other, const double tol=1e-4) const;
    bool operator== (const Field& other) const
        { return this->equals(other); }
    bool operator!= (const Field& other) const
        { return not (*this == other); }

    void setValues(double fieldValues); //"Same value everywhere" variant
    void setValues( // One-dimensional variant
            std::initializer_list<double> fieldValues);
    void setValues( // Two-dimensional variant
            std::initializer_list<
                std::initializer_list<double>> fieldValues);
    void setValues( // Three-dimensional variant
            std::initializer_list<
                std::initializer_list<
                    std::initializer_list<double>>> fieldValues);
    void copyValuesFrom(const Field& other); //Copies values at ghost nodes as well as those at physical nodes //[check] split into copyAllValuesFrom() and copyPhysicalValuesFrom()

    void saveAs(const std::string fileName, const std::string quantity="Unknown",
                const std::string units="Unknown") const; //[ToDo] Add std::list(Property) object as argument, where Property object is an object that can hold string, int or double.
    void display() const;
    void displayAsImage(ColorMap cm) const;
    void displayAsImage(ColorMap cm, char sliceDirection, int sliceIndex) const;

    //Returns number of ghost + physical nodes (= size of data_)
    int get_nbOfCells() const;
    //Returns number of physical nodes (*not* size of data_)
    int get_nbOfPhysicalCells() const 
        {return domain_.get_nbOfCells();}
    const Domain& get_domain() const { return domain_; }
    const Anchor& get_anchor() const { return anchor_; }

    bool isOneDimensional()   const {return domain_.isOneDimensional();  }
    bool isTwoDimensional()   const {return domain_.isTwoDimensional();  }
    bool isThreeDimensional() const {return domain_.isThreeDimensional();}
protected:
    Domain&     domain_;
    Anchor      anchor_;
    GhostCells  ghostCells_;

    //Array storing values of field at grid nodes
    std::unique_ptr<double[]> data_;
    //Offset in index between a column/line/plane and the next
    int dataOffX_, dataOffY_;
    static constexpr int dataOffZ_ = 1;
    //Index of first physical nodes (zero if no ghost nodes)
    int dataOffIni_;
};


// List of field names
class MassDensity : public Field
{
    using Field::Field;
    virtual MassDensity* clone() const override
        { return new MassDensity(*this); }
};
class Viscosity : public Field
{
    using Field::Field;
    virtual Viscosity* clone() const override
        { return new Viscosity(*this); }
};
//class Velocity    : public Field {using Field::Field;}; //Tricky one: is vector field, not a scalar field…

}

#endif

