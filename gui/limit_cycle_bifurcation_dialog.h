#ifndef DYNSOLVER_GUI_LIMIT_CYCLE_BIFURCATION_DIALOG_H_
#define DYNSOLVER_GUI_LIMIT_CYCLE_BIFURCATION_DIALOG_H_

#include "gui/generated.h"
#include "gui/model.h"

namespace dynsolver {
namespace gui {

class app;

class limit_cycle_bifurcation_dialog
  : public limit_cycle_bifurcation_dialog_base {
private:
  const app& appl;
  limit_cycle_bifurcation_specs specs;
  wxPGProperty* transversalAHorizPGItem;
  wxPGProperty* transversalAVertPGItem;
  wxPGProperty* transversalBHorizPGItem;
  wxPGProperty* transversalBVertPGItem;

public:
  limit_cycle_bifurcation_dialog(const app&);
 
  bool show_dialog(limit_cycle_bifurcation_specs& specs);
  virtual void cancel_button_on_button_click(wxCommandEvent&) override;
  virtual void ok_button_on_button_click(wxCommandEvent&) override;

private:
  void set_ui();

  bool validate_and_set();
};
} // namespace gui
} // namespace dynsolver

#endif
