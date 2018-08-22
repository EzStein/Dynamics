#ifndef MATH_EIGENVALUE_H_
#define MATH_EIGENVALUE_H_

#include <vector>

#include "math/vector.h"
#include "math/poly_root.h"

namespace math {

// Represents a single eigenvalue with its corresponding eigenvector.
// It contains a poly_root and a list a nonempty list of unit eigenvectors.
class eigenvalue {
private:
  // Eigenvalue
  poly_root value;

  // A list of unit, eigenvectors which are linearly independent.
  std::vector<math::vector> vectors;

public:
  eigenvalue(const poly_root&, const std::vector<math::vector>&);

  // Returns the eigenvalue.
  const poly_root& get_value() const;

  // Returns a list of linearly independent unit eigenvectors 
  const std::vector<math::vector>& get_vectors() const;
  
};
} // namespace math

#endif
