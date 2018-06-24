#include "gui/solution_dialog.h"

namespace dynsolver {
namespace gui {

solution_dialog::solution_dialog() :
  solution_dialog_base(nullptr) {
  initialValueDataViewCtrl->AppendTextColumn("Variable");
  initialValueDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);
}

bool solution_dialog::show_dialog(const solution_specification& spec,
				  solution_specification* ret) {
  solutionSpecification = spec;
  set_ui();
  if(ShowModal() == wxID_OK) {
    *ret = solutionSpecification;
    return true;
  } else {
    return false;
  }
}

void solution_dialog::cancel_button_on_button_click(wxCommandEvent& evt) {
  EndModal(wxID_CANCEL);
}

void solution_dialog::add_button_on_button_click(wxCommandEvent& evt) {
  if(validate_and_set_specification()) {
    EndModal(wxID_OK);
  }
}

void solution_dialog::set_ui() {
  initialValueDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  for(int i = 0; i != solutionSpecification.initialValue.size(); ++i) {
    if(i == 0) {
      data.push_back("t");
    } else {
      data.push_back("x" + std::to_string(i));
    }
    data.push_back(std::to_string(solutionSpecification.initialValue[i]));
    initialValueDataViewCtrl->AppendItem(data);
    data.clear();
  }
  solutionPropertyGrid->SetPropertyValue(tMinPropertyGridItem,
					 solutionSpecification.tMin);
  solutionPropertyGrid->SetPropertyValue(tMaxPropertyGridItem,
					 solutionSpecification.tMax);
  solutionPropertyGrid->SetPropertyValue(incrementPropertyGridItem,
					 solutionSpecification.increment);
}

bool solution_dialog::validate_and_set_specification() {
  solutionSpecification.tMin =
    solutionPropertyGrid->GetPropertyValue(tMinPropertyGridItem).GetDouble();
  solutionSpecification.tMax =
    solutionPropertyGrid->GetPropertyValue(tMaxPropertyGridItem).GetDouble();
  solutionSpecification.increment =
    solutionPropertyGrid->GetPropertyValue(incrementPropertyGridItem).GetDouble();
  int variables = initialValueDataViewCtrl->GetItemCount();
  solutionSpecification.initialValue = math::vector(variables);
  for(int i = 0; i != variables; ++i) {
    double value;
    try {
      value = std::stod(initialValueDataViewCtrl->GetTextValue(i, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
    solutionSpecification.initialValue[i] = value;
  }
  if(solutionSpecification.tMin >= solutionSpecification.initialValue[0]) {
    return false;
  } else if(solutionSpecification.initialValue[0] >= solutionSpecification.tMax) {
    return false;
  } else if(solutionSpecification.increment <= 0.0) {
    return false;
  }
  return true;
}
} // namespace gui
} // namespace dynslover
