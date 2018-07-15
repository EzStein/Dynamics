#ifndef DYNSOLVER_GUI_ISOCLINE_DIALOG_H_
#define DYNSOLVER_GUI_ISOCLINE_DIALOG_H_

#include "gui/generated.h"

#include "gui/model.h"

namespace dynsolver {
namespace gui {

class isocline_dialog : public isocline_dialog_base {
public:
  isocline_dialog(const app&);
  
  // Shows this dialog, filling in the default info with the given
  // isocline_specs. Returns false if the user cancelled the operation,
  // otherwise, returns true and updates the provided specs appropriately.
  bool show_dialog(isocline_specs&);

  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void ok_button_on_button_click(wxCommandEvent&) override;
  
private:
  const app& appl;
  isocline_specs specs;

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
