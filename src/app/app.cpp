#include "app/app.h"
#include "gui/top_frame.h"
#include <iostream>

bool app::OnInit() {
  top_frame* frame = new top_frame(NULL, wxID_ANY);
  frame->Show();
  return true;
}

int app::OnExit() {
  std::cout << "Terminated!" << std::endl;
  return 0;
}
