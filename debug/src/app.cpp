#include "debug/app.h"

#include <iostream>

#include <glad/glad.h>

#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/gdicmn.h>

namespace dynsolver {
namespace debug {

bool app::OnInit() {
  frame = new wxFrame(nullptr, wxID_ANY, "My Frame",
                      wxPoint(100, 100), wxSize(400, 400));

  wxGLAttributes glAttrs;
  glAttrs.Defaults().EndList();
  
  canvas = new wxGLCanvas(frame, glAttrs);
  
  wxGLContextAttrs glContextAttrs;
  glContextAttrs
      .PlatformDefaults()
      .CoreProfile()
      .OGLVersion(GL_VERSION_MAJOR, GL_VERSION_MINOR)
      .EndList();
  std::cout << "Running under opengl version: " <<
      std::to_string(GL_VERSION_MAJOR) << "."
            << std::to_string(GL_VERSION_MINOR) << std::endl;
  
  glContext = new wxGLContext(canvas, NULL, &glContextAttrs);
  assert(glContext->IsOK());

  frame->Show(true);
  
  bool success = glContext->SetCurrent(*canvas);
  assert(success);

  success = gladLoadGL();
  assert(success);

  auto onPaint =
      [&](wxPaintEvent& evt){
        // Call some gl functions
        glViewport(0, 0, 400, 400);
        glClearColor(1.0f, 1.0f, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        bool success = canvas->SwapBuffers();
        assert(success);
      };
  canvas->Bind(wxEVT_PAINT, onPaint);
  
  return true;
}

app::~app() {
  delete glContext;
}
}
}
