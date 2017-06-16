#ifndef RENDER_PANEL_GUARD
#define RENDER_PANEL_GUARD

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "../app/app_state.h"
#include "../math/vector_2d.h"

class app_state;
class render_panel : public wxPanel {
public:
  render_panel(wxWindow *, const app_state *);
  void on_paint(wxPaintEvent&);
  void paint_now();
  void render(wxDC&);
private:
  const app_state * state;

  void on_mouse_motion(wxMouseEvent&);
  void on_mouse_left_down(wxMouseEvent&);
  void on_mouse_left_up(wxMouseEvent&);
  void on_custom_refresh_event(wxCommandEvent&);
};

#endif
