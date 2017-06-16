#include "app.h"
#include "../gui/top_frame.h"
#include <algorithm>
#include "../math/util.h"
#include <iostream>
#include <thread>
#include <climits>
#include <vector>
#include <wx/rawbmp.h>
#include <mutex>
#include <cmath>

using std::abs;
using std::max;
using std::min;
using std::vector;
using std::thread;
using std::mutex;

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


void app::render_panel_on_mouse_left_down(wxMouseEvent& event) const {
    state->drawDragBox = true;
    state->boxDragStart = vector_2d<int>(event.GetX(), event.GetY());
    state->boxDragChange.x = 0;
    state->boxDragChange.y = 0;
}

void app::render_panel_on_mouse_motion(wxMouseEvent& event) const {


  if(state->drawDragBox) {
    vector_2d<int> dragChange(event.GetX() - state->boxDragStart.x, event.GetY() - state->boxDragStart.y);

    if(!dragChange.x || !dragChange.y)
      return;

    //This can be optimized if we know the binary representation of integers
    vector_2d<int> dragChangeSign(abs(dragChange.x)/dragChange.x, abs(dragChange.y)/dragChange.y);
    state->boxDragChange.x = dragChangeSign.x * max(dragChangeSign.x*dragChange.x, dragChangeSign.y*dragChange.y);
    state->boxDragChange.y = dragChangeSign.y * dragChangeSign.x * state->boxDragChange.x;
    frame->renderPanel->Refresh();
  }
}

void app::render_panel_on_mouse_left_up(wxMouseEvent& event) const {
  if(!state->acceptInput)
    return;


  state->drawDragBox = false;
  frame->renderPanel->Refresh();

  if(abs(state->boxDragChange.x) > 5) {
    vector_2d<int> boxDragBottomRight;
    vector_2d<int> boxDragTopLeft;

    if(state->boxDragChange.x > 0 && state->boxDragChange.y > 0) {
      boxDragBottomRight.x = state->boxDragStart.x + state->boxDragChange.x;
      boxDragBottomRight.y = state->boxDragStart.y + state->boxDragChange.y;
      boxDragTopLeft = state->boxDragStart;
    } else if(state->boxDragChange.x < 0 && state->boxDragChange.y > 0) {
      boxDragBottomRight.x = state->boxDragStart.x;
      boxDragBottomRight.y = state->boxDragStart.y + state->boxDragChange.y;
      boxDragTopLeft.x = state->boxDragStart.x + state->boxDragChange.x;
      boxDragTopLeft.y = state->boxDragStart.y;
    } else if(state->boxDragChange.x > 0 && state->boxDragChange.y < 0) {
      boxDragBottomRight.x = state->boxDragStart.x + state->boxDragChange.x;
      boxDragBottomRight.y = state->boxDragStart.y;
      boxDragTopLeft.x = state->boxDragStart.x;
      boxDragTopLeft.y = state->boxDragStart.y + state->boxDragChange.y;
    } else if(state->boxDragChange.x < 0 && state->boxDragChange.y < 0) {
      boxDragBottomRight = state->boxDragStart;
      boxDragTopLeft.x = state->boxDragStart.x + state->boxDragChange.x;
      boxDragTopLeft.y = state->boxDragStart.y + state->boxDragChange.y;
    }


    int width, height;
    frame->renderPanel->GetSize(&width, &height);
    vector_2d<int> pixelBoundary(width, height);

    vector_2d<double> tmpTopLeft =
      pixel_to_value(boxDragTopLeft, pixelBoundary, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
    state->boundaryBottomRightValue =
      pixel_to_value(boxDragBottomRight, pixelBoundary, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
    state->boundaryTopLeftValue = tmpTopLeft;
    compute_image();
    frame->renderPanel->Refresh();
  }

}



void app::compute_image() const {
  state->acceptInput = false;
  int width, height;
  frame->renderPanel->GetSize(&width, &height);
  vector_2d<int> size(width, height);
  unsigned int hardwareThreads = std::thread::hardware_concurrency();
  if(!hardwareThreads) {
    hardwareThreads = 2;
  }

  unsigned char * imageData = reinterpret_cast<unsigned char *>(malloc(width * height * 3));
  int numThreads = 1;
  int heightInterval = (height/numThreads);

  for(int i = 0; i != numThreads; ++i) {
    vector_2d<int> topLeft(i*heightInterval, 0);
    vector_2d<int> bottomRight((i + 1)*heightInterval, width);
    thread t([=](){
      thread_compute_region(topLeft, bottomRight, size, imageData);
    });
    t.join();
  }
  state->acceptInput=true;
  state->image = wxImage(size.x, size.y, imageData, false);
  frame->renderPanel->Refresh();
}

void app::thread_compute_region(const vector_2d<int>& topLeft,
  const vector_2d<int>& bottomRight, const vector_2d<int>& size, unsigned char * imageData) const {
  for(int y = topLeft.y; y != bottomRight.y; ++y) {
    for(int x = topLeft.x; x != bottomRight.x; ++x) {
      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(x, y), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      unsigned long val = mandelbrot(value, 100);
      unsigned char * ptr = reinterpret_cast<unsigned char *>(&val);
      imageData[y*size.x*3 + x*3] = ptr[0];
      imageData[y*size.x*3 + x*3 + 1] = ptr[1];
      imageData[y*size.x*3 + x*3 + 2] = ptr[2];
    }
  }
}
