#include "app_state.h"

app_state * app_state::get_pointer() {
  static app_state state;
  return &state;
}

app_state::~app_state() {

}

app_state::app_state() {

}
