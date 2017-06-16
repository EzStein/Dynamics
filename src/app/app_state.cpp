#include "app_state.h"

app_state::~app_state() {

}

app_state::app_state(app* controller) : controller(controller), image(500, 500), acceptInput(false),
    boundaryTopLeftValue(-2.0, 2.0), boundaryBottomRightValue(2.0, -2.0),
    drawDragBox(false), interruptComputationThreads(false) {

}
