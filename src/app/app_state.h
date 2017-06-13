#ifndef APP_STATE
#define APP_STATE
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif

#include "../math/vector_2d.h"
#include "app.h"

class app;

/*This class holds all the data for the application. It may be considered as the model in the model view controller design*/
class app_state {
public:
  app_state(app*);
  ~app_state();
  app_state(const app_state&) = delete;
  app_state& operator=(const app_state&) = delete;


  /*A reference to the controller*/
  app * const controller;

  /*The controller can modify state where as the view can only retrieve state through getters*/
  friend class app;


  const vector_2d<double>& get_boundary_top_left_value() const {
    return boundaryTopLeftValue;
  }

  const vector_2d<double>& get_boundary_bottom_right_value() const {
    return boundaryBottomRightValue;
  }

  const wxBitmap& get_image() const {
    return image;
  }

  const vector_2d<int>& get_box_drag_top_left() const {
    return boxDragTopLeft;
  }

  const bool& get_draw_drag_box() const {
    return drawDragBox;
  }

  int get_box_drag_size() const {
    return boxDragSize;
  }

private:
  vector_2d<double> boundaryTopLeftValue;
  vector_2d<double> boundaryBottomRightValue;
  wxBitmap image;
  vector_2d<int> boxDragTopLeft;
  int boxDragSize;
  bool drawDragBox;
};

#endif
