#include "glad/glad.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "app/app.h"
#include "top_frame.h"
#include "compiler/front/driver.h"
#include "math/vector.h"
#include "math/util.h"
#include <wx/glcanvas.h>
#include <string>
#include <list>
#include <iostream>

top_frame::top_frame(wxWindow* window, wxWindowID id) : top_frame_base(window, id) {
  glPanel = new wxGLCanvas(m_notebook2);
  wxGLContextAttrs contextAttrs;
  contextAttrs.CoreProfile().OGLVersion(4,5).EndList();
  glContext = new wxGLContext(glPanel, NULL, &contextAttrs);
  std::cout << glContext->IsOK() << std::endl;
  m_notebook2->AddPage( glPanel, wxT("GL Panel"), false );
  glPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( top_frame::on_paint_gl_renderer ), NULL, this);

  valueBoundaryTopLeft[0] = -5;
  valueBoundaryTopLeft[1] = 5;
  pixelToValueRatio[0] = 70;
  pixelToValueRatio[1] = 70;
  dynamicalPlane->Refresh();
}

/*
* Reads a file into the character buffer provided,
* appending a null terminating character if necessary.
* No more than size characters will be read into the buffer.
* Returns true if the whole file was read. False if it was truncated.
*/
bool read_file(const char * filePath, char* buffer, size_t size) {
  FILE* file = fopen(filePath, "rb");
  if(!file)
    std::cout << "No such file or director: " << filePath << std::endl;
  fseek(file, 0, SEEK_END);
  const size_t fileSize = ftell(file);
  rewind(file);
  size = size - 1 < fileSize?size-1:fileSize;
  fread(buffer, sizeof(char), size, file);
  fclose(file);
  buffer[size] = '\x00';
  return size == fileSize;
}

void initialize_gl() {
  if (!gladLoadGL()) {
      std::cout << "Failed to initialize OpenGL context" << std::endl;
  }
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char * path = PROJECT_PATH"/resources/gl/vertex.vert";
  /*Allocate a buffer to read the file into*/
  size_t bufferSize = 1024;
  char * buffer = static_cast<char*>(malloc(bufferSize*sizeof(char)));
  if(!read_file(path, buffer, bufferSize)) {
    std::cout << "BUFFER TOO SMALL TO READ IN FILE" << std::endl;
    return;
  }
  glShaderSource(vertexShader, 1, &buffer, NULL);
  glCompileShader(vertexShader);

  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "GL shader compilation error VERTEX: " << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  path = PROJECT_PATH"/resources/gl/fragment.frag";

  if(!read_file(path, buffer, bufferSize)) {
    std::cout << "BUFFER TOO SMALL TO READ IN FILE" << std::endl;
    return;
  }

  glShaderSource(fragmentShader, 1, &buffer, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "GL shader compilation error FRAGMENT: " << infoLog << std::endl;
  }
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "GL shader linking error: " << infoLog << std::endl;
  }
  free(buffer);
}

top_frame::~top_frame() {
  glPanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( top_frame::on_paint_gl_renderer ), NULL, glPanel);
}

void top_frame::on_paint_gl_renderer(wxPaintEvent& evt) {
  static int i = 0;
  glPanel->SetCurrent(*glContext);
  if(i == 0) initialize_gl();
  std::cout << ++i <<std::endl;

  int x, y;
  glPanel->GetSize(&x, &y);
  std::cout << x << std::endl;
  glViewport(0,0,x, y);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
  glPanel->SwapBuffers();
}

void top_frame::update_vals() {
  tMax = std::stod(std::string(tMaxValField->GetValue()));
  tMin = std::stod(std::string(tMinValField->GetValue()));
  tInc = std::stod(std::string(tIncrementField->GetValue()));
  initVals[1] = std::stod(std::string(xInitValField->GetValue()));
  initVals[2] = std::stod(std::string(yInitValField->GetValue()));
  initVals[3] = std::stod(std::string(zInitValField->GetValue()));
  initVals[0] = std::stod(std::string(tInitValField->GetValue()));
}

void top_frame::on_button_click_compile(wxCommandEvent& event) {
  std::string xFuncString = std::string(xFuncField->GetValue().mb_str());
  std::string yFuncString = std::string(yFuncField->GetValue().mb_str());
  std::string zFuncString = std::string(zFuncField->GetValue().mb_str());
  if(xFuncString.empty() || yFuncString.empty() || zFuncString.empty())
    return;
  update_vals();

  xFunc =
    dr.compile_as_function<driver::var4_double_func_t>(xFuncString);
  yFunc =
    dr.compile_as_function<driver::var4_double_func_t>(yFuncString);
  zFunc =
    dr.compile_as_function<driver::var4_double_func_t>(zFuncString);

  solutions = std::vector<solution_t>();
  dynamicalPlane->Refresh();
}

