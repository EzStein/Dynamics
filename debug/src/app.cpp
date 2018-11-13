#include "debug/app.h"

#include <iostream>

#include <glad/glad.h>

#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/gdicmn.h>

#include <gl/buffer.h>
#include <gl/shader.h>
#include <gl/program.h>
#include <gl/vertex_array.h>

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
          throw std::runtime_error("e");
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
      bool success = glContext->SetCurrent(*canvas);
      assert(success);

      success = gladLoadGL();
      assert(success);

      std::vector<gl::shader> shaders;
      shaders.push_back(gl::shader(util::read_file(
          util::get2dVertexShaderFilePath()), GL_VERTEX_SHADER));
      shaders.push_back(gl::shader(util::read_file(
          util::get2dFragmentShaderFilePath()), GL_FRAGMENT_SHADER));
      gl::program* program = new gl::program(shaders);
      gl::vertex_array* vao = new gl::vertex_array();
      glUseProgram(program->get_handle());
      glBindVertexArray(vao->get_handle());
      glEnableVertexAttribArray(0);
      GLfloat buf[4] = {0.0, 0.0, 1.0, 1.0};
      gl::buffer* buffer = new gl::buffer
          (reinterpret_cast<unsigned char*>(&buf),
           4 * sizeof(GLfloat), GL_DYNAMIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, buffer->get_handle());
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      GLfloat mat[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
      };
      GLint location = glGetUniformLocation(program->get_handle(),
                                            "transformation");
      glUniformMatrix4fv(location, 1, GL_TRUE, mat);
      location = glGetUniformLocation(program->get_handle(),
                                      "inColor");
      glUniform4f(location, 1, 0.5, 0.4, 1);
      auto onPaint =
	[&](wxPaintEvent& evt){
          glContext->SetCurrent(*canvas);
          glViewport(0, 0, canvas->GetSize().GetWidth(),
                     canvas->GetSize().GetHeight());
          glClearColor(1.0f, 1.0f, 0.0, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          glDrawArrays(GL_LINE_STRIP, 0, 4);
          bool success = canvas->SwapBuffers();
          process_gl_errors();
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
