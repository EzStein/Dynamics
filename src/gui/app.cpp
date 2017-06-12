#include "app.h"
#include "top_frame.h"
#include <iostream>

bool app::OnInit() {
  frame = new top_frame;
  frame->Show();
  return true;
}
