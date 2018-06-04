#ifndef DYNSOLVER_GUI_DYNAMICAL_DIALOG_H_
#define DYNSOLVER_GUI_DYNAMICAL_DIALOG_H_

#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

class dynamical_dialog : public dynamical_dialog_base {
 private:
  app& appl;
  
 public:
  dynamical_dialog(app&);
};
} // namespace gui
} // namespace dynsolver

#endif
