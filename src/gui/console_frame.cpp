#include "gui/console_frame.h"

#include <string>
#include <stdexcept>

#include <wx/msgdlg.h>

#include "gui/app.h"
#include "gui/dynamical_dialog.h"
#include "gui/singular_point_dialog.h"
#include "gui/solution_dialog.h"
#include "gui/isocline_dialog.h"

#include "util/util.h"

namespace dynsolver {
namespace gui {

console_frame::console_frame(app& app) : console_frame_base(nullptr, wxID_ANY, "Console"),
					 appl(app), variablesComboBoxValue("") {
  // Add in widgets and setup events not already done in console_frame_base.
  equationsDataViewCtrl->AppendTextColumn("Variable");
  equationsDataViewCtrl->AppendTextColumn("Expression", wxDATAVIEW_CELL_EDITABLE);

  solutionsDataViewCtrl->AppendTextColumn("ID");
  solutionsDataViewCtrl->AppendTextColumn("Increment");
  solutionsDataViewCtrl->AppendTextColumn("T Min");
  solutionsDataViewCtrl->AppendTextColumn("T Max");

  singularPointsDataViewCtrl->AppendTextColumn("ID");
  isoclinesDataViewCtrl->AppendTextColumn("ID");
  set_no_compile();
  solutionsEditButton->Disable();
  solutionsDeleteButton->Disable();
  singularPointsDeleteButton->Disable();
  isoclinesDeleteButton->Disable();
  isoclinesEditButton->Disable();
}

console_frame::~console_frame() { }

void console_frame::lorenz_example_menu_item_on_menu_selection(wxCommandEvent&) {
  variablesComboBox->SetValue("3");
  
  equationsDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  data.push_back(wxVariant("x1"));
  data.push_back(wxVariant("10*(x2-x1)"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();
  data.push_back(wxVariant("x2"));
  data.push_back(wxVariant("x1*(28-x3)-x2"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();
  data.push_back(wxVariant("x3"));
  data.push_back(wxVariant("x1*x2-x3*8/3"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();

  compileButton->Enable();
}

void console_frame::new_dynamical_window_menu_item_on_selection(wxCommandEvent&) {
  double width = 800;
  double height = 800;
  dynamical_specs spec;
  spec.viewport2d = math::window2d(math::vector2d(width, height),
				   math::vector2d(10, 10),
				   math::vector2d(-5, -5));
  
  spec.viewport3d = math::window3d(math::vector3d(10,10,10),
				   math::vector3d(-10,-10,-10),
				   math::vector3d(0,1,0),
				   1, 100, width, height);
  spec.dynamicalVars = appl.get_model().get_dynamical_vars();
  if(spec.dynamicalVars == 2) {
    spec.xAxisVar = 0;
    spec.yAxisVar = 1;
    spec.zAxisVar = 1;
    spec.horzAxisVar = 0;
    spec.vertAxisVar = 1;
  } else if(spec.dynamicalVars == 3) {
    spec.xAxisVar = 0;
    spec.yAxisVar = 1;
    spec.zAxisVar = 2;
    spec.horzAxisVar = 1;
    spec.vertAxisVar = 2;
  } else if(spec.dynamicalVars >= 4) {
    spec.xAxisVar = 1;
    spec.yAxisVar = 2;
    spec.zAxisVar = 3;
    spec.horzAxisVar = 1;
    spec.vertAxisVar = 2;
  } else {
    assert(false);
  }

  
  spec.is3d = false;
  if(appl.get_dynamical_dialog()->show_dialog(spec, &spec)) {
    appl.add_dynamical(spec);
  }
}

void console_frame::close_dynamical_windows_menu_item_on_selection(wxCommandEvent&) {
  appl.delete_all_dynamical_windows();
}

void console_frame::console_frame_on_close(wxCloseEvent& evt) {
  appl.close_application();
}

void console_frame::set_no_compile() {
  variablesComboBox->SetValue("");
  compileButton->Disable();
  equationsDataViewCtrl->DeleteAllItems();
  newDynamicalWindowMenuItem->Enable(false);
  solutionMenuItem->Enable(false);
  singularPointMenuItem->Enable(false);
  saveMenuItem->Enable(false);
  saveAsMenuItem->Enable(false);
  closeMenuItem->Enable(false);
  solutionsDataViewCtrl->DeleteAllItems();
  singularPointsDataViewCtrl->DeleteAllItems();
  isoclinesDataViewCtrl->DeleteAllItems();
}

void console_frame::set_yes_compile() {
  newDynamicalWindowMenuItem->Enable(true);
  solutionMenuItem->Enable(true);
  singularPointMenuItem->Enable(true);
  saveMenuItem->Enable(true);
  saveAsMenuItem->Enable(true);
  closeMenuItem->Enable(true);

  singularPointsDataViewCtrl->DeleteAllItems();
  solutionsDataViewCtrl->DeleteAllItems();
  isoclinesDataViewCtrl->DeleteAllItems();
  
  singularPointsDataViewCtrl->ClearColumns();
  singularPointsDataViewCtrl->AppendTextColumn("ID");
  singularPointsDataViewCtrl->AppendTextColumn("t");
  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    singularPointsDataViewCtrl->AppendTextColumn("x" + std::to_string(i));
  }

  isoclinesDataViewCtrl->ClearColumns();
  isoclinesDataViewCtrl->AppendTextColumn("ID");
}

void console_frame::compile_button_on_button_click(wxCommandEvent& event) {
  // We first verify that each expression is nonempty
  int variables = equationsDataViewCtrl->GetItemCount();
  if(variables < 1) {
    assert(false);
    return;
  }
  std::vector<std::string> expressions;
  for(int row = 0; row != variables; ++row) {
    std::string value(equationsDataViewCtrl->GetTextValue(row, 1));
    expressions.push_back(value);
    if(value.empty()) {
      wxMessageDialog messageDialog(nullptr, "One of the expressions is empty!",
				    "Error", wxOK);
      messageDialog.ShowModal();
      return;
    }
  }
  if(!appl.compile(expressions)) {
    wxMessageDialog messageDialog(nullptr, "The Compilation Failed!",
				  "Compilation Error", wxOK);
    messageDialog.ShowModal();
  }
}

void console_frame::read_variables_combo_box_input() {
  int variables(0);
  std::string variablesString(variablesComboBox->GetValue().ToStdString());
  if(!util::has_only_digits(variablesString)) {
    variablesComboBox->SetValue(variablesComboBoxValue);
    return;
  }
  try {
    variables = std::stoi(variablesString);
  } catch(const std::invalid_argument& exc) {
    variablesComboBox->SetValue(variablesComboBoxValue);
    return;
  } catch(const std::out_of_range& exc) {
    variablesComboBox->SetValue(variablesComboBoxValue);
    return;
  }

  if(variables <= 0) {
    variablesComboBox->SetValue(variablesComboBoxValue);
    return;
  }
  variablesComboBoxValue = variablesString;
  update_equations_data_view_ctrl(variables);
}

void console_frame::variables_combo_box_on_text_enter(wxCommandEvent& evt) {
  SetFocus(); // Sets the focus to the underlying frame.
  // No need to do anything since the text box loses focus anyway after this
  // event is called.
}

void console_frame::variables_combo_box_on_kill_focus(wxFocusEvent& evt) {
  read_variables_combo_box_input();
}

void console_frame::variables_combo_box_on_combo_box(wxCommandEvent& evt) {
  int variables(std::stoi(variablesComboBox->GetValue().ToStdString()));
  update_equations_data_view_ctrl(variables);
}

void console_frame::update_equations_data_view_ctrl(int variables) {
  assert(variables >= 1);
  if(variables == equationsDataViewCtrl->GetItemCount()) return;
  equationsDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  for(int i = 0; i != variables; ++i) {
    data.push_back(wxVariant("x" + std::to_string(i+1)));
    data.push_back(wxVariant(""));
    equationsDataViewCtrl->AppendItem(data);
    data.clear();
  }
  compileButton->Enable();
}

void console_frame::solutions_data_view_ctrl_on_selection_changed(wxDataViewEvent& evt) {
  if(solutionsDataViewCtrl->GetSelectedRow() != wxNOT_FOUND) {
    appl.select_solution(get_selected_solution());
  } else {
    appl.deselect_solution();
  }
}

void console_frame::isoclines_edit_button_on_button_click(wxCommandEvent&) {
  
}

void console_frame::isoclines_delete_button_on_button_click(wxCommandEvent&) {
  appl.delete_isocline(get_selected_isocline());
}

void console_frame::isoclines_data_view_ctrl_on_selection_changed(wxDataViewEvent&) {
  if(isoclinesDataViewCtrl->GetSelectedRow() != wxNOT_FOUND) {
    appl.select_isocline(get_selected_isocline());
  } else {
    appl.deselect_isocline();
  }
}

void console_frame::update_isoclines_list() {
  // We don't want to generate selection changed events
  isoclinesDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(false);
  isoclinesDataViewCtrl->DeleteAllItems();
  int row = 0;
  for(std::unordered_map<solution_id, isocline>::const_iterator
	iter = appl.get_model().get_isoclines().begin();
      iter != appl.get_model().get_isoclines().end(); ++iter) {
    wxVector<wxVariant> data;
    data.push_back(wxVariant(std::to_string(iter->first)));
    isoclinesDataViewCtrl->AppendItem(data);
    if(iter->first == appl.get_model().get_selected_isocline()) {
      isoclinesDataViewCtrl->SelectRow(row);
      
    }
    ++row;
  }
  if(appl.get_model().get_selected_isocline() == model::kNoIsoclineId) {
    isoclinesDeleteButton->Disable();
  } else {
    isoclinesDeleteButton->Enable();
  }
  isoclinesDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(true);
}

void console_frame::update_solutions_list() {
  solutionsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(false);
  solutionsDataViewCtrl->DeleteAllItems();
  int row = 0;
  for(std::unordered_map<solution_id, solution>::const_iterator
	iter = appl.get_model().get_solutions().begin();
      iter != appl.get_model().get_solutions().end(); ++iter) {
    wxVector<wxVariant> data;
    data.push_back(wxVariant(std::to_string(iter->first)));
    data.push_back(wxVariant(std::to_string(iter->second.specs.inc)));
    data.push_back(wxVariant(std::to_string(iter->second.specs.tMin)));
    data.push_back(wxVariant(std::to_string(iter->second.specs.tMax)));
    solutionsDataViewCtrl->AppendItem(data);
    if(iter->first == appl.get_model().get_selected_solution()) {
      solutionsDataViewCtrl->SelectRow(row);
    }
    ++row;
  }
  if(appl.get_model().get_selected_solution() == model::kNoSolutionId) {
    solutionsDeleteButton->Disable();
    solutionsEditButton->Disable();
  } else {
    solutionsDeleteButton->Enable();
    solutionsEditButton->Enable();
  }
  solutionsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(true);
}

void console_frame::update_singular_points_list() {
  singularPointsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(false);
  singularPointsDataViewCtrl->DeleteAllItems();
  int row = 0;
  for(std::unordered_map<singular_point_id, singular_point>::const_iterator iter
	= appl.get_model().get_singular_points().begin();
      iter != appl.get_model().get_singular_points().end(); ++iter) {
    wxVector<wxVariant> data;
    data.push_back(wxVariant(std::to_string(iter->first)));
    for(int i = 0; i != appl.get_model().get_dynamical_vars(); ++i) {
      data.push_back(wxVariant(std::to_string(iter->second.position[i])));
    }
    singularPointsDataViewCtrl->AppendItem(data);
    if(iter->first == appl.get_model().get_selected_singular_point()) {
      singularPointsDataViewCtrl->SelectRow(row);
    }
    ++row;
  }
  if(appl.get_model().get_selected_singular_point()
     == model::kNoSingularPointId) {
    singularPointsDeleteButton->Disable();
  } else {
    singularPointsDeleteButton->Enable();
  }
  singularPointsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(true);
}

void console_frame::singular_points_data_view_ctrl_on_selection_changed(wxDataViewEvent&) {
  if(singularPointsDataViewCtrl->GetSelectedRow() != wxNOT_FOUND) {
    appl.select_singular_point(get_selected_singular_point());
  } else {
    appl.deselect_singular_point();
  }
}

void console_frame::solution_menu_item_on_menu_selection(wxCommandEvent&) {
  solution_specs spec;
  spec.tMin = -10;
  spec.tMax = 10;
  spec.inc = 0.01;
  int dynamicalVariables = appl.get_model().get_dynamical_vars();
  spec.init = math::vector(dynamicalVariables, 0.0);
  solution_specs newSpec;
  if(appl.get_solution_dialog()->show_dialog(spec, &newSpec)) {
    appl.add_solution(newSpec);
  }
}
void console_frame::singular_point_menu_item_on_menu_selection(wxCommandEvent&) {
  struct singular_point_specs spec;
  int dynamicalVariables = appl.get_model().get_dynamical_vars();
  spec.init = math::vector(dynamicalVariables, 0.0);
  if(appl.get_singular_point_dialog()->show_dialog(spec, &spec)) {
    if(!appl.add_singular_point(spec)) {
      wxMessageDialog messageDialog(nullptr, "Could not find singular point.",
				    "Singular Point", wxOK);
      messageDialog.ShowModal();
    }
  }
}

void console_frame::isocline_menu_item_on_menu_selection(wxCommandEvent&) {
  isocline_specs specs;
  specs.inc = 0.01;
  specs.vertices = 100;
  specs.direction =
    math::vector(appl.get_model().get_dynamical_dimension(), 0.0);
  specs.init = math::vector(appl.get_model().get_dynamical_dimension(), 0.0);
  if(appl.get_isocline_dialog()->show_dialog(specs)) {
    if(!appl.add_isocline(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find isocline.",
				    "Isocline", wxOK);
      messageDialog.ShowModal();
    }
  }
}

void console_frame::solutions_edit_button_on_button_click(wxCommandEvent&) {
  solution_id solutionId(get_selected_solution());
  solution_specs spec(appl.get_model().get_solutions().at(solutionId).specs);
  if(appl.get_solution_dialog()->show_dialog(spec, &spec)) {
    appl.set_solution_specs(solutionId, spec);
  }
}

void console_frame::solutions_delete_button_on_button_click(wxCommandEvent&) {
  appl.delete_solution(get_selected_solution());
}

solution_id console_frame::get_selected_solution() {
  wxVariant value;
  solutionsDataViewCtrl->GetValue(value, solutionsDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
}

isocline_id console_frame::get_selected_isocline() {
  wxVariant value;
  isoclinesDataViewCtrl->GetValue(value, isoclinesDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
}

singular_point_id console_frame::get_selected_singular_point() {
  wxVariant value;
  singularPointsDataViewCtrl->GetValue(value,
				      singularPointsDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
}
void console_frame::close_menu_item_on_menu_selection(wxCommandEvent&) {
  appl.set_no_compile();
}
void console_frame::singular_points_delete_button_on_button_click(wxCommandEvent&) {
  appl.delete_singular_point(get_selected_singular_point());
}
} // namespace gui
} // namespace dynsolver
