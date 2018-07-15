
#include "gui/app.h"

#include <cassert>
#include <stdexcept>

#include <wx/msgdlg.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>

#include "constants.h"
#include "gui/console_frame.h"
#include "gui/dynamical_frame.h"
#include "gui/parameter_frame.h"
#include "gui/solution_dialog.h"
#include "gui/singular_point_dialog.h"
#include "gui/dynamical_dialog.h"
#include "gui/isocline_dialog.h"
#include "gui/parameter_dialog.h"

namespace dynsolver {
namespace gui {

namespace {
// Attempts to create the opengl context with the provided attributes.
wxGLContext create_context(const wxGLAttributes&, const wxGLContextAttrs&);

void APIENTRY opengl_debug_message_callback( GLenum source,
					       GLenum type,
					       GLuint id,
					       GLenum severity,
					       GLsizei length,
					       const GLchar* message,
					       const void* userParam ) {
  if(severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
    throw std::runtime_error("OpenGL exception: " + std::to_string(type)
			     + ": " + message);
  } else {
    std::cout << "GL Notification: " << message << std::endl;
  }
}
} // namespace anonymous

app::app() : customLogger(nullptr),
	     modelData(nullptr),
	     glContext(nullptr),
	     consoleFrame(nullptr),
	     solutionDialog(nullptr),
	     singularPointDialog(nullptr),
	     dynamicalDialog(nullptr),
	     parameterDialog(nullptr),
	     isoclineDialog(nullptr) { }

app::~app() {
  // This destructer is called multiple times for some stupid reason!
  delete modelData;
  modelData = nullptr;
  delete glContext;
  glContext = nullptr;
}

bool app::OnInit() {
  customLogger = new wxLogStderr();
  // Set wx global logging to output directly to stderr.
  wxLog::SetActiveTarget(customLogger);
  
  // Initializes the attributes which are used to generate an opengl context.
  glAttributes.Defaults().EndList();
  glContextAttributes.PlatformDefaults()
    .CoreProfile()
    .OGLVersion(constants::app::kGLMajorVersion, constants::app::kGLMinorVersion)
    .EndList();

  glContext = new wxGLContext(create_context(glAttributes, glContextAttributes));
  
  // Check to see if the context was created successfully. If not,
  // we try again. We try no more than 3 times before giving up.
  while(!glContext->IsOK()) {
    delete glContext;
    glContext = nullptr;
    wxMessageDialog messageDialog(nullptr, "Failed to create an opengl context. Try again?",
				  "OpenGL Error", wxOK | wxCANCEL);
    if(messageDialog.ShowModal() != wxID_OK) {
      return false;
    }
    glContext = new wxGLContext(create_context(glAttributes, glContextAttributes));
  }
  // We have now constructed a context!

  // In order to make the context current, we need a visible wxGLCanvas
  // We may then delete the canvas and still use the headless context.
  
  wxDialog* dummyDialog = new wxDialog(nullptr, wxID_ANY, "");
  wxGLCanvas* dummyCanvas = new wxGLCanvas(dummyDialog, glAttributes);
  dummyCanvas->Bind(wxEVT_PAINT, [&](wxPaintEvent&){
      // We only want to set the context current once,
      // but the paint event may be called several times.
      static bool first(true);
      if(first && dummyCanvas->IsShownOnScreen()) {
	glContext->SetCurrent(*dummyCanvas);
	first = false;
      }
      
      // We need this check because sometimes the paint
      // event is called before
      // the canvas is considered modal.
      if(dummyDialog->IsModal() && !first) {
	dummyDialog->EndModal(0);
      }
    });
  dummyDialog->ShowModal();
  delete dummyDialog; // Also deletes dummyCanvas
  // The context is now current!
  // We now load the function pointers
  bool success = gladLoadGL();
  assert(success);

  // We set up opengl debugging and callback info.
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(opengl_debug_message_callback, nullptr);
  
  // We may now finally construct the model.
  modelData = new model();
  
  // Setup a basic example.
  consoleFrame = new console_frame(*this);
  consoleFrame->Show();

  // Set up dialogs
  solutionDialog = new solution_dialog(*this);
  singularPointDialog = new singular_point_dialog(*this);
  dynamicalDialog = new dynamical_dialog(*this);
  isoclineDialog = new isocline_dialog(*this);
  parameterDialog = new parameter_dialog(*this);
  
  return true;
}

void app::delete_dynamical(dynamical_id id) {
  modelData->delete_dynamical(id);
  dynamicalFrames.at(id)->Destroy();
  dynamicalFrames.erase(id);
}

void app::delete_parameter(parameter_id id) {
  modelData->delete_parameter(id);
  parameterFrames.at(id)->Destroy();
  parameterFrames.erase(id);
}

// Called in order to delete all dynamical windows.
// Does not assume that the dynamical frames have been destroyed,
// so it destroys them
void app::delete_all_dynamical_windows() {
  for(std::unordered_map<dynamical_id, dynamical_frame*>::const_iterator iter
	= dynamicalFrames.begin(); iter != dynamicalFrames.end(); ++iter) {
    modelData->delete_dynamical(iter->first);
    iter->second->Destroy();
  }
  dynamicalFrames.clear();
}

void app::delete_all_parameter_windows() {
  for(std::unordered_map<parameter_id, parameter_frame*>::const_iterator iter
	= parameterFrames.begin(); iter != parameterFrames.end(); ++iter) {
    modelData->delete_parameter(iter->first);
    iter->second->Destroy();
  }
  parameterFrames.clear();
}

void app::close_application() {
  delete_all_dynamical_windows();
  solutionDialog->Destroy();
  singularPointDialog->Destroy();
  dynamicalDialog->Destroy();
  consoleFrame->Destroy();
  isoclineDialog->Destroy();
  parameterDialog->Destroy();
}

void app::add_dynamical(const dynamical_specs& spec) {
  int width = spec.viewport2d.get_size().x();
  int height = spec.viewport2d.get_size().y();
  dynamical_id id = modelData->add_dynamical(spec);
  dynamical_frame* frame = new dynamical_frame(*this, id, width, height);
  dynamicalFrames.insert(std::make_pair(id, frame));
  frame->Show();
}

void app::add_parameter(const parameter_specs& spec) {
  math::vector2d size(spec.viewport.get_size());
  parameter_id id = modelData->add_parameter(spec);
  parameter_frame* frame = new parameter_frame(*this, id, size.x(), size.y());
  parameterFrames.insert(std::make_pair(id, frame));
  frame->Show();
}

void app::set_dynamical_specs(dynamical_id id,
					     const dynamical_specs& spec) {
  modelData->set_dynamical_specs(id, spec);
  dynamicalFrames.at(id)->refresh_gl_canvas();
}

void app::set_parameter_specs(parameter_id id,
			      const parameter_specs& spec) {
  modelData->set_parameter_specs(id, spec);
  parameterFrames.at(id)->refresh_gl_canvas();
}


void app::set_dynamical_viewport_2d(dynamical_id id,
				    const math::window2d& window) {
  modelData->set_dynamical_viewport_2d(id, window);
  dynamicalFrames.at(id)->refresh_gl_canvas();
}

void app::set_dynamical_viewport_3d(dynamical_id id,
				    const math::window3d& window) {
  modelData->set_dynamical_viewport_3d(id, window);
  dynamicalFrames.at(id)->refresh_gl_canvas();
}

void app::set_parameter_viewport(parameter_id id, const math::window2d& wind) {
  modelData->set_parameter_viewport(id, wind);
  parameterFrames.at(id)->refresh_gl_canvas();
}

int app::OnExit() {
  return 0;
}

bool app::OnExceptionInMainLoop() {
  throw;
  return false;
}
  
void app::OnUnhandledException() {
  throw;
}

bool app::StoreCurrentException() {
  try {
    throw;
  } catch(const std::exception& exc) {
    std::cout << exc.what() << std::endl;
  }
  return false;
}

solution_dialog* app::get_solution_dialog() const {
  return solutionDialog;
}

singular_point_dialog* app::get_singular_point_dialog() const {
  return singularPointDialog;
}

dynamical_dialog* app::get_dynamical_dialog() const {
  return dynamicalDialog;
}

isocline_dialog* app::get_isocline_dialog() const {
  return isoclineDialog;
}

parameter_dialog* app::get_parameter_dialog() const {
  return parameterDialog;
}

const model& app::get_model() const {
  return *modelData;
}

const wxGLContext& app::get_gl_context() const {
  return *glContext;
}

const wxGLAttributes& app::get_gl_attributes() const {
  return glAttributes;
}

void app::set_no_compile() {
  modelData->set_no_compile();
  delete_all_dynamical_windows();
  delete_all_parameter_windows();
  consoleFrame->set_no_compile();
}

void app::paint_dynamical(dynamical_id id) {
  modelData->paint_dynamical(id);
}

void app::paint_parameter(parameter_id id) {
  modelData->paint_parameter(id);
}

void app::resize_dynamical(dynamical_id id, double width, double height) {
  modelData->resize_dynamical(id, width, height);
}

void app::resize_parameter(parameter_id id, int width, int height) {
  modelData->resize_parameter(id, width, height);
}

bool app::add_isocline(const isocline_specs& specs) {
  bool success = modelData->add_isocline(specs);
  if(success) {
    refresh_dynamical_windows();
    consoleFrame->update_isoclines_list();
  }
  return success;
}

void app::add_solution(const solution_specs& spec) {
  modelData->add_solution(spec);
  refresh_dynamical_windows();
  consoleFrame->update_solutions_list();
}

bool app::add_singular_point(const singular_point_specs& spec) {
  bool success = modelData->add_singular_point(spec);
  if(success) {
    refresh_dynamical_windows();
    consoleFrame->update_singular_points_list();
  }
  return success;
}

bool app::compile(const std::string& varDiffName,
		  const std::vector<std::string>& dynamicalVarNames,
		  const std::vector<std::string>& parameterNames,
		  const std::vector<std::string>& expressions) {
  if(modelData->compile(varDiffName,
			dynamicalVarNames,
			parameterNames,
			expressions)) {
    consoleFrame->set_yes_compile();
    delete_all_dynamical_windows();
    delete_all_parameter_windows();
    return true;
  } else {
    return false;
  }
}

void app::refresh_dynamical_windows() {
  for(std::unordered_map<dynamical_id, dynamical_frame*>::const_iterator iter
	= dynamicalFrames.begin(); iter != dynamicalFrames.end(); ++iter) {
    iter->second->refresh_gl_canvas();
  }
}

void app::set_solution_specs(solution_id id,
				     const solution_specs& spec) {
  modelData->set_solution_specs(id, spec);
  refresh_dynamical_windows();
  consoleFrame->update_solutions_list();
}

void app::delete_solution(solution_id id) {
  modelData->delete_solution(id);
  refresh_dynamical_windows();
  consoleFrame->update_solutions_list();
}

void app::delete_singular_point(singular_point_id id) {
  modelData->delete_singular_point(id);
  refresh_dynamical_windows();
  consoleFrame->update_singular_points_list();
}

void app::delete_isocline(isocline_id id) {
  modelData->delete_isocline(id);
  refresh_dynamical_windows();
  consoleFrame->update_isoclines_list();
}

void app::select_object(dynamical_id id, int x, int y) {
  if(modelData->select_singular_point(id, x, y)) {
    consoleFrame->update_singular_points_list();
    refresh_dynamical_windows();
  } else if(modelData->select_solution(id, x, y)) {
    consoleFrame->update_solutions_list();
    refresh_dynamical_windows();
  } else if(modelData->select_isocline(id, x, y)) {
    consoleFrame->update_isoclines_list();
    refresh_dynamical_windows();
  } else {
    deselect_isocline();
    deselect_solution();
    deselect_singular_point();
  }
}

// Since these functions are called by console frame, there is no need,
// to update the console frame.
void app::select_solution(solution_id id) {
  modelData->select_solution(id);
  refresh_dynamical_windows();
  consoleFrame->update_solutions_list();
}
void app::select_singular_point(singular_point_id id) {
  modelData->select_singular_point(id);
  refresh_dynamical_windows();
  consoleFrame->update_singular_points_list();
}
void app::select_isocline(isocline_id id) {
  modelData->select_isocline(id);
  refresh_dynamical_windows();
  consoleFrame->update_isoclines_list();
}

void app::deselect_solution() {
  modelData->deselect_solution();
  refresh_dynamical_windows();
  consoleFrame->update_solutions_list();
}
void app::deselect_singular_point() {
  modelData->deselect_singular_point();
  refresh_dynamical_windows();
  consoleFrame->update_singular_points_list();
}
void app::deselect_isocline() {
  modelData->deselect_isocline();
  refresh_dynamical_windows();
  consoleFrame->update_isoclines_list();
}

namespace {
wxGLContext create_context(const wxGLAttributes& glAttributes,
		    const wxGLContextAttrs& glContextAttributes) {
  // Create a dummy frame and canvas that will be used to generate the context.
  // This is essentially a hack that allows us to create an opengl context
  // using wxWidgets.
  wxFrame* dummyFrame = new wxFrame(NULL, wxID_ANY, "");
  wxGLCanvas* dummyCanvas = new wxGLCanvas(dummyFrame, glAttributes);
  
  // Initialize opengl and create an opengl context.
  wxGLContext glContext(dummyCanvas, nullptr, &glContextAttributes);

  dummyFrame->Close(); // Also deletes the dummy frame and canvas.
  
  return glContext;
}
} // namespace anonymous
} // namespace gui
} // namespace dynsolver
