#ifndef DYNSOLVER_GUI_DYNAMICAL_DIALOG_H_
#define DYNSOLVER_GUI_DYNAMICAL_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

class app;

class dynamical_dialog : public dynamical_dialog_base {
 private:
  // A reference to the application.
  const app& appl;
  dynamical_specs specs;
  wxPGProperty* horizAxisVarPGItem;
  wxPGProperty* horizAxisMinPGItem;
  wxPGProperty* horizAxisMaxPGItem;
  wxPGProperty* vertAxisVarPGItem;
  wxPGProperty* vertAxisMinPGItem;
  wxPGProperty* vertAxisMaxPGItem;
  wxPGProperty* cameraPositionXPropertyGridItem;
  wxPGProperty* cameraPositionYPropertyGridItem;
  wxPGProperty* cameraPositionZPropertyGridItem;
  wxPGProperty* cameraDirectionXPropertyGridItem;
  wxPGProperty* cameraDirectionYPropertyGridItem;
  wxPGProperty* cameraDirectionZPropertyGridItem;
  wxPGProperty* upDirectionXPropertyGridItem;
  wxPGProperty* upDirectionYPropertyGridItem;
  wxPGProperty* upDirectionZPropertyGridItem;
  wxPGProperty* zNearPropertyGridItem;
  wxPGProperty* zFarPropertyGridItem;
  wxPGProperty* xAxisVariablePropertyGridItem;
  wxPGProperty* yAxisVariablePropertyGridItem;
  wxPGProperty* zAxisVariablePropertyGridItem;

  // List of variable choices.
  wxPGChoices varChoices;
  
  
 public:
  dynamical_dialog(const app&);
  
  // Shows the dialog modally. The dialog is populated with imformation,
  // based on the specification provided. If the dialog is closed,
  // or canceled, this function returns false.
  // Otherwise it updates the dynamical_specs reference.
  // with the specification that the user generated. Only one instance of this
  // class is meant to be in the program. That is, you should reuse it.
  bool show_dialog(dynamical_specs&);
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void ok_button_on_button_click(wxCommandEvent&) override;
  virtual void viewport_radio_box_on_radio_box(wxCommandEvent&) override;
  
private:
  // Set's the UI according to the specification variable.
  void set_ui();

  // Returns true if the imformation in the UI gives a valid,
  // specification. If it does, this function sets specification,
  // to that specification, and returns true.
  bool validate_and_set();
};
} // namespace gui
} // namespace dynsolver

#endif
