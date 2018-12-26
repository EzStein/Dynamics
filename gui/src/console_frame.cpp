#include "gui/console_frame.h"

#include <string>
#include <stdexcept>
#include <unordered_set>

#include <wx/msgdlg.h>

#include "gui/app.h"
#include "gui/dynamical_dialog.h"
#include "gui/singular_point_dialog.h"
#include "gui/solution_dialog.h"
#include "gui/isocline_dialog.h"
#include "gui/parameter_dialog.h"
#include "gui/separatrix_dialog.h"
#include "gui/hopf_bifurcation_dialog.h"
#include "gui/saddle_node_bifurcation_dialog.h"
#include "gui/saddle_connection_bifurcation_dialog.h"
#include "gui/limit_cycle_bifurcation_dialog.h"

#include "regex/nfa.h"

#include "util/util.h"

namespace dynsolver {
namespace gui {

console_frame::console_frame(app& app)
  : console_frame_base(nullptr, wxID_ANY, "Console"), appl(app) {
  // Add in widgets and setup events not already done in console_frame_base.
  equationsDataViewCtrl->AppendTextColumn("Expression", wxDATAVIEW_CELL_EDITABLE);

  parameterNamesDataViewCtrl->AppendTextColumn("Name", wxDATAVIEW_CELL_EDITABLE);

  
  variableNamesDataViewCtrl->AppendTextColumn("Name", wxDATAVIEW_CELL_EDITABLE);
  compiledParametersDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);

  wxVector<wxVariant> data;
  data.push_back("x' = ");
  data.push_back("");
  equationsDataViewCtrl->AppendItem(data);

  data.clear();
  data.push_back("0");
  data.push_back("x");
  variableNamesDataViewCtrl->AppendItem(data);

  solutionsDataViewCtrl->AppendTextColumn("ID");
  solutionsDataViewCtrl->AppendTextColumn("Increment");
  solutionsDataViewCtrl->AppendTextColumn("T Min");
  solutionsDataViewCtrl->AppendTextColumn("T Max");

  saddleNodeBifurcationsDataViewCtrl->AppendTextColumn("ID");
  hopfBifurcationsDataViewCtrl->AppendTextColumn("ID");
  singularPointsDataViewCtrl->AppendTextColumn("ID");
  isoclinesDataViewCtrl->AppendTextColumn("ID");
  set_no_compile();
  solutionsEditButton->Disable();
  solutionsDeleteButton->Disable();
  singularPointsDeleteButton->Disable();
  isoclinesDeleteButton->Disable();
  isoclinesEditButton->Disable();

  variables = 1;
  variablesTextCtrlValue = "1";
  variablesTextCtrl->SetValue(variablesTextCtrlValue);
  parameters = 0;
  parametersTextCtrlValue = "0";
  parametersTextCtrl->SetValue(parametersTextCtrlValue);
}

void console_frame::variable_names_data_view_ctrl_on_value_changed(wxDataViewEvent& evt) {
  int row = variableNamesDataViewCtrl->ItemToRow(evt.GetItem());
  std::string newValue = variableNamesDataViewCtrl->GetTextValue(row, 1).ToStdString();
  equationsDataViewCtrl->SetTextValue(newValue + "' = ", row, 0);
}

void console_frame::variables_text_ctrl_on_text_enter(wxCommandEvent&) {
  variables_text_ctrl_process_enter();
}

void console_frame::variables_text_ctrl_process_enter() {
  std::string value = variablesTextCtrl->GetValue().ToStdString();
  int newVariables;
  if(!util::has_only_digits(value)) {
    variablesTextCtrl->SetValue(variablesTextCtrlValue);
    return;
  }
  try {
    newVariables = std::stoi(value);
  } catch(const std::invalid_argument& exc) {
    variablesTextCtrl->SetValue(variablesTextCtrlValue);
    return;
  } catch(const std::out_of_range& exc) {
    variablesTextCtrl->SetValue(variablesTextCtrlValue);
    return;
  }
  if(newVariables < 1) {
    variablesTextCtrl->SetValue(variablesTextCtrlValue);
    return;
  }
  variablesTextCtrlValue = value;
  if(variables == newVariables)
    return;

  variables = newVariables;

  // Update equationsDataViewCtrl and variablesDataViewCtrl
  std::vector<std::string> defaultVarNames(variables);
  switch(variables) {
  case 4:
    defaultVarNames[3] = "w";
  case 3:
    defaultVarNames[2] = "z";
  case 2:
    defaultVarNames[1] = "y";
  case 1:
    defaultVarNames[0] = "x";
    break;
  default:
    for(int i = 0; i != variables; ++i) {
      defaultVarNames[i] = "x" + std::to_string(i);
    }
  }
  variableNamesDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != variables; ++i) {
    wxVector<wxVariant> data;
    data.push_back(std::to_string(i));
    data.push_back(defaultVarNames[i]);
    variableNamesDataViewCtrl->AppendItem(data);
  }
  equationsDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != variables; ++i) {
    wxVector<wxVariant> data;
    data.push_back(variableNamesDataViewCtrl->GetTextValue(i, 1) + "' = ");
    data.push_back("");
    equationsDataViewCtrl->AppendItem(data);
  }
}

