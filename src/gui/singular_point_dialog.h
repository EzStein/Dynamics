#ifndef DYNSOLVER_GUI_SINGULAR_POINT_DIALOG_H_
#define DYNSOLVER_GUI_SINGULAR_POINT_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

class singular_point_dialog : public singular_point_dialog_base {
 private:
  singular_point_specification singularPointSpecification;
  
 public:
  singular_point_dialog();
  
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void add_button_on_button_click(wxCommandEvent&) override;

  // Shows the dialog modally. The dialog is populated with imformation,
  // based on the solution_specification provided. If the dialog is closed,
  // or canceled, this function returns false.
  // Otherwise it returns true and fills the specification pointer
  // with the specification that the user generated. Only one instance of this
  // class is meant to in the program. That is, you should reuse it.
  bool show_dialog(const singular_point_specification&, singular_point_specification*);

private:
  void set_ui();

  // Returns true if the imformation in the UI is valid. It it is,
  // this function also sets the specification member variable.
  bool validate_and_set_specification();
};
} // namespace gui
} // namespace dynsolver

#endif