void top_frame::on_left_down_dynamical_plane(wxMouseEvent& evt) {
  if(!xFunc || !yFunc || !zFunc) return;
  update_vals();
  math::vector<int, 2> mousePos;
  mousePos[0] = evt.GetPosition().x;
  mousePos[1] = evt.GetPosition().y;
  math::vector<double, 2> mouseValPos =
  math::pixel_to_value(mousePos, valueBoundaryTopLeft, pixelToValueRatio);
  math::vector<double, 4> vec;
  math::vector<int, 2> indices =
    get_axes_choice_indices(get_axes_choice_from_string(
      std::string(axesChoice->GetString(axesChoice->GetSelection()))));
  for(int i = 0; i != 4; ++i) {
    if(i == indices[0])
      vec[i] = mouseValPos[0];
    else if(i == indices[1])
      vec[i] = mouseValPos[1];
    else
      vec[i] = initVals[i];
  }

  std::list<math::vector<double, 4> > points = std::list<math::vector<double, 4> >();
  std::cout << vec << std::endl;
  points.push_back(vec);
  while(vec[0] < tMax) {

    vec = math::euler(vec, xFunc, yFunc, zFunc, tInc);
    points.push_back(vec);
  }
  vec = *points.begin();
  std::cout << tMin << std::endl;
  while(vec[0] > tMin) {

    vec = math::euler(vec, xFunc, yFunc, zFunc, -tInc);
    points.push_front(vec);

  }

  solutions.push_back(points);
  dynamicalPlane->Refresh();
}

void top_frame::on_motion_dynamical_plane(wxMouseEvent& evt) {
  math::vector<int, 2> mousePos;
  mousePos[0] = evt.GetPosition().x;
  mousePos[1] = evt.GetPosition().y;

  math::vector<double, 2> mouseValPos =
  math::pixel_to_value(mousePos, valueBoundaryTopLeft, pixelToValueRatio);
  statusBar->SetStatusText("t = " + std::to_string(mouseValPos[0]) + "\tx = " + std::to_string(mouseValPos[1]));
}

void top_frame::on_choice_dimension(wxCommandEvent& evt) {
  int dim = std::stoi(std::string(evt.GetString()));
  if(dim == 1) {

  } else if(dim == 2) {

  } else if(dim == 3) {

  }
}

void top_frame::on_choice_axes(wxCommandEvent& evt) {
  dynamicalPlane->Refresh();
}

