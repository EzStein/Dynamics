#include "gui/saddle_node_bifurcation_dialog.h"

#include "gui/app.h"

#include <string>

namespace dynsolver {
namespace gui {
saddle_node_bifurcation_dialog::saddle_node_bifurcation_dialog(const app& appl)
  : saddle_node_bifurcation_dialog_base(nullptr), appl(appl) {
  initialVarsDataViewCtrl->AppendTextColumn("Variable");
  initialVarsDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);

  initialParametersDataViewCtrl->AppendTextColumn("Parameter");
  initialParametersDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);
}

bool saddle_node_bifurcation_dialog::show_dialog(saddle_node_bifurcation_specs& newSpecs) {
  specs = newSpecs;
  set_ui();
  if(ShowModal() == wxID_OK) {
    newSpecs = specs;
    return true;
  } else {
    return false;
  }
}

void saddle_node_bifurcation_dialog::set_ui() {
  incrementPGItem->SetValue(specs.inc);
  spanPGItem->SetValue(specs.span);
  searchRadiusPGItem->SetValue(specs.searchRadius);
  searchIncrementPGItem->SetValue(specs.searchInc);

  initialVarsDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    wxVector<wxVariant> data;
    data.push_back(appl.get_model().get_dynamical_names()[i]);
    data.push_back(std::to_string(specs.init.dynamicalVars[i]));
    initialVarsDataViewCtrl->AppendItem(data);
  }

  initialParametersDataViewCtrl->DeleteAllItems();
  for(int i = 0; i != appl.get_model().get_parameters(); ++i) {
    wxVector<wxVariant> data;
    data.push_back(appl.get_model().get_parameter_names()[i]);
    data.push_back(std::to_string(specs.init.parameters[i]));
    initialParametersDataViewCtrl->AppendItem(data);
  }
}

bool saddle_node_bifurcation_dialog::validate_and_set() {
  specs.inc = incrementPGItem->GetValue().GetDouble();
  if(specs.inc <= 0) return false;
  specs.span = spanPGItem->GetValue().GetInteger();
  if(specs.span <= 0) return false;
  specs.searchRadius = searchRadiusPGItem->GetValue().GetDouble();
  if(specs.searchRadius <= 0) return false;
  specs.searchInc = searchIncrementPGItem->GetValue().GetDouble();
  if(specs.searchInc <= 0) return false;

  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    try {
      specs.init.dynamicalVars[i]
	= std::stod(initialVarsDataViewCtrl->GetTextValue(i, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
  }
  for(int i = 0; i != appl.get_model().get_parameters(); ++i) {
    try {
      specs.init.parameters[i]
	= std::stod(initialParametersDataViewCtrl->GetTextValue(i, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
  }
  return true;
}

void saddle_node_bifurcation_dialog::cancel_button_on_button_click(wxCommandEvent& evt) {
  EndModal(wxID_CANCEL);
}

void saddle_node_bifurcation_dialog::ok_button_on_button_click(wxCommandEvent& evt) {
  if(validate_and_set()) {
    EndModal(wxID_OK);
  }
}
} // namespace gui
} // namespace dynsolver
