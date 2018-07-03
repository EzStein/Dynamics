#ifndef DYNSOLVER_GUI_APP_H_
#define DYNSOLVER_GUI_APP_H_

#include <unordered_map>

// Included first since this has implications on how opengl functions.
#include <glad/glad.h>
#include <wx/glcanvas.h>


#include "gui/common.h"
#include "gui/model.h"

#include "math/window2d.h"

class wxDialog;

namespace dynsolver {
namespace gui {

class dynamical_frame;
class console_frame;
class solution_dialog;
class singular_point_dialog;
class dynamical_dialog;
class isocline_dialog;
struct solution_specs;

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
  wxLogStderr* customLogger;
  
  // Represents all the data used in this program.
  model* modelData;

  // glContextAttributes is used for initializing any opengl contexts in this
  // program. glAttributes is used for initializing any glCanvas in this program.
  wxGLContextAttrs glContextAttributes;
  wxGLAttributes glAttributes;
  
  // The opengl context used across all windows for rendering.
  wxGLContext* glContext;
  
  std::unordered_map<dynamical_id, dynamical_frame*> dynamicalFrames;

  // The main frame that is always shown.
  console_frame* consoleFrame;

  // Reusable dialogs are made at construction and destroyed when the console
  // frame is closed.
  solution_dialog* solutionDialog;
  singular_point_dialog* singularPointDialog;
  dynamical_dialog* dynamicalDialog;
  isocline_dialog* isoclineDialog;
  
 public:
  app();
  
  ~app();
  
  virtual bool OnInit() override;
  
  // Called when the last frame has been closed.
  virtual int OnExit() override;

  virtual bool OnExceptionInMainLoop() override;
  
  virtual void OnUnhandledException() override;
  
  virtual bool StoreCurrentException() override;

  // Closes the dynamical window, freeing up all of its info in the model.
  // This is called in the on close event of the dynamical_frame, so there
  // is no need to destroy the frame manually.
  void delete_dynamical(dynamical_id id);

  // Called in order to delete all dynamical windows.
  // Does not assume that the dynamical frames have been destroyed,
  // so it destroys them.
  void delete_all_dynamical_windows();

  // Tells the application to shut down. This is called by the console_frame on
  // close handler.
  void close_application();

  // Constructs a new dynamical window according to the provided specification.
  void new_dynamical(const dynamical_specs&);

  // Changes the specification of the dynamical window.
  void set_dynamical_specs(dynamical_id id,
					  const dynamical_specs&);

  // Changes the specification of the dynamical window.
  void set_dynamical_viewport_2d(dynamical_id id,
				 const math::window2d&);
  
  // Changes the specification of the dynamical window.
  void set_dynamical_viewport_3d(dynamical_id id,
				 const math::window3d&);

  // Called when we need to clear what is currently compiled
  void set_no_compile();

  // Returns a const model that the GUI's will use to update their
  // displays.
  const model& get_model();

  // Returns a reference to the opengl context used in this program.
  const wxGLContext& get_gl_context();

  // Returns a reference to the attributes used for constructing all opengl
  // canvas's in this program.
  const wxGLAttributes& get_gl_attributes();

  void paint_dynamical(dynamical_id);
  void resize_dynamical(dynamical_id, double, double);

  void add_solution(const solution_specs&);

  // Attempts to add the singular point. Returns false if it fails.
  bool add_singular_point(const singular_point_specs&);

  // Deletes the singular point.
  void delete_singular_point(solution_id id);

  void set_solution_color(solution_id id, const color&);
  void set_singular_point_color(singular_point_id id, const color&);

  // Changes the existing solution.
  void set_solution_specs(solution_id, const solution_specs&);

  // Deletes the existing solution.
  void delete_solution(solution_id);

  // Compiles the provided system and updates the UI. Returns true
  // on success.
  bool compile(const std::vector<std::string>);

  // Returns the dialogs.
  solution_dialog* get_solution_dialog();
  singular_point_dialog* get_singular_point_dialog();
  dynamical_dialog* get_dynamical_dialog();
  isocline_dialog* get_isocline_dialog();

  // Attempts to selects the solution under the mouse cursor specified
  // by the coordnates x, y and the dynamical window given by id.
  // Returns true on success.
  bool select_solution(dynamical_id id, int x, int y);

  // Attempts to add the isocline returning true on success.
  bool add_isocline(const isocline_specs&);

private:
  // Sends a refresh request to the glCanvas's of all dynamical windows.
  void refresh_dynamical_windows();
};

} // namespace gui
} // namespace dynsolver
#endif
