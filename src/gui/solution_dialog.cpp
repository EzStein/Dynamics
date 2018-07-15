#include "gui/solution_dialog.h"
#include "gui/app.h"

namespace dynsolver {
namespace gui {

solution_dialog::solution_dialog(const app& appl)
  : solution_dialog_base(nullptr),
    appl(appl) {
  initialValueDataViewCtrl->AppendTextColumn("Variable");
  initialValueDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);
}

bool solution_dialog::show_dialog(solution_specs& inSpecs) {
  specs = inSpecs;
  set_ui();
  if(ShowModal() == wxID_OK) {
    inSpecs = specs;
    return true;
  } else {
    return false;
  }
}

void solution_dialog::cancel_button_on_button_click(wxCommandEvent& evt) {
  EndModal(wxID_CANCEL);
}

void solution_dialog::add_button_on_button_click(wxCommandEvent& evt) {
  if(validate_and_set()) {
    EndModal(wxID_OK);
  }
}

void solution_dialog::set_ui() {
  initialValueDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  data.push_back(appl.get_model().get_var_diff_name());
  data.push_back(std::to_string(specs.tStart));
  initialValueDataViewCtrl->AppendItem(data);
  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    data.clear();
    data.push_back(appl.get_model().get_dynamical_names()[i]);
    data.push_back(std::to_string(specs.init[i]));
    initialValueDataViewCtrl->AppendItem(data);
  }
  solutionPropertyGrid->SetPropertyValue(tMinPropertyGridItem,
					 specs.tMin);
  solutionPropertyGrid->SetPropertyValue(tMaxPropertyGridItem,
					 specs.tMax);
  solutionPropertyGrid->SetPropertyValue(incrementPropertyGridItem,
					 specs.inc);
}

bool solution_dialog::validate_and_set() {
  specs.tMin =
    solutionPropertyGrid->GetPropertyValue(tMinPropertyGridItem).GetDouble();
  specs.tMax =
    solutionPropertyGrid->GetPropertyValue(tMaxPropertyGridItem).GetDouble();
  specs.inc =
    solutionPropertyGrid->GetPropertyValue(incrementPropertyGridItem).GetDouble();
  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    double value;
    try {
      value = std::stod(initialValueDataViewCtrl->GetTextValue(i + 1, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
    specs.init[i] = value;
  }
  double value;
  try {
    value = std::stod(initialValueDataViewCtrl->GetTextValue(0, 1).ToStdString());
  } catch (const std::invalid_argument& exc) {
    return false;
  } catch (const std::out_of_range& exc) {
    return false;
  }
  specs.tStart = value;
  if(specs.tMin >= specs.init[0]) {
    return false;
  } else if(specs.tStart >= specs.tMax) {
    return false;
  } else if(specs.inc <= 0.0) {
    return false;
  }
  return true;
}
} // namespace gui
} // namespace dynslover
