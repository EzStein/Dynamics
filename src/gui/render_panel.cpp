#include <iostream>
#include "gui/render_panel.h"
#include "app/app_traits.h"
#include "gui/custom_events.h"


render_panel::render_panel(wxWindow * parent, const app_state * state) :
wxPanel(parent, wxID_ANY, wxDefaultPosition,
  wxSize(APP_TRAITS_RENDER_PANEL_INIT_SIZE, APP_TRAITS_RENDER_PANEL_INIT_SIZE)), state(state) {

  Bind(wxEVT_PAINT, &render_panel::on_paint, this);
  Bind(wxEVT_MOTION, &render_panel::on_mouse_motion, this);
  Bind(wxEVT_LEFT_DOWN, &render_panel::on_mouse_left_down, this);
  Bind(wxEVT_LEFT_UP, &render_panel::on_mouse_left_up, this);
  Bind(CUSTOM_REFRESH_EVENT, &render_panel::on_custom_refresh_event, this);
}

/*Schedules a refresh of the render panel. This method is scheduled by several threads
by adding it to the event queue.*/
void render_panel::on_custom_refresh_event(wxCommandEvent& event) {
  Refresh();
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

  //Lock and draw the image
  state->imageLock->lock();
  dc.DrawBitmap(state->image,0,0);
  state->imageLock->unlock();

  if(state->drawDragBox) {
    dc.SetPen(wxPen(wxColor(255,125,100),1));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(state->boxDragStart.x, state->boxDragStart.y,
      state->boxDragChange.x, state->boxDragChange.y);

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
