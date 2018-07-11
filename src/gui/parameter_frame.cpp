#include "gui/parameter_frame.h"

#include <wx/sizer.h>

#include "gui/app.h"

namespace dynsolver {
namespace gui {

const int parameter_frame::kMagnificationTimerEventId = 0;
const int parameter_frame::kMagnificationTimerWait = 500;
const double parameter_frame::kMagnificationRate = 1.01;
const double parameter_frame::kClickTolerance = 2.0;
const double parameter_frame::kMaxAxesScale = 1e5;

parameter_frame::parameter_frame(app& app, parameter_id id,
				 int width, int height)
  : parameter_frame_base(nullptr, wxID_ANY),
    appl(app),
    glCanvas(nullptr),
    id(id),
    rightClickMousePos(0, 0),
    leftClickMousePos(0, 0),
    moveViewport(math::vector2d(0,0),
		 math::vector2d(0,0),
		 math::vector2d(0,0)),
    totalMagnification(1.0),
    magnificationPos(0, 0),
    magnificationViewport(math::vector2d(0,0),
			  math::vector2d(0,0),
			  math::vector2d(0,0)),
    firstWheelEvent(true),
    magnificationTimer(this, kMagnificationTimerEventId),
    verticalScaling(false),
    horizontalScaling(false),
    axesScalingViewport(math::vector2d(0,0),
			math::vector2d(0,0),
			math::vector2d(0,0)) {
  glCanvas = new wxGLCanvas(this, appl.get_gl_attributes(), wxID_ANY,
			    wxDefaultPosition, wxSize(width, height));
  parameterWindowBox->Add(glCanvas, 1, wxEXPAND);
  Fit();
  glCanvas->Bind(wxEVT_PAINT, &parameter_frame::gl_canvas_on_paint, this);
  glCanvas->Bind(wxEVT_KEY_DOWN, &parameter_frame::gl_canvas_on_key_down, this);
  glCanvas->Bind(wxEVT_KEY_UP, &parameter_frame::gl_canvas_on_key_up, this);
  glCanvas->Bind(wxEVT_LEFT_DOWN, &parameter_frame::gl_canvas_on_left_down,
		 this);
  glCanvas->Bind(wxEVT_LEFT_UP, &parameter_frame::gl_canvas_on_left_up, this);
  glCanvas->Bind(wxEVT_MOTION, &parameter_frame::gl_canvas_on_motion, this);
  glCanvas->Bind(wxEVT_MOUSEWHEEL, &parameter_frame::gl_canvas_on_mouse_wheel,
		 this);
  glCanvas->Bind(wxEVT_RIGHT_DOWN, &parameter_frame::gl_canvas_on_right_down,
		 this);
  glCanvas->Bind(wxEVT_RIGHT_UP, &parameter_frame::gl_canvas_on_right_up,
		 this);
  glCanvas->Bind(wxEVT_SIZE, &parameter_frame::gl_canvas_on_size, this);
  Bind(wxEVT_TIMER, &parameter_frame::gl_canvas_on_mouse_wheel_end, this,
       kMagnificationTimerEventId);
}

parameter_frame::~parameter_frame() {
  glCanvas->Unbind(wxEVT_PAINT, &parameter_frame::gl_canvas_on_paint, this);
  glCanvas->Unbind(wxEVT_KEY_DOWN, &parameter_frame::gl_canvas_on_key_down,
		   this);
  glCanvas->Unbind(wxEVT_KEY_UP, &parameter_frame::gl_canvas_on_key_up, this);
  glCanvas->Unbind(wxEVT_LEFT_DOWN, &parameter_frame::gl_canvas_on_left_down,
		   this);
  glCanvas->Unbind(wxEVT_LEFT_UP, &parameter_frame::gl_canvas_on_left_up, this);
  glCanvas->Unbind(wxEVT_MOTION, &parameter_frame::gl_canvas_on_motion, this);
  glCanvas->Unbind(wxEVT_MOUSEWHEEL, &parameter_frame::gl_canvas_on_mouse_wheel,
		   this);
  glCanvas->Unbind(wxEVT_RIGHT_DOWN, &parameter_frame::gl_canvas_on_right_down,
		   this);
  glCanvas->Unbind(wxEVT_RIGHT_UP, &parameter_frame::gl_canvas_on_right_up,
		 this);
  glCanvas->Unbind(wxEVT_SIZE, &parameter_frame::gl_canvas_on_size, this);
}

void parameter_frame::gl_canvas_on_key_down(wxKeyEvent&) {
}
void parameter_frame::gl_canvas_on_key_up(wxKeyEvent&) {
}

void parameter_frame::gl_canvas_on_motion(wxMouseEvent& evt) {
  math::vector2d pos(get_window_coordinates(evt));
  if(evt.LeftIsDown()) {
    // Left Drag
    if(verticalScaling || horizontalScaling) {
      math::vector2d realPos = axesScalingViewport.real_coordinate_of(pos);
      math::vector2d realLeftMousePos = axesScalingViewport.real_coordinate_of(leftClickMousePos);
      double scale;
      if(horizontalScaling) {
	scale = std::abs(realLeftMousePos.x() / realPos.x());
      } else {
	scale = std::abs(realLeftMousePos.y() / realPos.y());
      }
      if(scale > kMaxAxesScale) {
	scale = kMaxAxesScale;
      }
      math::window2d newViewport(axesScalingViewport);
      if(horizontalScaling) {
	newViewport.scale_real(math::vector2d(scale, 1.0), math::vector2d(0,0));
      } else {
	newViewport.scale_real(math::vector2d(1.0, scale), math::vector2d(0,0));
      }
      appl.set_parameter_viewport(id, newViewport);
    } else {
      math::window2d newViewport(moveViewport);
      newViewport.move_pixel(leftClickMousePos - pos);
      appl.set_parameter_viewport(id, newViewport);
    }
  } else if(evt.RightIsDown()) {
    // Right Drag
    
  } else {
    set_cursor(pos);
  }
}

void parameter_frame::gl_canvas_on_left_down(wxMouseEvent& evt) {
  math::vector2d pos(get_window_coordinates(evt));
  leftClickMousePos = pos;
  if(appl.get_model().on_parameter_vert_axis(id, leftClickMousePos)) {
    verticalScaling = true;
    horizontalScaling = false;
  } else if(appl.get_model().on_parameter_horiz_axis(id, leftClickMousePos)) {
    horizontalScaling = true;
    verticalScaling = false;
  } else {
    horizontalScaling = false;
    verticalScaling = false;
  }
  axesScalingViewport = appl.get_model().get_parameter_specs(id).viewport;
  moveViewport = appl.get_model().get_parameter_specs(id).viewport;
}

void parameter_frame::gl_canvas_on_left_up(wxMouseEvent& evt) {
  math::vector2d pos(get_window_coordinates(evt));
  set_cursor(pos);
  if(leftClickMousePos.distance(pos) <= kClickTolerance) {
    gl_canvas_on_left_mouse_click(pos);
  }
}

void parameter_frame::gl_canvas_on_left_mouse_click(const math::vector2d& pos) {
}

void parameter_frame::gl_canvas_on_right_down(wxMouseEvent& evt) {
  math::vector2d pos(get_window_coordinates(evt));
  rightClickMousePos = pos;
}

void parameter_frame::gl_canvas_on_right_up(wxMouseEvent& evt) {
  math::vector2d pos(get_window_coordinates(evt));
  if(rightClickMousePos.distance(pos) <= kClickTolerance) {
    gl_canvas_on_right_mouse_click(pos);
  }
}

void parameter_frame::gl_canvas_on_right_mouse_click(const math::vector2d& pos) {
}

void parameter_frame::gl_canvas_on_paint(wxPaintEvent&) {
  if(glCanvas->IsShownOnScreen()) {
    appl.get_gl_context().SetCurrent(*glCanvas);
    appl.paint_parameter(id);
    glCanvas->SwapBuffers();
  }
}
void parameter_frame::gl_canvas_on_size(wxSizeEvent& evt) {
  wxSize size(evt.GetSize());
  appl.resize_parameter(id, size.GetWidth(), size.GetHeight());
}

void parameter_frame::gl_canvas_on_mouse_wheel(wxMouseEvent& evt) {
  math::vector2d pos(get_window_coordinates(evt));
  if(firstWheelEvent) {
    totalMagnification = 1.0;
    magnificationPos = pos;
    magnificationViewport = appl.get_model().get_parameter_specs(id).viewport;
    firstWheelEvent = false;
  }
  magnificationTimer.Start(kMagnificationTimerWait, wxTIMER_ONE_SHOT);
  totalMagnification *=
    evt.GetWheelRotation() < 0 ? -1 * kMagnificationRate : kMagnificationRate;
  math::window2d newViewport(magnificationViewport);
  newViewport.scale_pixel(math::vector2d(totalMagnification,
						      totalMagnification),
				       magnificationPos);
  appl.set_parameter_viewport(id, newViewport);
}

void parameter_frame::gl_canvas_on_mouse_wheel_end(wxTimerEvent&) {
  firstWheelEvent = true;
}

math::vector2d parameter_frame::get_window_coordinates(const wxMouseEvent& evt) {
  int posX, posY;
  evt.GetPosition(&posX, &posY);
  int width, height;
  glCanvas->GetSize(&width, &height);
  posY = height - posY;
  return math::vector2d(posX, posY);
}

void parameter_frame::set_cursor(const math::vector2d& pos) {
  if(appl.get_model().on_parameter_vert_axis(id, pos)) {
    SetCursor(wxCURSOR_SIZENS);
  } else if(appl.get_model().on_parameter_horiz_axis(id, pos)) {
    SetCursor(wxCURSOR_SIZEWE);
  } else {
    SetCursor(wxCURSOR_DEFAULT);
  }
}

void parameter_frame::refresh_gl_canvas() {
  glCanvas->Refresh();
}
} // namespace gui
} // namespace dynsolver
