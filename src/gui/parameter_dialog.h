#ifndef DYNSOLVER_GUI_PARAMETER_DIALOG_H_
#define DYNSOLVER_GUI_PARAMETER_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

class parameter_dialog : public parameter_dialog_base {
private:
  const app& appl;
  wxPGProperty* horizAxisVarPGItem;
  wxPGProperty* horizAxisMinPGItem;
  wxPGProperty* horizAxisMaxPGItem;
  wxPGProperty* vertAxisVarPGItem;
  wxPGProperty* vertAxisMinPGItem;
  wxPGProperty* vertAxisMaxPGItem;

  wxPGChoices varChoices;

public:
  parameter_dialog(const app&);

  // Shows the dialog modally. The dialog is populated with imformation,
  // based on the specification provided. If the dialog is closed,
  // or canceled, this function returns false.
  // Otherwise it returns true and updates the specification provided.
  // Only one instance of this
  // class is meant to be in the program. That is, you should reuse it.
  bool show_dialog(parameter_specs&);
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void ok_button_on_button_click(wxCommandEvent&) override;

private:
  parameter_specs specs;

  // Sets the UI according to the specs field. Assumes that the
  // specs field is valid.
  void set_ui();

  // Validates and sets the specs field according to the UI.
  // Returns true if the validation was successful. Otherwise returns
  // false and does not update the specification.
  bool validate_and_set();
};
} // namespace gui
} // namespace dynsolver
#endif
