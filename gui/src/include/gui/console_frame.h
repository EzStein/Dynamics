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

  // Holds the value of the variables/parameters text ctrl string and value.
  // It is updated only when a new value is validated.
  int variables;
  int parameters;
  std::string variablesTextCtrlValue;
  std::string parametersTextCtrlValue;

 public:
  console_frame(app&);
  virtual ~console_frame();
  virtual void compile_button_on_button_click(wxCommandEvent&) override;
  virtual void console_frame_on_close(wxCloseEvent&) override;
  virtual void variable_names_data_view_ctrl_on_value_changed(wxDataViewEvent&) override;
  virtual void variables_text_ctrl_on_text_enter(wxCommandEvent&) override;
  virtual void parameters_text_ctrl_on_text_enter(wxCommandEvent&) override;
  virtual void new_dynamical_menu_item_on_selection(wxCommandEvent&) override;
  virtual void close_dynamical_menu_item_on_selection(wxCommandEvent&) override;
  virtual void new_parameter_menu_item_on_selection(wxCommandEvent&) override;
  virtual void close_parameter_menu_item_on_selection(wxCommandEvent&) override;
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
  virtual void isoclines_edit_button_on_button_click(wxCommandEvent&) override;
  virtual void isoclines_delete_button_on_button_click(wxCommandEvent&) override;
  virtual void isoclines_data_view_ctrl_on_selection_changed(wxDataViewEvent&) override;
  virtual void van_der_pol_example_on_menu_selection(wxCommandEvent&) override;
  virtual void separatrices_data_view_ctrl_on_selection_changed(wxDataViewEvent&) override;
  virtual void hopf_bifurcations_data_view_ctrl_on_selection_changed(wxDataViewEvent&) override;
  virtual void saddle_node_bifurcations_data_view_ctrl_on_selection_changed(wxDataViewEvent&) override;

  virtual void separatrices_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void hopf_bifurcation_menu_item_on_selection(wxCommandEvent&) override;
  virtual void saddle_node_bifurcation_menu_item_on_selection(wxCommandEvent&) override;
  virtual void limit_cycle_bifurcation_menu_item_on_selection(wxCommandEvent&) override;
  virtual void saddle_connection_bifurcation_menu_item_on_selection(wxCommandEvent&) override;
  virtual void draw_all_separatrices_menu_item_on_menu_selection(wxCommandEvent&) override;

  virtual void equationsDataViewCtrlOnDataViewListCtrlSelectionChanged(wxDataViewEvent&) override;
  virtual void equationsDataViewCtrlOnDataViewListCtrlItemActivated(wxDataViewEvent&) override;
  virtual void equationsDataViewCtrlOnDataViewListCtrlItemStartEditing(wxDataViewEvent&) override;
  virtual void equationsDataViewCtrlOnDataViewListCtrlItemEditingStarted(wxDataViewEvent&) override;
  virtual void equationsDataViewCtrlOnDataViewListCtrlItemEditingDone(wxDataViewEvent&) override;

  // Tells the console frame that nothing is currently compiled.
  void set_no_compile();
  void set_yes_compile();

  // Updates the table of solutions/singular points
  void update_solutions_list();
  void update_singular_points_list();
  void update_isoclines_list();
  void update_parameters_data_view_ctrl();
  void update_separatrices_list();
  void update_hopf_bifurcation_list();
  void update_saddle_node_bifurcation_list();
  void update_limit_cycle_bifurcation_list();
  void update_saddle_connection_bifurcation_list();

private:
  // Returns the id of the selected solution.
  solution_id get_selected_solution();
  singular_point_id get_selected_singular_point();
  isocline_id get_selected_isocline();
  separatrix_id get_selected_separatrix();
  hopf_bifurcation_id get_selected_hopf_bifurcation();
  saddle_node_bifurcation_id get_selected_saddle_node_bifurcation();
  limit_cycle_bifurcation_id get_selected_limit_cycle_bifurcation();
  saddle_connection_bifurcation_id get_selected_saddle_connection_bifurcation();

  void parameters_text_ctrl_process_enter();
  void variables_text_ctrl_process_enter();
};
} // namespace gui
} // namespace dynsolver

#endif
