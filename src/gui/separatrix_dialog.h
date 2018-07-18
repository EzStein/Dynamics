#ifndef DYNSOLVER_GUI_SEPARATRIX_DIALOG_H_
#define DYNSOLVER_GUI_SEPARATRIX_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

class app;

class separatrix_dialog : public separatrix_dialog_base {
private:
  const app& appl;
  separatrix_specs specs;

public:
  separatrix_dialog(const app&);

  // Returns true if the user selected a valid specification.
  // Returns false if the user canceled. The initial specification
  // to provide is given by specs. Specs is also filled with the new
  // specification if the user does not cancel.
  bool show_dialog(separatrix_specs& specs);
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void ok_button_on_button_click(wxCommandEvent&) override;

private:
  // Sets the UI according to the specs field.
  void set_ui();

  // Returns true if the imformation provided is valid.
  // If so, it sets the specs field.
  bool validate_and_set();
};

} // namespace gui
} // namespace dynsolver
#endif
