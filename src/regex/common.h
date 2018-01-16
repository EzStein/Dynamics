#ifndef DYNSOLVER_REGEX_COMMON_H_
#define DYNSOLVER_REGEX_COMMON_H_
#include <limits>
// This header holds the common dependencies of translation units in the
// dynsolver::regex namespace. In this way, we can prevent circular include
// dependencies and confusing forward declarations.
namespace dynsolver {
namespace regex {
// The maximum value that an unsigned char can attain.
constexpr int kMaxCharacterValue{std::numeric_limits<unsigned char>::max()};

// This variable stores the number of distinct character objects. It will
// always be one more than the largest unsigned character value.
static constexpr int kNumberOfCharacters{kMaxCharacterValue + 1};
} // namespace regex
} // namespace dynsolver

#endif
