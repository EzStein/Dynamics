#include "gui/app.h"

#include <cassert>
#include <vector>

namespace dynsolver {
namespace gui {

app::app() : glContext(nullptr) { }

app::~app() {
  delete glContext;
}

bool app::OnInit() {
  glContextAttributes.PlatformDefaults().EndList();
  glAttributes.Defaults().EndList();

  // Create a dummy frame and canvas that will be used to generate the context.
  wxFrame* dummyFrame = new wxFrame(NULL, wxID_ANY, "");
  wxGLCanvas* dummyCanvas = new wxGLCanvas(dummyFrame, glAttributes);
  
  // Initialize opengl and create an opengl context.
  glContext = new wxGLContext(dummyCanvas, nullptr, &glContextAttributes);
  delete dummyFrame;

  while(!glContext->IsOK()) {
    //Display error message.
    wxMessageDialog* messageDialog =
      new wxMessageDialog(nullptr, "Failed to create an openGL context. This"
			  "could be because your graphics drivers are out of date.",
			  "OpenGL Error", wxOK);
    messageDialog->ShowModal();
    delete messageDialog;
    
    // ReCreate context
    delete glContext;
    dummyFrame = new wxFrame(NULL, wxID_ANY, "");
    dummyCanvas = new wxGLCanvas(dummyFrame, glAttributes);
    glContext = new wxGLContext(dummyCanvas, nullptr, &glContextAttributes);
    delete dummyFrame;
  }

  // Load openGL function pointers
  while (gladLoadGL()) {
    // Display error message.
    wxMessageDialog* messageDialog =
      new wxMessageDialog(nullptr, "Failed to load openGL function pointers.",
			  "OpenGL Error", wxOK);
    messageDialog->ShowModal();
    delete messageDialog;
  }
  


  
  // Setup a basic example.
  modelData.set_parameters(2);
  std::vector<std::string> initialExpressions;
  initialExpressions.push_back("b*x^2 - a*y");
  initialExpressions.push_back("x*y - a - t");
  bool success = modelData.compile(initialExpressions);
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

// Called exactly once upon starting the program.
void app::setup_opengl(wxCommandEvent&) {
}

} // namespace gui
} // namespace dynsolver
