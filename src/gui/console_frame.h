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

  // Holds the value of the variables combo box.
  // It is updated only when a new value is validated.
  std::string variablesComboBoxValue;
  
 public:
  console_frame(app&);
  virtual ~console_frame();
  virtual void compile_button_on_button_click(wxCommandEvent&) override;
  virtual void variables_combo_box_on_combo_box(wxCommandEvent&) override;
  virtual void variables_combo_box_on_text_enter(wxCommandEvent&) override;
  virtual void variables_combo_box_on_kill_focus(wxFocusEvent&) override;
  virtual void console_frame_on_close(wxCloseEvent&) override;
  virtual void new_dynamical_window_menu_item_on_selection(wxCommandEvent&) override;
  virtual void close_dynamical_windows_menu_item_on_selection(wxCommandEvent&) override;
  virtual void lorenz_example_menu_item_on_menu_selection(wxCommandEvent&) override;

  // Tells the console frame that nothing is currently compiled.
  void set_no_compile();
  void set_yes_compile();
  
private:
  // Clears the list of equations in the data view ctrl and adds
  // the provided number of empty items to it. Requires that variables
  // is greater than or equal to 1.
  void update_equations_data_view_ctrl(int variables);

  void read_variables_combo_box_input();
};
} // namespace gui
} // namespace dynsolver

#endif
