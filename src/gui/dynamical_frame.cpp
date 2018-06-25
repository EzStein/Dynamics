#include "gui/dynamical_frame.h"

#include "gui/generated.h"
#include <wx/glcanvas.h>

#include "gui/solution_dialog.h"
#include "gui/singular_point_dialog.h"
#include "gui/dynamical_dialog.h"

namespace dynsolver {
namespace gui {

const int dynamical_frame::kMagnificationTimerEventId = 0;

dynamical_frame::dynamical_frame(app& app, int id, int width, int height) :
  dynamical_frame_base(nullptr, wxID_ANY, "Dynamical Space"),
  appl(app), id(id),
  magnificationTimer(this, kMagnificationTimerEventId),
  magnificationViewport(point2d(0,0), point2d(0,0), point2d(0,0)),
  axesScalingViewport(point2d(0,0), point2d(0,0), point2d(0,0)),
  firstWheelEvent(true),
  horizontalScaling(false),
  verticalScaling(false) {
  // Setup events and widgets not already done in dynamical_frame_base
  glCanvas = new wxGLCanvas(this, appl.getGlAttributes(), wxID_ANY,
			    wxDefaultPosition, wxSize(width, height));
  dynamicalWindowBox->Add(glCanvas, 1, wxEXPAND);
  Fit();
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
  Bind(wxEVT_TIMER, &dynamical_frame::gl_canvas_on_mouse_wheel_end, this,
       kMagnificationTimerEventId);
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
  spec.verticalAxisMin = window.get_position().y();

  spec.horizontalAxisMax = window.get_position().x() + window.get_span().x();
  spec.verticalAxisMax = window.get_position().y() + window.get_span().y();

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
  evt.GetPosition(&leftClickMouseX, &leftClickMouseY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  leftClickMouseY = height - leftClickMouseY;
  
  realDragPosition = point2d(appl.get_model().get_dynamical_window(id)
			     .get_window().get_position());
  
  if(appl.get_model().on_vertical_axis(leftClickMouseX, leftClickMouseY, id)) {
    verticalScaling = true;
  } else if(appl.get_model().on_horizontal_axis(leftClickMouseX, leftClickMouseY, id)) {
    horizontalScaling = true;
  }
  axesScalingViewport = appl.get_model().get_dynamical_window(id).get_window();
}

void dynamical_frame::gl_canvas_on_left_up(wxMouseEvent& evt) {
  evt.GetPosition(&leftClickMouseX, &leftClickMouseY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  leftClickMouseY = height - leftClickMouseY;
  
  realDragPosition = point2d(appl.get_model().get_dynamical_window(id)
			     .get_window().get_position());
  verticalScaling = false;
  horizontalScaling = false;
  SetCursor(wxCURSOR_DEFAULT);
}

void dynamical_frame::gl_canvas_on_motion(wxMouseEvent& evt) {
  int posX, posY;
  evt.GetPosition(&posX, &posY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  posY = height - posY;
  
  if(evt.LeftIsDown()) {
    // Yes dragging
    if(verticalScaling) {
      SetCursor(wxCURSOR_SIZENS);
      double realLeftClickMouseY =
	axesScalingViewport.real_coordinate_of(point2d(0, leftClickMouseY)).y();
      double realPosY =
	axesScalingViewport.real_coordinate_of(point2d(0, posY)).y();

      double scale = std::abs(realLeftClickMouseY / realPosY);
      double maxScale = 100;
      if(std::abs(scale) > maxScale) {
	scale = maxScale;
      }
      window2d tmp(axesScalingViewport);
      tmp.scale_real(point2d(1.0, scale), point2d(0,0));
      appl.set_dynamical_window(tmp, id);
    } else if(horizontalScaling) {
      SetCursor(wxCURSOR_SIZEWE);
      double realLeftClickMouseX =
	axesScalingViewport.real_coordinate_of(point2d(leftClickMouseX,0)).x();
      double realPosX =
	axesScalingViewport.real_coordinate_of(point2d(posX,0)).x();

      double scale = std::abs(realLeftClickMouseX / realPosX);
      double maxScale = 100;
      if(std::abs(scale) > maxScale) {
	scale = maxScale;
      }
      window2d tmp(axesScalingViewport);
      tmp.scale_real(point2d(scale, 1.0), point2d(0,0));
      appl.set_dynamical_window(tmp, id);
    } else {
      window2d window(appl.get_model().get_dynamical_window(id).get_window());
      double changeX = leftClickMouseX - posX;
      double changeY = leftClickMouseY - posY;
      point2d newPosition(window.get_span().x()*changeX/window.get_size().x()
			  + realDragPosition.x(),
			  window.get_span().y()*changeY/window.get_size().y()
			  + realDragPosition.y());
      window2d newViewport(window.get_size(), window.get_span(), newPosition);
      appl.set_dynamical_window(newViewport, id);
    }
  } else {
    // Not dragging
    if(appl.get_model().on_vertical_axis(posX, posY, id)) {
      SetCursor(wxCURSOR_SIZENS);
    } else if(appl.get_model().on_horizontal_axis(posX, posY, id)) {
      SetCursor(wxCURSOR_SIZEWE);
    } else {
      SetCursor(wxCURSOR_DEFAULT);
    }
  }
}

void dynamical_frame::gl_canvas_on_mouse_wheel_start(int posX, int posY) {
  totalMagnification = 1.0;
  magnificationX = posX;
  magnificationY = posY;
  magnificationViewport = appl.get_model().get_dynamical_window(id).get_window();
}

void dynamical_frame::gl_canvas_on_mouse_wheel_end(wxTimerEvent&) {
  firstWheelEvent = true;
}

void dynamical_frame::gl_canvas_on_mouse_wheel(wxMouseEvent& evt) {
  int posX, posY;
  evt.GetPosition(&posX, &posY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  posY = height - posY;
  if(firstWheelEvent) {
    gl_canvas_on_mouse_wheel_start(posX, posY);
    firstWheelEvent = false;
  }
  double magnificationUnit = std::pow(1.0002, evt.GetWheelRotation());
  totalMagnification *= magnificationUnit;
  window2d tmp(magnificationViewport);
  tmp.scale_pixel(point2d(totalMagnification, totalMagnification),
		  point2d(magnificationX, magnificationY));
  appl.set_dynamical_window(tmp, id);
  magnificationTimer.Start(500, wxTIMER_ONE_SHOT);
}

void dynamical_frame::gl_canvas_on_right_down(wxMouseEvent& evt) {
  evt.GetPosition(&rightClickMouseX, &rightClickMouseY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  rightClickMouseY = height - rightClickMouseY;
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

void dynamical_frame::dynamical_frame_on_iconize(wxIconizeEvent& evt) {
  std::cout << "ICONIZE" << std::endl;
  Layout();
}
} // namespace dynslover
} // namespace gui
