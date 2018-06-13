#include "gui/app.h"

#include <cassert>

#include <wx/msgdlg.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>

#include "gui/gl_util.h"
#include "constants.h"
#include "gui/console_frame.h"
#include "gui/dynamical_frame.h"
#include "gui/parameter_frame.h"

namespace dynsolver {
namespace gui {

namespace {
// Attempts to create the opengl context with the provided attributes.
wxGLContext create_context(const wxGLAttributes&, const wxGLContextAttrs&);

} // namespace anonymous

app::app() : glContext(nullptr) { }

app::~app() {
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

  modelData.initialize_opengl();
  
  // Setup a basic example.
  modelData.set_parameters(2);
  std::vector<std::string> initialExpressions;
  initialExpressions.push_back("b*x^2 - a*y");
  initialExpressions.push_back("x*y - a - t");
  success = modelData.compile(initialExpressions);
  assert(success);

  window2d win(point2d(500, 500), point2d(10, 10), point2d(-5, 5));
  int parameterId = modelData.add_parameter_window(win,0,1);
  int dynamicalId = modelData.add_dynamical_window(win,1,2);
  parameterFrames.insert(
      std::make_pair(parameterId, new parameter_frame(*this, parameterId)));
  dynamicalFrames.insert(
      std::make_pair(dynamicalId, new dynamical_frame(*this, dynamicalId)));
  consoleFrame = new console_frame(*this);
  consoleFrame->Show();
  dynamicalFrames.begin()->second->Show();
  parameterFrames.begin()->second->Show();
  return true;
}

int app::OnExit() {
  return 0;
}

const model& app::get_model() {
  return modelData;
}

const wxGLContext& app::get_gl_context() {
  return *glContext;
}

const wxGLAttributes& app::get_gl_attributes() {
  return glAttributes;
}

void app::paint_dynamical_window(dynamical_window_id id) {
  modelData.paint_dynamical_window(id);
}

void app::resize_dynamical_window(dynamical_window_id id) {
  modelData.resize_dynamical_window(id);
}

wxGLContextAttrs app::getGlContextAttributes() {
  return glContextAttributes;
}
					   
wxGLAttributes app::getGlAttributes() {
  return glAttributes;
}

void app::add_solution(const math::vector& initialVector, double tMin,
		       double tMax, double increment) {
  modelData.add_initial_value_solution(initialVector,
				       tMin, tMax, increment);
  refresh_dynamical_windows();
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
  
  // Deletes both the frame and the canvas automatically.
  delete dummyFrame;
  
  return glContext;
}
} // namespace anonymous
} // namespace gui
} // namespace dynsolver
