#include "gui/console_frame.h"

#include <string>
#include <stdexcept>

#include <wx/msgdlg.h>

#include "gui/app.h"

namespace dynsolver {
namespace gui {

console_frame::console_frame(app& app) : console_frame_base(nullptr, wxID_ANY, "Console"),
					 appl(app) {
  // Add in widgets and setup events not already done in console_frame_base.
  equationsDataViewCtrl->AppendTextColumn("Variable");
  equationsDataViewCtrl->AppendTextColumn("Expression", wxDATAVIEW_CELL_EDITABLE);
  update_equations_data_view_ctrl(2);
}

console_frame::~console_frame() { }

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

void console_frame::variables_combo_box_on_text_enter(wxCommandEvent& evt) {
  SetFocus(); // Sets the focus to the underlying frame.
  int variables(0);
  try {
    variables = std::stoi(variablesComboBox->GetValue().ToStdString());
  } catch(const std::invalid_argument& exc) {
    evt.Skip();
    return;
  } catch(const std::out_of_range& exc) {
    evt.Skip();
    return;
  }

  if(variables <= 0) {
    evt.Skip();
    return;
  }
  update_equations_data_view_ctrl(variables);
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
}
} // namespace gui
} // namespace dynsolver
