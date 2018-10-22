#include "debug/app.h"

#include <iostream>

#include <glad/glad.h>

#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/gdicmn.h>

namespace dynsolver {
namespace debug {

namespace {
void process_gl_errors() {
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR) {
    printf("Found GL ERROR: 0x%x\n", err);
  }
}
}

bool app::OnInit() {
  frame = new wxFrame(nullptr, wxID_ANY, "My Frame",
                      wxPoint(100, 100), wxSize(800, 600));

  wxGLAttributes glAttrs;
  glAttrs.PlatformDefaults().DoubleBuffer().EndList();

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
        glContext->SetCurrent(*canvas);
        // Call some gl functions
        glViewport(0, 0, canvas->GetSize().GetWidth(),
            canvas->GetSize().GetHeight());
        glClearColor(1.0f, 1.0f, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        bool success = canvas->SwapBuffers();
        assert(success);
        process_gl_errors();
      };
  canvas->Bind(wxEVT_PAINT, onPaint);

  // wxDataViewCtrl Tests
  dataViewFrame = new wxFrame(nullptr, wxID_ANY, "My Frame",
                              wxPoint(100, 100), wxSize(300, 300));
  dataView = new wxDataViewListCtrl(dataViewFrame, wxID_ANY);
  dataView->AppendTextColumn("My Non editables", wxDATAVIEW_CELL_INERT);
  dataView->AppendTextColumn("My editables", wxDATAVIEW_CELL_EDITABLE);
  wxVector<wxVariant> cells;
  cells.push_back(wxVariant("A"));
  cells.push_back(wxVariant("B"));
  dataView->AppendItem(cells);
  dataViewFrame->Show();
  return true;
}

app::~app() {
  delete glContext;
}
}
}
