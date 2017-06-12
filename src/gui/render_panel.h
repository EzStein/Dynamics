#ifndef RENDER_PANEL_GUARD
#define RENDER_PANEL_GUARD

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

class render_panel : public wxPanel {
public:
  render_panel(wxWindow *);
  void on_paint(wxPaintEvent&);
  void paint_now();
  void render(wxDC&);
private:
  wxBitmap image;
};

#endif
