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
  magnificationViewport(math::vector2d(0,0), math::vector2d(0,0), math::vector2d(0,0)),
  axesScalingViewport(math::vector2d(0,0), math::vector2d(0,0), math::vector2d(0,0)),
  moveViewport(math::vector2d(0,0), math::vector2d(0,0), math::vector2d(0,0)),
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
  dynamical_window_specification
    spec(appl.get_model().get_dynamical_window_specification(id));
  if(appl.get_dynamical_window_dialog()->show_dialog(spec, &spec)) {
    appl.set_dynamical_window_specification(id, spec);
  }
}

void dynamical_frame::dynamical_frame_on_set_focus(wxFocusEvent& evt) { }

void dynamical_frame::solution_menu_on_menu_selection(wxCommandEvent& evt) {
  math::window2d window(appl.get_model().get_dynamical_window_specification(id)
			.viewport2d);
  math::vector2d initialPoint
    (window.real_coordinate_of(math::vector2d(rightClickMouseX, rightClickMouseY)));
  math::vector initialValue(appl.get_model().get_dynamical_variables(), 0.0);
  int horz = appl.get_model().get_dynamical_window_specification(id)
    .horizontalAxisVariable;
  int vert = appl.get_model().get_dynamical_window_specification(id)
    .verticalAxisVariable;
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
void dynamical_frame::gl_canvas_on_key_up(wxKeyEvent& evt) { }
void dynamical_frame::gl_canvas_on_key_down(wxKeyEvent& evt) {
  if(appl.get_model().get_dynamical_window_specification(id).is3d) {
    math::window3d
      viewport(appl.get_model().get_dynamical_window_specification(id).viewport3d);
    math::vector3d position(viewport.get_position());
    if(evt.GetUnicodeKey() == WXK_CONTROL_W + 64) {
      position.z() += 0.1;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_S + 64) {
      position.z() -= 0.1;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_A + 64) {
      position.x() -= 0.1;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_D + 64) {
      position.x() += 0.1;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_E + 64) {
      position.y() -= 0.1;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_C + 64) {
      position.y() += 0.1;
    }
    viewport.set_position(position);
    appl.set_dynamical_viewport_3d(id, viewport);
    std::cout << position.x() << std::endl;
  } else {
    
  }
}
void dynamical_frame::isocline_menu_on_menu_selection(wxCommandEvent& evt) { }
void dynamical_frame::singular_point_menu_on_menu_selection(wxCommandEvent& evt) {
  math::window2d window(appl.get_model().get_dynamical_window_specification(id)
			.viewport2d);
  math::vector2d initialPoint(window.real_coordinate_of(math::vector2d(rightClickMouseX, rightClickMouseY)));
  math::vector initialValue(appl.get_model().get_dynamical_variables(), 0.0);
  int horz = appl.get_model().get_dynamical_window_specification(id)
    .horizontalAxisVariable;
  int vert = appl.get_model().get_dynamical_window_specification(id)
    .verticalAxisVariable;
  initialValue[horz] = initialPoint[0];
  initialValue[vert] = initialPoint[1];
  
  struct singular_point_specification spec;
  spec.initialValue = initialValue;
  if(appl.get_singular_point_dialog()->show_dialog(spec, &spec)) {
    // Attempt to add the singular point, indicating a failure if error occurs.
  }
}

void dynamical_frame::gl_canvas_on_left_down(wxMouseEvent& evt) {
  evt.GetPosition(&leftClickMouseX, &leftClickMouseY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  leftClickMouseY = height - leftClickMouseY;
  
  if(appl.get_model().on_dynamical_vertical_axis(id, leftClickMouseX, leftClickMouseY)) {
    verticalScaling = true;
  } else if(appl.get_model().on_dynamical_horizontal_axis(id, leftClickMouseX, leftClickMouseY)) {
    horizontalScaling = true;
  }
  axesScalingViewport = appl.get_model().get_dynamical_window_specification(id).viewport2d;
  moveViewport = appl.get_model().get_dynamical_window_specification(id).viewport2d;
  rotationViewport = appl.get_model().get_dynamical_window_specification(id).viewport3d;
}

void dynamical_frame::set_cursor(int x, int y) {
  if(appl.get_model().on_dynamical_vertical_axis(id, x, y)) {
    SetCursor(wxCURSOR_SIZENS);
  } else if(appl.get_model().on_dynamical_horizontal_axis(id, x, y)) {
    SetCursor(wxCURSOR_SIZEWE);
  } else {
    SetCursor(wxCURSOR_DEFAULT);
  }
}

void dynamical_frame::gl_canvas_on_left_up(wxMouseEvent& evt) {
  int posX, posY;
  evt.GetPosition(&posX, &posY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  posY = height - posY;

  const int tolerance = 2;
  if(std::abs(leftClickMouseX - posX) <= tolerance
     && std::abs(leftClickMouseY - posY) <= tolerance) {
    // A mouse click occured. We now attempt to find and select
    // an object.
    appl.select_solution(posX, posY, id);
  }

  leftClickMouseX = posX;
  leftClickMouseY = posY;
  moveViewport = appl.get_model().get_dynamical_window_specification(id).viewport2d;
  verticalScaling = false;
  horizontalScaling = false;

  set_cursor(leftClickMouseX, leftClickMouseY);
}

void dynamical_frame::gl_canvas_on_motion(wxMouseEvent& evt) {
  int posX, posY;
  evt.GetPosition(&posX, &posY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  posY = height - posY;

  if(appl.get_model().get_dynamical_window_specification(id).is3d) {
    if(evt.LeftIsDown()) {
      math::window3d tmp(rotationViewport);
      tmp.rotate(math::vector2d(leftClickMouseX, leftClickMouseY),
			 math::vector2d(posX, posY));
      appl.set_dynamical_viewport_3d(id, tmp);
    }
  } else {
    if(verticalScaling && evt.LeftIsDown()) {
      SetCursor(wxCURSOR_SIZENS);
      double realLeftClickMouseY =
	axesScalingViewport.real_coordinate_of(math::vector2d(0, leftClickMouseY)).y();
      double realPosY =
	axesScalingViewport.real_coordinate_of(math::vector2d(0, posY)).y();
      double scale = std::abs(realLeftClickMouseY / realPosY);
      double maxScale = 100;
      if(std::abs(scale) > maxScale) {
	scale = maxScale;
      }
      math::window2d tmp(axesScalingViewport);
      tmp.scale_real(math::vector2d(1.0, scale), math::vector2d(0,0));
      appl.set_dynamical_viewport_2d(id, tmp);
    } else if(horizontalScaling && evt.LeftIsDown()) {
      SetCursor(wxCURSOR_SIZEWE);
      double realLeftClickMouseX =
	axesScalingViewport.real_coordinate_of(math::vector2d(leftClickMouseX,0)).x();
      double realPosX =
	axesScalingViewport.real_coordinate_of(math::vector2d(posX,0)).x();

      double scale = std::abs(realLeftClickMouseX / realPosX);
      double maxScale = 100;
      if(std::abs(scale) > maxScale) {
	scale = maxScale;
      }
      math::window2d tmp(axesScalingViewport);
      tmp.scale_real(math::vector2d(scale, 1.0), math::vector2d(0,0));
      appl.set_dynamical_viewport_2d(id, tmp);
    } else if(evt.LeftIsDown()) {
      math::window2d newViewport(moveViewport);
      double changeX = leftClickMouseX - posX;
      double changeY = leftClickMouseY - posY;
      newViewport.move_pixel(math::vector2d(changeX, changeY));
      appl.set_dynamical_viewport_2d(id, newViewport);
    } else {
      set_cursor(posX, posY);
    }
  }
}

void dynamical_frame::gl_canvas_on_mouse_wheel_start(int posX, int posY) {
  totalMagnification = 1.0;
  magnificationX = posX;
  magnificationY = posY;
  magnificationViewport = appl.get_model().get_dynamical_window_specification(id).viewport2d;
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
  math::window2d tmp(magnificationViewport);
  tmp.scale_pixel(math::vector2d(totalMagnification, totalMagnification),
		  math::vector2d(magnificationX, magnificationY));
  appl.set_dynamical_viewport_2d(id, tmp);
  magnificationTimer.Start(500, wxTIMER_ONE_SHOT);
}

void dynamical_frame::gl_canvas_on_right_down(wxMouseEvent& evt) {
  evt.GetPosition(&rightClickMouseX, &rightClickMouseY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  rightClickMouseY = height - rightClickMouseY;
  PopupMenu(objectsPopupMenu);
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

void dynamical_frame::selection_delete_menu_item_on_menu_selection(wxCommandEvent& evt) {
}
void dynamical_frame::selection_edit_menu_item_on_menu_selection(wxCommandEvent& evt) {
}
void dynamical_frame::selection_select_menu_item_on_menu_selection(wxCommandEvent& evt) {
}
} // namespace dynslover
} // namespace gui
