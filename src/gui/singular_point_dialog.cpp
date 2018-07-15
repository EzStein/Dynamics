#include "gui/singular_point_dialog.h"
#include "gui/app.h"
namespace dynsolver {
namespace gui {

singular_point_dialog::singular_point_dialog(const app& appl)
  : singular_point_dialog_base(nullptr),
    appl(appl) {
  initialValueDataViewCtrl->AppendTextColumn("Variable");
  initialValueDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);
}

bool singular_point_dialog::show_dialog(singular_point_specs& inSpecs) {
  specs = inSpecs;
  set_ui();
  if(ShowModal() == wxID_OK) {
    inSpecs = specs;
    return true;
  } else {
    return false;
  }
}

void singular_point_dialog::cancel_button_on_button_click(wxCommandEvent& evt) {
  EndModal(wxID_CANCEL);
}

void singular_point_dialog::add_button_on_button_click(wxCommandEvent& evt) {
  if(validate_and_set()) {
    EndModal(wxID_OK);
  }
}

void singular_point_dialog::set_ui() {
  initialValueDataViewCtrl->DeleteAllItems();
  wxVector<wxVariant> data;
  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    data.push_back(appl.get_model().get_dynamical_names()[i]);
    data.push_back(std::to_string(specs.init[i]));
    initialValueDataViewCtrl->AppendItem(data);
    data.clear();
  }
}

bool singular_point_dialog::validate_and_set() {
  for(int i = 0; i != appl.get_model().get_dynamical_dimension(); ++i) {
    double value;
    try {
      value = std::stod(initialValueDataViewCtrl->GetTextValue(i, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
    specs.init[i] = value;
  }
  return true;
}
} // namespace gui
} // namespace dynslover
