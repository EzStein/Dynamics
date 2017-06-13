#include "app.h"
#include "../gui/top_frame.h"
#include <iostream>

bool app::OnInit() {
  state = new app_state(this);
  frame = new top_frame(state);
  frame->Show();
  return true;
}

int app::OnExit() {
  delete state;
  return 0;
}

void app::render_panel_on_mouse_motion(wxMouseEvent& event) {
  if(state->drawDragBox) {
    state->boxDragBottomRight = vector_2d<int>(event.GetX(), event.GetY());
    frame->renderPanel->Refresh();
  }
}

void app::render_panel_on_mouse_left_up(wxMouseEvent& event) {
  state->drawDragBox = false;
  frame->renderPanel->Refresh();
}

void app::render_panel_on_mouse_left_down(wxMouseEvent& event){
  state->drawDragBox = true;
  state->boxDragTopLeft = vector_2d<int>(event.GetX(), event.GetY());
}
