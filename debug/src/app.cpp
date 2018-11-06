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
#include "util/util.h"

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
      std::cout << util::getDefaultFontPath() << std::endl;
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

      auto onPaint =
	[&](wxPaintEvent& evt){
	static bool first = true;
	if(first) {
	  bool success = glContext->SetCurrent(*canvas);
	  assert(success);

	  success = gladLoadGL();
	  assert(success);
	  first = false;
	  return;
	}
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
      return true;
    }

    app::~app() {
      delete glContext;
    }
  }
}
