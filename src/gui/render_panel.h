#ifndef RENDER_PANEL_GUARD
#define RENDER_PANEL_GUARD

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "../math/vector_2d.h"

class render_panel : public wxPanel {
public:
  render_panel(wxWindow *);
  void on_paint(wxPaintEvent&);
  void paint_now();
  void render(wxDC&);
private:
  wxBitmap image;
  void on_mouse_motion(wxMouseEvent&);
  void on_mouse_left_down(wxMouseEvent&);
  void on_mouse_left_up(wxMouseEvent&);

  bool draw_rectangle;
  int x1, x2, y1, y2;
};

#endif
