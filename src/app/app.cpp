#include "app.h"
#include "../gui/top_frame.h"
#include <algorithm>
#include "../math/util.h"
#include <iostream>

using std::max;
bool app::OnInit() {
  state = new app_state(this);
  frame = new top_frame(state);
  frame->Show();
  compute_image();
  frame->renderPanel->Refresh();
  return true;
}

int app::OnExit() {
  delete state;
  return 0;
}

void app::render_panel_on_mouse_motion(wxMouseEvent& event) const {
  if(state->drawDragBox) {
    state->boxDragSize = max(event.GetX() - state->boxDragTopLeft.x, state->boxDragTopLeft.y - event.GetY());
    frame->renderPanel->Refresh();
  }
}

void app::render_panel_on_mouse_left_up(wxMouseEvent& event) const {
  state->drawDragBox = false;
  frame->renderPanel->Refresh();

  if(state->boxDragSize > 5) {
    vector_2d<int> boxDragBottomRight(state->boxDragTopLeft.x + state->boxDragSize,
      state->boxDragTopLeft.y + state->boxDragSize);

    int width, height;
    frame->renderPanel->GetSize(&width, &height);
    vector_2d<int> pixelBoundary(width, height);
    vector_2d<double> tmpTopLeft =
      pixel_to_value(state->boxDragTopLeft, pixelBoundary, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
    state->boundaryBottomRightValue =
      pixel_to_value(boxDragBottomRight, pixelBoundary, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
    state->boundaryTopLeftValue = tmpTopLeft;
    compute_image();
    frame->renderPanel->Refresh();
  }
}

void app::render_panel_on_mouse_left_down(wxMouseEvent& event) const {
  state->drawDragBox = true;
  state->boxDragTopLeft = vector_2d<int>(event.GetX(), event.GetY());
}

void app::compute_image() const {
  int width, height;

  frame->renderPanel->GetSize(&width, &height);
  vector_2d<int> size(width, height);

  std::cout << width << std::endl;
  std::cout << height << std::endl;
  unsigned char * imageData = reinterpret_cast<unsigned char *>(malloc(width * height * 3));
  for(int i = 0; i != width; ++i) {
    for(int j = 0; j != height; ++j) {
      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(i, j), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      unsigned long val = mandelbrot(value, 1000);
      unsigned char * ptr = reinterpret_cast<unsigned char *>(&val);
      imageData[j*height*3 + i*3] = ptr[0];
      imageData[j*height*3 + i*3 + 1] = ptr[1];
      imageData[j*height*3 + i*3 + 2] = ptr[2];
    }
  }
  wxImage tmpImage(width, height, imageData);
  state->image = tmpImage;
}
