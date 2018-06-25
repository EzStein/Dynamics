#ifndef DYNSOLVER_GUI_DYNAMICAL_FRAME_H_
#define DYNSOLVER_GUI_DYNAMICAL_FRAME_H_
#include "gui/app.h"
#include <wx/glcanvas.h>
#include <wx/timer.h>

#include "gui/common.h"
#include "gui/generated.h"

namespace dynsolver {
namespace gui {

class app;

// Subclass of the dynamical_frame_base implements the view controller for each
// instance of a dynamical frame.
class dynamical_frame : public dynamical_frame_base {
 private:
  // A reference to the model object of this program. 
  app& appl;

  // The opengl canvas and opengl context used to render the dynamical window
  wxGLCanvas* glCanvas;
  
  // The id associated to this frame.
  dynamical_window_id id;

  // Variables which are set whenever the mouse is right clicked.
  // The value is stored until the next right click.
  int rightClickMouseX, rightClickMouseY;

  // These variables store the position of the mouse after the left mouse
  // button is clicked. The values are updated during mouse down events,
  // and mouse up events, but not during a drag.
  int leftClickMouseX, leftClickMouseY;

  // Represents the magnification due to scrolling.
  double totalMagnification;

  // The X and Y pixels of the mouse when magnification begins.
  double magnificationX, magnificationY;

  // The viewport as it exists when magnifcation starts.
  // Locked in on magnification end.
  window2d magnificationViewport;

  // These values are the real x and y positions associated with the
  // top left corner of the window. They are locked in when the left
  // mouse button is depressed and when it is released but not when
  // it is dragged.
  point2d realDragPosition;

  // True when the first wheel event has note yet been processed. False
  // after the first wheel event has been processed and the wheel event
  // has not ended.
  bool firstWheelEvent;

  // This timer is used to signal the wheel end event once
  // mouse_wheel events have been idle for long enough.
  wxTimer magnificationTimer;
  static const int kMagnificationTimerEventId;

  // Set to true when we are dragging is meant to cause a scaling on,
  // the vertical or horizontal axes.
  bool verticalScaling, horizontalScaling;
 public:
  dynamical_frame(app&, dynamical_window_id id, int width, int height);
  ~dynamical_frame();

  virtual void dynamical_frame_on_set_focus(wxFocusEvent&) override;
  virtual void solution_menu_on_menu_selection(wxCommandEvent&) override;
  virtual void isocline_menu_on_menu_selection(wxCommandEvent&) override;
  virtual void singular_point_menu_on_menu_selection(wxCommandEvent&) override;
  virtual void dynamical_frame_on_close(wxCloseEvent&) override;
  virtual void edit_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void dynamical_frame_on_iconize(wxIconizeEvent& evt) override;
  void gl_canvas_on_key_down(wxKeyEvent&);
  void gl_canvas_on_key_up(wxKeyEvent&);
  void gl_canvas_on_left_down(wxMouseEvent&);
  void gl_canvas_on_left_up(wxMouseEvent&);
  void gl_canvas_on_motion(wxMouseEvent&);
  void gl_canvas_on_mouse_wheel(wxMouseEvent&);
  void gl_canvas_on_right_down(wxMouseEvent&);
  void gl_canvas_on_right_up(wxMouseEvent&);
  void gl_canvas_on_paint(wxPaintEvent&);
  void gl_canvas_on_size(wxSizeEvent&);
  void refresh_gl_canvas();

  // Called each once before mouse wheel events starto and end.
  void gl_canvas_on_mouse_wheel_start(int, int);
  void gl_canvas_on_mouse_wheel_end(wxTimerEvent&);
};
} // namespace gui
} // namespace dynsolver

#endif
