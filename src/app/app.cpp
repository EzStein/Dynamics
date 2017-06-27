#include "app.h"
#include "gui/top_frame.h"
#include <algorithm>
#include "math/util.h"
#include <iostream>
#include <thread>
#include <climits>
#include <vector>
#include <wx/rawbmp.h>
#include <mutex>
#include <cmath>
#include "gui/custom_events.h"
#include <cassert>
#include <iomanip>
#include <stack>
#include <chrono>
#include <cstdlib>
#include "concurrency/thread_pool.h"
#include <list>


using std::abs;
using std::max;
using std::min;
using std::vector;
using std::thread;
using std::stack;
using std::mutex;
using std::list;
using std::find;

const unsigned char visitedFlag = 0x80;
const unsigned char validFlag = 0x40;
unsigned long pixelsCalculated = 0;

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

  /*Decline input during computation*/
  state->acceptInputLock->lock();
  state->acceptInput = false;
  state->acceptInputLock->unlock();

  /*Load the current width and height*/
  int width, height;
  frame->renderPanel->GetSize(&width, &height);
  vector_2d<int> size(width, height);

  /*Determine the ideal concurrency*/
  unsigned int hardwareThreads = std::thread::hardware_concurrency();
  if(!hardwareThreads) {
    hardwareThreads = 2;
  }
  int numThreads = 1;

  /*Allocate memory to be shared accross multiple threads. The array holds information for
  what iterations is assigned to each pixel.
  A pixel is accessed by data[y*width + x] for a point (x, y).
  * The data is valid if the valid bit is set. You may access the valid bit at a pixel (x, y) through:
  data[y*width + x] & MSB_MASK.

  You can set the value with:
  data[y*width + x] |= MSB_MASK

  And unset it with:
  data[y*width + x] &= ~MSB_MASK*/
  unsigned long * data = new unsigned long[width * height];

  /*Compute the height interval that each thread is responsible for*/
  int heightInterval = (height/numThreads);

  /*Construct the computation threads with the appropriate regions and run them,
  moving them into the array*/
  state->computationThreadsRunning = numThreads;
  for(int i = 0; i != numThreads; ++i) {
    vector_2d<int> topLeft(0, i*heightInterval);
    vector_2d<int> bottomRight(width, (i + 1)*heightInterval);

    /*The last thread gets whatever is leftover*/
    if(i == numThreads - 1) {
      bottomRight.x = width;
      bottomRight.y = height;
    }

    /*Construct and run a thread and move it into the array*/
    thread([=](){
      //thread_compute_region(topLeft, bottomRight, size, data);
      fast_fill(size, data);
    }).detach();
  }


  /*A thread that waits for the others to finish an then cleans up*/
  thread renderer([=](){
    render_thread(size, data);
  });
  renderer.detach();
}

/*
* Converts the given iterations into color values.
*/
void convert_to_rgb(unsigned long iterations, unsigned char& R, unsigned char& G, unsigned char& B) {
  R = static_cast<unsigned char>(iterations * 34);
  G = static_cast<unsigned char>(iterations * 10);
  B = static_cast<unsigned char>(iterations * 100);
}

