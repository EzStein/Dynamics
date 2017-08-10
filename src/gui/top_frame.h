#include "top_frame_base.h"

/*The top level frame that inherets from the generated class top_frame_base*/
class top_frame : public top_frame_base {
public:
  top_frame(wxWindow*, wxWindowID);
private:
  virtual void dynamical_plane_on_paint(wxPaintEvent&) override;
  virtual void render_button_on_button_click(wxCommandEvent&) override;
};
