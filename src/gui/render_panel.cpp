#include "render_panel.h"

render_panel::render_panel(wxWindow * parent, app_state * state) :
wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 500)), state(state) {
  Bind(wxEVT_PAINT, &render_panel::on_paint, this);
  Bind(wxEVT_MOTION, &render_panel::on_mouse_motion, this);
  Bind(wxEVT_LEFT_DOWN, &render_panel::on_mouse_left_down, this);
  Bind(wxEVT_LEFT_UP, &render_panel::on_mouse_left_up, this);
}

/*Called automatically when the panel should be repainted*/
void render_panel::on_paint(wxPaintEvent& event) {
  wxPaintDC dc(this);
  render(dc);
}

void render_panel::paint_now() {
  wxClientDC dc(this);
  render(dc);
}

void render_panel::render(wxDC& dc) {
  dc.SetBrush(*wxWHITE_BRUSH);
  int width, height;
  GetSize(&width, &height);
  dc.DrawRectangle(0,0,width, height);
  dc.DrawBitmap(state->get_image(),0,0);
  if(state->get_draw_drag_box()) {
    dc.SetPen(wxPen(wxColor(255,125,100),1));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(state->get_box_drag_start().x, state->get_box_drag_start().y,
      state->get_box_drag_change().x, state->get_box_drag_change().y);
  }
}

void render_panel::on_mouse_motion(wxMouseEvent& event) {
  state->controller->render_panel_on_mouse_motion(event);
}
void render_panel::on_mouse_left_down(wxMouseEvent& event) {
  state->controller->render_panel_on_mouse_left_down(event);
}
void render_panel::on_mouse_left_up(wxMouseEvent& event) {
  state->controller->render_panel_on_mouse_left_up(event);
}
