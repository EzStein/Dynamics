#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <iostream>
#include "../math/vector_2d.h"
#include "../math/matrix_2d.h"

class MyPanel : public wxPanel {
public:
  MyPanel(wxFrame*);
  void paintNow();
  void paintEvent(wxPaintEvent&);
  void motionEvent(wxMouseEvent&);
  void mouseEvent(wxMouseEvent&);
private:
  void render(wxDC&);
  wxBitmap image;
};






class Fractal {
public:
  int compute(const vector_2d<double>&);
};
