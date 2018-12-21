#ifndef DYNSOLVER_GUI_APP_H_
#define DYNSOLVER_GUI_APP_H_
#include "gui/common.h"

#include <unordered_map>
#include <wx/app.h>

#include <glad/glad.h>

struct GLFWwindow;
class wxDialog;
class wxStdLog;

namespace math {
class window3d;
class window2d;
class vector2d;
}

namespace dynsolver {
namespace gui {
struct dynamical_specs;
struct parameter_specs;
struct saddle_connection_bifurcation_specs;
struct limit_cycle_bifurcation_specs;
struct singular_point_specs;
struct separatrix_specs;
struct solution_specs;
struct isocline_specs;
struct hopf_bifurcation_specs;
struct saddle_node_bifurcation_specs;
class model;
class dynamical_frame;
class parameter_frame;
class console_frame;
class solution_dialog;
class singular_point_dialog;
class dynamical_dialog;
class parameter_dialog;
class isocline_dialog;
class separatrix_dialog;
class hopf_bifurcation_dialog;
class saddle_node_bifurcation_dialog;
class limit_cycle_bifurcation_dialog;
class saddle_connection_bifurcation_dialog;

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
  // OpenGL state for offscreen rendering
  // A dummy window used to create the gl context. This window is invisible.
  GLFWwindow* glWindow;

  // A framebuffer object to which we render.
  GLuint fbo;

  // The color and depth Render buffer objects which are attached to the fbo.
  GLuint colorRbo, depthRbo;

  // The size of the FBO rendering area. All on screen panels to which we display
  // the FBO must have a smaller window.
  GLsizei fboWidth = 1000, fboHeight = 1000;


  // A custom logger for handling error and warning messages.
  // Currently we direct everything to standard error.
  wxLogStderr* customLogger;

  // Represents all the data used in this program.
  model* modelData;

  std::unordered_map<dynamical_id, dynamical_frame*> dynamicalFrames;
  std::unordered_map<parameter_id, parameter_frame*> parameterFrames;

  // The main frame that is always shown.
  console_frame* consoleFrame;

  // Reusable dialogs are made at construction and destroyed when the console
  // frame is closed.
  solution_dialog* solutionDialog;
  singular_point_dialog* singularPointDialog;
  dynamical_dialog* dynamicalDialog;
  parameter_dialog* parameterDialog;
  isocline_dialog* isoclineDialog;
  separatrix_dialog* separatrixDialog;
  hopf_bifurcation_dialog* hopfBifurcationDialog;
  saddle_node_bifurcation_dialog* saddleNodeBifurcationDialog;
  limit_cycle_bifurcation_dialog* limitCycleBifurcationDialog;
  saddle_connection_bifurcation_dialog* saddleConnectionBifurcationDialog;

 public:

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

  void delete_parameter(parameter_id id);

  // Called in order to delete all dynamical windows.
  // Does not assume that the dynamical frames have been destroyed,
  // so it destroys them.
  void delete_all_dynamical_windows();

  void delete_all_parameter_windows();

  // Tells the application to shut down. This is called by the console_frame on
  // close handler.
  void close_application();

  // Constructs a new dynamical window according to the provided specification.
  void add_dynamical(const dynamical_specs&);

  void add_parameter(const parameter_specs&);

  // Changes the specification of the dynamical window.
  void set_dynamical_specs(dynamical_id id, const dynamical_specs&);

  // Changes the specification of the dynamical window.
  void set_dynamical_viewport_2d(dynamical_id id,
				 const ::math::window2d&);

  // Changes the specification of the dynamical window.
  void set_dynamical_viewport_3d(dynamical_id id,
				 const ::math::window3d&);

  void set_parameter_specs(parameter_id, const parameter_specs&);
  void set_parameter_viewport(parameter_id id, const ::math::window2d&);

  // Called when we need to clear what is currently compiled
  void set_no_compile();

