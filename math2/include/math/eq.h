///
/// \file eq.h
/// \author Ezra Stein
/// Interface class for equivalence relations.
///
#ifndef DYNSOLVER_MATH_EQ_H_
#define DYNSOLVER_MATH_EQ_H_

/// Project wide namespace for the dynsolver application.
/// All project entities are contained within this namespace.
namespace dynsolver {

/// A namespace for the math submodule of dynsolver.
/// This namespace contains all entities for numerical
/// algorithms and mathematical abstractions.
namespace math {

/// \c eq is an interface for implementing an equivalence class relation
/// on a set of type \c T.
/// \tparam T The type of the objects on which a notion of
/// equivalence is defined.
template<class T> class eq {
 public:
  /// Implementation of an equality check.
  /// \param a The left side of the equality check.
  /// \param b The right side of the equality check.
  /// \return \c true if \p a equals \p b. \c false otherwise.
  /// \invariant The equality operator must satisfy all the
  /// properties of an equivalence relation.
  /// - Reflexivity: We require that `equal(a, a)`
  /// - Symmetry: If `equal(a, b)` then `equal(b, a)`
  /// - Transitivity: If `equal(a, b)` and `equal(b, c)` then `equal(a, c)`
  virtual bool equal(const T& a, const T& b) const = 0;
};
} // namespace math
} // namespace dynsolver   
#endif
