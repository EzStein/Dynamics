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
/*Each variable in this class will be marked by which threads will access it. The wxWidgets event thread will
* be called the event dispatch thread (EDT). Threads for computing the image to be drawn will be called computation threads (CTs)*/
class app_state {
public:
  /*Constructs the state class with a pointer to the constructing app*/
  app_state(app*);
  ~app_state();

  /*The state class cannot be copied or moved*/
  app_state(const app_state&) = delete;
  app_state& operator=(const app_state&) = delete;
  app_state(const app_state&&) = delete;
  app_state& operator=(const app_state&&) = delete;

  /*The controller can modify state where as the view can only retrieve state through getters*/
  friend class app;

  const wxBitmap& get_image() const {
    return image;
  }

  const vector_2d<int>& get_box_drag_start() const {
    return boxDragStart;
  }

  const bool& get_draw_drag_box() const {
    return drawDragBox;
  }

  const vector_2d<int>& get_box_drag_change() const {
    return boxDragChange;
  }

  /*A reference to the controller*/
  //ACCESS: EDT
  app * const controller;

private:
  //ACCESS: EDT
  vector_2d<double> boundaryTopLeftValue;
  vector_2d<double> boundaryBottomRightValue;
  vector_2d<int> boxDragStart;
  vector_2d<int> boxDragChange;
  bool drawDragBox;

  //ACCESS: CTs
  bool acceptInput;
  bool interruptComputationThreads;
  std::mutex imageLock;
  wxBitmap image;
};

#endif
