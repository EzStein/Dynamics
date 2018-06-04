#ifndef DYNSOLVER_GUI_APP_H_
#define DYNSOLVER_GUI_APP_H_

#include <vector>
#include <unordered_map>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include "gui/model.h"
#include "gui/parameter_frame.h"
#include "gui/dynamical_frame.h"
#include "gui/console_frame.h"

#define PROJECT_PATH "/home/ezra/Documents/projects/hubbard_research"

namespace dynsolver {
namespace gui {

// WxWidgets will call the OnInit method of this class on program startup.
// This class handles communication between all the windows and dialogs.
// In general, all events are handled by this class. Some events are caught
// by the consoleFrame or by one of the parameterFrames or dynamicalFrames,
// however, they will be passed along to this class using a reference to *this.
// Sometimes extra imformation, such as which parameter/dynamical frame
// generated the event will also be necessary. Usually, the class which generated
// the event will handle input validation. If the event passes validation,
// it will be forwarded to this class which will handle calling the model
// and subsequently updating the user interface. THUS, the class which generates
// the event is not responsible for updating after the event.
class app : public wxApp {
 private:
  // Represents all the data used in this program.
  model modelData;

  // A mapping from the id's of the dynamical/parameter windows to their
  // gui frames.
  std::unordered_map<int, dynamical_frame*> dynamicalFrames;
  std::unordered_map<int, parameter_frame*> parameterFrames;

  // The main frame that is always shown.
  console_frame* consoleFrame;
  
 public:
  virtual bool OnInit() override;
  
  // Called when the last frame has been closed.
  virtual int OnExit() override;

  // Returns a const model that the GUI's will use to update their
  // displays.
  const model& get_model();
};
} // namespace gui
} // namespace dynsolver

#endif