/*Continuosly reads the data and updates the view until all threads have been completed.*/
void app::render_thread(const vector_2d<int>& size, unsigned long * data) const {

  /*Allocate memory for an image buffer
  * A pixel may be drawn by accessing imageData[y*size.x*3 + x*3 + j]
  where j is either 0, 1, or 2 indicating red green and blue components respectively*/
  unsigned char * imageData = new unsigned char[size.x * size.y * 3];
  wxImage image(size.x, size.y, imageData, true);

  while(1) {
    for(int y = 0; y < size.y; ++y) {
      for(int x = 0; x < size.x; ++x) {



        unsigned long iterations = data[y*size.x + x];

        /*If its not valid we won't draw it*/
        //if(! (iterations & MSB_MASK)) {
        //  continue;
        //}
        /*We unset the valid bit*/
      //  iterations = iterations &= ~MSB_MASK;

        /*Load the RGB values*/
        unsigned char R, G, B;
        convert_to_rgb(iterations, R, G, B);
        int pixel = y*size.x*3 + x*3;
        imageData[pixel] = R;
        imageData[pixel+1] = G;
        imageData[pixel+2] = B;

        /*if((x - 9/2) % 9 == 0 && (y - 9/2) % 9 == 0) {
          for(int fillX = -9/2; fillX != 9/2 + 1; ++fillX) {
            for(int fillY = -9/2; fillY != 9/2 + 1; ++ fillY) {
              int pixel = (y + fillY)*size.x*3 + (x + fillX)*3;
              imageData[pixel] = R;
              imageData[pixel+1] = G;
              imageData[pixel+2] = B;
            }
          }
        } else if((x - 3/2) % 3 == 0 && (y - 3/2) % 3 == 0) {
          for(int fillX = -3/2; fillX != 3/2 + 1; ++fillX) {
            for(int fillY = -3/2; fillY != 3/2 + 1; ++ fillY) {
              int pixel = (y + fillY)*size.x*3 + (x + fillX)*3;
              imageData[pixel] = R;
              imageData[pixel+1] = G;
              imageData[pixel+2] = B;
            }
          }
        } else {
          int pixel = y*size.x*3 + x*3;
          imageData[pixel] = R;
          imageData[pixel+1] = G;
          imageData[pixel+2] = B;
        }*/
      }
    }

    state->imageLock->lock();
    state->image = wxBitmap(image);
    state->imageLock->unlock();

    wxCommandEvent* evt = new wxCommandEvent(CUSTOM_REFRESH_EVENT);
    frame->renderPanel->GetEventHandler()->QueueEvent(evt);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    state->computationThreadsRunningLock->lock();
    if(!state->computationThreadsRunning) {
      state->computationThreadsRunningLock->unlock();
      break;
    }
    state->computationThreadsRunningLock->unlock();
  }


  /*Update the view one last time*/
  for(int y = 0; y < size.y; ++y) {
    for(int x = 0; x < size.x; ++x) {
      int pixel = y*size.x*3 + x*3;
      unsigned char R, G, B;
      unsigned long iterations = data[y*size.x + x];
      convert_to_rgb(iterations, R, G, B);
      imageData[pixel] = R;
      imageData[pixel+1] = G;
      imageData[pixel+2] = B;
    }
  }
  state->imageLock->lock();
  state->image = wxBitmap(image);
  state->imageLock->unlock();
  wxCommandEvent* evt = new wxCommandEvent(CUSTOM_REFRESH_EVENT);
  frame->renderPanel->GetEventHandler()->QueueEvent(evt);

  delete[] imageData;
  delete[] data;

  /*Begin Accepting input*/
  state->acceptInputLock->lock();
  state->acceptInput = true;
  state->acceptInputLock->unlock();
}

void app::thread_compute_region(const vector_2d<int>& topLeft,
  const vector_2d<int>& bottomRight, const vector_2d<int>& size, unsigned long * data) const {
  calculate_rough_image(topLeft, bottomRight, size, data, 81, INT_MAX);
  calculate_rough_image(topLeft, bottomRight, size, data, 9, 81);
  calculate_rough_image(topLeft, bottomRight, size, data, 3, 9);
  calculate_rough_image(topLeft, bottomRight, size, data, 1, 3);

  int originalCenter = 9/2;
  for(int y = bottomRight.y - originalCenter; y < bottomRight.y; ++y) {
    for(int x = 0; x < bottomRight.x; ++x) {
      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(x, y), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      data[y*size.x + x] = mandelbrot(value, state->iterations);// | MSB_MASK;
    }
  }

  for(int x = bottomRight.x - originalCenter; x < bottomRight.x; ++x) {
    for(int y = 0; y < bottomRight.y; ++y) {
      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(x, y), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      data[y*size.x + x] = mandelbrot(value, state->iterations);// | MSB_MASK;
    }
  }

  state->computationThreadsRunningLock->lock();
  --state->computationThreadsRunning;
  state->computationThreadsRunningLock->unlock();
}

