#ifndef DYNSOLVER_GUI_PREFERENCES_DIALOG_H_
#define DYNSOLVER_GUI_PREFERENCES_DIALOG_H_

#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

class preferences_dialog : public preferences_dialog_base {
 private:
  app& appl;
  
 public:
  preferences_dialog(app&);
};
} // namespace gui
} // namespace dynsolver

#endif
