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
    throw std::runtime_error("OpenGL exception: " + std::to_string(type) + ": " + message);
  } else {
    std::cout << "GL Notification: " << message << std::endl;
  }
}

} // namespace anonymous

app::app() : glContext(nullptr), solutionDialog(nullptr), modelData(nullptr) { }

app::~app() {
  delete modelData;
  delete glContext;
}

bool app::OnInit() {
  // Set wx global logging to output directly to stderr.
  wxLog::SetActiveTarget(&customLogger);
  
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
  
  int initialSize = 500;
  window2d win(point2d(initialSize, initialSize), point2d(10, 10), point2d(-5, 5));
  int dynamicalId = modelData->add_dynamical_window(win, 1, 2);
  dynamicalFrames.insert( std::make_pair(dynamicalId,
					 new dynamical_frame(*this, consoleFrame, dynamicalId,
							     initialSize, initialSize)));
  dynamicalFrames.begin()->second->Show();
  
  solutionDialog = new solution_dialog(consoleFrame);
  
  return true;
}

int app::OnExit() {
  return 0;
}

bool app::OnExceptionInMainLoop() {
  try {
    throw;
  } catch(const std::exception& exc) {
    std::cout << exc.what() << std::endl;
  }
  return false;
}
  
void app::OnUnhandledException() {
  try {
    throw;
  } catch(const std::exception& exc) {
    std::cout << exc.what() << std::endl;
  }
}

bool app::StoreCurrentException() {
  try {
    throw;
  } catch(const std::exception& exc) {
    std::cout << exc.what() << std::endl;
  }
  return false;
}

solution_dialog* app::get_solution_dialog() {
  return solutionDialog;
}

const model& app::get_model() {
  return *modelData;
}

const wxGLContext& app::get_gl_context() {
  return *glContext;
}

const wxGLAttributes& app::get_gl_attributes() {
  return glAttributes;
}

void app::paint_dynamical_window(dynamical_window_id id) {
  modelData->paint_dynamical_window(id);
}

void app::resize_dynamical_window(dynamical_window_id id, double width, double height) {
  modelData->resize_dynamical_window(id, width, height);
}

wxGLContextAttrs app::getGlContextAttributes() {
  return glContextAttributes;
}
					   
wxGLAttributes app::getGlAttributes() {
  return glAttributes;
}

void app::add_solution(const solution_specification& spec) {
  modelData->add_solution(spec);
  refresh_dynamical_windows();
}

bool app::compile(const std::vector<std::string> system) {
  if(modelData->compile(system)) {
    refresh_dynamical_windows();
    return true;
  } else {
    return false;
  }
}

void app::refresh_dynamical_windows() {
  for(std::unordered_map<dynamical_window_id, dynamical_frame*>::const_iterator iter
	= dynamicalFrames.begin(); iter != dynamicalFrames.end(); ++iter) {
    iter->second->refresh_gl_canvas();
  }
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
