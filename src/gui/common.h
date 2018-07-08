#ifndef DYNSOLVER_GUI_COMMON_H_
#define DYNSOLVER_GUI_COMMON_H_

namespace dynsolver {
namespace gui {
// Common is used for common forward declarations in the dynsolver::gui
// namespace.

// A mapping from the id's of the dynamical/parameter windows to their
// gui frames.
typedef size_t dynamical_id;
typedef size_t parameter_id;
typedef size_t singular_point_id;
typedef size_t solution_id;
typedef size_t isocline_id;
} // namespace gui
} // namespace dynsolver
#endif
