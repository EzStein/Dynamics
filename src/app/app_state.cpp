#include "app_state.h"

app_state::~app_state() {

}

app_state::app_state(app* controller) : controller(controller), image(100, 100) {
  
}
