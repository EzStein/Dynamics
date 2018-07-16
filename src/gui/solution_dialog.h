#ifndef DYNSOLVER_GUI_SOLUTION_DIALOG_H_
#define DYNSOLVER_GUI_SOLUTION_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

// This is used for selecting an initial value solution.
// It is a dialog that may only be shown as modal.
// To show, use the 
class solution_dialog : public solution_dialog_base {
 private:
  const app& appl;
  solution_specs specs;
  
  
 public:
  solution_dialog(const app&);

  // Shows the dialog modally. The dialog is populated with imformation,
  // based on the solution_specs provided. If the dialog is closed,
  // or canceled, this function returns false.
  // Otherwise it returns true and fills the solution_specs pointer
  // with the specification that the user generated. Only one instance of this
  // class is meant to be in the program. That is, you should reuse it.
  bool show_dialog(solution_specs&);
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void add_button_on_button_click(wxCommandEvent&) override;
private:
  // Set's the UI according to the solutionSpecification variable.
  void set_ui();

  // Returns true if the imformation in the UI gives a valid,
  // specification. If it does, this function sets solutionSpecification,
  // to that specification, and returns true.
  bool validate_and_set();
};
} // namespace gui
} // namespace dynsolver

#endif
