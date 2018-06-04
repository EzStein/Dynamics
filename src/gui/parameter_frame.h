#ifndef DYNSOLVER_GUI_PARAMETER_FRAME_H_
#define DYNSOLVER_GUI_PARAMETER_FRAME_H_

#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

// Subclass of the parameter_frame_base implements the view controller for
// each instance of a parameter frame.
class parameter_frame : public parameter_frame_base {
 private:
  app& appl;
  int id;
  
 public:
  parameter_frame(app& app, int id);
};
} // namespace gui
} // namespace dynsolver

#endif
