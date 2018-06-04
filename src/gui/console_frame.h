#ifndef DYNSOLVER_GUI_CONSOLE_FRAME_H_
#define DYNSOLVER_GUI_CONSOLE_FRAME_H_

#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

// Subclass of the console_frame_base implements the view controller for the
// main frame of the app.
class console_frame : public console_frame_base {
 private:
  app& appl;
  
 public:
  console_frame(app&);
};
} // namespace gui
} // namespace dynsolver

#endif
