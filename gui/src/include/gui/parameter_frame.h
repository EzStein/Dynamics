#ifndef DYNSOLVER_GUI_PARAMETER_FRAME_H_
#define DYNSOLVER_GUI_PARAMETER_FRAME_H_

#include <wx/timer.h>

#include "gui/generated.h"
#include "gui/common.h"
#include "math/window2d.h"
#include "math/vector2d.h"

namespace dynsolver {
namespace gui {

class app;

// Subclass of parameter_frame_base which implements the view for the parameter
// windows.
class parameter_frame : public parameter_frame_base {
private:
  app& appl;

  // The parameter id associated with this window.
  parameter_id id;

  // Note that all positions are given in window coordinates. Window
  // coordinates are defined to be pixel position relative to the bottom
  // left corner of the window. They are represented as doubles.

  // The pixel distance used to determine whether a mouse up and
  // mouse down event should constitute a mouse click.
  static const double kClickTolerance;

  // Variables which are set whenever the mouse is right clicked
  // (mouse down event). The value is stored until the next right click.
  ::math::vector2d rightClickMousePos;

  // These variables store the position of the mouse after the left mouse
  // button is clicked. The values are updated during mouse down events.
  ::math::vector2d leftClickMousePos;

  // The window as it exists before a move is initiated.
  // Locked in when the left mouse is depressed.
  ::math::window2d moveViewport;

  // Represents the magnification due to scrolling.
  double totalMagnification;

  // The X and Y pixels of the mouse when magnification begins.
  // These are set on a magnification start.
  ::math::vector2d magnificationPos;

  // The viewport as it exists when magnifcation starts.
  // Locked in on magnification start and end.
  ::math::window2d magnificationViewport;

  // True when the first wheel event has note yet been processed. False
  // after the first wheel event has been processed and the wheel event
  // has not ended.
  bool firstWheelEvent;

  // This timer is used to signal the wheel end event once
  // mouse_wheel events have been idle for long enough.
  wxTimer magnificationTimer;
  static const int kMagnificationTimerEventId;

  // The rate of magnification
  static const double kMagnificationRate;

  // The elapsed time in milliseconds after the last wheel event before
  // magnification stops.
  static const int kMagnificationTimerWait;

  // Set to true when we dragging is meant to cause a scaling on,
  // the vertical or horizontal axes. We lock these values in along with
  // the axesScalingViewport when we left click on an axis.
  bool verticalScaling, horizontalScaling;
  ::math::window2d axesScalingViewport;

  // The maximum axes scaling allowed.
  static const double kMaxAxesScale;

  // Indicates that a left click drag event corresponds to moving the
  // parameter position.
  bool parameterMoving;

public:
  parameter_frame(app& app, parameter_id id, int width, int height);
  ~parameter_frame();
  virtual void canvas_on_key_down(wxKeyEvent&) override;
  virtual void canvas_on_key_up(wxKeyEvent&) override;
  virtual void canvas_on_left_down(wxMouseEvent&) override;
  virtual void canvas_on_left_up(wxMouseEvent&) override;
  virtual void canvas_on_motion(wxMouseEvent&) override;
  virtual void canvas_on_right_down(wxMouseEvent&) override;
  virtual void canvas_on_right_up(wxMouseEvent& evt) override;
  virtual void canvas_on_paint(wxPaintEvent&) override;
  virtual void canvas_on_size(wxSizeEvent&) override;

  virtual void parameter_frame_on_close(wxCloseEvent&) override;
  virtual void hopf_bifurcation_menu_item_on_selection(wxCommandEvent&) override;
  virtual void saddle_node_bifurcation_menu_item_on_selection(wxCommandEvent&) override;
  virtual void limit_cycle_bifurcation_menu_item_on_selection(wxCommandEvent&) override;
  virtual void saddle_connection_bifurcation_menu_item_on_selection(wxCommandEvent&) override;

  virtual void canvas_on_mouse_wheel(wxMouseEvent&) override;

  // Called when the mouse wheel ends by a timer.
  void canvas_on_mouse_wheel_end(wxTimerEvent&);

  // Returns the window coordinates of the mouse cursor for the given mouse
  // event.
  ::math::vector2d get_window_coordinates(const wxMouseEvent&);

  // When the left or right mouse button are pressed and released in the
  // same spot, a mouse_click event is generated.
  void canvas_on_right_mouse_click(const ::math::vector2d& pos);
  void canvas_on_left_mouse_click(const ::math::vector2d& pos);

  // Set's the cursors image according to its position on the canvas.
  void set_cursor(const ::math::vector2d& pos);

  // Refreshes the canvas.
  void refresh_canvas();
};
} // namespace gui
} // namespace dynsolver

#endif
