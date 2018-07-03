#include "gui/singular_point_dialog.h"

namespace dynsolver {
namespace gui {

singular_point_dialog::singular_point_dialog() :
  singular_point_dialog_base(nullptr) {
  initialValueDataViewCtrl->AppendTextColumn("Variable");
  initialValueDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);
}

bool singular_point_dialog::show_dialog(const singular_point_specs& spec,
					singular_point_specs* ret) {
  singularPointSpecification = spec;
  set_ui();
  if(ShowModal() == wxID_OK) {
    *ret = singularPointSpecification;
    return true;
  } else {
    return false;
  }
}

void singular_point_dialog::cancel_button_on_button_click(wxCommandEvent& evt) {
  EndModal(wxID_CANCEL);
}

void singular_point_dialog::add_button_on_button_click(wxCommandEvent& evt) {
  if(validate_and_set_specification()) {
    EndModal(wxID_OK);
  }
}

void singular_point_dialog::set_ui() {
  initialValueDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  for(int i = 0; i != singularPointSpecification.init.size(); ++i) {
    if(i == 0) {
      data.push_back("t");
    } else {
      data.push_back("x" + std::to_string(i));
    }
    data.push_back(std::to_string(singularPointSpecification.init[i]));
    initialValueDataViewCtrl->AppendItem(data);
    data.clear();
  }
}

bool singular_point_dialog::validate_and_set_specification() {
  int variables = initialValueDataViewCtrl->GetItemCount();
  singularPointSpecification.init = math::vector(variables);
  for(int i = 0; i != variables; ++i) {
    double value;
    try {
      value = std::stod(initialValueDataViewCtrl->GetTextValue(i, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
    singularPointSpecification.init[i] = value;
  }
  return true;
}
} // namespace gui
} // namespace dynslover
