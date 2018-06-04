#include "gui/console_frame.h"

namespace dynsolver {
namespace gui {

console_frame::console_frame(app& app) : console_frame_base(NULL), appl(app) {
  // Add in widgets and setup events not already done in console_frame_base.
}

} // namespace gui
} // namespace dynsolver
