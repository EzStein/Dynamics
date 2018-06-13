#include "gui/console_frame.h"

#include "gui/app.h"

namespace dynsolver {
namespace gui {

console_frame::console_frame(app& app) : console_frame_base(NULL), appl(app) {
  // Add in widgets and setup events not already done in console_frame_base.
}

void console_frame::compile_button_on_button_click(wxCommandEvent& event) {
  
}

} // namespace gui
} // namespace dynsolver
