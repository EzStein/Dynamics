#ifndef DYNSOLVER_GUI_SINGULAR_POINT_DIALOG_H_
#define DYNSOLVER_GUI_SINGULAR_POINT_DIALOG_H_

#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

class singular_point_dialog : public singular_point_dialog_base {
 private:
  app& appl;
  
 public:
  singular_point_dialog(app&);
};
} // namespace gui
} // namespace dynsolver

#endif
