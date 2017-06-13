#include "app.h"
#include "../gui/top_frame.h"
#include <iostream>

bool app::OnInit() {
  state = new app_state;
  frame = new top_frame(state);
  frame->Show();
  return true;
}

int app::OnExit() {
  delete state;
  return 0;
}
