#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <iostream>
#include <sstream>

#include <unistd.h>
#include "../app/app_traits.h"
#include "MyApp.h"



bool MyApp::OnInit() {
  frame = new MyFrame("HELLO", wxPoint(100,100), wxSize(100,100));
  frame->Show();
  return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, wxID_ANY, title, pos, size) {
    wxMenuBar * menu_bar = new wxMenuBar;
    wxMenu * file_menu = new wxMenu;
    wxMenu * edit_menu = new wxMenu;
    wxMenu * about_menu = new wxMenu;



    file_menu->Append(1, "Open...\tCTRL+O");
    file_menu->Append(2, "Close...\tCTRL+X");
    file_menu->AppendSeparator();
    file_menu->Append(3, "Save...\tCTRL+S");
    file_menu->Append(4, "Save As...\tCTRL+D");

    edit_menu->Append(5, "Copy...\tCTRL+C");
    about_menu->Append(6, "About...\tCTRL+A");

    menu_bar->Append(file_menu, "File");
    menu_bar->Append(edit_menu, "Edit");
    menu_bar->Append(about_menu, "About");
    SetMenuBar(menu_bar);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::onButtonClick, this);


    wxBoxSizer *sizerV = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerH = new wxBoxSizer(wxHORIZONTAL);
    sizerV->Add(new MyPanel(this), 1, wxGROW, 20);
    sizerV->Add(sizerH, 0, wxGROW, 0);
    sizerH->Add(new wxButton(this, -1, "A Button"), 1, wxALL, 0);
    sizerH->Add(new wxButton(this, -1, "Tiny Button"), 0, wxALL, 0);
    sizerV->SetSizeHints(this);
    SetSizer(sizerV);

    Center();
}

void MyFrame::onButtonClick(wxCommandEvent& event) {

  event.Skip();
}
void MyFrame::onHelpMenuClick(wxCommandEvent& event) {

}
void MyFrame::onQuitMenuClick(wxCommandEvent& event) {

}

