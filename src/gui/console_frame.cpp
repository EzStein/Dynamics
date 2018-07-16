#include "gui/console_frame.h"

#include <string>
#include <stdexcept>

#include <wx/msgdlg.h>

#include "gui/app.h"
#include "gui/dynamical_dialog.h"
#include "gui/singular_point_dialog.h"
#include "gui/solution_dialog.h"
#include "gui/isocline_dialog.h"
#include "gui/parameter_dialog.h"

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
  parametersComboBox->SetValue("3");
  
  equationsDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  data.push_back(wxVariant("x"));
  data.push_back(wxVariant("a1*(y-x)"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();
  data.push_back(wxVariant("y"));
  data.push_back(wxVariant("x*(a2-z)-y"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();
  data.push_back(wxVariant("z"));
  data.push_back(wxVariant("x*y-z*a3"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();

  compileButton->Enable();
}

void console_frame::new_parameter_menu_item_on_selection(wxCommandEvent&) {
  double width = 400;
  double height = 400;
  parameter_specs spec;
  spec.viewport = math::window2d(math::vector2d(width, height),
				 math::vector2d(10, 10),
				 math::vector2d(-5, -5));
  spec.horizAxisVar = appl.get_model().get_parameter_names()[0];
  spec.vertAxisVar = appl.get_model().get_parameter_names()[1];
  if(appl.get_parameter_dialog()->show_dialog(spec)) {
    appl.add_parameter(spec);
  }
}

void console_frame::new_dynamical_menu_item_on_selection(wxCommandEvent&) {
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
  int vars = appl.get_model().get_dynamical_dimension() + 1;
  if(vars == 2) {
    spec.xAxisVar = appl.get_model().get_var_diff_name();
    spec.yAxisVar = appl.get_model().get_dynamical_names()[0];
    spec.zAxisVar = appl.get_model().get_dynamical_names()[0];
    spec.horzAxisVar = appl.get_model().get_var_diff_name();
    spec.vertAxisVar = appl.get_model().get_dynamical_names()[0];
  } else if(vars == 3) {
    spec.xAxisVar = appl.get_model().get_var_diff_name();
    spec.yAxisVar = appl.get_model().get_dynamical_names()[0];
    spec.zAxisVar = appl.get_model().get_dynamical_names()[1];
    spec.horzAxisVar = appl.get_model().get_dynamical_names()[0];
    spec.vertAxisVar = appl.get_model().get_dynamical_names()[1];
  } else if(vars >= 4) {
    spec.xAxisVar = appl.get_model().get_dynamical_names()[0];
    spec.yAxisVar = appl.get_model().get_dynamical_names()[1];
    spec.zAxisVar = appl.get_model().get_dynamical_names()[2];
    spec.horzAxisVar = appl.get_model().get_dynamical_names()[0];
    spec.vertAxisVar = appl.get_model().get_dynamical_names()[1];
  } else {
    assert(false);
  }

  
  spec.is3d = false;
  if(appl.get_dynamical_dialog()->show_dialog(spec)) {
    appl.add_dynamical(spec);
  }
}

void console_frame::close_dynamical_menu_item_on_selection(wxCommandEvent&) {
  appl.delete_all_dynamical_windows();
}

void console_frame::close_parameter_menu_item_on_selection(wxCommandEvent&) {
  appl.delete_all_parameter_windows();
}

void console_frame::console_frame_on_close(wxCloseEvent& evt) {
  appl.close_application();
}

void console_frame::set_no_compile() {
  variablesComboBox->SetValue("");
  compileButton->Disable();
  equationsDataViewCtrl->DeleteAllItems();
  newDynamicalMenuItem->Enable(false);
  newParameterMenuItem->Enable(false);
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
  newDynamicalMenuItem->Enable(true);
  solutionMenuItem->Enable(true);
  singularPointMenuItem->Enable(true);
  saveMenuItem->Enable(true);
  saveAsMenuItem->Enable(true);
  closeMenuItem->Enable(true);

  if(appl.get_model().get_parameters() >= 2) {
    newParameterMenuItem->Enable(true);
  }

  singularPointsDataViewCtrl->DeleteAllItems();
  solutionsDataViewCtrl->DeleteAllItems();
  isoclinesDataViewCtrl->DeleteAllItems();
  
  singularPointsDataViewCtrl->ClearColumns();
  singularPointsDataViewCtrl->AppendTextColumn("ID");

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
  std::vector<std::string> dynamicalVarNames;
  for(int row = 0; row != variables; ++row) {
    dynamicalVarNames.push_back(equationsDataViewCtrl->GetTextValue(row, 0).ToStdString());
    std::string value(equationsDataViewCtrl->GetTextValue(row, 1));

    expressions.push_back(value);
    if(value.empty()) {
      wxMessageDialog messageDialog(nullptr, "One of the expressions is empty!",
				    "Error", wxOK);
      messageDialog.ShowModal();
      return;
    }
  }
  
  
  std::vector<std::string> parameterNames;
  int params = std::stoi(parametersComboBox->GetValue().ToStdString());
  for(int i = 0; i != params; ++i) {
    parameterNames.push_back("a" + std::to_string(i + 1));
  }
  std::string varDiffName("t");
  if(!appl.compile(varDiffName, dynamicalVarNames,
		   parameterNames, expressions)) {
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

  if(variables == 1) {
    wxVector<wxVariant> data;
    data.push_back("x");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
  } else if(variables == 2) {
    wxVector<wxVariant> data;
    data.push_back("x");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
    data.clear();
    data.push_back("y");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
  } else if(variables == 3) {
    wxVector<wxVariant> data;
    data.push_back("x");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
    data.clear();
    data.push_back("y");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
    data.clear();
    data.push_back("z");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
  } else if(variables == 4) {
    wxVector<wxVariant> data;
    data.push_back("x");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
    data.clear();
    data.push_back("y");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
    data.clear();
    data.push_back("z");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
    data.clear();
    data.push_back("w");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
  } else {
    for(int i = 0; i != variables; ++i) {
      wxVector<wxVariant> data;
      data.push_back(wxVariant("x" + std::to_string(i+1)));
      data.push_back(wxVariant(""));
      equationsDataViewCtrl->AppendItem(data);
      data.clear();
    }
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
  int dynamicalVariables = appl.get_model().get_dynamical_dimension() + 1;
  spec.init = math::vector(dynamicalVariables, 0.0);
  if(appl.get_solution_dialog()->show_dialog(spec)) {
    appl.add_solution(spec);
  }
}
void console_frame::singular_point_menu_item_on_menu_selection(wxCommandEvent&) {
  struct singular_point_specs spec;
  int dynamicalVariables = appl.get_model().get_dynamical_dimension() + 1;
  spec.init = math::vector(dynamicalVariables, 0.0);
  if(appl.get_singular_point_dialog()->show_dialog(spec)) {
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
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
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
  if(appl.get_solution_dialog()->show_dialog(spec)) {
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

void console_frame::parameters_combo_box_on_combo_box(wxCommandEvent&) {
}
void console_frame::parameters_combo_box_on_text_enter(wxCommandEvent&) {
}
} // namespace gui
} // namespace dynsolver
