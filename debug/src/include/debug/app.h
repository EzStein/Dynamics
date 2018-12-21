#ifndef DYNSOLVER_DEBUG_APP_H_
#define DYNSOLVER_DEBUG_APP_H_

#include <wx/app.h>

#include "glad/glad.h"

class wxFrame;
class wxPanel;
struct GLFWwindow;

namespace dynsolver {
namespace debug {

class app : public wxApp {
 private:
  wxFrame* frame;
  wxPanel* canvas;
  GLFWwindow* glWindow;

  // Non default framebuffer.
  GLuint fbo;
  GLuint colorRbo, depthRbo;
  GLsizei fboWidth = 1000, fboHeight = 1000;

 public:
  virtual bool OnInit() override;
  virtual int OnExit() override;
};
}
}

#endif