void console_frame::parameters_text_ctrl_on_text_enter(wxCommandEvent&) {
  parameters_text_ctrl_process_enter();
}

void console_frame::parameters_text_ctrl_process_enter() {
  std::string value = parametersTextCtrl->GetValue().ToStdString();
  int newParameters;
  if(!util::has_only_digits(value)) {
    parametersTextCtrl->SetValue(parametersTextCtrlValue);
    return;
  }
  try {
    newParameters = std::stoi(value);
  } catch(const std::invalid_argument& exc) {
    parametersTextCtrl->SetValue(parametersTextCtrlValue);
    return;
  } catch(const std::out_of_range& exc) {
    parametersTextCtrl->SetValue(parametersTextCtrlValue);
    return;
  }
  if(newParameters < 0) {
    parametersTextCtrl->SetValue(parametersTextCtrlValue);
    return;
  }
  parametersTextCtrlValue = value;
  if(parameters == newParameters)
    return;

  parameters = newParameters;

  std::vector<std::string> defaultParameterNames(parameters);
  switch(parameters) {
  case 4:
    defaultParameterNames[3] = "d";
  case 3:
    defaultParameterNames[2] = "c";
  case 2:
    defaultParameterNames[1] = "b";
  case 1:
    defaultParameterNames[0] = "a";
    break;
  default:
    for(int i = 0; i != parameters; ++i) {
      defaultParameterNames[i] = "a" + std::to_string(i);
    }
  }
  parameterNamesDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != parameters; ++i) {
    wxVector<wxVariant> data;
    data.push_back(std::to_string(i));
    data.push_back(defaultParameterNames[i]);
    parameterNamesDataViewCtrl->AppendItem(data);
  }
}

console_frame::~console_frame() { }

