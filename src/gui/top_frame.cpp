
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "app/app.h"
#include "top_frame.h"
#include "compiler/front/driver.h"
#include "math/static_vector.h"
#include "math/util.h"
#include "gl_program.h"
#include <wx/glcanvas.h>
#include <string>
#include <list>
#include <iostream>

top_frame::top_frame(wxWindow* window, wxWindowID id) :
 top_frame_base(window, id), program2d(PROJECT_PATH"/resources/gl/vert2d.vert", PROJECT_PATH"/resources/gl/frag2d.frag"),
 program3d(PROJECT_PATH"/resources/gl/vert3d.vert", PROJECT_PATH"/resources/gl/frag3d.frag") {
  dynamicalPlane = new wxGLCanvas(m_notebook2);
  wxGLContextAttrs contextAttrs;
  contextAttrs.CoreProfile().OGLVersion(4,5).EndList();
  glContext = new wxGLContext(dynamicalPlane, NULL, &contextAttrs);
  std::cout << glContext->IsOK() << std::endl;
  m_notebook2->AddPage( dynamicalPlane, wxT("Dynamical Plane"), false );

  dynamicalPlane->Connect( wxEVT_PAINT, wxPaintEventHandler(top_frame::on_paint_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler(top_frame::on_left_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_MOTION, wxMouseEventHandler(top_frame::on_motion_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_SIZE, wxSizeEventHandler(top_frame::on_size_dynamical_plane ), NULL, this);

  /*Preallocate the info*/
  initVals = std::vector<double>(4);
  functions = {nullptr, nullptr, nullptr};

  data.viewportCenterX = 0;
  data.viewportCenterY = 0;
  data.viewportSpanX = 10;
  data.viewportSpanY = 10;
  dynamicalPlane->Refresh();
}



void top_frame::initialize_gl() {
  /*We now set up opengl*/
  /*Loads function pointers to gl functions*/
  if (!gladLoadGL()) {
      std::cout << "Failed to load gl function pointers!" << std::endl;
  }



  /*We now load the shader program*/
  program3d.compile();
  program2d.compile();
  program2d.bind();


  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  /*We now set up TWO VAO's. One which
  draws a triangle and one which draws a square*/

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glVertexAttribPointer(program2d.get_attribute_location("pos"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(program2d.get_attribute_location("pos"));
  glBindVertexArray(0);
}

top_frame::~top_frame() {

  dynamicalPlane->Disconnect( wxEVT_PAINT, wxPaintEventHandler(top_frame::on_paint_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler(top_frame::on_left_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_MOTION, wxMouseEventHandler(top_frame::on_motion_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_SIZE, wxSizeEventHandler(top_frame::on_size_dynamical_plane ), NULL, this);
}

void top_frame::on_paint_dynamical_plane(wxPaintEvent& evt) {
  static int a = 0;
  dynamicalPlane->SetCurrent(*glContext);
  if(!a) {
    initialize_gl();
    a = 1;
    std::cout << "INITIALIZED" << std::endl;
  }
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  /*We first need to calculate the total number of vertices in the solution set
  We do this by iterating over the solutions and suming their sizes*/
  int totalFloats = 0;
  for(solution sol : data.solutions) {
    totalFloats += sol.points.size();
  }
  /*Since there are two floats per vertex*/
  totalFloats *= 2;
  if(totalFloats == 0) {
    /*In this event, there are no solutions given,
    so we don't draw anything*/
    return;
  }
  float* vertexData = new float[totalFloats];

  /*We now fill in the data accordingly*/
  int index = -1;
  for(solution sol : data.solutions) {
    for(math::vector<double> vec : sol.points) {
      vertexData[++index] = static_cast<float>(vec[0]);
      vertexData[++index] = static_cast<float>(vec[1]);
    }
  }

  /*Copy the vertex data to the GPU*/
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
    totalFloats * sizeof(float), vertexData, GL_STREAM_DRAW);
  delete[] vertexData;

  glBindVertexArray(vao);
  math::static_matrix<float, 4,4> transformationMatrix(data.generate_2d_transformation_matrix());
  program2d.set_float_mat4_uniform("transformation", true, transformationMatrix.data());
  int offset = 0;
  for(solution sol : data.solutions) {
    glDrawArrays(GL_LINE_STRIP,offset,sol.points.size());
    offset += sol.points.size();
  }

  glBindVertexArray(0);
  dynamicalPlane->SwapBuffers();
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

  for(driver::double_func_t func : functions) {
    dr.mark_available(func);
  }
  functions.clear();

  functions.push_back(
    dr.compile_as_function<driver::double_func_t>(xFuncString));
  functions.push_back(
    dr.compile_as_function<driver::double_func_t>(yFuncString));
  functions.push_back(
    dr.compile_as_function<driver::double_func_t>(zFuncString));


  /*Clear all the solutions*/
  data.solutions = std::vector<solution>();
  dynamicalPlane->Refresh();
}

void top_frame::on_left_down_dynamical_plane(wxMouseEvent& evt) {
  if(!functions[0] || !functions[1] || !functions[2]) return;
  update_vals();
  math::static_vector<int, 2> canvasSize;
  dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  /*we set the mouse position to be a standard vec of size 4 whose last element
  is 1 (this allows for translations)*/
  math::static_vector<double, 4> mousePos;
  mousePos[0] = 2*static_cast<double>(evt.GetPosition().x)/static_cast<double>(canvasSize[0]) - 1;
  mousePos[1] = 1 - 2*static_cast<double>(evt.GetPosition().y)/static_cast<double>(canvasSize[1]);
  mousePos[2] = 0;
  mousePos[3] = 1;
  math::static_vector<double, 2> mouseValPos;// =
  //math::pixel_to_value(mousePos, valueBoundaryTopLeft, pixelToValueRatio);
  math::static_vector<double,4> tmp = data.generate_2d_inverse_transformation_matrix() * mousePos;
  mouseValPos[0] = tmp[0];
  mouseValPos[1] = tmp[1];

  math::static_vector<int, 2> indices =
    get_axes_choice_indices(get_axes_choice_from_string(
      std::string(axesChoice->GetString(axesChoice->GetSelection()))));

  /*Set up initial condition*/
  math::vector<double> vec(4);
  for(int i = 0; i != 4; ++i) {
    if(i == indices[0])
      vec[i] = mouseValPos[0];
    else if(i == indices[1])
      vec[i] = mouseValPos[1];
    else
      vec[i] = initVals[i];
  }

  /*Construct a solution using the appropriate method*/

  solution sol;
  sol.initVals = vec;
  sol.points.push_back(vec);
  while(vec[0] < tMax) {
    math::euler(vec, functions, tInc);

    sol.points.push_back(vec);
  }
  vec = *sol.points.begin();

  while(vec[0] > tMin) {
    math::euler(vec, functions, -tInc);
    sol.points.push_front(vec);
  }

  data.solutions.push_back(sol);
  dynamicalPlane->Refresh();
}

void top_frame::set_nullclines() {
  // isoclines.clear();
  // /*Calculuate the nullclines*/
  // math::static_vector<int, 2> canvasSize;
  // dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  //
  // solution_t nullPoints;
  // math::static_vector<int, 2> indices =
  //   get_axes_choice_indices(get_axes_choice_from_string(
  //     std::string(axesChoice->GetString(axesChoice->GetSelection()))));
  //
  // /*Set to 0 when invalid. 1 if positive or zero, -1 if negative*/
  // for(driver::double_func_t func : functions) {
  //   if(!func) return;
  //   int sign = 0;
  //   for(int x = 0; x != canvasSize[0]; ++x) {
  //     for(int y = 0; y != canvasSize[1]; ++y) {
  //       math::static_vector<double,2> tmp;
  //       tmp[0] = x;
  //       tmp[1] = y;
  //       math::static_vector<double, 2> pts =
  //       math::pixel_to_value(tmp, valueBoundaryTopLeft, pixelToValueRatio);
  //       std::vector<double> vec(4);
  //       for(int i = 0; i != 4; ++i) {
  //         if(i == indices[0])
  //           vec[i] = pts[0];
  //         else if(i == indices[1])
  //           vec[i] = pts[1];
  //         else
  //           vec[i] = initVals[i];
  //       }
  //
  //       double val = func(vec.data());
  //       int nextSign;
  //       if(val <= 0) nextSign = -1;
  //       else if(val > 0) nextSign = 1;
  //       if((sign < 0 && nextSign > 0) || (sign > 0 && nextSign < 0) && sign != 0) {
  //         /*There was a change in sign.*/
  //         nullPoints.push_back(vec);
  //       }
  //       sign = nextSign;
  //     }
  //   }
  //
  //   sign = 0;
  //   for(int y = 0; y != canvasSize[0]; ++y) {
  //     for(int x = 0; x != canvasSize[1]; ++x) {
  //       math::static_vector<double,2> tmp;
  //       tmp[0] = x;
  //       tmp[1] = y;
  //       math::static_vector<double, 2> pts =
  //       math::pixel_to_value(tmp, valueBoundaryTopLeft, pixelToValueRatio);
  //       std::vector<double> vec(4);
  //       for(int i = 0; i != 4; ++i) {
  //         if(i == indices[0])
  //           vec[i] = pts[0];
  //         else if(i == indices[1])
  //           vec[i] = pts[1];
  //         else
  //           vec[i] = initVals[i];
  //       }
  //
  //       double val = func(vec.data());
  //       int nextSign;
  //       if(val <= 0) nextSign = -1;
  //       else if(val > 0) nextSign = 1;
  //       if((sign < 0 && nextSign > 0) || (sign > 0 && nextSign < 0) && sign != 0) {
  //         /*There was a change in sign.*/
  //         nullPoints.push_back(vec);
  //       }
  //       sign = nextSign;
  //     }
  //   }
  //   isoclines.push_back(nullPoints);
  // }
}

void top_frame::on_motion_dynamical_plane(wxMouseEvent& evt) {
  math::static_vector<int, 2> canvasSize;
  dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  /*we set the mouse position to be a standard vec of size 4 whose last element
  is 1 (this allows for translations)*/
  math::static_vector<double, 4> mousePos;
  mousePos[0] = 2*static_cast<double>(evt.GetPosition().x)/static_cast<double>(canvasSize[0]) - 1;
  mousePos[1] = 1 - 2*static_cast<double>(evt.GetPosition().y)/static_cast<double>(canvasSize[1]);
  mousePos[2] = 0;
  mousePos[3] = 1;
  math::static_vector<double, 2> mouseValPos;// =
  //math::pixel_to_value(mousePos, valueBoundaryTopLeft, pixelToValueRatio);
  math::static_vector<double,4> tmp = data.generate_2d_inverse_transformation_matrix() * mousePos;

  mouseValPos[0] = tmp[0];
  mouseValPos[1] = tmp[1];

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
  set_nullclines();
  dynamicalPlane->Refresh();
}

//void top_frame::on_paint_dynamical_plane(wxPaintEvent& evt) {
  // wxPaintDC dc(dynamicalPlane);
  // math::static_vector<int, 2> canvasSize;
  // dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  // dc.SetBrush(*wxWHITE_BRUSH);
  // dc.DrawRectangle(0,0,canvasSize[0], canvasSize[1]);
  // dc.SetPen(*wxBLACK_PEN);
  //
  //
  // /*Paint the axes*/
  // math::static_vector<int, 2> tmp;
  // for(int xInc = 40; xInc < canvasSize[0]; xInc+=80) {
  //   tmp[0] = xInc;
  //   tmp[1] = 0;
  //   math::static_vector<double, 2> val = math::pixel_to_value(tmp, valueBoundaryTopLeft, pixelToValueRatio);
  //   dc.DrawText(std::to_string(val[0]), xInc, canvasSize[1] - 20);
  //   dc.DrawLine(xInc, canvasSize[1], xInc, canvasSize[1] - 10);
  // }
  // for(int yInc = 20; yInc < canvasSize[0]; yInc+=40) {
  //   tmp[0] = 0;
  //   tmp[1] = yInc;
  //   math::static_vector<double, 2> val = math::pixel_to_value(tmp, valueBoundaryTopLeft, pixelToValueRatio);
  //   dc.DrawText(std::to_string(val[1]), 20, yInc);
  //   dc.DrawLine(0, yInc, 10, yInc);
  // }
  //
  //
  // math::static_vector<int, 2> indices =
  //   get_axes_choice_indices(get_axes_choice_from_string(std::string(axesChoice->GetString(axesChoice->GetSelection()))));
  // std::vector<solution_t>::const_iterator solution;
  //
  // if(solutions.empty()) return;
  // /*Plot static_vector field*/
  // if(indices[0] == 1 && indices[1] == 2) {
  //   for(int xp = 0; xp < canvasSize[0]; xp+=50) {
  //     for(int yp = 0; yp < canvasSize[1]; yp+=50) {
  //       math::static_vector<int, 2> p;
  //       p[0] = xp;
  //       p[1] = yp;
  //       math::static_vector<double, 2> v = math::pixel_to_value(p,
  //       valueBoundaryTopLeft, pixelToValueRatio);
  //       std::vector<double> tmp = {initVals[0], v[0], v[1], initVals[3]};
  //       double xSlope = functions[1](tmp.data());
  //       double ySlope = functions[2](tmp.data());
  //       if(xSlope == 0 || ySlope == 0) continue;
  //       double m = ySlope/xSlope;
  //       if(m <= 1 && m >= -1)
  //         dc.DrawLine(xp-10, yp + 10*ySlope/xSlope, xp+10, yp-10*ySlope/xSlope);
  //       else
  //         dc.DrawLine(xp-10/m, yp + 10, xp+10/m, yp-10);
  //     }
  //   }
  // }
  //
  // for(solution = solutions.begin(); solution != solutions.end(); ++solution) {
  //   if(solution->size() < 2) continue; //There are not enough points in the solution
  //
  //   //We iterate over the points of the solution which are ordered by time
  //   std::list<std::vector<double> >::const_iterator iter;
  //   std::vector<double> vec1 = *solution->begin(), vec2;
  //
  //   for(iter = solution->begin()++; iter != solution->end(); ++iter) {
  //     vec2 = *iter;
  //
  //     math::static_vector<double,2> tmp2;
  //     tmp2[0] = vec1[indices[0]];
  //     tmp2[1] = vec1[indices[1]];
  //     math::static_vector<int, 2> pixel1 = math::value_to_pixel(tmp2,
  //       valueBoundaryTopLeft, pixelToValueRatio);
  //
  //     tmp2[0] = vec2[indices[0]];
  //     tmp2[1] = vec2[indices[1]];
  //     math::static_vector<int, 2> pixel2 = math::value_to_pixel(tmp2,
  //       valueBoundaryTopLeft, pixelToValueRatio);
  //     if(pixel1[0] < 0 || pixel1[0] > canvasSize[0] ||
  //       pixel1[1] < 0 || pixel1[1] > canvasSize[1] ||
  //       pixel2[0] < 0 || pixel2[0] > canvasSize[0] ||
  //         pixel2[1] < 0 || pixel2[1] > canvasSize[1]) {
  //           vec1 = vec2;
  //           continue;
  //         }
  //
  //     dc.DrawLine(pixel1[0], pixel1[1], pixel2[0], pixel2[1]);
  //     vec1 = vec2;
  //   }
  // }
  //
  // /*Paint isoclines*/
  // dc.SetPen(*wxRED_PEN);
  // for(int i = 0; i != isoclines.size(); ++i) {
  //   std::list<std::vector<double> > nullcline = isoclines[i];
  //   std::list<std::vector<double> >::const_iterator iter = nullcline.begin();
  //   std::list<std::vector<double> >::const_iterator end = nullcline.end();
  //   for(; iter != end; ++iter) {
  //     math::static_vector<double,2> tmp;
  //     tmp[0] = (*iter)[indices[0]];
  //     tmp[1] = (*iter)[indices[1]];
  //     math::static_vector<int, 2> pixel1 = math::value_to_pixel(tmp,
  //       valueBoundaryTopLeft, pixelToValueRatio);
  //     dc.DrawLine(pixel1[0], pixel1[1], pixel1[0]+1, pixel1[1]+1);
  //   }
  // }
//}

math::static_vector<int, 2> get_axes_choice_indices(axes_choice axesChoice) {
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
  math::static_vector<int, 2> ret;
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
  dynamicalPlane->Refresh();
}


void top_frame::on_menu_selection_vector_field(wxCommandEvent&) {

}
