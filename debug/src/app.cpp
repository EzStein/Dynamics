#include "debug/app.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/bitmap.h>
#include <wx/image.h>

#include <iostream>
#include <string>

namespace dynsolver {
namespace debug {
namespace {
// Called by glfw upon errors in window/context creation/management
void glfw_error_callback(int err, const char * msg) {
  std::cerr << "GLFW Error (code = " << std::to_string(err) << "): " << msg << std::endl;
}

#ifdef GL_ERROR_CALLBACK
// Called by opengl when it has something interesting to report. Requirels gl version 4.3
void GLAPIENTRY gl_msg_callback( GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar* message,
                                 const void* userParam ) {
  std::cerr << "GL Message Callback: " << message << std::endl;
}
#else
// Called by glad after each gl function call. This method will print an error
// generated by the function. This is the alternative if we have gl < 3.3
void glad_callback(const char* name, void* funcptr, int len_args, ...) {
  GLenum err = glad_glGetError();
  if(err != GL_NO_ERROR) {
    std::cerr << "OpenGL Error in function " << name << ": 0x" << std::hex << err << std::endl;
  }
}
#endif
}
bool app::OnInit() {
  // Initialize opengl
  glfwSetErrorCallback(glfw_error_callback);
  if(!glfwInit()) {
    assert(false);
  }
  // Core, debug, context attached to a hidden window.
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glWindow = glfwCreateWindow(1, 1, "", NULL, NULL);
  if(!glWindow) {
    assert(false);
  }
  glfwMakeContextCurrent(glWindow);
#ifndef GL_MESSAGE_CALLBACK
  glad_set_post_callback(glad_callback);
#endif
// Load function pointers. No gl calls can occur before this method.
  if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    assert(false);
  }
#ifdef GL_MESSAGE_CALLBACK
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_msg_callback, nullptr);
#endif
  // Initialize non standard frame buffer for offscreen rendering.
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glGenRenderbuffers(1, &colorRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, colorRbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, fboWidth, fboHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbo);
  glGenRenderbuffers(1, &depthRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fboWidth, fboHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    assert(false);
  }
  // Allows tightly packed RGB data when reading from framebuffer.
  glPixelStorei(GL_PACK_ALIGNMENT, 1);



  frame = new wxFrame(nullptr, wxID_ANY, "Debug");
  canvas = new wxPanel(frame);
  auto onPaint = [&](const wxPaintEvent& evt) {
    wxPaintDC dc(canvas);
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.Clear();

    // Enough space for RGB data where each component is an unsigned byte.
    int width, height;
    canvas->GetSize(&width, &height);
    glViewport(0,0,width,height);
    glClearColor(1,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    GLubyte* data = new GLubyte[width * height * 3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    wxBitmap bitmap(wxImage(width, height, data, true));
    dc.DrawBitmap(bitmap, 0, 0);
    delete[] data;
  };
  canvas->Bind(wxEVT_PAINT, onPaint);
  frame->Show();
  return true;
}

int app::OnExit() {
  // Cleanup
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteRenderbuffers(1, &colorRbo);
  glDeleteRenderbuffers(1, &depthRbo);
  glDeleteFramebuffers(1, &fbo);
  glfwDestroyWindow(glWindow);
  glfwTerminate();
  return wxApp::OnExit();
}
}
}
