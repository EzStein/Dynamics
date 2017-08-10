#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "top_frame.h"

top_frame::top_frame(wxWindow* window, wxWindowID id) : top_frame_base(window, id) { };

void top_frame::dynamical_plane_on_paint(wxPaintEvent& event) {
  wxPaintDC dc(dynamicalPlane);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(0,0,80, 80);
}
