#ifndef DYNSOLVER_GUI_CONSOLE_FRAME_H_
#define DYNSOLVER_GUI_CONSOLE_FRAME_H_

#include "gui/generated.h"

#include "gui/model.h"

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
  virtual void solutions_data_view_ctrl_on_selection_changed(wxDataViewEvent&) override;
  virtual void singular_points_data_view_ctrl_on_selection_changed(wxDataViewEvent&) override;
  virtual void solutions_edit_button_on_button_click(wxCommandEvent&) override;
  virtual void solutions_delete_button_on_button_click(wxCommandEvent&) override;
  virtual void singular_points_delete_button_on_button_click(wxCommandEvent&) override;
  virtual void solution_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void singular_point_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void close_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void isocline_menu_item_on_menu_selection(wxCommandEvent&) override;

  // Tells the console frame that nothing is currently compiled.
  void set_no_compile();
  void set_yes_compile();

  // Updates the table of solutions/singular points
  void update_solutions_list();
  void update_singular_points_list();

  // For programatic selection and deselection. Note that simply calling
  // the deselect/select functions do not generate the selection changed
  // events automatically.
  void select_solution(solution_id id);
  void unselect_solution();
  void select_singular_point(singular_point_id id);
  void unselect_singular_point();
  
private:
  // Clears the list of equations in the data view ctrl and adds
  // the provided number of empty items to it. Requires that variables
  // is greater than or equal to 1.
  void update_equations_data_view_ctrl(int variables);

  void read_variables_combo_box_input();

  // Returns the id of the selected solution.
  solution_id get_selected_solution_id();

  singular_point_id get_selected_singular_point_id();

  // Called by the selection changed event handler.
  void process_select_solution();

  void process_select_singular_point();

  void clear_solution_color();
  void clear_singular_point_color();
};
} // namespace gui
} // namespace dynsolver

#endif
