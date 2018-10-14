#ifndef DYNSOLVER_DEBUG_APP_H_
#define DYNSOLVER_DEBUG_APP_H_

#include <wx/app.h>

class wxGLCanvas;
class wxFrame;
class wxGLContext;

namespace dynsolver {
namespace debug {

class app : public wxApp {
 private:
  wxGLCanvas* canvas;
  wxFrame* frame;
  
  wxGLContext* glContext;
  
 public:
  virtual bool OnInit() override;
  ~app();
};
}
}

#endif
