
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
#include <exception>

top_frame::top_frame(wxWindow* window, wxWindowID id) :
 top_frame_base(window, id), program2d(PROJECT_PATH"/resources/gl/vert2d.vert", PROJECT_PATH"/resources/gl/frag2d.frag"),
 program3d(PROJECT_PATH"/resources/gl/vert3d.vert", PROJECT_PATH"/resources/gl/frag3d.frag") {
  dynamicalPlane = new wxGLCanvas(m_notebook2);
  wxGLContextAttrs contextAttrs;
  contextAttrs.CoreProfile().OGLVersion(4,5).EndList();
  glContext = new wxGLContext(dynamicalPlane, NULL, &contextAttrs);
  std::cout << glContext->IsOK() << std::endl;
  m_notebook2->AddPage( dynamicalPlane, wxT("Dynamical Plane"), false );

  {
    wxListItem col0;
    col0.SetText(wxT("Variable"));
    col0.SetWidth(80);
    functionListCtrl->InsertColumn(0, col0);
    wxListItem col1;
    col1.SetText(wxT("Value"));
    col1.SetWidth(150);
    functionListCtrl->InsertColumn(1, col1);
  }

  {
    wxListItem col0;
    col0.SetText(wxT("Variable"));
    col0.SetWidth(80);
    initialValueListCtrl->InsertColumn(0, col0);
    wxListItem col1;
    col1.SetText(wxT("Value"));
    col1.SetWidth(150);
    initialValueListCtrl->InsertColumn(1, col1);
  }

  dynamicalPlane->Connect( wxEVT_PAINT, wxPaintEventHandler(top_frame::on_paint_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler(top_frame::on_left_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler(top_frame::on_right_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_MOTION, wxMouseEventHandler(top_frame::on_motion_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_SIZE, wxSizeEventHandler(top_frame::on_size_dynamical_plane ), NULL, this);

  dynamicalPlane->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler(top_frame::on_key_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_KEY_UP, wxKeyEventHandler(top_frame::on_key_up_dynamical_plane ), NULL, this);

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
  dynamicalPlane->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler(top_frame::on_right_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_SIZE, wxSizeEventHandler(top_frame::on_size_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler(top_frame::on_key_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler(top_frame::on_key_up_dynamical_plane ), NULL, this);
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

  if(data.render2d) {
    program2d.bind();
    /*We are performing a 2d render*/
    /*We first need to calculate the total number of vertices in the solution set
    We do this by iterating over the solutions and suming their sizes*/
    int totalFloats = 0;
    for(solution sol : data.solutions) {
      totalFloats += sol.points.size();
    }

    /*Since there are two floats per vertex in 2d mode*/
    totalFloats *= 2;

    if(totalFloats == 0) {
      /*In this event, there are no solutions given,
      so we don't draw anything*/
      return;
    }

    if(data.redraw) {
      float* vertexData = new float[totalFloats];

      /*We now fill in the data accordingly*/
      int index = -1;
      for(solution sol : data.solutions) {
        for(math::vector<double> vec : sol.points) {
          vertexData[++index] = static_cast<float>(vec[data.axesVariable[0]]);
          vertexData[++index] = static_cast<float>(vec[data.axesVariable[1]]);
        }
      }

      /*Copy the vertex data to the GPU*/
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER,
        totalFloats * sizeof(float), vertexData, GL_STREAM_DRAW);
      delete[] vertexData;
      data.redraw = false;
      std::cout << "EXPENSIVE ALLOC: " << totalFloats * sizeof(float) << " bytes"<< std::endl;
    }

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
  } else {

  }
}

void top_frame::on_button_click_compile(wxCommandEvent& event) {
  for(wxTextCtrl* funcField : functionTextCtrlList) {
    /*If a single field is empty, we do nothing*/
    if(funcField->IsEmpty()) return;
  }
  /*Otherwise, release the space allocated by previous compilations*/
  for(driver::double_func_t func : functions) {
    dr.mark_available(func);
  }
  /*And reset the functions*/
  functions.clear();

  for(wxTextCtrl* funcField : functionTextCtrlList) {
    std::string funcString = std::string(funcField->GetValue().mb_str());

    functions.push_back(
      dr.compile_as_function<driver::double_func_t>(funcString)
    );
  }

  /*Clear all the solutions*/
  data.solutions = std::vector<solution>();
  data.redraw = true;
  dynamicalPlane->Refresh();
}

void top_frame::on_left_down_dynamical_plane(wxMouseEvent& evt) {
  /*Set the initial mouse positions*/
  initMousePos[0] = evt.GetPosition().x;
  initMousePos[1] = evt.GetPosition().y;

  /*If there are no functions, which happens at the begging, then we do nothgin*/
  if(functions.empty()) return;



  std::vector<double> initVals;
  for(wxTextCtrl* initValField : initialValueTextCtrlList) {
    std::string strVal(std::string(initValField->GetValue().mb_str()));
    double val;
    try {
      val = std::stod(strVal);
    } catch (const std::invalid_argument& arg) {
      std::cout << "Initival value string, '" << strVal << "' is not a number!" << std::endl;
      return;
    }
    initVals.push_back(val);
  }

  data.redraw = true;

  math::static_vector<int, 2> canvasSize;
  dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);

  /*we set the mouse position to be a standard vec of size 4 whose last element
  is 1 (this allows for translations)*/
  /*Compute the mouse position value for 2d*/
  math::static_vector<double, 4> mousePos;
  mousePos[0] = 2*static_cast<double>(evt.GetPosition().x)/static_cast<double>(canvasSize[0]) - 1;
  mousePos[1] = 1 - 2*static_cast<double>(evt.GetPosition().y)/static_cast<double>(canvasSize[1]);
  mousePos[2] = 0;
  mousePos[3] = 1;
  math::static_vector<double, 2> mouseValPos;
  math::static_vector<double,4> tmp = data.generate_2d_inverse_transformation_matrix() * mousePos;
  mouseValPos[0] = tmp[0];
  mouseValPos[1] = tmp[1];

  // math::static_vector<int, 2> indices =
  //   get_axes_choice_indices(get_axes_choice_from_string(
  //     std::string(axesChoice->GetString(axesChoice->GetSelection()))));

  data.axesVariable[0] = 0;
  data.axesVariable[1] = 1;

  /*Set up initial condition*/
  math::vector<double> vec(4);
  for(int i = 0; i != initVals.size(); ++i) {
    if(i == data.axesVariable[0])
      vec[i] = mouseValPos[0];
    else if(i == data.axesVariable[1])
      vec[i] = mouseValPos[1];
    else
      vec[i] = initVals[i];
  }

  /*Retrieve tMax, tInc, and tMin*/
  double tMax = std::stod(std::string(tMaxValField->GetValue().mb_str()));
  double tMin = std::stod(std::string(tMinValField->GetValue().mb_str()));
  double tInc = std::stod(std::string(tIncrementField->GetValue().mb_str()));

  /*Construct a solution using the appropriate method*/
  solution sol;
  sol.initVals = vec;
  sol.points.push_back(vec);
  double tolerance = 0.1;
  while(vec[0] < tMax) {
    math::euler(vec, functions, tInc);
    if((sol.points.back() - vec).norm() > tolerance)
      sol.points.push_back(vec);
  }

  vec = *sol.points.begin();
  while(vec[0] > tMin) {
    math::euler(vec, functions, -tInc);
    if((sol.points.front() - vec).norm() > tolerance)
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

  if(evt.Dragging() && evt.RightIsDown()) {
    data.viewportCenterX = initViewportCenter[0] +
    data.viewportSpanX*(static_cast<double>( initMousePos[0] - evt.GetPosition().x))
    /static_cast<double>(canvasSize[0]);
    data.viewportCenterY = initViewportCenter[1] +
    data.viewportSpanY*(static_cast<double>(evt.GetPosition().y - initMousePos[1]))
    /static_cast<double>(canvasSize[1]);
    dynamicalPlane->Refresh();
  }
}

void top_frame::on_right_down_dynamical_plane(wxMouseEvent& evt) {
  initMousePos[0] = evt.GetPosition().x;
  initMousePos[1] = evt.GetPosition().y;
  initViewportCenter[0] = data.viewportCenterX;
  initViewportCenter[1] = data.viewportCenterY;
}

void top_frame::on_text_enter_dimension(wxCommandEvent& evt) {

  int dimension = std::stoi(std::string(dimensionField->GetValue().mb_str()));
  if(dimension < 1) return;

  /*Clear the functionTextCtrlList and the initialValueTextCtrlList*/
  functionListCtrl->DeleteAllItems();
  functionTextCtrlList.clear();

  initialValueListCtrl->DeleteAllItems();
  initialValueTextCtrlList.clear();

  wxListItem item;
  item.SetId(0);
  initialValueListCtrl->InsertItem(item);

  initialValueListCtrl->SetItem(0,0, wxT("A"));
  initialValueListCtrl->SetItem(0,1, wxT("B"));
  initialValueTextCtrlList.push_back(tInitialValueField);

}


void top_frame::on_size_dynamical_plane( wxSizeEvent& event ) {

}

void top_frame::on_size_top_frame( wxSizeEvent& event ) {

}


void top_frame::on_menu_selection_vector_field(wxCommandEvent&) {

}

void top_frame::on_key_down_dynamical_plane( wxKeyEvent& evt) {
  //std::cout << "Down: " << evt.GetKeyCode() << std::endl;
  /*The x and y increment is the distance represented by one pixel of the dynamicalPlane canvas*/
  /*It is data.viewportSpanX/canvasSize.x*/
  math::static_vector<int, 2> canvasSize;
  dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  double xInc = 4*data.viewportSpanX/static_cast<double>(canvasSize[0]);
  double yInc = 4*data.viewportSpanY/static_cast<double>(canvasSize[1]);
  if(evt.GetKeyCode() == WXK_LEFT) {
    data.viewportCenterX -= xInc;
    dynamicalPlane->Refresh();
  } else if(evt.GetKeyCode() == WXK_RIGHT) {
    data.viewportCenterX += xInc;
    dynamicalPlane->Refresh();
  } else if(evt.GetKeyCode() == WXK_UP) {
    data.viewportCenterY += yInc;
    dynamicalPlane->Refresh();
  } else if(evt.GetKeyCode() == WXK_DOWN) {
    data.viewportCenterY -= yInc;
    dynamicalPlane->Refresh();
  }
}

void top_frame::on_key_up_dynamical_plane( wxKeyEvent& evt) {
  //std::cout << "Up: " << evt.GetKeyCode() << std::endl;
}

void top_frame::on_axis_choice(wxCommandEvent&) {

}