  // Read
  // Returns the dialogs.
  solution_dialog* get_solution_dialog() const;
  singular_point_dialog* get_singular_point_dialog() const;
  dynamical_dialog* get_dynamical_dialog() const;
  isocline_dialog* get_isocline_dialog() const;
  parameter_dialog* get_parameter_dialog() const;
  separatrix_dialog* get_separatrix_dialog() const;
  hopf_bifurcation_dialog* get_hopf_bifurcation_dialog() const;
  saddle_node_bifurcation_dialog* get_saddle_node_bifurcation_dialog() const;
  limit_cycle_bifurcation_dialog* get_limit_cycle_bifurcation_dialog() const;
  saddle_connection_bifurcation_dialog* get_saddle_connection_bifurcation_dialog() const;

  // Returns a const model that the GUI's will use to update their
  // displays.
  const model& get_model() const;

  void paint_dynamical(dynamical_id);
  void paint_parameter(parameter_id);
  void resize_dynamical(dynamical_id, double, double);
  void resize_parameter(parameter_id, int, int);

  // Compiles the provided system and updates the UI. Returns true
  // on success.
  bool compile(const std::string& varDiffName,
	       const std::vector<std::string>& dynamicalVarNames,
	       const std::vector<std::string>& parameterNames,
	       const std::vector<std::string>& expressions);

  // Add
  // Adds a solution and updates the necessary frames.
  void add_solution(const solution_specs&);

  // Attempts to add the singular point. Returns false if it fails.
  bool add_singular_point(const singular_point_specs&);

  // Attempts to add the isocline returning true on success.
  bool add_isocline(const isocline_specs&);

  void add_separatrix(const separatrix_specs&);

  void add_all_separatrices();

  bool add_hopf_bifurcation(const hopf_bifurcation_specs&);
  bool add_saddle_node_bifurcation(const saddle_node_bifurcation_specs&);
  bool add_limit_cycle_bifurcation(const limit_cycle_bifurcation_specs&);
  bool add_saddle_connection_bifurcation(const saddle_connection_bifurcation_specs&);

  // Delete
  // Deletes the solution. and updates the necessary frames
  void delete_solution(solution_id);

  void delete_singular_point(solution_id id);

  void delete_isocline(isocline_id id);

  void delete_separatrix(separatrix_id id);

  void delete_hopf_bifurcation(hopf_bifurcation_id id);

  void delete_saddle_node_bifurcation(saddle_node_bifurcation_id id);

  void delete_limit_cycle_bifurcation(limit_cycle_bifurcation_id id);

  void delete_saddle_connection_bifurcation(saddle_connection_bifurcation_id id);

  // Update
  // Changes the existing solution.
  void set_solution_specs(solution_id, const solution_specs&);

  // Select
  // Attempts to selects the object near the mouse cursor specified
  // by the coordnates x, y and the dynamical window given by id.
  // It attempts to select objects in this order:
  // singular_point, solution, isocline.
  void select_dynamical_object(dynamical_id id, int x, int y);
  void select_parameter_object(parameter_id id, int x, int y);


  // Programatically selects an object.
  void select_solution(solution_id id);
  void select_singular_point(singular_point_id id);
  void select_isocline(isocline_id id);
  void select_separatrix(separatrix_id id);
  void select_hopf_bifurcation(hopf_bifurcation_id id);
  void select_saddle_node_bifurcation(saddle_node_bifurcation_id id);
  void select_limit_cycle_bifurcation(limit_cycle_bifurcation_id id);
  void select_saddle_connection_bifurcation(saddle_connection_bifurcation_id id);

  // Deselects the object programatically
  void deselect_solution();
  void deselect_singular_point();
  void deselect_isocline();
  void deselect_separatrix();
  void deselect_hopf_bifurcation();
  void deselect_saddle_node_bifurcation();
  void deselect_limit_cycle_bifurcation();
  void deselect_saddle_connection_bifurcation();

  // Sets the parameter position according to the position provided in window
  // coordinates. Updates all parameter and dynamical windows accordingly.
  void set_parameter_position(parameter_id, const ::math::vector2d&);

  // Returns the subset of the FBO with the appropriate width and height
  // as a bitmap.
  wxBitmap get_fbo_bitmap(int width, int height);

private:
  void refresh_dynamical_windows();
  void refresh_parameter_windows();

  // Sets up offscreen rendering to the FBO. After this call,
  // function pointers are initialized and all opengl calls affect the
  // FBO
  void initialize_opengl();
};

} // namespace gui
} // namespace dynsolver
#endif
