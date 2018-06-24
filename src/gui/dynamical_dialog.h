#ifndef DYNSOLVER_GUI_DYNAMICAL_DIALOG_H_
#define DYNSOLVER_GUI_DYNAMICAL_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

class app;

class dynamical_dialog : public dynamical_dialog_base {
 private:
  dynamical_window_specification dynamicalSpecification;
  wxPGProperty* horizontalAxisVariablePropertyGridItem;
  wxPGProperty* horizontalAxisMinPropertyGridItem;
  wxPGProperty* horizontalAxisMaxPropertyGridItem;
  wxPGProperty* verticalAxisVariablePropertyGridItem;
  wxPGProperty* verticalAxisMinPropertyGridItem;
  wxPGProperty* verticalAxisMaxPropertyGridItem;
  
 public:
  dynamical_dialog();
  
  // Shows the dialog modally. The dialog is populated with imformation,
  // based on the specification provided. If the dialog is closed,
  // or canceled, this function returns false.
  // Otherwise it returns true and fills the solution_specification pointer
  // with the specification that the user generated. Only one instance of this
  // class is meant to be in the program. That is, you should reuse it.
  bool show_dialog(const dynamical_window_specification&, dynamical_window_specification*);
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void ok_button_on_button_click(wxCommandEvent&) override;
  
private:
  // Set's the UI according to the specification variable.
  void set_ui();

  // Returns true if the imformation in the UI gives a valid,
  // specification. If it does, this function sets specification,
  // to that specification, and returns true.
  bool validate_and_set_specification();
};
} // namespace gui
} // namespace dynsolver

#endif
