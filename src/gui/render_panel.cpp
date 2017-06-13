#include "render_panel.h"

render_panel::render_panel(wxWindow * parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 500)), image(500, 500) {
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
  dc.SetBrush(*wxBLACK_BRUSH);
  if(draw_rectangle) {
    dc.DrawRectangle(x1, y1, x2 - x1, y2 - y1);
  }
  //dc.DrawBitmap(image,0,0);
}

void render_panel::on_mouse_motion(wxMouseEvent& event) {
  x2 = event.GetX();
  y2 = event.GetY();
  if(event.Dragging()) {
    this->Refresh();
  }
}
void render_panel::on_mouse_left_down(wxMouseEvent& event) {
  draw_rectangle = true;
  x1 = event.GetX();
  y1 = event.GetY();
}
void render_panel::on_mouse_left_up(wxMouseEvent& event) {
  draw_rectangle = false;
}
