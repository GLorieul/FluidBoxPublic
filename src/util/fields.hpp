#ifndef UTIL_FIELDS___HPP
#define UTIL_FIELDS___HPP

#include <cmath>

namespace fb
{

class Field;

//[check] move util files into "util/" directory (util/misc.hpp + util/field.hpp + util/error.hpp)

double computeL1Error(const Field& A, const Field& B);
double computeL2Error(const Field& A, const Field& B);
double computeMaxError(const Field& A, const Field& B);

}

#endif

