#include "render_panel.h"

render_panel::render_panel(wxWindow * parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 500)), image(500, 500) {
  Bind(wxEVT_PAINT, &render_panel::on_paint, this);

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
  dc.DrawBitmap(image,0,0);
}
