#ifndef DOMAIN___HPP
#define DOMAIN___HPP

#include <string>
#include "util/all.hpp"

namespace fb
{

struct Resolution
{
public:
    Resolution() = default;
    Resolution(int x, int y=1, int z=1) : x(x), y(y), z(z) {/*Do nothing*/}
    const bool operator== (const Resolution& other) const
    {
        return (this->x == other.x)
           and (this->y == other.y)
           and (this->z == other.z);
    }
    const bool operator!= (const Resolution& other) const
        { return not (*this == other); }
    operator std::vector<int>()
        { return std::vector<int>({x,y,z}); }
    operator std::vector<unsigned long long>()
    {
        unsigned long long xCast = static_cast<unsigned long long>(x);
        unsigned long long yCast = static_cast<unsigned long long>(y);
        unsigned long long zCast = static_cast<unsigned long long>(z);
        return std::vector<unsigned long long>({xCast,yCast,zCast});
    }

    std::string toString() const
    {
        return std::string("(") + std::to_string(x) + ","
                                + std::to_string(y) + ","
                                + std::to_string(z) + ")";
    }

    int x,y,z;
};

class Domain
{
public:
    Domain() = default;
    Domain(Resolution res, Point origin, Distance length);
    int get_nbOfCells() const {return res_.x*res_.y*res_.z;}
    Resolution get_resolution() const {return res_;}
    Point get_origin() const {return origin_;}
    Distance get_length() const {return length_;}
    Distance get_gridSpacing() const
    {
        return Distance(length_.x / ((double) res_.x),
                        length_.y / ((double) res_.y),
                        length_.z / ((double) res_.z));
    }

    bool operator== (const Domain& other) const
    {
        return (this->res_ == other.res_)
           and (this->origin_ == other.origin_)
           and (this->length_ == other.length_);
    }
    bool operator!= (const Domain& other) const
        { return not (*this == other); }

    std::string toString() const
    {
        return std::string("Resolution = ") + res_.toString()    + ", "
                         + "Origin = "      + origin_.toString() + ", "
                         + "Length = "      + length_.toString();
    }

    // In order to find out whether the domain is one-, two-, or three-
    // dimensional, it is safer to test the grid spacing than the resolution.
    // Because a domain with a z-resolution of 1 might be a flat 3D domain,
    // whereas a domain with a zero z-grid-spacing can only be 2D
    bool isOneDimensional()
    {
        return (   (not areFloatsEqual(get_gridSpacing().x, 0.0))
               and      areFloatsEqual(get_gridSpacing().y, 0.0) 
               and      areFloatsEqual(get_gridSpacing().z, 0.0)  );
    }
    bool isTwoDimensional()
    {
        return (   (not areFloatsEqual(get_gridSpacing().x, 0.0))
               and (not areFloatsEqual(get_gridSpacing().y, 0.0))
               and      areFloatsEqual(get_gridSpacing().z, 0.0)  );
    }
    bool isThreeDimensional()
    {
        return (   (not areFloatsEqual(get_gridSpacing().x, 0.0))
               and (not areFloatsEqual(get_gridSpacing().y, 0.0))
               and (not areFloatsEqual(get_gridSpacing().z, 0.0)) );
    }

private:
    Resolution res_;
    Point      origin_;
    Distance   length_;
};

}

#endif

