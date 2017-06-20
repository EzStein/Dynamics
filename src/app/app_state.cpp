#include "app_state.h"
#include "app_traits.h"
#include <mutex>

using std::mutex;
using std::lock_guard;

app_state::~app_state() {

}

app_state::app_state(app* controller) : controller(controller),
    image(APP_TRAITS_RENDER_PANEL_INIT_SIZE, APP_TRAITS_RENDER_PANEL_INIT_SIZE), acceptInput(false),
    boundaryTopLeftValue(-2.0, 2.0), boundaryBottomRightValue(2.0, -2.0),
    drawDragBox(false), interruptComputationThreads(false), iterations(5000) {
      imageLock = &imageLockVal;
      interruptComputationThreadsLock = &interruptComputationThreadsLockVal;
      acceptInputLock = &acceptInputLockVal;
      computationThreadsRunningLock = &computationThreadsRunningLockVal;
}
