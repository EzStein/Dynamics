#include "gui/parameter_frame.h"

#include <wx/sizer.h>

#include "gui/app.h"

#include <wx/msgdlg.h>
#ifndef USE_GLFW
#include <wx/glcanvas.h>
#endif
#include "gui/hopf_bifurcation_dialog.h"
#include "gui/saddle_node_bifurcation_dialog.h"
#include "gui/limit_cycle_bifurcation_dialog.h"
#include "gui/saddle_connection_bifurcation_dialog.h"

namespace dynsolver {
namespace gui {

const int parameter_frame::kMagnificationTimerEventId = 0;
const int parameter_frame::kMagnificationTimerWait = 700;
const double parameter_frame::kMagnificationRate = 1.01;
const double parameter_frame::kClickTolerance = 2.0;
const double parameter_frame::kMaxAxesScale = 1e5;

parameter_frame::parameter_frame(app& app, parameter_id id,
				 int width, int height)
  : parameter_frame_base(nullptr, wxID_ANY),
    appl(app),
    id(id),
    rightClickMousePos(0, 0),
    leftClickMousePos(0, 0),
    moveViewport(::math::vector2d(0,0),
		 ::math::vector2d(0,0),
		 ::math::vector2d(0,0)),
    totalMagnification(1.0),
    magnificationPos(0, 0),
    magnificationViewport(::math::vector2d(0,0),
			  ::math::vector2d(0,0),
			  ::math::vector2d(0,0)),
    firstWheelEvent(true),
    magnificationTimer(this, kMagnificationTimerEventId),
    verticalScaling(false),
    horizontalScaling(false),
    axesScalingViewport(::math::vector2d(0,0),
			::math::vector2d(0,0),
			::math::vector2d(0,0)) {
#ifdef USE_GLFW
	canvas = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(width, height));
#else
	canvas = new wxGLCanvas(this, appl.get_gl_attributes(), wxID_ANY,
		wxDefaultPosition, wxSize(width, height));
#endif
	parameterWindowBox->Add(canvas, 1, wxEXPAND);
	Fit();
	canvas->Bind(wxEVT_PAINT, &parameter_frame::canvas_on_paint, this);
	canvas->Bind(wxEVT_KEY_DOWN, &parameter_frame::canvas_on_key_down, this);
	canvas->Bind(wxEVT_KEY_UP, &parameter_frame::canvas_on_key_up, this);
	canvas->Bind(wxEVT_LEFT_DOWN, &parameter_frame::canvas_on_left_down,
		 this);
	canvas->Bind(wxEVT_LEFT_UP, &parameter_frame::canvas_on_left_up, this);
	canvas->Bind(wxEVT_MOTION, &parameter_frame::canvas_on_motion, this);
	canvas->Bind(wxEVT_MOUSEWHEEL, &parameter_frame::canvas_on_mouse_wheel,
		 this);
	canvas->Bind(wxEVT_RIGHT_DOWN, &parameter_frame::canvas_on_right_down,
		 this);
	canvas->Bind(wxEVT_RIGHT_UP, &parameter_frame::canvas_on_right_up,
		 this);
	canvas->Bind(wxEVT_SIZE, &parameter_frame::canvas_on_size, this);
  Bind(wxEVT_TIMER, &parameter_frame::canvas_on_mouse_wheel_end, this,
       kMagnificationTimerEventId);
}

parameter_frame::~parameter_frame() {
	canvas->Unbind(wxEVT_PAINT, &parameter_frame::canvas_on_paint, this);
	canvas->Unbind(wxEVT_KEY_DOWN, &parameter_frame::canvas_on_key_down, this);
	canvas->Unbind(wxEVT_KEY_UP, &parameter_frame::canvas_on_key_up, this);
	canvas->Unbind(wxEVT_LEFT_DOWN, &parameter_frame::canvas_on_left_down,
		 this);
	canvas->Unbind(wxEVT_LEFT_UP, &parameter_frame::canvas_on_left_up, this);
	canvas->Unbind(wxEVT_MOTION, &parameter_frame::canvas_on_motion, this);
	canvas->Unbind(wxEVT_MOUSEWHEEL, &parameter_frame::canvas_on_mouse_wheel,
		 this);
	canvas->Unbind(wxEVT_RIGHT_DOWN, &parameter_frame::canvas_on_right_down,
		 this);
	canvas->Unbind(wxEVT_RIGHT_UP, &parameter_frame::canvas_on_right_up,
		 this);
	canvas->Unbind(wxEVT_SIZE, &parameter_frame::canvas_on_size, this);
	Unbind(wxEVT_TIMER, &parameter_frame::canvas_on_mouse_wheel_end, this,
       kMagnificationTimerEventId);
}

void parameter_frame::canvas_on_key_down(wxKeyEvent&) {
}
void parameter_frame::canvas_on_key_up(wxKeyEvent&) {
}

void parameter_frame::canvas_on_motion(wxMouseEvent& evt) {
  ::math::vector2d pos(get_window_coordinates(evt));
  if(evt.LeftIsDown()) {
    // Left Drag
    if(parameterMoving) {
      appl.set_parameter_position(id, pos);
    } else if(verticalScaling || horizontalScaling) {
      ::math::vector2d realPos = axesScalingViewport.real_coordinate_of(pos);
      ::math::vector2d realLeftMousePos = axesScalingViewport.real_coordinate_of(leftClickMousePos);
      double scale;
      if(horizontalScaling) {
	scale = std::abs(realLeftMousePos.x() / realPos.x());
      } else {
	scale = std::abs(realLeftMousePos.y() / realPos.y());
      }
      if(scale > kMaxAxesScale) {
	scale = kMaxAxesScale;
      }
      ::math::window2d newViewport(axesScalingViewport);
      if(horizontalScaling) {
	newViewport.scale_real(::math::vector2d(scale, 1.0), ::math::vector2d(0,0));
      } else {
	newViewport.scale_real(::math::vector2d(1.0, scale), ::math::vector2d(0,0));
      }
      appl.set_parameter_viewport(id, newViewport);
    } else {
      ::math::window2d newViewport(moveViewport);
      newViewport.move_pixel(leftClickMousePos - pos);
      appl.set_parameter_viewport(id, newViewport);
    }
  } else if(evt.RightIsDown()) {
    // Right Drag

  } else {
    set_cursor(pos);
  }
}

void parameter_frame::canvas_on_left_down(wxMouseEvent& evt) {
  ::math::vector2d pos(get_window_coordinates(evt));
  leftClickMousePos = pos;
  parameterMoving = false;
  verticalScaling = false;
  horizontalScaling = false;
  if(appl.get_model().on_parameter_position(id, leftClickMousePos)) {
    parameterMoving = true;
  } else if(appl.get_model().on_parameter_vert_axis(id, leftClickMousePos)) {
    verticalScaling = true;
  } else if(appl.get_model().on_parameter_horiz_axis(id, leftClickMousePos)) {
    horizontalScaling = true;
  }
  axesScalingViewport = appl.get_model().get_parameter_specs(id).viewport;
  moveViewport = appl.get_model().get_parameter_specs(id).viewport;
}

void parameter_frame::canvas_on_left_up(wxMouseEvent& evt) {
  ::math::vector2d pos(get_window_coordinates(evt));
  set_cursor(pos);
  if(leftClickMousePos.distance(pos) <= kClickTolerance) {
    canvas_on_left_mouse_click(pos);
  }
}

void parameter_frame::canvas_on_left_mouse_click(const ::math::vector2d& pos) {
}

void parameter_frame::canvas_on_right_down(wxMouseEvent& evt) {
  ::math::vector2d pos(get_window_coordinates(evt));
  rightClickMousePos = pos;
}

void parameter_frame::canvas_on_right_up(wxMouseEvent& evt) {
  ::math::vector2d pos(get_window_coordinates(evt));
  if(rightClickMousePos.distance(pos) <= kClickTolerance) {
    canvas_on_right_mouse_click(pos);
  }
}

void parameter_frame::canvas_on_right_mouse_click(const ::math::vector2d& pos) {
  PopupMenu(parameterPopupMenu);
}

void parameter_frame::canvas_on_paint(wxPaintEvent&) {
#ifdef USE_GLFW
	wxPaintDC dc(canvas);
  appl.paint_parameter(id);
	int width, height;
	canvas->GetSize(&width, &height);
	dc.DrawBitmap(appl.get_fbo_bitmap(width, height), 0, 0);
#else
	if(canvas->IsShownOnScreen()) {
		appl.get_gl_context().SetCurrent(*canvas);
		appl.paint_parameter(id);
		canvas->SwapBuffers();
	}
#endif
}
void parameter_frame::canvas_on_size(wxSizeEvent& evt) {
  wxSize size(evt.GetSize());
  appl.resize_parameter(id, size.GetWidth(), size.GetHeight());
}

void parameter_frame::canvas_on_mouse_wheel(wxMouseEvent& evt) {
  ::math::vector2d pos(get_window_coordinates(evt));
  if(firstWheelEvent) {
    totalMagnification = 1.0;
    magnificationPos = pos;
    magnificationViewport = appl.get_model().get_parameter_specs(id).viewport;
    firstWheelEvent = false;
  }
  magnificationTimer.Start(kMagnificationTimerWait, wxTIMER_ONE_SHOT);
  totalMagnification *=
    evt.GetWheelRotation() < 0 ? 1 / kMagnificationRate : kMagnificationRate;
  ::math::window2d newViewport(magnificationViewport);
  newViewport.scale_pixel(::math::vector2d(totalMagnification,
					 totalMagnification),
			  magnificationPos);
  appl.set_parameter_viewport(id, newViewport);
}

void parameter_frame::canvas_on_mouse_wheel_end(wxTimerEvent&) {
  firstWheelEvent = true;
}

::math::vector2d parameter_frame::get_window_coordinates(const wxMouseEvent& evt) {
  int posX, posY;
  evt.GetPosition(&posX, &posY);
  int width, height;
  canvas->GetSize(&width, &height);
  posY = height - posY;
  return ::math::vector2d(posX, posY);
}

void parameter_frame::set_cursor(const ::math::vector2d& pos) {
  if(appl.get_model().on_parameter_vert_axis(id, pos)) {
    SetCursor(wxCURSOR_SIZENS);
  } else if(appl.get_model().on_parameter_horiz_axis(id, pos)) {
    SetCursor(wxCURSOR_SIZEWE);
  } else {
    SetCursor(wxCURSOR_DEFAULT);
  }
}

void parameter_frame::refresh_canvas() {
  canvas->Refresh();
}

void parameter_frame::parameter_frame_on_close(wxCloseEvent&) {
  appl.delete_parameter(id);
}

void parameter_frame::hopf_bifurcation_menu_item_on_selection(wxCommandEvent&) {
  hopf_bifurcation_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.init.dynamicalVars = ::math::vector(2, 0.0);
  specs.init.parameters =
    appl.get_model().get_parameter_point(id, rightClickMousePos);
  if(appl.get_hopf_bifurcation_dialog()->show_dialog(specs)) {
    if(!appl.add_hopf_bifurcation(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find hopf bifurcation.",
				    "Hopf Bifurcation", wxOK);
      messageDialog.ShowModal();
    }
  }
}

void parameter_frame::saddle_node_bifurcation_menu_item_on_selection(wxCommandEvent&) {
  saddle_node_bifurcation_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.init.dynamicalVars = ::math::vector(2, 0.0);
  specs.init.parameters =
    appl.get_model().get_parameter_point(id, rightClickMousePos);
  if(appl.get_saddle_node_bifurcation_dialog()->show_dialog(specs)) {
    if(!appl.add_saddle_node_bifurcation(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find saddle-node bifurcation.",
				    "Hopf Bifurcation", wxOK);
      messageDialog.ShowModal();
    }
  }
}
void parameter_frame::limit_cycle_bifurcation_menu_item_on_selection(wxCommandEvent&) {
}
void parameter_frame::saddle_connection_bifurcation_menu_item_on_selection(wxCommandEvent&) {
  saddle_connection_bifurcation_specs specs;
  specs.inc = 0.1;
  specs.span = 100;
  specs.searchRadius = 10;
  specs.searchInc = 1;
  specs.transversalA = ::math::vector2d(0.0, 0.0);
  specs.transversalB = ::math::vector2d(0.0, 0.0);
  specs.separatrix1 = model::kNoSeparatrixId;
  specs.separatrix2 = model::kNoSeparatrixId;
  specs.init = appl.get_model().get_parameter_point(id, rightClickMousePos);
  if(appl.get_saddle_connection_bifurcation_dialog()->show_dialog(specs)) {
    if(!appl.add_saddle_connection_bifurcation(specs)) {
      wxMessageDialog messageDialog(nullptr, "Could not find saddle connection.",
				    "Saddle Connection Bifurcation", wxOK);
      messageDialog.ShowModal();
    }
  }
}
} // namespace gui
} // namespace dynsolver
