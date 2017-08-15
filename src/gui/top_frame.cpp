#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "top_frame.h"
#include "compiler/front/driver.h"
#include <string>
#include <iostream>

top_frame::top_frame(wxWindow* window, wxWindowID id) : top_frame_base(window, id) { };

void top_frame::dynamical_plane_on_paint(wxPaintEvent& event) {
  wxPaintDC dc(dynamicalPlane);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(0,0,80, 80);
}

void top_frame::render_button_on_button_click(wxCommandEvent& event) {
  std::string str(text_field->GetValue().mb_str());
  std::cout << dr.compile_as_function<driver::var4_double_func_t>(str)(1,2,3,4) << std::endl;
}
