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
#include "../gui/custom_events.h"
#include <cassert>
#include <iomanip>

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
  state->acceptInputLock->lock();
  if(!state->acceptInput) {
    state->acceptInputLock->unlock();
    return;
  }
  state->acceptInputLock->unlock();




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
  state->acceptInputLock->lock();
  state->acceptInput = false;
  state->acceptInputLock->unlock();


  int width, height;
  frame->renderPanel->GetSize(&width, &height);
  vector_2d<int> size(width, height);
  unsigned int hardwareThreads = std::thread::hardware_concurrency();
  if(!hardwareThreads) {
    hardwareThreads = 2;
  }
  unsigned char * imageData = reinterpret_cast<unsigned char *>(malloc(width * height * 3));
  int numThreads = 8;
  int heightInterval = (height/numThreads);

  thread * threads = new thread[numThreads];

  for(int i = 0; i != numThreads; ++i) {
    vector_2d<int> topLeft(0, i*heightInterval);
    vector_2d<int> bottomRight(width, (i + 1)*heightInterval);
    if(i == numThreads - 1) {
      bottomRight.x = width;
      bottomRight.y = height;
    }
    threads[i] = thread([=](){
      thread_compute_region(topLeft, bottomRight, size, imageData);
    });
    //threads[i].join();
  }

  /*A thread that waits for the others to finish an then cleans up*/
  thread wait([=]()->void {
    thread* end = threads + numThreads;
    for(thread* t = threads; t != end; ++t) {
      t->join();
    }
    free(imageData);
    delete[] threads;

    state->acceptInputLock->lock();
    state->acceptInput = true;
    state->acceptInputLock->unlock();
    wxCommandEvent* evt = new wxCommandEvent(CUSTOM_REFRESH_EVENT);
    frame->renderPanel->GetEventHandler()->QueueEvent(evt);
  });
  wait.detach();
}

void app::thread_compute_region(const vector_2d<int>& topLeft,
  const vector_2d<int>& bottomRight, const vector_2d<int>& size, unsigned char * imageData) const {
  calculate_rough_image(topLeft, bottomRight, size, imageData, 81, INT_MAX);
  calculate_rough_image(topLeft, bottomRight, size, imageData, 9, 81);
  calculate_rough_image(topLeft, bottomRight, size, imageData, 3, 9);
  calculate_rough_image(topLeft, bottomRight, size, imageData, 1, 3);

  int originalCenter = 9/2;
  for(int y = bottomRight.y - originalCenter; y < bottomRight.y; ++y) {
    for(int x = 0; x < bottomRight.x; ++x) {
      //assert((x - previousRoughness/2) % previousRoughness != 0 || (y - previousRoughness/2) % previousRoughness != 0);

      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(x, y), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      unsigned long val = mandelbrot(value, state->iterations);
      int rgb = 0;
      if(val != 0) {
        rgb = (val % 100)*10000000;
      }
      unsigned char * ptr = reinterpret_cast<unsigned char *>(&rgb);

      imageData[y*size.x*3 + x*3] = ptr[0];
      imageData[y*size.x*3 + x*3 + 1] = ptr[1];
      imageData[y*size.x*3 + x*3 + 2] = ptr[2];
    }
  }

  for(int x = bottomRight.x - originalCenter; x < bottomRight.x; ++x) {
    for(int y = 0; y < bottomRight.y; ++y) {
      //assert((x - previousRoughness/2) % previousRoughness != 0 || (y - previousRoughness/2) % previousRoughness != 0);

      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(x, y), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      unsigned long val = mandelbrot(value, state->iterations);
      int rgb = 0;
      if(val != 0) {
        rgb = (val % 100)*10000000;
      }
      unsigned char * ptr = reinterpret_cast<unsigned char *>(&rgb);

      imageData[y*size.x*3 + x*3] = ptr[0];
      imageData[y*size.x*3 + x*3 + 1] = ptr[1];
      imageData[y*size.x*3 + x*3 + 2] = ptr[2];
    }
  }

  wxBitmap im(wxImage(size.x, size.y, imageData, true));
  state->imageLock->lock();
  state->image = im;
  state->imageLock->unlock();
  wxCommandEvent* evt = new wxCommandEvent(CUSTOM_REFRESH_EVENT);
  frame->renderPanel->GetEventHandler()->QueueEvent(evt);
}

void app::calculate_rough_image(const vector_2d<int>& topLeft, const vector_2d<int>& bottomRight,
  const vector_2d<int>& size, unsigned char * imageData, int roughness, int previousRoughness) const {
  int center = roughness/2;
  for(int y = topLeft.y + center; y < bottomRight.y - center; y+=roughness) {
    for(int x = topLeft.x + center; x < bottomRight.x - center; x+=roughness) {
      if((x - previousRoughness/2 - topLeft.x) % previousRoughness == 0 && (y - previousRoughness/2 - topLeft.y) % previousRoughness == 0) {
        continue;
      }
      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(x, y), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      unsigned long val = mandelbrot(value, state->iterations);
      int rgb = 0;
      if(val != 0) {
        rgb = (val % 100)*10000000;
      }
      unsigned char * ptr = reinterpret_cast<unsigned char *>(&rgb);

      int xEnd = x + center + 1;
      int yEnd = y + center + 1;
      if(y + roughness >= bottomRight.y - center) {
        yEnd = bottomRight.y;
      }
      if(x + roughness >= bottomRight.x - center) {
        xEnd = bottomRight.x;
      }


      for(int xToPaint = x-center; xToPaint != xEnd; ++xToPaint) {
        for(int yToPaint = y-center; yToPaint != yEnd; ++yToPaint) {
          imageData[yToPaint*size.x*3 + xToPaint*3] = ptr[0];
          imageData[yToPaint*size.x*3 + xToPaint*3 + 1] = ptr[1];
          imageData[yToPaint*size.x*3 + xToPaint*3 + 2] = ptr[2];

        }
      }
    }

    wxBitmap im(wxImage(size.x, size.y, imageData, true));
    state->imageLock->lock();
    state->image = im;
    state->imageLock->unlock();
    wxCommandEvent* evt = new wxCommandEvent(CUSTOM_REFRESH_EVENT);
    frame->renderPanel->GetEventHandler()->QueueEvent(evt);
  }
}
