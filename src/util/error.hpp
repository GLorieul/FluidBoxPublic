#ifndef UTIL_ERROR___HPP
#define UTIL_ERROR___HPP

namespace fb
{

class Distance;
class Field;

[[noreturn]] void
raiseError(const std::string file, const int lineNb,
           const std::string functionName, const std::string message);

void assume(const std::string file, const int lineNb,
            const std::string functionName, bool proposition,
            const std::string messageIfFalse="");

void assume_isNotThreeDimensional(const std::string file, const int lineNb,
                                  const std::string functionName,
                                  const Field& field);
void assume_gridIsIsotropic(const std::string file, const int lineNb,
                   const std::string functionName, const Distance& gridSpacing);
void assume_shareTheSameDomain(const std::string file, const int lineNb,
        const std::string functionName, const Field& A, const Field& B);
void assume_shareTheSameAnchor(const std::string file, const int lineNb,
        const std::string functionName, const Field& A, const Field& B);

}

#endif

