#include "gui/dynamical_frame.h"

#include "gui/generated.h"
#include <wx/glcanvas.h>
#include <wx/msgdlg.h>

#include "gui/solution_dialog.h"
#include "gui/singular_point_dialog.h"
#include "gui/dynamical_dialog.h"
#include "gui/isocline_dialog.h"
#include "math/vector3d.h"

namespace dynsolver {
namespace gui {

const int dynamical_frame::kMagnificationTimerEventId = 0;

dynamical_frame::dynamical_frame(app& app, dynamical_id id, int width, int height) :
  dynamical_frame_base(nullptr, wxID_ANY, "Dynamical Space"),
  appl(app), id(id),
  magnificationViewport(math::vector2d(0,0),
			math::vector2d(0,0),
			math::vector2d(0,0)),
  moveViewport(math::vector2d(0,0),
	       math::vector2d(0,0),
	       math::vector2d(0,0)),
  firstWheelEvent(true),
  magnificationTimer(this, kMagnificationTimerEventId),
  verticalScaling(false),
  horizontalScaling(false),
  axesScalingViewport(math::vector2d(0,0),
		      math::vector2d(0,0),
		      math::vector2d(0,0)) {
  // Setup events and widgets not already done in dynamical_frame_base
  glCanvas = new wxGLCanvas(this, appl.get_gl_attributes(), wxID_ANY,
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
  appl.delete_dynamical(id);
}

void dynamical_frame::edit_menu_item_on_menu_selection(wxCommandEvent& evt) {
  dynamical_specs
    spec(appl.get_model().get_dynamical_specs(id));
  if(appl.get_dynamical_dialog()->show_dialog(spec)) {
    appl.set_dynamical_specs(id, spec);
  }
}

void dynamical_frame::dynamical_frame_on_set_focus(wxFocusEvent& evt) { }

void dynamical_frame::solution_menu_item_on_menu_selection(wxCommandEvent& evt) {
  solution_specs spec;
  spec.tMin = -10;
  spec.tMax = 10;
  spec.inc = 0.001;
  dynamical_point point = appl.get_model().get_dynamical_point(id, math::vector2d(rightClickMouseX,
									    rightClickMouseY));
  spec.init = point.vars;
  spec.tStart = point.t;
  if(appl.get_solution_dialog()->show_dialog(spec)) {
    appl.add_solution(spec);
  }
}
void dynamical_frame::gl_canvas_on_key_up(wxKeyEvent& evt) { }
void dynamical_frame::gl_canvas_on_key_down(wxKeyEvent& evt) {
  if(appl.get_model().get_dynamical_specs(id).is3d) {
    math::window3d
      viewport(appl.get_model().get_dynamical_specs(id).viewport3d);
    math::vector3d position(viewport.get_position());
    math::vector3d viewDirection(viewport.get_view_direction());
    math::vector3d upDirection(viewport.get_up_direction());
    math::vector3d rightDirection(math::vector3d::cross(viewDirection, upDirection));
    double speed = 1.0;
    if(evt.GetKeyCode() == 'W' || evt.GetKeyCode() == WXK_UP) {
      position += speed * viewDirection;
    } else if(evt.GetKeyCode() == 'S' || evt.GetKeyCode() == WXK_DOWN) {
      position -= speed * viewDirection;
    } else if(evt.GetKeyCode() == 'A' || evt.GetKeyCode() == WXK_LEFT) {
      position -= speed * rightDirection;
    } else if(evt.GetKeyCode() == 'D' || evt.GetKeyCode() == WXK_RIGHT) {
      position += speed * rightDirection;
    } else if(evt.GetKeyCode() == 'E') {
      position += speed * upDirection;
    } else if(evt.GetKeyCode() == 'C') {
      position -= speed * upDirection;
    }
    viewport.set_position(position);
    appl.set_dynamical_viewport_3d(id, viewport);
  } else {
    math::window2d viewport(appl.get_model()
			    .get_dynamical_specs(id)
			    .viewport2d);
    double speed = 10.0;
    double changeX = 0.0;
    double changeY = 0.0;
    if(evt.GetKeyCode() == 'W' || evt.GetKeyCode() == WXK_UP) {
      changeY = speed;
    } else if(evt.GetKeyCode() == 'S' || evt.GetKeyCode() == WXK_DOWN) {
      changeY = -speed;
    } else if(evt.GetKeyCode() == 'A' || evt.GetKeyCode() == WXK_LEFT) {
      changeX = -speed;
    } else if(evt.GetKeyCode() == 'D' || evt.GetKeyCode() == WXK_RIGHT) {
      changeX = speed;
    }
    viewport.move_pixel(math::vector2d(changeX, changeY));
    appl.set_dynamical_viewport_2d(id, viewport);
  }
}
void dynamical_frame::singular_point_menu_item_on_menu_selection(wxCommandEvent& evt) {
  struct singular_point_specs spec;
  dynamical_point point = appl.get_model().get_dynamical_point(id, math::vector2d(rightClickMouseX,
									    rightClickMouseY));
  spec.init = point.vars;
  if(appl.get_singular_point_dialog()->show_dialog(spec)) {
    if(!appl.add_singular_point(spec)) {
      wxMessageDialog messageDialog(nullptr, "Could not find singular point.",
				    "Singular Point", wxOK);
      messageDialog.ShowModal();
    }
  }
}

void dynamical_frame::gl_canvas_on_left_down(wxMouseEvent& evt) {
  evt.GetPosition(&leftClickMouseX, &leftClickMouseY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  leftClickMouseY = height - leftClickMouseY;
  if(appl.get_model().get_dynamical_specs(id).is3d) {
    rotationViewport = appl.get_model().get_dynamical_specs(id).viewport3d;
  } else {
    if(appl.get_model().on_dynamical_vert_axis(id, math::vector2d(leftClickMouseX, leftClickMouseY))) {
      verticalScaling = true;
    } else if(appl.get_model().on_dynamical_horiz_axis(id, math::vector2d(leftClickMouseX, leftClickMouseY))) {
      horizontalScaling = true;
    }
    axesScalingViewport = appl.get_model().get_dynamical_specs(id).viewport2d;
    moveViewport = appl.get_model().get_dynamical_specs(id).viewport2d;
  }
}

void dynamical_frame::set_cursor(int x, int y) {
  if(appl.get_model().on_dynamical_vert_axis(id, math::vector2d(x, y))) {
    SetCursor(wxCURSOR_SIZENS);
  } else if(appl.get_model().on_dynamical_horiz_axis(id, math::vector2d(x, y))) {
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
    appl.select_dynamical_object(id, posX, posY);
  }

  leftClickMouseX = posX;
  leftClickMouseY = posY;
  
  if(!appl.get_model().get_dynamical_specs(id).is3d) {
    moveViewport = appl.get_model().get_dynamical_specs(id).viewport2d;
    verticalScaling = false;
    horizontalScaling = false;
    set_cursor(leftClickMouseX, leftClickMouseY);
  }
}

void dynamical_frame::isocline_menu_item_on_menu_selection(wxCommandEvent&) {
  isocline_specs specs;
  specs.inc = 0.1;
  specs.span = 200;
  specs.searchRadius = 10;
  specs.searchInc = 0.1;
  specs.direction =
    math::vector(appl.get_model().get_dynamical_dimension(), 0.0);
  dynamical_point point = appl.get_model().get_dynamical_point(id, math::vector2d(rightClickMouseX,
									    rightClickMouseY));
  specs.init = point.vars;
  if(appl.get_isocline_dialog()->show_dialog(specs)) {
    if(!appl.add_isocline(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find isocline.",
				    "Isocline", wxOK);
      messageDialog.ShowModal();
    }
  }
  std::cout << "Computed Isocline" << std::endl;
}

void dynamical_frame::gl_canvas_on_motion(wxMouseEvent& evt) {
  int posX, posY;
  evt.GetPosition(&posX, &posY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  posY = height - posY;

  if(appl.get_model().get_dynamical_specs(id).is3d) {
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
  if(appl.get_model().get_dynamical_specs(id).is3d) {
  } else {
    totalMagnification = 1.0;
    magnificationX = posX;
    magnificationY = posY;
    magnificationViewport = appl.get_model().get_dynamical_specs(id).viewport2d;
  }
}

void dynamical_frame::gl_canvas_on_mouse_wheel_end(wxTimerEvent&) {
  if(appl.get_model().get_dynamical_specs(id).is3d) {
  } else {
    firstWheelEvent = true;
  }
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
  magnificationTimer.Start(500, wxTIMER_ONE_SHOT);
  if(appl.get_model().get_dynamical_specs(id).is3d) {
  } else {
    double magnificationUnit = std::pow(1.0002, evt.GetWheelRotation());
    totalMagnification *= magnificationUnit;
    math::window2d tmp(magnificationViewport);
    tmp.scale_pixel(math::vector2d(totalMagnification, totalMagnification),
		    math::vector2d(magnificationX, magnificationY));
    appl.set_dynamical_viewport_2d(id, tmp);
  }
}

void dynamical_frame::gl_canvas_on_right_down(wxMouseEvent& evt) {
  evt.GetPosition(&rightClickMouseX, &rightClickMouseY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  rightClickMouseY = height - rightClickMouseY;
  if(appl.get_model().get_dynamical_specs(id).is3d) {
  } else {
    PopupMenu(objectsPopupMenu);
  }
}
void dynamical_frame::gl_canvas_on_right_up(wxMouseEvent& evt) {
}

void dynamical_frame::gl_canvas_on_size(wxSizeEvent& evt) {
  wxSize size(evt.GetSize());
  appl.resize_dynamical(id, size.GetWidth(), size.GetHeight());
}

void dynamical_frame::refresh_gl_canvas() {
  glCanvas->Refresh();
}

void dynamical_frame::gl_canvas_on_paint(wxPaintEvent& evt) {
  if(glCanvas->IsShownOnScreen()) {
    appl.get_gl_context().SetCurrent(*glCanvas);
    appl.paint_dynamical(id);
    glCanvas->SwapBuffers();
  }
}

void dynamical_frame::dynamical_frame_on_iconize(wxIconizeEvent& evt) {
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
