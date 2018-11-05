#include "debug/app.h"

#include <iostream>

#include <glad/glad.h>

#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/gdicmn.h>

#include <gl/buffer.h>


#include <iostream>
#include "compiler/expression_parser.h"
#include <climits>
#include <stdexcept>
#ifdef IS_WINDOWS
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#endif

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
#ifdef IS_WINDOWS
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

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
  gl::buffer buf1(nullptr, 0, GL_DYNAMIC_DRAW);
  gl::buffer buf2(nullptr, 0, GL_DYNAMIC_DRAW);
  gl::buffer buf3(nullptr, 0, GL_DYNAMIC_DRAW);
  gl::buffer buf4(nullptr, 0, GL_DYNAMIC_DRAW);
  gl::buffer buf5(nullptr, 0, GL_DYNAMIC_DRAW);
  gl::buffer buf6(nullptr, 0, GL_DYNAMIC_DRAW);
  gl::buffer buf6_ref(std::move(buf6));
  gl::buffer buf6_ref1(std::move(buf6_ref));
  buf6.~buffer();
  buf6_ref.~buffer();
  gl::buffer buf7(nullptr, 0, GL_DYNAMIC_DRAW);


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

  std::cout << "Hello World." << std::endl;

  try {
	  compiler::expression_parser parse;
	  std::list<symbol> symbolTable;
	  symbolTable.push_back(symbol("x", 0, 0));
	  std::cout << "Expression:" << std::endl;
	  std::string expr;
	  std::getline(std::cin, expr);
	  std::cout << "Value:" << std::endl;
	  double val;
	  std::cin >> val;
	  AST ast = parse.parse(expr, symbolTable);
	  compiler::function<double, const double*> function = ast.compile();
	  std::cout << "Result: " << function(&val) << std::endl;
  }
  catch (const std::exception& exc) {
	  std::cout << exc.what() << std::endl;
  }
  return true;
}

app::~app() {
  delete glContext;
}
}
}
