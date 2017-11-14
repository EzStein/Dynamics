
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
  dynamicalPlane = new wxGLCanvas(m_notebook2, wxID_ANY);
  m_notebook2->AddPage( dynamicalPlane, wxT("Dynamical Plane"), false );


  dynamicalPlane->Connect( wxEVT_PAINT, wxPaintEventHandler(top_frame::on_paint_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler(top_frame::on_left_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler(top_frame::on_right_down_dynamical_plane ), NULL, this);

  dynamicalPlane->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler(top_frame::on_mouse_wheel_dynamical_plane ), NULL, this);

  dynamicalPlane->Connect( wxEVT_MOTION, wxMouseEventHandler(top_frame::on_motion_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_SIZE, wxSizeEventHandler(top_frame::on_size_dynamical_plane ), NULL, this);

  dynamicalPlane->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler(top_frame::on_key_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Connect( wxEVT_KEY_UP, wxKeyEventHandler(top_frame::on_key_up_dynamical_plane ), NULL, this);

  data.viewportCenterX = 0;
  data.viewportCenterY = 0;
  data.viewportSpanX = 10;
  data.viewportSpanY = 10;

  functionsListCtrl->AppendTextColumn("Variable");
  functionsListCtrl->AppendTextColumn("Equation", wxDATAVIEW_CELL_EDITABLE);
  wxVector<wxVariant> data;
  data.push_back( wxVariant("x' = ") );
  data.push_back( wxVariant("a(y-x)") );
  functionsListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("y' = ") );
  data.push_back( wxVariant("x(b-z)-y") );
  functionsListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("z' = ") );
  data.push_back( wxVariant("x*y - c*z") );
  functionsListCtrl->AppendItem( data );
  data.clear();

  initialValuesListCtrl->AppendTextColumn("Variable");
  initialValuesListCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);

  data.push_back( wxVariant("t = ") );
  data.push_back( wxVariant("0") );
  initialValuesListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("x = ") );
  data.push_back( wxVariant("0") );
  initialValuesListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("y = ") );
  data.push_back( wxVariant("0") );
  initialValuesListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("z = ") );
  data.push_back( wxVariant("0") );
  initialValuesListCtrl->AppendItem( data );
  data.clear();

  parametersListCtrl->AppendTextColumn("Parameter");
  parametersListCtrl->AppendTextColumn("Value", wxDATAVIEW_CELL_EDITABLE);

  data.push_back( wxVariant("a = ") );
  data.push_back( wxVariant("0") );
  parametersListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("b = ") );
  data.push_back( wxVariant("0") );
  parametersListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("c = ") );
  data.push_back( wxVariant("0") );
  parametersListCtrl->AppendItem( data );
  data.clear();
  /*data.push_back( wxVariant("x2' = ") );
  data.push_back( wxVariant("") );
  functionsListCtrl->AppendItem( data );
  data.clear();
  data.push_back( wxVariant("x3' = ") );
  data.push_back( wxVariant("") );
  functionsListCtrl->AppendItem( data );
  data.clear();*/
  std::cout << "Initialization Done"<< std::endl;
}



