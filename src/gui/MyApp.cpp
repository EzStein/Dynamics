#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <iostream>
#include <sstream>

#include <unistd.h>
#include "../app/app_traits.h"
#include "MyApp.h"
#include "../math/matrix_2d.h"
#include "../math/vector_2d.h"


/*MyPanel::MyPanel(wxFrame* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(800, 800)) {
  wxImage::AddHandler(new wxPNGHandler);
  wxImage im(APP_TRAITS_RESOURCE_PATH(Spiral.png), wxBITMAP_TYPE_PNG);


  image = wxBitmap(im);
  Bind(wxEVT_PAINT, &MyPanel::paintEvent, this);
  Bind(wxEVT_MOTION, &MyPanel::motionEvent, this);
  Bind(wxEVT_LEFT_DCLICK, &MyPanel::mouseEvent, this);
}

void MyPanel::mouseEvent(wxMouseEvent& event) {
  std::cout << "ABC" << std::endl;
  Fractal frac;
  int width, height;
  GetSize(&width, &height);
  unsigned char * data = static_cast<unsigned char *>(malloc(width * height * 3));
  for(int j = 0; j < height; j++) {
    for(int i = 0; i < width; i++) {
      vector_2d<double> res = pixelToValue(vector_2d<int>(i, j), vector_2d<int>(width, height), vector_2d<double>(-2, 2), vector_2d<double>(2,-2));
      if(frac.compute(res)) {
        data[j * width * 3 + i * 3] = 0x00;
        data[j * width * 3 + i * 3 + 1] = 0x00;
        data[j * width * 3 + i * 3 + 2] = 0x00;
      } else {
        data[j * width * 3 + i * 3] = 0xFF;
        data[j * width * 3 + i * 3 + 1] = 0xFF;
        data[j * width * 3 + i * 3 + 2] = 0xFF;
      }
    }
  }
  wxImage im(width, height, data, false);
  image = wxBitmap(im);
  paintNow();
}

void MyPanel::motionEvent(wxMouseEvent& event) {
  vector_2d<int> vec(event.GetX(), event.GetY());
  int width, height;
  GetSize(&width, &height);
  vector_2d<double> res = pixelToValue(vec, vector_2d<int>(width, height), vector_2d<double>(-2, 2), vector_2d<double>(2,-2));
  std::string s = res;
}

void MyPanel::paintEvent(wxPaintEvent& event) {
  wxPaintDC dc(this);
  render(dc);
}

void MyPanel::paintNow() {
  wxClientDC dc(this);
  render(dc);
}

void MyPanel::render(wxDC& dc) {
  dc.SetBrush(*wxWHITE_BRUSH);
  int width, height;
  GetSize(&width, &height);
  dc.DrawRectangle(0, 0, width, height);
  dc.DrawBitmap(image, 0, 0);
}





int Fractal::compute(const vector_2d<double>& point) {
  int iterations = 0;
  double x = point.x;
  double y = point.y;
  for(; iterations < 1000 && (x * x + y * y < 4); iterations++) {
    double tmp = x;
    x = x * x - y * y + point.x;
    y = 2 * tmp * y + point.y;
  }
  return (x * x + y * y >= 4);
}*/
