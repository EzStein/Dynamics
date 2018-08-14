#include "gui/separatrix_dialog.h"
#include "gui/app.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

separatrix_dialog::separatrix_dialog(const app& appl)
  : separatrix_dialog_base(nullptr), appl(appl) { }

bool separatrix_dialog::show_dialog(separatrix_specs& newSpecs) {
  specs = newSpecs;
  set_ui();
  if(ShowModal() == wxID_OK) {
    newSpecs = specs;
    return true;
  }
  return false;
}

void separatrix_dialog::cancel_button_on_button_click(wxCommandEvent&) {
  EndModal(wxID_CANCEL);
}

void separatrix_dialog::ok_button_on_button_click(wxCommandEvent&) {
  if(validate_and_set()) {
    EndModal(wxID_OK);
  }
}

void separatrix_dialog::set_ui() {
  incrementPGItem->SetValue(specs.inc);
  timePGItem->SetValue(specs.time);
  singularPointComboBox->Clear();
  if(specs.type == separatrix_specs::type::STABLE) {
    typeComboBox->SetSelection(0);
  } else {
    typeComboBox->SetSelection(1);
  }
  int row = 0;
  for(std::unordered_map<singular_point_id, singular_point>::const_iterator iter
	= appl.get_model().get_singular_points().begin();
      iter != appl.get_model().get_singular_points().end(); ++iter) {
    if(iter->second.type == singular_point::type::SADDLE) {
      singularPointComboBox->Append(std::to_string(iter->first));
      if(specs.singularPoint == iter->first) {
	singularPointComboBox->SetSelection(row);
      }
      ++row;
    }
  }
}

bool separatrix_dialog::validate_and_set() {
  specs.inc = incrementPGItem->GetValue().GetDouble();
  if(specs.inc <= 0) {
    return false;
  }
  specs.time = timePGItem->GetValue().GetDouble();
  if(specs.time <= 0) {
    return false;
  }
  std::string value(singularPointComboBox->GetStringSelection().ToStdString());
  if(value == "") {
    return false;
  }
  specs.singularPoint = std::stoi(value);
  std::string type = typeComboBox->GetStringSelection().ToStdString();
  if(type == "STABLE") {
    specs.type = separatrix_specs::type::STABLE;
  } else if(type == "UNSTABLE") {
    specs.type = separatrix_specs::type::UNSTABLE;
  } else {
    return false;
  }
  return true;
}
} // namespace gui
} // namespace dynsolver
