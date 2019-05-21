#ifndef UTIL__MATH___HPP
#define UTIL__MATH___HPP

namespace fb
{

// It is not clear how efficient the standard abs(), min(), max() functionts are.
// Hence we implement our own.
template<typename T> static inline T abs(T arg)
    { return (arg >= 0.0 ? arg : -arg); }
template<typename T> static inline T min(T argA, T argB)
    { return (argA < argB ? argA : argB); }
template<typename T> static inline T min(T argA, T argB, T argC)
    { return min(min(argA, argB), argC); }
template<typename T> static inline T max(T argA, T argB)
    { return (argA > argB ? argA : argB); }
template<typename T> static inline T max(T argA, T argB, T argC)
    { return max(max(argA, argB), argC); }

template<uint pow>
inline double intPow(double arg)
    { return arg*intPow<pow-1>(arg); }
template<>
inline double intPow<0>(double arg)
    { return 1.0; }


static inline bool areFloatsEqual(const double argA, const double argB,
                                  const double tol=1e-4)
{
    const double maxVal = max(abs(argA),abs(argB));
    const bool areBothArgsCloseToZero = maxVal < 1e-10;
    if(areBothArgsCloseToZero)
        { return true; }
    else
    {
        const double relErr = abs(argA - argB) / maxVal;
        const bool areBothArgsSimilar = relErr < tol;
        return areBothArgsSimilar;
    }
}


}

#endif