void top_frame::initialize_gl() {
  wxGLContextAttrs contextAttrs;
  contextAttrs.CoreProfile().OGLVersion(4,5).EndList();
  wxGLCanvas* tmp = new wxGLCanvas(this, wxID_ANY);
  glContext = new wxGLContext(tmp, NULL, &contextAttrs);
  delete tmp;

  dynamicalPlane->SetCurrent(*glContext);
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

  glGenVertexArrays(1, &vao2d);
  glBindVertexArray(vao2d);
  glVertexAttribPointer(program2d.get_attribute_location("pos"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(program2d.get_attribute_location("pos"));
  glBindVertexArray(0);

  glGenVertexArrays(1, &vao3d);
  glBindVertexArray(vao3d);
  glVertexAttribPointer(program3d.get_attribute_location("pos"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(program3d.get_attribute_location("pos"));
  glBindVertexArray(0);

  glGenBuffers(1, &axes3dVbo);
  glBindBuffer(GL_ARRAY_BUFFER, axes3dVbo);
  float axes[6*3] = {
    0.0,0.0,0.0, 100, 0, 0,
    0,0,0, 0,100,0,
    0,0,0, 0,0,100};
  glBufferData(GL_ARRAY_BUFFER,
    6*3 * sizeof(float), axes, GL_STATIC_DRAW);

  glGenVertexArrays(1, &axes3dVao);
  glBindVertexArray(axes3dVao);
  glVertexAttribPointer(program3d.get_attribute_location("pos"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(program3d.get_attribute_location("pos"));
  glBindVertexArray(0);
}

top_frame::~top_frame() {

  dynamicalPlane->Disconnect( wxEVT_PAINT, wxPaintEventHandler(top_frame::on_paint_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler(top_frame::on_left_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler(top_frame::on_mouse_wheel_dynamical_plane ), NULL, this);

  dynamicalPlane->Disconnect( wxEVT_MOTION, wxMouseEventHandler(top_frame::on_motion_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler(top_frame::on_right_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_SIZE, wxSizeEventHandler(top_frame::on_size_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler(top_frame::on_key_down_dynamical_plane ), NULL, this);
  dynamicalPlane->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler(top_frame::on_key_up_dynamical_plane ), NULL, this);
}

void top_frame::on_mouse_wheel_dynamical_plane(wxMouseEvent& evt) {
  const double magnificationUnit = std::pow(1.0001, evt.GetWheelRotation());
  totalMag *= magnificationUnit;
  data.viewportSpanX = totalMag * initViewportSpan[0];
  data.viewportSpanY = totalMag * initViewportSpan[1];
  data.viewportCenterX = totalMag * (initZoomViewportCenter[0] - initZoomCenter[0]) + initZoomCenter[0];
  data.viewportCenterY = totalMag * (initZoomViewportCenter[1] - initZoomCenter[1]) + initZoomCenter[1];
  dynamicalPlane->Refresh();
}

/*
* Converts the 2d screen coordinates to real values.
* Used when in 2d mode
*/
math::static_vector<double, 2> top_frame::get_real_coordinates(int x, int y) {
  math::static_vector<int, 2> canvasSize;
  dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  /*we set the mouse position to be a standard vec of size 4 whose last element
  is 1 (this allows for translations)*/
  /*Compute the mouse position value for 2d*/
  math::static_vector<double, 4> mousePos;
  mousePos[0] = 2*static_cast<double>(x)/static_cast<double>(canvasSize[0]) - 1;
  mousePos[1] = 1 - 2*static_cast<double>(y)/static_cast<double>(canvasSize[1]);
  mousePos[2] = 0;
  mousePos[3] = 1;
  math::static_vector<double, 2> mouseValPos;
  math::static_vector<double,4> tmp = data.generate_2d_inverse_transformation_matrix() * mousePos;
  mouseValPos[0] = tmp[0];
  mouseValPos[1] = tmp[1];
  return mouseValPos;
}

void top_frame::on_paint_dynamical_plane(wxPaintEvent& evt) {
  static int a = 0;

  if(!a) {
    initialize_gl();
    a = 1;
    std::cout << "INITIALIZED" << std::endl;
  }

  dynamicalPlane->SetCurrent(*glContext);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if(data.redraw) {
    /*We first need to calculate the total number of vertices in the solution set
    We do this by iterating over the solutions and suming their sizes*/
    int totalFloats = 0;
    for(solution sol : data.solutions) {
      totalFloats += sol.points.size();
    }

    /*In 2d mode we have 2 floats per vertex, otherwise we have 3*/
    totalFloats *= data.render2d?2:3;

    if(totalFloats == 0) {
      /*In this event, there are no solutions given,
      so we don't draw anything*/
      return;
    }

    float* vertexData = new float[totalFloats];

    /*We now fill in the data accordingly*/
    if(data.render2d) {
      int index = -1;
      for(solution sol : data.solutions) {
        for(math::vector<double> vec : sol.points) {
          vertexData[++index] = static_cast<float>(vec[data.axesVariable[0]]);
          vertexData[++index] = static_cast<float>(vec[data.axesVariable[1]]);
        }
      }
    } else {
      int index = -1;
      for(solution sol : data.solutions) {
        for(math::vector<double> vec : sol.points) {
          vertexData[++index] = static_cast<float>(vec[data.axesVariable[0]]);
          vertexData[++index] = static_cast<float>(vec[data.axesVariable[1]]);
          vertexData[++index] = static_cast<float>(vec[data.axesVariable[2]]);
        }
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

  if(data.render2d) {
    program2d.bind();
    glBindVertexArray(vao2d);
    math::static_matrix<float, 4,4> transformationMatrix(data.generate_2d_transformation_matrix());
    program2d.set_float_mat4_uniform("transformation", true, transformationMatrix.data());
  } else {

    program3d.bind();

    math::static_matrix<float, 4,4> model(data.generate_model_matrix());
    math::static_matrix<float, 4,4> view(data.generate_view_matrix());
    math::static_matrix<float, 4,4> projection(data.generate_projection_matrix());

    program3d.set_float_mat4_uniform("model", true, model.data());
    program3d.set_float_mat4_uniform("view", true, view.data());
    program3d.set_float_mat4_uniform("projection", true, projection.data());



    glBindVertexArray(axes3dVao);
    float col[4] = {1,0,0,1};
    program3d.set_float_vec4_uniform("color", col);
    glDrawArrays(GL_LINES, 0, 2);
    col[0] = 0;
    col[1] = 1;
    program3d.set_float_vec4_uniform("color", col);
    glDrawArrays(GL_LINES, 2, 2);
    col[1] = 0;
    col[2] = 1;
    program3d.set_float_vec4_uniform("color", col);
    glDrawArrays(GL_LINES, 4, 2);

    col[0] = 0.5;
    col[1] = 0.5;
    col[2] = 0.5;
    program3d.set_float_vec4_uniform("color", col);
    glBindVertexArray(vao3d);
  }

  int offset = 0;
  for(solution sol : data.solutions) {
    glDrawArrays(GL_LINE_STRIP,offset,sol.points.size());
    offset += sol.points.size();
  }
  glBindVertexArray(0);
  dynamicalPlane->SwapBuffers();
}

void top_frame::on_3d_render_check(wxCommandEvent& evt) {
  data.cameraPosition[0] = 20;
  data.cameraPosition[1] = 20;
  data.cameraPosition[2] = 20;
  data.cameraDirection[0] = 20;
  data.cameraDirection[1] = 20;
  data.cameraDirection[2] = 20;
  data.redraw = true;

  data.render2d = !evt.IsChecked();
  dynamicalPlane->Refresh();
}

void top_frame::on_button_click_compile(wxCommandEvent& event) {
  // for(wxTextCtrl* funcField : functionTextCtrlList) {
  //   /*If a single field is empty, we do nothing*/
  //   if(funcField->IsEmpty()) return;
  // }
  /*Otherwise, release the space allocated by previous compilations*/
  for(driver::double_func_t func : functions) {
    dr.mark_available(func);
  }
  /*And reset the functions*/
  functions.clear();

  /*for(wxTextCtrl* funcField : functionTextCtrlList) {
    std::string funcString = std::string(funcField->GetValue().mb_str());

    functions.push_back(
      dr.compile_as_function<driver::double_func_t>(funcString)
    );
  }*/

  functions.push_back(
    dr.compile_as_function<driver::double_func_t>("10(y-x)")
  );
  functions.push_back(
    dr.compile_as_function<driver::double_func_t>("28x-xz-y")
  );
  functions.push_back(
    dr.compile_as_function<driver::double_func_t>("xy-8z/3")
  );

  /*Clear all the solutions*/
  data.solutions = std::vector<solution>();
  data.redraw = true;
  dynamicalPlane->Refresh();
}

void top_frame::on_left_down_dynamical_plane(wxMouseEvent& evt) {
  /*Set the initial mouse positions*/
  dynamicalPlane->SetFocus();
  initMousePos[0] = evt.GetPosition().x;
  initMousePos[1] = evt.GetPosition().y;

  /*If there are no functions, which happens at the begging, then we do nothgin*/
  if(functions.empty()) return;


  std::vector<double> initVals;
  /*for(wxTextCtrl* initValField : initialValueTextCtrlList) {
    std::string strVal(std::string(initValField->GetValue().mb_str()));
    double val;
    try {
      val = std::stod(strVal);
    } catch (const std::invalid_argument& arg) {
      std::cout << "Initival value string, '" << strVal << "' is not a number!" << std::endl;
      return;
    }
    initVals.push_back(val);
  }*/
  initVals.push_back(0);
  initVals.push_back(0);
  initVals.push_back(0);
  initVals.push_back(0);

  data.redraw = true;


  math::static_vector<double, 2> mouseValPos(get_real_coordinates(evt.GetPosition().x, evt.GetPosition().y));


  // math::static_vector<int, 2> indices =
  //   get_axes_choice_indices(get_axes_choice_from_string(
  //     std::string(axesChoice->GetString(axesChoice->GetSelection()))));

  /*We now set the axis variables*/
  data.axesVariable[0] = axisVariableChoice1->GetSelection();
  data.axesVariable[1] = axisVariableChoice2->GetSelection();
  data.axesVariable[2] = axisVariableChoice3->GetSelection();
  if(data.axesVariable[0] == wxNOT_FOUND || data.axesVariable[1] == wxNOT_FOUND || data.axesVariable[2] == wxNOT_FOUND) {
    std::cout << "No variable selected for one or more axes" << std::endl;
    return;
  }

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
  if(data.render2d) {
    if(evt.GetWheelRotation() == 0) {
      initZoomViewportCenter[0] = data.viewportCenterX;
      initZoomViewportCenter[1] = data.viewportCenterY;
      initViewportSpan[0] = data.viewportSpanX;
      initViewportSpan[1] = data.viewportSpanY;
      totalMag = 1;
      initZoomCenter = get_real_coordinates(evt.GetPosition().x, evt.GetPosition().y);
    }

    math::static_vector<int, 2> canvasSize;
    dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);

    math::static_vector<double, 2> mouseValPos(get_real_coordinates(evt.GetPosition().x, evt.GetPosition().y));

    statusBar->SetStatusText("(" + std::to_string(mouseValPos[0]) + ", " + std::to_string(mouseValPos[1]) + ")");

    if(evt.Dragging() && evt.RightIsDown()) {
      data.viewportCenterX = initViewportCenter[0] +
      data.viewportSpanX*(static_cast<double>( initMousePos[0] - evt.GetPosition().x))
      /static_cast<double>(canvasSize[0]);
      data.viewportCenterY = initViewportCenter[1] +
      data.viewportSpanY*(static_cast<double>(evt.GetPosition().y - initMousePos[1]))
      /static_cast<double>(canvasSize[1]);
      dynamicalPlane->Refresh();
    }
  } else if (evt.Dragging() && evt.RightIsDown()) {
    math::static_matrix<float,4,4> inverse(math::invert(data.generate_projection_matrix()));

    math::static_vector<int,2> mousePos;
    mousePos[0] = evt.GetPosition().x;
    mousePos[1] = evt.GetPosition().y;

    math::static_vector<float,4> initVec;
    initVec[0] = initMousePos[0];
    initVec[1]= initMousePos[1];
    initVec[2]= 0;
    initVec[3]= 1;
    initVec = inverse * initVec;

    math::static_vector<float,4> finalVec;
    finalVec[0] = mousePos[0];
    finalVec[1]= mousePos[1];
    finalVec[2]= 0;
    finalVec[3]= 1;
    finalVec = inverse * finalVec;

    math::static_vector<float, 3> tmpInitVec, tmpFinalVec;
    tmpInitVec[0] = initVec[0];
    tmpInitVec[1] = initVec[1];
    tmpInitVec[2] = initVec[2];

    tmpFinalVec[0] = finalVec[0];
    tmpFinalVec[1] = finalVec[1];
    tmpFinalVec[2] = finalVec[3];

    math::static_vector<float, 3> tmpQuaternian(math::cross(tmpInitVec - tmpFinalVec, initCameraDirection));
    math::static_vector<float, 4> quaternian;
    quaternian[0] = 0;//tmpQuaternian[0];
    quaternian[1] = 1;//tmpQuaternian[1];
    quaternian[2] = 0;//tmpQuaternian[2];
    quaternian[3] = 0.01 * (initMousePos - mousePos).norm();

    //std::cout << data.cameraDirection << std::endl;
    math::static_matrix<float, 3,3> rot = math::quaternian_to_matrix(quaternian);
    data.cameraDirection = -(rot * initCameraDirection);
    dynamicalPlane->Refresh();
  }
}

void top_frame::on_right_down_dynamical_plane(wxMouseEvent& evt) {
  initMousePos[0] = evt.GetPosition().x;
  initMousePos[1] = evt.GetPosition().y;
  initViewportCenter[0] = data.viewportCenterX;
  initViewportCenter[1] = data.viewportCenterY;
  initCameraDirection = data.cameraDirection;
  std::cout << "A"<< std::endl;

}

void top_frame::on_text_enter_parameters(wxCommandEvent& evt) {
  int params = std::stoi(std::string(parametersField->GetValue().mb_str()));
  if(params < 0) return;
  data.parameters = params;
  parameterAxisVariableChoice1->Clear();
  parameterAxisVariableChoice2->Clear();

  parametersListCtrl->DeleteAllItems();
  wxVector<wxVariant> row;

  if(data.parameters == 0) {
    return;
  } else if(data.parameters == 1) {
    parameterAxisVariableChoice1->Append("a");
    parameterAxisVariableChoice2->Append("a");

    row.push_back(wxVariant("a = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();

    parameterAxisVariableChoice1->SetSelection(0);
    parameterAxisVariableChoice2->SetSelection(0);
  } else if(data.parameters == 2) {
    parameterAxisVariableChoice1->Append("a");
    parameterAxisVariableChoice2->Append("a");
    parameterAxisVariableChoice1->Append("b");
    parameterAxisVariableChoice2->Append("b");

    row.push_back(wxVariant("a = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();
    row.push_back(wxVariant("b = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();

    parameterAxisVariableChoice1->SetSelection(0);
    parameterAxisVariableChoice2->SetSelection(1);
  } else if(data.parameters == 3) {
    parameterAxisVariableChoice1->Append("a");
    parameterAxisVariableChoice2->Append("a");
    parameterAxisVariableChoice1->Append("b");
    parameterAxisVariableChoice2->Append("b");
    parameterAxisVariableChoice1->Append("c");
    parameterAxisVariableChoice2->Append("c");

    row.push_back(wxVariant("a = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();
    row.push_back(wxVariant("b = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();
    row.push_back(wxVariant("c = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();

    parameterAxisVariableChoice1->SetSelection(0);
    parameterAxisVariableChoice2->SetSelection(1);
  } else if(data.parameters == 4) {
    parameterAxisVariableChoice1->Append("a");
    parameterAxisVariableChoice2->Append("a");
    parameterAxisVariableChoice1->Append("b");
    parameterAxisVariableChoice2->Append("b");
    parameterAxisVariableChoice1->Append("c");
    parameterAxisVariableChoice2->Append("c");
    parameterAxisVariableChoice1->Append("d");
    parameterAxisVariableChoice2->Append("d");

    row.push_back(wxVariant("a = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();
    row.push_back(wxVariant("b = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();
    row.push_back(wxVariant("c = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();
    row.push_back(wxVariant("d = "));
    row.push_back(wxVariant("0"));
    parametersListCtrl->AppendItem(row);
    row.clear();

    parameterAxisVariableChoice1->SetSelection(0);
    parameterAxisVariableChoice2->SetSelection(1);
  } else {
    for(int i = 0; i != data.parameters; ++i) {
      std::string paramName = "a" + std::to_string(i+1);
      parameterAxisVariableChoice1->Append(paramName);
      parameterAxisVariableChoice2->Append(paramName);

      row.push_back(wxVariant(paramName + " = "));
      row.push_back(wxVariant("0"));
      parametersListCtrl->AppendItem(row);
      row.clear();
    }
    parameterAxisVariableChoice1->SetSelection(0);
    parameterAxisVariableChoice2->SetSelection(1);
  }
}

void top_frame::on_parameter_axis_choice(wxCommandEvent& evt) {

}

void top_frame::on_text_enter_dimension(wxCommandEvent& evt) {


  int dim = std::stoi(std::string(dimensionField->GetValue().mb_str()));
  if(dim < 1) return;
  data.dimension = dim;

  axisVariableChoice1->Clear();
  axisVariableChoice2->Clear();
  axisVariableChoice3->Clear();

  axisVariableChoice1->Append("t");
  axisVariableChoice2->Append("t");
  axisVariableChoice3->Append("t");

  initialValuesListCtrl->DeleteAllItems();
  functionsListCtrl->DeleteAllItems();
  wxVector<wxVariant> row;
  row.push_back( wxVariant("t = ") );
  row.push_back( wxVariant("0") );
  initialValuesListCtrl->AppendItem( row );
  row.clear();

  if(data.dimension == 1) {
    axisVariableChoice1->Append("x");
    axisVariableChoice2->Append("x");
    axisVariableChoice3->Append("x");
    axisVariableChoice1->SetSelection(0);
    axisVariableChoice2->SetSelection(1);
    axisVariableChoice3->SetSelection(1);

    row.push_back( wxVariant("x = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();

    row.push_back( wxVariant("x' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
  } else if(data.dimension == 2) {
    axisVariableChoice1->Append("x");
    axisVariableChoice2->Append("x");
    axisVariableChoice3->Append("x");
    axisVariableChoice1->Append("y");
    axisVariableChoice2->Append("y");
    axisVariableChoice3->Append("y");
    axisVariableChoice1->SetSelection(0);
    axisVariableChoice2->SetSelection(1);
    axisVariableChoice3->SetSelection(2);

    row.push_back( wxVariant("x = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("y = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();

    row.push_back( wxVariant("x' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();

    row.push_back( wxVariant("y' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
  } else if(data.dimension == 3) {
    axisVariableChoice1->Append("x");
    axisVariableChoice2->Append("x");
    axisVariableChoice3->Append("x");
    axisVariableChoice1->Append("y");
    axisVariableChoice2->Append("y");
    axisVariableChoice3->Append("y");
    axisVariableChoice1->Append("z");
    axisVariableChoice2->Append("z");
    axisVariableChoice3->Append("z");
    axisVariableChoice1->SetSelection(1);
    axisVariableChoice2->SetSelection(2);
    axisVariableChoice3->SetSelection(3);

    row.push_back( wxVariant("x = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("y = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("z = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();

    row.push_back( wxVariant("x' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("y' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("z' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
  } else if(data.dimension == 4) {
    axisVariableChoice1->Append("x");
    axisVariableChoice2->Append("x");
    axisVariableChoice3->Append("x");
    axisVariableChoice1->Append("y");
    axisVariableChoice2->Append("y");
    axisVariableChoice3->Append("y");
    axisVariableChoice1->Append("z");
    axisVariableChoice2->Append("z");
    axisVariableChoice3->Append("z");
    axisVariableChoice1->Append("w");
    axisVariableChoice2->Append("w");
    axisVariableChoice3->Append("w");
    axisVariableChoice1->SetSelection(1);
    axisVariableChoice2->SetSelection(2);
    axisVariableChoice3->SetSelection(3);

    row.push_back( wxVariant("x = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("y = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("z = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("w = ") );
    row.push_back( wxVariant("0") );
    initialValuesListCtrl->AppendItem( row );
    row.clear();

    row.push_back( wxVariant("x' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("y' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("z' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
    row.push_back( wxVariant("w' = ") );
    row.push_back( wxVariant("") );
    functionsListCtrl->AppendItem( row );
    row.clear();
  } else  {
    /*We need to set the values in the axes choice boxes*/
    for(int i = 0; i != data.dimension; ++i) {
      axisVariableChoice1->Append(wxT("x" + std::to_string(i+1)));
      axisVariableChoice2->Append(wxT("x" + std::to_string(i+1)));
      axisVariableChoice3->Append(wxT("x" + std::to_string(i+1)));

      row.push_back( wxVariant("x" + std::to_string(i+1) + " = ") );
      row.push_back( wxVariant("0") );
      initialValuesListCtrl->AppendItem( row );
      row.clear();

      row.push_back( wxVariant("x" + std::to_string(i+1) + "' = ") );
      row.push_back( wxVariant("") );
      functionsListCtrl->AppendItem( row );
      row.clear();
    }
    axisVariableChoice1->SetSelection(1);
    axisVariableChoice2->SetSelection(2);
    axisVariableChoice3->SetSelection(3);
  }
}


void top_frame::on_size_dynamical_plane( wxSizeEvent& event ) {

}

void top_frame::on_size_top_frame( wxSizeEvent& event ) {

}


void top_frame::on_menu_selection_vector_field(wxCommandEvent&) {

}

void top_frame::on_key_down_dynamical_plane( wxKeyEvent& evt) {
  //std::cout << "Down: " << evt.GetKeyCode() << std::endl;
  if(data.render2d) {
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
  } else {
    std::cout << evt.GetUnicodeKey()<< std::endl;
    std::cout << WXK_CONTROL_W << "," << WXK_NONE<< std::endl;
    if(evt.GetUnicodeKey() == 64 + WXK_CONTROL_W) {
      data.cameraPosition[0] -= 2;
    } else if(evt.GetUnicodeKey() == 64 + WXK_CONTROL_S) {
      data.cameraPosition[0] += 2;
    } else if(evt.GetUnicodeKey() == 64 + WXK_CONTROL_A) {
      data.cameraPosition[1] -= 2;
    } else if(evt.GetUnicodeKey() == 64 + WXK_CONTROL_D) {
      std::cout << "D"<< std::endl;
      data.cameraPosition[1] += 2;
    } else if(evt.GetKeyCode() == WXK_SPACE) {
      data.cameraPosition[2] += 2;
    } else if(evt.GetKeyCode() == WXK_SHIFT) {
      data.cameraPosition[2] -= 2;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_W) {
      data.cameraPosition[0] -= 2;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_S) {
      data.cameraPosition[0] += 2;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_A) {
      data.cameraPosition[1] -= 2;
    } else if(evt.GetUnicodeKey() == WXK_CONTROL_D) {
      data.cameraPosition[1] += 2;
    } else if(evt.GetKeyCode() == WXK_SPACE) {
      data.cameraPosition[2] += 2;
    } else if(evt.GetKeyCode() == WXK_SHIFT) {
      data.cameraPosition[2] -= 2;
    }

    dynamicalPlane->Refresh();
  }
}

void top_frame::on_key_up_dynamical_plane( wxKeyEvent& evt) {
  //std::cout << "Up: " << evt.GetKeyCode() << std::endl;
}

void top_frame::on_axis_choice(wxCommandEvent&) {
  std::cout <<"AA" << std::endl;
  data.redraw = true;
  data.axesVariable[0] = axisVariableChoice1->GetSelection();
  data.axesVariable[1] = axisVariableChoice2->GetSelection();
  data.axesVariable[2] = axisVariableChoice3->GetSelection();
  dynamicalPlane->Refresh();
}
