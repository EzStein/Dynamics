#ifndef DYNSOLVER_GUI_DYNAMICAL_FRAME_H_
#define DYNSOLVER_GUI_DYNAMICAL_FRAME_H_
#include "gui/app.h"
#include <wx/timer.h>

#include "gui/common.h"
#include "gui/generated.h"
#include "math/window3d.h"
#include "math/window2d.h"

namespace dynsolver {
namespace gui {

class app;

// Subclass of the dynamical_frame_base implements the view controller for each
// instance of a dynamical frame.
class dynamical_frame : public dynamical_frame_base {
 private:
  // A reference to the model object of this program.
  app& appl;

  // The id associated to this frame.
  dynamical_id id;

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
  // Locked in on magnification start and end.
  ::math::window2d magnificationViewport;

  // The window as it exists before a move is initiated.
  // Locked in when the left mouse is dpressed and released.
  ::math::window2d moveViewport;

  // True when the first wheel event has note yet been processed. False
  // after the first wheel event has been processed and the wheel event
  // has not ended.
  bool firstWheelEvent;

  // This timer is used to signal the wheel end event once
  // mouse_wheel events have been idle for long enough.
  wxTimer magnificationTimer;
  static const int kMagnificationTimerEventId;

  // Set to true when we are dragging is meant to cause a scaling on,
  // the vertical or horizontal axes. We lock these values in along with
  // the axesScalingViewport when we left click on an axis.
  bool verticalScaling, horizontalScaling;
  ::math::window2d axesScalingViewport;

  // This is a viewport which is locked in
  // when the left mouse button is clicked. It is used to compute the camera,
  // rotation when dragging the mouse.
  ::math::window3d rotationViewport;
 public:
  dynamical_frame(app&, dynamical_id id, int width, int height);
  ~dynamical_frame();

  virtual void dynamical_frame_on_set_focus(wxFocusEvent&) override;
  virtual void solution_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void singular_point_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void dynamical_frame_on_close(wxCloseEvent&) override;
  virtual void edit_menu_item_on_menu_selection(wxCommandEvent&) override;
  virtual void dynamical_frame_on_iconize(wxIconizeEvent& evt) override;
  virtual void selection_delete_menu_item_on_menu_selection(wxCommandEvent& evt) override;
  virtual void selection_edit_menu_item_on_menu_selection(wxCommandEvent& evt) override;
  virtual void selection_select_menu_item_on_menu_selection(wxCommandEvent& evt) override;
  virtual void isocline_menu_item_on_menu_selection(wxCommandEvent&) override;

  virtual void canvas_on_key_down(wxKeyEvent&) override;
  virtual void canvas_on_key_up(wxKeyEvent&) override;
  virtual void canvas_on_left_down(wxMouseEvent&) override;
  virtual void canvas_on_left_up(wxMouseEvent&) override;
  virtual void canvas_on_motion(wxMouseEvent&) override;
  virtual void canvas_on_right_down(wxMouseEvent&) override;
  virtual void canvas_on_right_up(wxMouseEvent&) override;
  virtual void canvas_on_paint(wxPaintEvent&) override;
  virtual void canvas_on_size(wxSizeEvent&) override;

  void canvas_on_mouse_wheel(wxMouseEvent&);
  void refresh_canvas();

private:
  // Sets the cursor according to the position provided.
  // If the cursor lies near an axes it is given a non default icon.
  void set_cursor(int x, int y);

  // Called each once before mouse wheel events starto and end.
  void canvas_on_mouse_wheel_start(int, int);
  void canvas_on_mouse_wheel_end(wxTimerEvent&);
};
} // namespace gui
} // namespace dynsolver

#endif
