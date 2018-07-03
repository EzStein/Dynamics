#include "gui/isocline_dialog.h"

namespace dynsolver {
namespace gui {

isocline_dialog::isocline_dialog() : isocline_dialog_base(nullptr) {
  initialDataViewCtrl->AppendTextColumn("Variable");
  initialDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);

  directionDataViewCtrl->AppendTextColumn("Variable");
  directionDataViewCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);
}

void isocline_dialog::cancel_button_on_button_click(wxCommandEvent&) {
  EndModal(wxID_CANCEL);
}

void isocline_dialog::ok_button_on_button_click(wxCommandEvent&) {
  if(validate_and_set()) {
    EndModal(wxID_OK);
  }
}

bool isocline_dialog::show_dialog(isocline_specs& in) {
  specs = in;
  set_ui();
  if(ShowModal() == wxID_OK) {
    in = specs;
    return true;
  } else {
    return false;
  }
}

void isocline_dialog::set_ui() {
  initialDataViewCtrl->DeleteAllItems();
  directionDataViewCtrl->DeleteAllItems();

  for(int i = 0; i != specs.init.size(); ++i) {
    wxVector<wxVariant> data;
    data.push_back("x" + std::to_string(i + 1));
    data.push_back(std::to_string(specs.init[i]));
    initialDataViewCtrl->AppendItem(data);
  }
  for(int i = 0; i != specs.direction.size(); ++i) {
    wxVector<wxVariant> data;
    data.push_back("x" + std::to_string(i + 1));
    data.push_back(std::to_string(specs.direction[i]));
    directionDataViewCtrl->AppendItem(data);
  }
}

bool isocline_dialog::validate_and_set() {
  for(int i = 0; i != initialDataViewCtrl->GetItemCount(); ++i) {
    double value;
    try {
      value = std::stod(initialDataViewCtrl->GetTextValue(i, 1).ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
    specs.init[i] = value;
  }
  for(int i = 0; i != directionDataViewCtrl->GetItemCount(); ++i) {
    double value;
    try {
      value = std::stod(directionDataViewCtrl
			->GetTextValue(i, 1)
			.ToStdString());
    } catch (const std::invalid_argument& exc) {
      return false;
    } catch (const std::out_of_range& exc) {
      return false;
    }
    specs.direction[i] = value;
  }
  return true;
}
} // namespace gui
} // namespace dynsolver
