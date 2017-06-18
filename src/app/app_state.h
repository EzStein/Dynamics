#ifndef APP_STATE
#define APP_STATE
#include <mutex>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif


#include "../math/vector_2d.h"
#include "app.h"


class app;

/*This class holds all the data for the application. It may be considered as the model in the model view controller design*/
/*Each variable in this class will be marked by which threads will access it. The wxWidgets event thread will
* be called the event dispatch thread (EDT). Threads for computing the image to be drawn will be called computation threads (CTs)
* Access to these variables is restricted by constness. All classes that have read but not write access to this state
* will receive a const version of it. While other classes receive a nonconst version
* This class is merely a container, it is the responsibility of the other classes to ensure proper synchronization on the variables
*/
class app_state {
public:
  /*Constructs the state class with a pointer to the constructing app*/
  app_state(app*);
  ~app_state();

  /*The state class cannot be copied or moved*/
  app_state(const app_state&) = delete;
  app_state& operator=(const app_state&) = delete;
  app_state(const app_state&&) = delete;
  app_state& operator=(const app_state&&) = delete;

  const wxBitmap& get_image() const;
  bool accepting_input() const;


  //ACCESS: EDT
  vector_2d<double> boundaryTopLeftValue;
  vector_2d<double> boundaryBottomRightValue;
  vector_2d<int> boxDragStart;
  vector_2d<int> boxDragChange;
  bool drawDragBox;

  /*A reference to the controller. Mostly used to forward events*/
  app * const controller;

  //Made private so that we can handle synchronization
  //ACCESS: CTs
  bool acceptInput;
  std::mutex* acceptInputLock;

  std::mutex* imageLock;
  wxBitmap image;

  bool interruptComputationThreads;
  std::mutex* interruptComputationThreadsLock;

  std::mutex* computationThreadsRunningLock;
  int computationThreadsRunning;

  unsigned long iterations;

  private:
    std::mutex imageLockVal;
    std::mutex acceptInputLockVal;
    std::mutex interruptComputationThreadsLockVal;
    std::mutex computationThreadsRunningLockVal;
};

#endif