void top_frame::on_paint_dynamical_plane(wxPaintEvent& evt) {
  wxPaintDC dc(dynamicalPlane);
  math::vector<int, 2> canvasSize;
  dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(0,0,canvasSize[0], canvasSize[1]);
  dc.SetBrush(*wxRED_BRUSH);


  /*Paint the axes*/
  math::vector<int, 2> tmp;
  for(int xInc = 40; xInc < canvasSize[0]; xInc+=80) {
    tmp[0] = xInc;
    tmp[1] = 0;
    math::vector<double, 2> val = math::pixel_to_value(tmp, valueBoundaryTopLeft, pixelToValueRatio);
    dc.DrawText(std::to_string(val[0]), xInc, canvasSize[1] - 20);
    dc.DrawLine(xInc, canvasSize[1], xInc, canvasSize[1] - 10);
  }
  for(int yInc = 20; yInc < canvasSize[0]; yInc+=40) {
    tmp[0] = 0;
    tmp[1] = yInc;
    math::vector<double, 2> val = math::pixel_to_value(tmp, valueBoundaryTopLeft, pixelToValueRatio);
    dc.DrawText(std::to_string(val[1]), 20, yInc);
    dc.DrawLine(0, yInc, 10, yInc);
  }


  math::vector<int, 2> indices =
    get_axes_choice_indices(get_axes_choice_from_string(std::string(axesChoice->GetString(axesChoice->GetSelection()))));
  std::vector<std::list<math::vector<double, 4> > >::const_iterator solution;

  if(solutions.empty()) return;
  /*Plot vector field*/
  if(indices[0] == 1 && indices[1] == 2) {
    for(int xp = 0; xp < canvasSize[0]; xp+=50) {
      for(int yp = 0; yp < canvasSize[1]; yp+=50) {
        math::vector<int, 2> p;
        p[0] = xp;
        p[1] = yp;
        math::vector<double, 2> v = math::pixel_to_value(p,
        valueBoundaryTopLeft, pixelToValueRatio);
        double xSlope = xFunc(initVals[0], v[0], v[1], initVals[3]);
        double ySlope = yFunc(initVals[0], v[0], v[1], initVals[3]);
        if(xSlope == 0 || ySlope == 0) continue;
        double m = ySlope/xSlope;
        if(m <= 1 && m >= -1)
          dc.DrawLine(xp-10, yp + 10*ySlope/xSlope, xp+10, yp-10*ySlope/xSlope);
        else
          dc.DrawLine(xp-10/m, yp + 10, xp+10/m, yp-10);
      }
    }
  }

  for(solution = solutions.begin(); solution != solutions.end(); ++solution) {
    if(solution->size() < 2) continue; //There are not enough points in the solution

    //We iterate over the points of the solution which are ordered by time
    std::list<math::vector<double, 4> >::const_iterator iter;
    math::vector<double, 4> vec1 = *solution->begin(), vec2;
    for(iter = solution->begin()++; iter != solution->end(); ++iter) {
      vec2 = *iter;

      math::vector<double,2> tmp2;
      tmp2[0] = vec1[indices[0]];
      tmp2[1] = vec1[indices[1]];
      math::vector<int, 2> pixel1 = math::value_to_pixel(tmp2,
        valueBoundaryTopLeft, pixelToValueRatio);

      tmp2[0] = vec2[indices[0]];
      tmp2[1] = vec2[indices[1]];
      math::vector<int, 2> pixel2 = math::value_to_pixel(tmp2,
        valueBoundaryTopLeft, pixelToValueRatio);
      if(pixel1[0] < 0 || pixel1[0] > canvasSize[0] ||
        pixel1[1] < 0 || pixel1[1] > canvasSize[1] ||
        pixel2[0] < 0 || pixel2[0] > canvasSize[0] ||
          pixel2[1] < 0 || pixel2[1] > canvasSize[1]) {
            vec1 = vec2;
            continue;
          }

      dc.DrawLine(pixel1[0], pixel1[1], pixel2[0], pixel2[1]);
      vec1 = vec2;
    }
  }
  std::cout << "DONE"<<std::endl;
}

math::vector<int, 2> get_axes_choice_indices(axes_choice axesChoice) {
  int indexH, indexV;
  if(axesChoice == axes_choice::XT) {
    indexH = 1;
    indexV = 0;
  } else if(axesChoice == axes_choice::TX) {
    indexH = 0;
    indexV = 1;
  } else if(axesChoice == axes_choice::TY) {
    indexH = 0;
    indexV = 2;
  } else if(axesChoice == axes_choice::YT) {
    indexH = 2;
    indexV = 0;
  } else if(axesChoice == axes_choice::TZ) {
    indexH = 0;
    indexV = 3;
  } else if(axesChoice == axes_choice::ZT) {
    indexH = 3;
    indexV = 0;
  } else if(axesChoice == axes_choice::XY) {
    indexH = 1;
    indexV = 2;
  } else if(axesChoice == axes_choice::YX) {
    indexH = 2;
    indexV = 1;
  } else if(axesChoice == axes_choice::XZ) {
    indexH = 1;
    indexV = 3;
  } else if(axesChoice == axes_choice::ZX) {
    indexH = 3;
    indexV = 1;
  } else if(axesChoice == axes_choice::YZ) {
    indexH = 2;
    indexV = 3;
  } else if(axesChoice == axes_choice::ZY) {
    indexH = 3;
    indexV = 2;
  }
  math::vector<int, 2> ret;
  ret[0] = indexH;
  ret[1] = indexV;
  return ret;
}

axes_choice get_axes_choice_from_string(std::string str){
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  if(str == "XT") {
    return axes_choice::XT;
  } else if(str == "TX") {
    return axes_choice::TX;
  } else if(str == "TY") {
    return axes_choice::TY;
  } else if(str == "YT") {
    return axes_choice::YT;
  } else if(str == "TZ") {
    return axes_choice::TZ;
  } else if(str == "ZT") {
    return axes_choice::ZT;
  } else if(str == "XY") {
    return axes_choice::XY;
  } else if(str == "YX") {
    return axes_choice::YX;
  } else if(str == "XZ") {
    return axes_choice::XZ;
  } else if(str == "ZX") {
    return axes_choice::ZX;
  } else if(str == "YZ") {
    return axes_choice::YZ;
  } else if(str == "ZY") {
    return axes_choice::ZY;
  }
}


void top_frame::on_size_dynamical_plane( wxSizeEvent& event ) {

}

void top_frame::on_size_top_frame( wxSizeEvent& event ) {
  glPanel->Refresh();
}


void top_frame::on_menu_selection_vector_field(wxCommandEvent&) {

}