void app::calculate_rough_image(const vector_2d<int>& topLeft, const vector_2d<int>& bottomRight,
  const vector_2d<int>& size, unsigned long * data, int roughness, int previousRoughness) const {
  int center = roughness/2;
  for(int y = topLeft.y + center; y < bottomRight.y - center; y+=roughness) {
    for(int x = topLeft.x + center; x < bottomRight.x - center; x+=roughness) {
      if((x - previousRoughness/2 - topLeft.x) % previousRoughness == 0 && (y - previousRoughness/2 - topLeft.y) % previousRoughness == 0) {
        continue;
      }
      vector_2d<double> value =
        pixel_to_value(vector_2d<int>(x, y), size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
      data[y*size.x + x] = mandelbrot(value, state->iterations);// | MSB_MASK;
    }
  }
}

void app::fast_fill(const vector_2d<int>& size, unsigned long * data) const {




  pixelsCalculated = 0;
  /*Allocate memory for meta info*/
  unsigned char * metaData = new unsigned char[size.x * size.y];

  /*initialize data and metadata*/
  for(int i = 0; i < size.y; ++i) {
    for(int j =0; j < size.x; ++j) {
      //Initialize data and metadata
      data[i*size.x + j] = 0xFFFFFFF;
      //Not valid, not visited
      metaData[i*size.x + j] = 0;
    }
  }

  /*Iterate over each pixel. The only guarentee is that after each loop iteration the appropriate pixel will be calculated although
  It is likely that many more pixels will be calculated on each iteration*/

  vector_2d<int> point;
  for(point.y = 0; point.y < size.y; ++point.y) {
    for(point.x = 0; point.x < size.x; ++point.x) {
      int pixelIndex = point.y*size.x + point.x;
      unsigned long val = get_value(point, size, data, metaData);

      if(!(metaData[pixelIndex] & visitedFlag)) {
        if(is_boundary_point(point, size, data, metaData, val)) {
          calculate_boundary(point, size, data, metaData, val);
        } else {
          fill_interior(point, size, data, metaData, val);
        }
      }
    }
  }


  double percentage = static_cast<double>(pixelsCalculated)/(size.x * size.y) * 100;
  std::cout << percentage << '%' << std::endl;
  delete[] metaData;
  state->computationThreadsRunningLock->lock();
  state->computationThreadsRunning = 0;
  state->computationThreadsRunningLock->unlock();

}

/*Returns the value of data at the given point, calculating it if necessary*/
unsigned long app::get_value(const vector_2d<int>& point, const vector_2d<int>& size,
  unsigned long * data, unsigned char * metaData) const {


  int pixelIndex = point.y*size.x + point.x;

  /*If the data is already valid we return it*/
  if(metaData[pixelIndex] & validFlag)
    return data[pixelIndex];

  /*Otherwise we calculated it and set it as valid*/
  ++pixelsCalculated;
  metaData[pixelIndex] |= validFlag;
  vector_2d<double> value =
    pixel_to_value(point, size, state->boundaryTopLeftValue, state->boundaryBottomRightValue);
  data[pixelIndex] = mandelbrot(value, state->iterations);
  return data[pixelIndex];
}

/*Given an interior initPoint, this function fills in */
void app::fill_interior(const vector_2d<int>& initPoint, const vector_2d<int>& size,
  unsigned long * data, unsigned char * metaData, unsigned long value) const {
  stack<vector_2d<int>> fillStack;
  fillStack.push(initPoint);
  while(!fillStack.empty()) {
    vector_2d<int> point = fillStack.top();
    fillStack.pop();

    /*If the point is out of bounds, we skip it*/
    if(point.x < 0 || point.x >= size.x || point.y < 0 || point.y >= size.y) continue;

    int pixelIndex = point.y*size.x + point.x;
    /*If we already visited this point, or it is a boundary point we skip it*/
    if(metaData[pixelIndex] & visitedFlag) continue;
    metaData[pixelIndex] |= visitedFlag;

    /*We set the point and enable the valid flag*/
    data[pixelIndex] = value;
    metaData[pixelIndex] |= validFlag;

    fillStack.push(vector_2d<int>(point.x + 1, point.y));
    fillStack.push(vector_2d<int>(point.x - 1, point.y));
    fillStack.push(vector_2d<int>(point.x, point.y + 1));
    fillStack.push(vector_2d<int>(point.x, point.y - 1));
  }
}

/*Given a boundary initPoint, this function calculates the rest of the boundary*/
void app::calculate_boundary(const vector_2d<int>& initPoint, const vector_2d<int>& size,
  unsigned long * data, unsigned char * metaData, unsigned long value) const {

  /*Traverse until we get to a boundary point*/

  stack<vector_2d<int> > recursiveStack;
  recursiveStack.push(initPoint);

  while(!recursiveStack.empty()) {

    vector_2d<int> point = recursiveStack.top();
    recursiveStack.pop();

    //Check if visited, skip it
    if(metaData[point.y*size.x + point.x] & visitedFlag) {
      continue;
    }

    //MARK AS VISITED
    metaData[point.y * size.x + point.x] |= visitedFlag;

    for(int i = -1; i != 2; ++i) {
      for(int j = -1; j != 2; ++j) {
        if(i==0 && j == 0) continue;
        vector_2d<int> traversePoint = point + vector_2d<int>(i, j);
        bool traverse = is_boundary_point(traversePoint, size, data, metaData, value);
        if(traverse) {
          recursiveStack.push(traversePoint);
        }
      }
    }
  }
}

/*A point is a boundary point if it is part of the set, and it is adjacent to a pixel that is not part of the set*/
bool app::is_boundary_point(const vector_2d<int>& point, const vector_2d<int>& size,
  unsigned long * data, unsigned char * metaData, unsigned long value) const {
  //If it is not in bounds, then it is not a boundary point.
  if(point.x < 0 || point.x >= size.x || point.y < 0 || point.y >= size.y) {
    return false;
  }
  unsigned long val = get_value(point, size, data, metaData);

  //In the case that the data is not in the prescribed region
  if(val != value)
    return false;

  /*Iterate over the adjacent elements*/
  for(int x = -1 + point.x; x != 2 + point.x; ++x) {
    for(int y = -1 + point.y; y != 2 + point.y; ++y) {
      //If the adjacent point is out of bounds, we consider this point to be a boundary
      if(x < 0 || x >= size.x || y < 0 || y >= size.y) {
        return true;
      }

      val = get_value(vector_2d<int>(x, y), size, data, metaData);

      /*If this adjacent point is not in the prescribed region*/
      if(val != value) {
        return true;
      }
    }
  }
  return false;
}
