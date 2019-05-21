#ifndef UTIL__STRUCT___HPP
#define UTIL__STRUCT___HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "util/error.hpp"
#include "util/math.hpp"

namespace fb
{

struct Point
{
public:
    Point() = default;
    Point(double x, double y=0.0, double z=0.0) : x(x), y(y), z(z) {/*Do nothing*/};

    const bool operator== (const Point& other) const
    {
        return areFloatsEqual(this->x, other.x)
           and areFloatsEqual(this->y, other.y)
           and areFloatsEqual(this->z, other.z);
    }
    const bool operator!= (const Point& other) const
        { return not (*this == other); }

    operator std::vector<double>() const
        { return std::vector<double>({x,y,z}); }

    std::string toString() const
    {
        return std::string("(") + std::to_string(x) + ","
                                + std::to_string(y) + ","
                                + std::to_string(z) + ")";
    }

    double x,y,z;
};

struct Distance
{
public:
    Distance() = default;
    Distance(double x, double y=0.0, double z=0.0) : x(x), y(y), z(z) {/*Do nothing*/};

    const bool operator== (const Distance& other) const
    {
        return areFloatsEqual(this->x, other.x)
           and areFloatsEqual(this->y, other.y)
           and areFloatsEqual(this->z, other.z);
    }
    const bool operator!= (const Distance& other) const
        { return not (*this == other); }

    operator std::vector<double>() const
        { return std::vector<double>({x,y,z}); }

    std::string toString() const
    {
        return std::string("(") + std::to_string(x) + ","
                                + std::to_string(y) + ","
                                + std::to_string(z) + ")";
    }

    double x,y,z;
};

struct Range
{
    Range(double min, double max) : min(min), max(max) {/*Do nothing*/}

    Range(std::vector<double> values)
        : Range(values[0], values[1])
    {/*Do nothing*/}

    double min;
    double max;
};

// Class that behaves like map<string,T> but which always perform error checking
// and displays the list of available keys if the key given is invalid.
template<typename T>
class Dictionary : public std::map<std::string, T>
{
public:
    using std::map<std::string, T>::map;

    T& operator[] (std::string key)
    {
        const bool isFound = (this->find(key) != this->end());
        if(isFound) { return this->at(key); }
        else        { raiseError_keyNotFound_(key); }
    }

    const T& operator[] (std::string key) const
    {
        const bool isFound = (this->find(key) != this->end());
        if(isFound) { return this->at(key); }
        else        { raiseError_keyNotFound_(key); }
    }

private:
    [[noreturn]] void raiseError_keyNotFound_(std::string key) const
    {
        std::string msg = "";
        msg += "Key not found in dictionary: key=\"" + key + "\"\n";
        msg += "\tAvailable keys: ";
        for(auto& pair : *this)
        {
            std::string legalKey = pair.first;
            msg += "\"" + legalKey + "\"";

            const bool isLastKey = (std::next(&pair) == &*this->end());
            if(not isLastKey)
                { msg += ", "; }
        }
        raiseError(__FILE__, __LINE__, __FUNCTION__, msg);
    }
};

}

#endif
