#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include <iostream>

class MyApp : public wxApp {
  public:
    virtual bool OnInit();
  private:
    wxFrame * frame;
};

class MyFrame : public wxFrame {
public:
  MyFrame(const wxString&, const wxPoint&, const wxSize&);
private:
  void onButtonClick(wxCommandEvent&);
  void onHelpMenuClick(wxCommandEvent&);
  void onQuitMenuClick(wxCommandEvent&);
};

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

//Represents a vector of two elements x and y whose type is the template numeric type.
template <class NUM_TYPE>
class Vector2D {
public:
  const NUM_TYPE x;
  const NUM_TYPE y;
  Vector2D(NUM_TYPE _x, NUM_TYPE _y) : x(_x), y(_y) { };
  template <class A> Vector2D(const Vector2D<A>& vec)
  : x(vec.x), y(vec.y) { };
  operator std::string() const;
};

template<class A, class B> Vector2D<A> operator+(Vector2D<A> vec1, Vector2D<B> vec2);
template<class A, class B> Vector2D<A> operator-(Vector2D<A> vec1, Vector2D<B> vec2);
template<class A, class B> Vector2D<B> operator*(Vector2D<B> vec, A scalar);
template<class A, class B> Vector2D<B> operator*(A scalar, Vector2D<B> vec);


/*
* Represents a 2x2 matrix for 2D operations.
* Induvidual elements, MIJ represents the element in the Ith row and Jth column
*/
template <class NUM_TYPE>
class Matrix2D {
public:
  const NUM_TYPE M11, M12, M21, M22;
  Matrix2D(NUM_TYPE _M11,NUM_TYPE _M12,NUM_TYPE _M21,NUM_TYPE _M22)
  : M11(_M11), M12(_M12), M21(_M21), M22(_M22) { }

  template<class A> Matrix2D(const Matrix2D<A>& mat)
  : M11(mat.M11), M12(mat.M12), M21(mat.M21), M22(mat.M22) { };

  Matrix2D<NUM_TYPE> transpose() const;
  Matrix2D<NUM_TYPE> inverse() const;
  NUM_TYPE determinant() const;
  operator std::string() const;
};

template<class A, class B> Matrix2D<A> operator+(Matrix2D<A> mat1, Matrix2D<B> mat2);
template<class A, class B> Matrix2D<A> operator-(Matrix2D<A> mat1, Matrix2D<B> mat2);
template<class A, class B> Matrix2D<A> operator*(Matrix2D<A> mat1, Matrix2D<B> mat2);
template<class A, class B> Vector2D<B> operator*(Matrix2D<A> mat, Vector2D<B> vec);
template<class A, class B> Matrix2D<B> operator*(Matrix2D<B> mat, A scalar);
template<class A, class B> Matrix2D<B> operator*(A scalar, Matrix2D<B> mat);

Vector2D<int> valueToPixel(const Vector2D<double>&, const Vector2D<int>&,
  const Vector2D<double>&, const Vector2D<double>&);

Vector2D<double> pixelToValue(const Vector2D<int>&, const Vector2D<int>&,
  const Vector2D<double>&, const Vector2D<double>&);

class Fractal {
public:
  int compute(const Vector2D<double>&);
};
