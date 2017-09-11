#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include "top_frame.h"
#include "compiler/front/driver.h"
#include "math/vector.h"
#include "math/util.h"
#include <string>
#include <list>
#include <iostream>

top_frame::top_frame(wxWindow* window, wxWindowID id) : top_frame_base(window, id) {
  valueBoundaryTopLeft[0] = -5;
  valueBoundaryTopLeft[1] = 5;
  pixelToValueRatio[0] = 70;
  pixelToValueRatio[1] = 70;
  dynamicalPlane->Refresh();
}

void top_frame::on_button_click_compile(wxCommandEvent& event) {
  std::string xFuncString = std::string(xFuncField->GetValue().mb_str());
  std::string yFuncString = std::string(yFuncField->GetValue().mb_str());
  std::string zFuncString = std::string(zFuncField->GetValue().mb_str());
  if(xFuncString.empty() || yFuncString.empty() || zFuncString.empty())
    return;
  double tMax = std::stod(std::string(tMaxValField->GetValue()));
  double tMin = std::stod(std::string(tMinValField->GetValue()));
  initVals[1] = std::stod(std::string(xInitValField->GetValue()));
  initVals[2] = std::stod(std::string(yInitValField->GetValue()));
  initVals[3] = std::stod(std::string(zInitValField->GetValue()));
  initVals[0] = std::stod(std::string(tInitValField->GetValue()));

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
  double inc = 0.01;
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
  points.push_back(vec);
  while(vec[0] < tMax) {
    std::cout << vec << std::endl;
    vec = math::euler(vec, xFunc, yFunc, zFunc, inc);
    points.push_back(vec);
  }
  vec = *points.begin();
  while(vec[0] > tMin) {
    std::cout << vec << std::endl;
    vec = math::euler(vec, xFunc, yFunc, zFunc, -inc);
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
  for(solution = solutions.begin(); solution != solutions.end(); ++solution) {
    if(solution->size() < 2) continue; //There are not enough points in the solution

    //We iterate over the points of the solution which are ordered by time
    std::list<math::vector<double, 4> >::const_iterator iter;
    math::vector<double, 4> vec1 = *solution->begin(), vec2;
    for(iter = solution->begin()++; iter != solution->end(); ++iter) {
      vec2 = *iter;

      /*If the points are off screen, we skip them.
      There are probably ways to make this more efficient*/
      /*if(vec1[0] < valueBoundaryTopLeft[0] && vec1[0] > valueBoundaryBottomRight[0]
      && vec1[1] > valueBoundaryTopLeft[1] && vec1[1] < valueBoundaryBottomRight[1]
      && vec2[0] < valueBoundaryTopLeft[0] && vec2[0] > valueBoundaryBottomRight[0]
      && vec2[1] > valueBoundaryTopLeft[1] && vec2[1] < valueBoundaryBottomRight[1]) {
        vec1 = vec2;
        continue;
      }*/
      math::vector<double,2> tmp2;
      tmp2[0] = vec1[indices[0]];
      tmp2[1] = vec1[indices[1]];
      math::vector<int, 2> pixel1 = math::value_to_pixel(tmp2,
        valueBoundaryTopLeft, pixelToValueRatio);

      tmp2[0] = vec2[indices[0]];
      tmp2[1] = vec2[indices[1]];
      math::vector<int, 2> pixel2 = math::value_to_pixel(tmp2,
        valueBoundaryTopLeft, pixelToValueRatio);

      dc.DrawLine(pixel1[0], pixel1[1], pixel2[0], pixel2[1]);
      vec1 = vec2;
    }
  }
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

void top_frame::on_menu_selection_vector_field(wxCommandEvent&) {

}
