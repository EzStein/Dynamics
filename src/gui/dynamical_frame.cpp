#include "gui/dynamical_frame.h"

#include "gui/generated.h"
#include <wx/glcanvas.h>

namespace dynsolver {
namespace gui {

dynamical_frame::dynamical_frame(app& app, int id) :
  dynamical_frame_base(NULL), appl(app), id(id) {
  // Setup events and widgets not already done in dynamical_frame_base
  glCanvas = new wxGLCanvas(this, appl.getGlAttributes());
  dynamicalWindowBox->Add(glCanvas, 1, wxEXPAND);
  glCanvas->Bind(wxEVT_PAINT, &dynamical_frame::gl_canvas_on_paint, this);
  glCanvas->Bind(wxEVT_KEY_DOWN, &dynamical_frame::gl_canvas_on_key_down, this);
  glCanvas->Bind(wxEVT_KEY_UP, &dynamical_frame::gl_canvas_on_key_up, this);
  glCanvas->Bind(wxEVT_LEFT_DOWN, &dynamical_frame::gl_canvas_on_left_down, this);
  glCanvas->Bind(wxEVT_LEFT_UP, &dynamical_frame::gl_canvas_on_left_up, this);
  glCanvas->Bind(wxEVT_MOTION, &dynamical_frame::gl_canvas_on_motion, this);
  glCanvas->Bind(wxEVT_MOUSEWHEEL, &dynamical_frame::gl_canvas_on_mouse_wheel, this);
  glCanvas->Bind(wxEVT_RIGHT_DOWN, &dynamical_frame::gl_canvas_on_right_down, this);
  glCanvas->Bind(wxEVT_RIGHT_UP, &dynamical_frame::gl_canvas_on_right_up, this);
  glCanvas->Bind(wxEVT_SIZE, &dynamical_frame::gl_canvas_on_size, this);
}

dynamical_frame::~dynamical_frame() {
  glCanvas->Unbind(wxEVT_PAINT, &dynamical_frame::gl_canvas_on_paint, this);
  glCanvas->Unbind(wxEVT_KEY_DOWN, &dynamical_frame::gl_canvas_on_key_down, this);
  glCanvas->Unbind(wxEVT_KEY_UP, &dynamical_frame::gl_canvas_on_key_up, this);
  glCanvas->Unbind(wxEVT_LEFT_DOWN, &dynamical_frame::gl_canvas_on_left_down, this);
  glCanvas->Unbind(wxEVT_LEFT_UP, &dynamical_frame::gl_canvas_on_left_up, this);
  glCanvas->Unbind(wxEVT_MOTION, &dynamical_frame::gl_canvas_on_motion, this);
  glCanvas->Unbind(wxEVT_MOUSEWHEEL, &dynamical_frame::gl_canvas_on_mouse_wheel, this);
  glCanvas->Unbind(wxEVT_RIGHT_DOWN, &dynamical_frame::gl_canvas_on_right_down, this);
  glCanvas->Unbind(wxEVT_RIGHT_UP, &dynamical_frame::gl_canvas_on_right_up, this);
  glCanvas->Unbind(wxEVT_SIZE, &dynamical_frame::gl_canvas_on_size, this);
}

void dynamical_frame::dynamical_frame_on_set_focus(wxFocusEvent& evt) { }
void dynamical_frame::gl_canvas_on_key_down(wxKeyEvent& evt) { }
void dynamical_frame::gl_canvas_on_key_up(wxKeyEvent& evt) { }
void dynamical_frame::gl_canvas_on_left_down(wxMouseEvent& evt) {
  int x, y;
  evt.GetPosition(&x, &y);
  window2d window(appl.get_model().get_dynamical_window(id).get_window());
  point2d initialPoint(window.real_coordinate_of(point2d(x, y)));
  math::vector initialVector(appl.get_model().get_dynamical_variables(), 0.0);
  appl.add_solution(initialVector, -10, 10, 0.01);
}
void dynamical_frame::gl_canvas_on_left_up(wxMouseEvent& evt) { }
void dynamical_frame::gl_canvas_on_motion(wxMouseEvent& evt) { }
void dynamical_frame::gl_canvas_on_mouse_wheel(wxMouseEvent& evt) { }

void dynamical_frame::gl_canvas_on_right_down(wxMouseEvent& evt) { }
void dynamical_frame::gl_canvas_on_right_up(wxMouseEvent& evt) { }

void dynamical_frame::gl_canvas_on_size(wxSizeEvent& evt) {
  appl.resize_dynamical_window(id);
}

void dynamical_frame::refresh_gl_canvas() {
  glCanvas->Refresh();
}

void dynamical_frame::gl_canvas_on_paint(wxPaintEvent& evt) {
  if(glCanvas->IsShownOnScreen()) {
    appl.get_gl_context().SetCurrent(*glCanvas);
    appl.paint_dynamical_window(id);
    glCanvas->SwapBuffers();
  }
}
} // namespace dynslover
} // namespace gui
