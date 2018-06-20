#ifndef DYNSOLVER_GUI_APP_H_
#define DYNSOLVER_GUI_APP_H_

#include <unordered_map>

// Included first since this has implications on how opengl functions.
#include <glad/glad.h>
#include <wx/glcanvas.h>


#include "gui/common.h"
#include "gui/model.h"

class wxDialog;

namespace dynsolver {
namespace gui {

class dynamical_frame;
class parameter_frame;
class console_frame;
class solution_dialog;
struct solution_specification;

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
  // A custom logger for handling error and warning messages.
  // Currently we direct everything to standard error.
  wxLogStderr customLogger;
  
  // Represents all the data used in this program.
  model* modelData;

  // glContextAttributes is used for initializing any opengl contexts in this
  // program. glAttributes is used for initializing any glCanvas in this program.
  wxGLContextAttrs glContextAttributes;
  wxGLAttributes glAttributes;
  
  // The opengl context used across all windows for rendering.
  wxGLContext* glContext;
  
  std::unordered_map<dynamical_window_id, dynamical_frame*> dynamicalFrames;
  std::unordered_map<parameter_window_id, parameter_frame*> parameterFrames;

  // The main frame that is always shown.
  console_frame* consoleFrame;

  solution_dialog* solutionDialog;
  
 public:
  app();
  
  ~app();
  
  virtual bool OnInit() override;
  
  // Called when the last frame has been closed.
  virtual int OnExit() override;

  // Returns a const model that the GUI's will use to update their
  // displays.
  const model& get_model();

  // Returns a reference to the opengl context used in this program.
  const wxGLContext& get_gl_context();

  // Returns a reference to the attributes used for constructing all opengl
  // canvas's in this program.
  const wxGLAttributes& get_gl_attributes();

  void paint_dynamical_window(dynamical_window_id);
  void resize_dynamical_window(dynamical_window_id, double, double);

  wxGLContextAttrs getGlContextAttributes();
  wxGLAttributes getGlAttributes();

  void add_solution(const solution_specification&);

  // Compiles the provided system and updates the UI. Returns true
  // on success.
  bool compile(const std::vector<std::string>);

  solution_dialog* get_solution_dialog();

private:
  // Sends a refresh request to the glCanvas's of all dynamical windows.
  void refresh_dynamical_windows();
};

} // namespace gui
} // namespace dynsolver
#endif
