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
  virtual ~console_frame();
  virtual void compile_button_on_button_click(wxCommandEvent&) override;
  virtual void variables_combo_box_on_combo_box(wxCommandEvent&) override;
  virtual void variables_combo_box_on_text_enter(wxCommandEvent&) override;
private:
  // Clears the list of equations in the data view ctrl and adds
  // the provided number of empty items to it. Requires that variables
  // is greater than or equal to 1.
  void update_equations_data_view_ctrl(int variables);
};
} // namespace gui
} // namespace dynsolver

#endif