void console_frame::lorenz_example_menu_item_on_menu_selection(wxCommandEvent&) {
  variablesTextCtrl->SetValue("3");
  variables_text_ctrl_process_enter();
  parametersTextCtrl->SetValue("3");
  parameters_text_ctrl_process_enter();

  equationsDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  data.push_back(wxVariant("x"));
  data.push_back(wxVariant("a*(y-x)"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();
  data.push_back(wxVariant("y"));
  data.push_back(wxVariant("x*(b-z)-y"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();
  data.push_back(wxVariant("z"));
  data.push_back(wxVariant("x*y-z*c"));
  equationsDataViewCtrl->AppendItem(data);
  data.clear();

  compileButton->Enable();
}

void console_frame::van_der_pol_example_on_menu_selection(wxCommandEvent&) {
  variablesTextCtrl->SetValue("2");
  variables_text_ctrl_process_enter();
  parametersTextCtrl->SetValue("2");
  parameters_text_ctrl_process_enter();

  equationsDataViewCtrl->SetValue(wxVariant("x^2+y^2-a^2"), 0, 1);
  equationsDataViewCtrl->SetValue(wxVariant("x^2-y^2-b^2"), 1, 1);

  compileButton->Enable();
}

void console_frame::new_parameter_menu_item_on_selection(wxCommandEvent&) {
  double width = 400;
  double height = 400;
  parameter_specs spec;
  spec.viewport = ::math::window2d(::math::vector2d(width, height),
				 ::math::vector2d(10, 10),
				 ::math::vector2d(-5, -5));
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
  spec.viewport2d = ::math::window2d(::math::vector2d(width, height),
				   ::math::vector2d(10, 10),
				   ::math::vector2d(-5, -5));

  spec.viewport3d = ::math::window3d(::math::vector3d(10,10,10),
				   ::math::vector3d(-10,-10,-10),
				   ::math::vector3d(0,1,0),
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
  newDynamicalMenuItem->Enable(false);
  newParameterMenuItem->Enable(false);
  solutionMenuItem->Enable(false);
  singularPointMenuItem->Enable(false);
  isoclineMenuItem->Enable(false);
  saveMenuItem->Enable(false);
  saveAsMenuItem->Enable(false);
  closeMenuItem->Enable(false);
  separatricesMenuItem->Enable(false);
  hopfBifurcationMenuItem->Enable(false);
  saddleNodeBifurcationMenuItem->Enable(false);
  limitCycleBifurcationMenuItem->Enable(false);
  saddleConnectionBifurcationMenuItem->Enable(false);
  solutionsDataViewCtrl->DeleteAllItems();
  singularPointsDataViewCtrl->DeleteAllItems();
  isoclinesDataViewCtrl->DeleteAllItems();
  separatricesDataViewCtrl->DeleteAllItems();
}

void console_frame::set_yes_compile() {
  newDynamicalMenuItem->Enable(true);
  solutionMenuItem->Enable(true);
  singularPointMenuItem->Enable(true);
  isoclineMenuItem->Enable(true);
  saveMenuItem->Enable(true);
  saveAsMenuItem->Enable(true);
  closeMenuItem->Enable(true);
  if(appl.get_model().get_dynamical_dimension() == 2) {
    separatricesMenuItem->Enable(true);
    if(appl.get_model().get_parameters() == 2) {
      hopfBifurcationMenuItem->Enable(true);
      saddleNodeBifurcationMenuItem->Enable(true);
      limitCycleBifurcationMenuItem->Enable(true);
      saddleConnectionBifurcationMenuItem->Enable(true);
    }
  }

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
  regex::nfa nfa("\\a+\\d*");
  // We first verify that each expression is nonempty
  int variables = variableNamesDataViewCtrl->GetItemCount();
  std::vector<std::string> expressions;
  std::vector<std::string> dynamicalVarNames;
  for(int row = 0; row != variables; ++row) {
    std::string name(variableNamesDataViewCtrl->GetTextValue(row, 1).ToStdString());
    if(!nfa.accepts(name)) {
      wxMessageDialog messageDialog(nullptr,
				    "Variable number " + std::to_string(row) +
				    "is not a valid variable name!",
				    "Error", wxOK);
      messageDialog.ShowModal();
      return;
    }
    dynamicalVarNames.push_back(name);

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
  for(int row = 0; row != parameters; ++row) {
    std::string name(parameterNamesDataViewCtrl->GetTextValue(row, 1).ToStdString());
    if(!nfa.accepts(name)) {
      wxMessageDialog messageDialog(nullptr,
				    "Parameter number " + std::to_string(row) +
				    "is not a valid parameter name!",
				    "Error", wxOK);
      messageDialog.ShowModal();
      return;
    }
    parameterNames.push_back(name);
  }


  std::string varDiffName(varDiffTextCtrl->GetValue().ToStdString());
  if(!nfa.accepts(varDiffName)) {
    wxMessageDialog messageDialog(nullptr,
				  "The variable of differentiation is not a valid "
				  "variable name!",
				  "Error", wxOK);
    messageDialog.ShowModal();
    return;
  }
  std::unordered_set<std::string> duplicates;
  for(const std::string& name : dynamicalVarNames) {
    if(duplicates.find(name) != duplicates.end()) {
      wxMessageDialog messageDialog(nullptr,
				    "Duplicate name!",
				    "Error", wxOK);
      messageDialog.ShowModal();
      return;
    }
    duplicates.insert(name);
  }
  for(const std::string& name : parameterNames) {
    if(duplicates.find(name) != duplicates.end()) {
      wxMessageDialog messageDialog(nullptr,
				    "Duplicate name!",
				    "Error", wxOK);
      messageDialog.ShowModal();
      return;
    }
    duplicates.insert(name);
  }
  if(duplicates.find(varDiffName) != duplicates.end()) {
    wxMessageDialog messageDialog(nullptr,
				  "Duplicate name!",
				  "Error", wxOK);
    messageDialog.ShowModal();
    return;
  }
  try {
	  if (!appl.compile(varDiffName, dynamicalVarNames,
		  parameterNames, expressions)) {
		  wxMessageDialog messageDialog(nullptr, "The Compilation Failed!",
			  "Compilation Error", wxOK);
		  messageDialog.ShowModal();
		  return;
	  }
  }
  catch (const std::exception& exc) {
	  wxMessageDialog messageDialog(nullptr, "The Compilation Failed!",
		  exc.what(), wxOK);
	  messageDialog.ShowModal();
	  std::cout << exc.what() << std::endl;
  }

  compiledEquationsDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    wxVector<wxVariant> data;
    data.push_back(equationsDataViewCtrl->GetTextValue(i, 0));
    data.push_back(equationsDataViewCtrl->GetTextValue(i, 1));
    compiledEquationsDataViewCtrl->AppendItem(data);
  }
  compiledParametersDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != appl.get_model().get_parameters(); ++i) {
    wxVector<wxVariant> data;
    data.push_back(appl.get_model().get_parameter_names()[i]);
    data.push_back(std::to_string(appl.get_model().get_parameter_position()[i]));
    compiledParametersDataViewCtrl->AppendItem(data);
  }
}

void console_frame::solutions_data_view_ctrl_on_selection_changed(wxDataViewEvent& evt) {
  if(solutionsDataViewCtrl->GetSelectedRow() != wxNOT_FOUND) {
    appl.select_solution(get_selected_solution());
  } else {
    appl.deselect_solution();
  }
}

void console_frame::separatrices_data_view_ctrl_on_selection_changed(wxDataViewEvent&) {
  if(separatricesDataViewCtrl->GetSelectedRow() != wxNOT_FOUND) {
    appl.select_separatrix(get_selected_separatrix());
  } else {
    appl.deselect_separatrix();
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
  spec.init = ::math::vector(dynamicalVariables, 0.0);
  if(appl.get_solution_dialog()->show_dialog(spec)) {
    appl.add_solution(spec);
  }
}
void console_frame::singular_point_menu_item_on_menu_selection(wxCommandEvent&) {
  struct singular_point_specs spec;
  int dynamicalVariables = appl.get_model().get_dynamical_dimension() + 1;
  spec.init = ::math::vector(dynamicalVariables, 0.0);
  if(appl.get_singular_point_dialog()->show_dialog(spec)) {
    if(!appl.add_singular_point(spec)) {
      wxMessageDialog messageDialog(nullptr, "Could not find singular point.",
				    "Singular Point", wxOK);
      messageDialog.ShowModal();
    }
  }
}

void console_frame::separatrices_menu_item_on_menu_selection(wxCommandEvent&) {
  separatrix_specs specs;
  specs.inc = 0.01;
  specs.time = 20;
  specs.type = separatrix_specs::type::STABLE;
  specs.direction = true;
  if(appl.get_separatrix_dialog()->show_dialog(specs)) {
    appl.add_separatrix(specs);
  }
}

void console_frame::draw_all_separatrices_menu_item_on_menu_selection(wxCommandEvent&) {
  appl.add_all_separatrices();
}

void console_frame::isocline_menu_item_on_menu_selection(wxCommandEvent&) {
  isocline_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.direction =
    ::math::vector(appl.get_model().get_dynamical_dimension(), 0.0);
  specs.init = ::math::vector(appl.get_model().get_dynamical_dimension(), 0.0);
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

separatrix_id console_frame::get_selected_separatrix() {
  wxVariant value;
  separatricesDataViewCtrl->GetValue(value, separatricesDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
}

hopf_bifurcation_id console_frame::get_selected_hopf_bifurcation() {
  wxVariant value;
  hopfBifurcationsDataViewCtrl->GetValue(value, hopfBifurcationsDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
}

saddle_node_bifurcation_id console_frame::get_selected_saddle_node_bifurcation() {
  wxVariant value;
  saddleNodeBifurcationsDataViewCtrl
    ->GetValue(value, saddleNodeBifurcationsDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
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
limit_cycle_bifurcation_id console_frame::get_selected_limit_cycle_bifurcation() {
  wxVariant value;
  limitCycleBifurcationsDataViewCtrl
    ->GetValue(value,
	       limitCycleBifurcationsDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
}
saddle_connection_bifurcation_id console_frame::get_selected_saddle_connection_bifurcation() {
  wxVariant value;
  saddleConnectionBifurcationsDataViewCtrl
    ->GetValue(value,
	       saddleConnectionBifurcationsDataViewCtrl->GetSelectedRow(), 0);
  return std::stoi(value.GetString().ToStdString());
}

void console_frame::update_parameters_data_view_ctrl() {
  compiledParametersDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != appl.get_model().get_parameters(); ++i) {
    wxVector<wxVariant> data;
    data.push_back(appl.get_model().get_parameter_names()[i]);
    data.push_back(std::to_string(appl.get_model().get_parameter_position()[i]));
    compiledParametersDataViewCtrl->AppendItem(data);
  }
}

void console_frame::update_separatrices_list() {
  separatricesDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(false);
  separatricesDataViewCtrl->DeleteAllItems();
  int row = 0;
  for(std::unordered_map<separatrix_id, separatrix>::const_iterator iter
	= appl.get_model().get_separatrices().begin();
      iter != appl.get_model().get_separatrices().end(); ++iter) {
    wxVector<wxVariant> data;
    data.push_back(std::to_string(iter->first));
    separatricesDataViewCtrl->AppendItem(data);
    if(iter->first == appl.get_model().get_selected_separatrix()) {
      separatricesDataViewCtrl->SelectRow(row);
    }
    ++row;
  }
  separatricesDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(true);
}

void console_frame::update_hopf_bifurcation_list() {
  hopfBifurcationsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(false);
  hopfBifurcationsDataViewCtrl->DeleteAllItems();
  for(std::unordered_map<hopf_bifurcation_id, hopf_bifurcation>::const_iterator iter
	= appl.get_model().get_hopf_bifurcations().begin();
      iter != appl.get_model().get_hopf_bifurcations().end(); ++iter) {
    wxVector<wxVariant> data;
    data.push_back(std::to_string(iter->first));
    hopfBifurcationsDataViewCtrl->AppendItem(data);
  }
  hopfBifurcationsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(true);
}
void console_frame::update_saddle_node_bifurcation_list() {
  saddleNodeBifurcationsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(false);
  saddleNodeBifurcationsDataViewCtrl->DeleteAllItems();
  for(std::unordered_map<saddle_node_bifurcation_id, saddle_node_bifurcation>::const_iterator iter
	= appl.get_model().get_saddle_node_bifurcations().begin();
      iter != appl.get_model().get_saddle_node_bifurcations().end(); ++iter) {
    wxVector<wxVariant> data;
    data.push_back(std::to_string(iter->first));
    saddleNodeBifurcationsDataViewCtrl->AppendItem(data);
  }
  saddleNodeBifurcationsDataViewCtrl->GetEventHandler()->SetEvtHandlerEnabled(true);
}

void console_frame::update_limit_cycle_bifurcation_list() {
}

void console_frame::update_saddle_connection_bifurcation_list() {
}

void console_frame::hopf_bifurcations_data_view_ctrl_on_selection_changed(wxDataViewEvent&) {
    if(hopfBifurcationsDataViewCtrl->GetSelectedRow() != wxNOT_FOUND) {
    appl.select_hopf_bifurcation(get_selected_hopf_bifurcation());
  } else {
    appl.deselect_hopf_bifurcation();
  }
}
void console_frame::saddle_node_bifurcations_data_view_ctrl_on_selection_changed(wxDataViewEvent&) {
  if(saddleNodeBifurcationsDataViewCtrl->GetSelectedRow() != wxNOT_FOUND) {
    appl.select_saddle_node_bifurcation(get_selected_saddle_node_bifurcation());
  } else {
    appl.deselect_saddle_node_bifurcation();
  }
}

void console_frame::hopf_bifurcation_menu_item_on_selection(wxCommandEvent&) {
  hopf_bifurcation_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.init.dynamicalVars = ::math::vector(2, 0.0);
  specs.init.parameters = ::math::vector(2, 0.0);
  if(appl.get_hopf_bifurcation_dialog()->show_dialog(specs)) {
    if(!appl.add_hopf_bifurcation(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find hopf bifurcation.",
				    "Hopf Bifurcation", wxOK);
      messageDialog.ShowModal();
    }
  }
}
void console_frame::saddle_node_bifurcation_menu_item_on_selection(wxCommandEvent&) {
  saddle_node_bifurcation_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.init.dynamicalVars = ::math::vector(2, 0.0);
  specs.init.parameters = ::math::vector(2, 0.0);
  if(appl.get_saddle_node_bifurcation_dialog()->show_dialog(specs)) {
    if(!appl.add_saddle_node_bifurcation(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find saddle-node bifurcation.",
				    "Hopf Bifurcation", wxOK);
      messageDialog.ShowModal();
    }
  }
}
void console_frame::limit_cycle_bifurcation_menu_item_on_selection(wxCommandEvent&) {
  /*  limit_cycle_bifurcation_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.transversalA = ::math::vector(2, 0.0);
  specs.transversalB = ::math::vector(2, 0.0);
  specs.init = ::math::vector(2, 0.0);
  if(appl.get_limit_cycle_bifurcation_dialog()->show_dialog(specs)) {
    if(!appl.add_limit_cycle_bifurcation(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find semi-stable limit cycle",
				    "Semi-stable limit cycle Bifurcation", wxOK);
      messageDialog.ShowModal();
    }
  }*/
}
void console_frame::saddle_connection_bifurcation_menu_item_on_selection(wxCommandEvent&) {
  saddle_connection_bifurcation_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.transversalA = ::math::vector2d(0.0, 0.0);
  specs.transversalB = ::math::vector2d(0.0, 0.0);
  specs.separatrix1 = model::kNoSeparatrixId;
  specs.separatrix2 = model::kNoSeparatrixId;
  specs.init = ::math::vector(2, 0.0);
  if(appl.get_saddle_connection_bifurcation_dialog()->show_dialog(specs)) {
    if(!appl.add_saddle_connection_bifurcation(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find saddle connection.",
				    "Saddle Connection Bifurcation", wxOK);
      messageDialog.ShowModal();
    }
  }
}

void console_frame::equationsDataViewCtrlOnDataViewListCtrlSelectionChanged(wxDataViewEvent&) {
  std::cout << "EVT Selection Changed" << std::endl;
}
void console_frame::equationsDataViewCtrlOnDataViewListCtrlItemActivated(wxDataViewEvent&) {
  std::cout << "EVT Item Activated" << std::endl;
}
void console_frame::equationsDataViewCtrlOnDataViewListCtrlItemStartEditing(wxDataViewEvent&) {
  std::cout << "EVT Start Editing" << std::endl;
}
void console_frame::equationsDataViewCtrlOnDataViewListCtrlItemEditingStarted(wxDataViewEvent&) {
  std::cout << "EVT Editing Started" << std::endl;
}
void console_frame::equationsDataViewCtrlOnDataViewListCtrlItemEditingDone(wxDataViewEvent&) {
  std::cout << "EVT Editing Done" << std::endl;
}
} // namespace gui
} // namespace dynsolver