MyPanel::MyPanel(wxFrame* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(800, 800)) {
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
      Vector2D<double> res = pixelToValue(Vector2D<int>(i, j), Vector2D<int>(width, height), Vector2D<double>(-2, 2), Vector2D<double>(2,-2));
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
  Vector2D<int> vec(event.GetX(), event.GetY());
  int width, height;
  GetSize(&width, &height);
  Vector2D<double> res = pixelToValue(vec, Vector2D<int>(width, height), Vector2D<double>(-2, 2), Vector2D<double>(2,-2));
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





template<class NUM_TYPE> Vector2D<NUM_TYPE>::operator std::string() const {
  std::ostringstream sstream;
  sstream << '(' << x << ',' << y << ')';
  return sstream.str();
}

/*
* NOTE: When the type of both inputs is the same, this function will behave as expected.
* However if the types differ, the resultant type will be the same as that of the
* first operand! Thus be wary about adding ints to doubles!
*/
template<class A, class B> Vector2D<A> operator+(Vector2D<A> vec1, Vector2D<B> vec2) {
  return Vector2D<A>(vec1.x + vec2.x, vec1.y + vec2.y);
}

/*
* NOTE: When the type of both inputs is the same, this function will behave as expected.
* However if the types differ, the resultant type will be the same as that of the
* first operand! Thus be wary about subtracting doubles from ints!
*/
template<class A, class B> Vector2D<A> operator-(Vector2D<A> vec1, Vector2D<B> vec2) {
  return Vector2D<A>(vec1.x - vec2.x, vec1.y - vec2.y);
}

/*
* Note the resultant type will be a Vector2D of the same type as the input operand.
*/
template<class A, class B> Vector2D<B> operator*(A scalar, Vector2D<B> vec) {
  return Vector2D<B>(scalar * vec.x, scalar * vec.y);
}

/*
* Note the resultant type will be a Vector2D of the same type as the input operand.
*/
template<class A, class B> Vector2D<B> operator*(Vector2D<B> vec, A scalar) {
  return Vector2D<B>(scalar * vec.x, scalar * vec.y);
}


template<class A, class B> Matrix2D<A> operator+(Matrix2D<A> mat1, Matrix2D<B> mat2) {
  return Matrix2D<A>(mat1.M11 + mat2.M11, mat1.M12 + mat2.M12, mat1.M21 + mat2.M21, mat1.M22 + mat2.M22);
}

template<class A, class B> Matrix2D<A> operator-(Matrix2D<A> mat1, Matrix2D<B> mat2) {
  return Matrix2D<A>(mat1.M11 - mat2.M11, mat1.M12 - mat2.M12, mat1.M21 - mat2.M21, mat1.M22 - mat2.M22);
}
template<class A, class B> Matrix2D<A> operator*(Matrix2D<A> mat1, Matrix2D<B> mat2) {
  return Matrix2D<A>(
    mat1.M11 * mat2.M11 + mat1.M12 * mat2.M21,
    mat1.M11 * mat2.M12 + mat1.M12 * mat2.M22,
    mat1.M21 * mat2.M11 + mat1.M22 * mat2.M21,
    mat1.M21 * mat2.M12 + mat1.M22 * mat2.M22);
}
template<class A, class B> Vector2D<B> operator*(Matrix2D<A> mat, Vector2D<B> vec) {
  return Vector2D<B>(
    mat.M11 * vec.x + mat.M12 * vec.y,
    mat.M21 * vec.x + mat.M22 * vec.y
  );
}

template<class A, class B> Matrix2D<B> operator*(Matrix2D<B> mat, A scalar) {
  return Matrix2D<B>(scalar * mat.M11, scalar * mat.M12, scalar * mat.M21, scalar * mat.M22);
}

template<class A, class B> Matrix2D<B> operator*(A scalar, Matrix2D<B> mat) {
  return Matrix2D<B>(scalar * mat.M11, scalar * mat.M12, scalar * mat.M21, scalar * mat.M22);
}

template<class NUM_TYPE> Matrix2D<NUM_TYPE> Matrix2D<NUM_TYPE>::transpose() const {
  return Matrix2D<NUM_TYPE>(M11, M21, M12, M22);
}

template<class NUM_TYPE> NUM_TYPE Matrix2D<NUM_TYPE>::determinant() const {
  return M11 * M22 - M12 * M21;
}

template<class NUM_TYPE> Matrix2D<NUM_TYPE> Matrix2D<NUM_TYPE>::inverse() const {
  NUM_TYPE det = determinant();
  return Matrix2D<NUM_TYPE>(M22/det, -M12/det, -M21/det, M11/det);
}

template<class NUM_TYPE> Matrix2D<NUM_TYPE>::operator std::string() const {
  std::ostringstream sstream;
  sstream << '\n' << '|' << M11 << ' ' << M12 << '|' << '\n';
  sstream << '|' << M21 << ' ' << M22 << '|' << '\n';
  return sstream.str();
}

Vector2D<double> pixelToValue(const Vector2D<int>& pixel, const Vector2D<int>& pixelBoundary,
  const Vector2D<double>& valueBoundaryTop, const Vector2D<double>& valueBoundaryBottom) {

  double scaling = (valueBoundaryBottom.x - valueBoundaryTop.x)/pixelBoundary.x;
  Matrix2D<double> mat(scaling, 0, 0, -scaling);
  return valueBoundaryTop + mat * Vector2D<double>(pixel);
}

Vector2D<int> valueToPixel(const Vector2D<double>& value, const Vector2D<int>& pixelBoundary,
  const Vector2D<double>& valueBoundaryTop, const Vector2D<double>& valueBoundaryBottom) {

  double scaling = (valueBoundaryBottom.x - valueBoundaryTop.x)/pixelBoundary.x;
  Matrix2D<double> mat(1/scaling, 0, 0, -1/scaling);
  return mat * (value - valueBoundaryTop);
}

int Fractal::compute(const Vector2D<double>& point) {
  int iterations = 0;
  double x = point.x;
  double y = point.y;
  for(; iterations < 1000 && (x * x + y * y < 4); iterations++) {
    double tmp = x;
    x = x * x - y * y + point.x;
    y = 2 * tmp * y + point.y;
  }
  return (x * x + y * y >= 4);
}
