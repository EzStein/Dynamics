#include "app.h"
#include "../gui/top_frame.h"
#include <algorithm>
#include "../math/util.h"
#include <iostream>
#include <thread>
#include <climits>
#include <vector>
#include <wx/rawbmp.h>

using std::max;
using std::min;
using std::vector;
using std::thread;

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
  if(!state->acceptInput)
    return;


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
    state->boxDragSize = 0;
}


void myFunc() {

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

  int numThreads = hardwareThreads;


  vector<thread> threads;
  threads.reserve(numThreads);
  int heightInterval = (height/numThreads);
  for(int i = 0; i != numThreads; ++i) {

    vector_2d<int> topLeft(i*heightInterval, width);
    vector_2d<int> bottomRight((i + 1)*heightInterval, width);
    threads.push_back(thread(myFunc));
  }
  vector<thread>::iterator end = threads.end();
  for(vector<thread>::iterator i = threads.begin(); i != end; ++i) {
    i->join();
  }
  state->acceptInput = true;
  frame->renderPanel->Refresh();
}

void app::thread_compute_region(const vector_2d<int>& topLeft, const vector_2d<int>& bottomRight, const vector_2d<int>& size) {
  wxNativePixelData im(state->image);
  wxNativePixelData::Iterator imageData(im.GetPixels());
  for(int i = topLeft.x; i != bottomRight.x; ++i) {
    for(int j = topLeft.y; j != bottomRight.y; ++j) {
      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(i, j), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      unsigned long val = mandelbrot(value, 1000);
      unsigned char * ptr = reinterpret_cast<unsigned char *>(&val);
      imageData.MoveTo(im, i, j);
      imageData.Red() = ptr[0];
      imageData.Green() = ptr[1];
      imageData.Blue() = ptr[2];
    }
    //frame->renderPanel->Refresh();
  }
}
