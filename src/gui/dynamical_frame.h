#ifndef DYNSOLVER_GUI_DYNAMICAL_FRAME_H_
#define DYNSOLVER_GUI_DYNAMICAL_FRAME_H_

#include <wx/glcanvas.h>

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
  typedef int dynamical_window_id;
  dynamical_window_id id;
  
 public:
  dynamical_frame(app&, int id);

  void dynamical_frame_on_set_focus(wxFocusEvent&) override;
  void dynamical_window_box_on_key_down(wxKeyEvent&) override;
  void dynamical_window_box_on_key_up(wxKeyEvent&) override;
  void dynamical_window_box_on_left_down(wxMouseEvent&) override;
  void dynamical_window_box_on_left_up(wxMouseEvent&) override;
  void dynamical_window_box_on_motion(wxMouseEvent&) override;
  void dynamical_window_box_on_mouse_wheel(wxMouseEvent&) override;
  void dynamical_window_box_on_right_down(wxMouseEvent&) override;
  void dynamical_window_box_on_right_up(wxMouseEvent&) override;
  void dynamical_window_box_on_paint(wxPaintEvent&) override;
  void dynamical_window_box_on_size(wxSizeEvent&) override;
};
} // namespace gui
} // namespace dynsolver

#endif
