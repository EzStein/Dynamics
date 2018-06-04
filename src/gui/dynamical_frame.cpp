#include "gui/dynamical_frame.h"

namespace dynsolver {
namespace gui {

dynamical_frame::dynamical_frame(app& app, int id) :
    dynamical_frame_base(NULL), appl(app), id(id) {
  // Setup events and widgets not already done in dynamical_frame_base
}

void dynamical_frame::dynamical_frame_on_set_focus(wxFocusEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_key_down(wxKeyEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_key_up(wxKeyEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_left_down(wxMouseEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_left_up(wxMouseEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_motion(wxMouseEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_mouse_wheel(wxMouseEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_right_down(wxMouseEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_right_up(wxMouseEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_paint(wxPaintEvent& evt) { }
void dynamical_frame::dynamical_window_box_on_size(wxSizeEvent& evt) { }

} // namespace dynslover
} // namespace gui
