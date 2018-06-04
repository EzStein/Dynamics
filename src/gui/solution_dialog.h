#ifndef DYNSOLVER_GUI_SOLUTION_DIALOG_H_
#define DYNSOLVER_GUI_SOLUTION_DIALOG_H_

#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

class solution_dialog : public solution_dialog_base {
 private:
  app& appl;
  
 public:
  solution_dialog(app&);
};
} // namespace gui
} // namespace dynsolver

#endif
