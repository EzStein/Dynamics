#ifndef DYNSOLVER_GUI_HOPF_BIFURCATION_DIALOG_H_
#define DYNSOLVER_GUI_HOPF_BIFURCATION_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

class app;

class hopf_bifurcation_dialog
  : public hopf_bifurcation_dialog_base {
private:
  const app& appl;
  hopf_bifurcation_specs specs;

public:
  hopf_bifurcation_dialog(const app&);
 
  bool show_dialog(hopf_bifurcation_specs& specs);
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void ok_button_on_button_click(wxCommandEvent&) override;

private:
  void set_ui();

  bool validate_and_set();
};
} // namespace gui
} // namespace dynsolver

#endif
