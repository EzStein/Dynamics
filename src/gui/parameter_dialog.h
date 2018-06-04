#ifndef DYNSOLVER_GUI_PARAMETER_DIALOG_H_
#define DYNSOLVER_GUI_PARAMETER_DIALOG_H_

#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

class parameter_dialog : public parameter_dialog_base {
 private:
  app& appl;
  
 public:
  parameter_dialog(app&);
};
} // namespace gui
} // namespace dynsolver

#endif
