#ifndef DYNSOLVER_GUI_COMMON_H_
#define DYNSOLVER_GUI_COMMON_H_

#include <cstddef>

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
typedef size_t separatrix_id;
typedef size_t hopf_bifurcation_id;
typedef size_t saddle_node_bifurcation_id;
typedef size_t limit_cycle_bifurcation_id;
typedef size_t saddle_connection_bifurcation_id;
typedef size_t periodic_solution_id;
} // namespace gui
} // namespace dynsolver
#endif
