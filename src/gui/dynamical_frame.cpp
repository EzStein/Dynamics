#include "gui/dynamical_frame.h"

#include "gui/generated.h"
#include <wx/glcanvas.h>

#include "gui/solution_dialog.h"
#include "gui/singular_point_dialog.h"
#include "gui/dynamical_dialog.h"

namespace dynsolver {
namespace gui {

dynamical_frame::dynamical_frame(app& app, int id, int width, int height) :
  dynamical_frame_base(nullptr, wxID_ANY, "Dynamical Space", wxPoint(500,0)), appl(app), id(id) {
  // Setup events and widgets not already done in dynamical_frame_base
  glCanvas = new wxGLCanvas(this, appl.getGlAttributes(), wxID_ANY,
			    wxDefaultPosition, wxSize(width, height));
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

void dynamical_frame::dynamical_frame_on_close(wxCloseEvent&) {
  appl.delete_dynamical_window(id);
}

void dynamical_frame::edit_menu_item_on_menu_selection(wxCommandEvent& evt) {
  dynamical_window_specification spec;
  spec.dynamicalVariables = appl.get_model().get_dynamical_variables();
  window2d window(appl.get_model().get_dynamical_window(id).get_window());
  spec.horizontalAxisMin = window.get_position().x();
  spec.verticalAxisMax = window.get_position().y();

  spec.horizontalAxisMax = window.get_position().x() + window.get_span().x();
  spec.verticalAxisMin = window.get_position().y() - window.get_span().y();

  spec.horizontalAxisVariable = appl.get_model().get_dynamical_window(id)
    .get_horizontal_axis_variable();
  spec.verticalAxisVariable = appl.get_model().get_dynamical_window(id)
    .get_vertical_axis_variable();
  if(appl.get_dynamical_window_dialog()->show_dialog(spec, &spec)) {
    appl.set_dynamical_window_specification(spec, id);
  }
}

void dynamical_frame::dynamical_frame_on_set_focus(wxFocusEvent& evt) { }

void dynamical_frame::solution_menu_on_menu_selection(wxCommandEvent& evt) {
  window2d window(appl.get_model().get_dynamical_window(id).get_window());
  point2d initialPoint(window.real_coordinate_of(point2d(rightClickMouseX, rightClickMouseY)));
  math::vector initialValue(appl.get_model().get_dynamical_variables(), 0.0);
  int horz = appl.get_model().get_dynamical_window(id).get_horizontal_axis_variable();
  int vert = appl.get_model().get_dynamical_window(id).get_vertical_axis_variable();
  initialValue[horz] = initialPoint[0];
  initialValue[vert] = initialPoint[1];
  
  solution_specification spec;
  spec.tMin = -10;
  spec.tMax = 10;
  spec.increment = 0.01;
  spec.initialValue = initialValue;
  solution_specification newSpec;
  if(appl.get_solution_dialog()->show_dialog(spec, &newSpec)) {
    appl.add_solution(newSpec);
  }
}
void dynamical_frame::isocline_menu_on_menu_selection(wxCommandEvent& evt) { }
void dynamical_frame::singular_point_menu_on_menu_selection(wxCommandEvent& evt) {
  window2d window(appl.get_model().get_dynamical_window(id).get_window());
  point2d initialPoint(window.real_coordinate_of(point2d(rightClickMouseX, rightClickMouseY)));
  math::vector initialValue(appl.get_model().get_dynamical_variables(), 0.0);
  int horz = appl.get_model().get_dynamical_window(id).get_horizontal_axis_variable();
  int vert = appl.get_model().get_dynamical_window(id).get_vertical_axis_variable();
  initialValue[horz] = initialPoint[0];
  initialValue[vert] = initialPoint[1];
  
  struct singular_point_specification spec;
  spec.initialValue = initialValue;
  if(appl.get_singular_point_dialog()->show_dialog(spec, &spec)) {
    // Attempt to add the singular point, indicating a failure if error occurs.
  }
}

void dynamical_frame::gl_canvas_on_key_down(wxKeyEvent& evt) { }
void dynamical_frame::gl_canvas_on_key_up(wxKeyEvent& evt) { }
void dynamical_frame::gl_canvas_on_left_down(wxMouseEvent& evt) {
}

void dynamical_frame::gl_canvas_on_left_up(wxMouseEvent& evt) {
}

void dynamical_frame::gl_canvas_on_motion(wxMouseEvent& evt) {
  if(evt.LeftIsDown()) {
    // We are left dragging the mouse.
  }
}
void dynamical_frame::gl_canvas_on_mouse_wheel(wxMouseEvent& evt) { }

void dynamical_frame::gl_canvas_on_right_down(wxMouseEvent& evt) {
  evt.GetPosition(&rightClickMouseX, &rightClickMouseY);
  PopupMenu(popupMenu);
}
void dynamical_frame::gl_canvas_on_right_up(wxMouseEvent& evt) {
}

void dynamical_frame::gl_canvas_on_size(wxSizeEvent& evt) {
  wxSize size(evt.GetSize());
  appl.resize_dynamical_window(id, size.GetWidth(), size.GetHeight());
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
