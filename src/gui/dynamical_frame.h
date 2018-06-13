#ifndef DYNSOLVER_GUI_DYNAMICAL_FRAME_H_
#define DYNSOLVER_GUI_DYNAMICAL_FRAME_H_
#include "gui/app.h"
#include <wx/glcanvas.h>

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
  
 public:
  dynamical_frame(app&, dynamical_window_id id);
  ~dynamical_frame();

  void dynamical_frame_on_set_focus(wxFocusEvent&) override;
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
};
} // namespace gui
} // namespace dynsolver

#endif
