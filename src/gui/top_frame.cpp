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

top_frame::top_frame(wxWindow* window, wxWindowID id) : top_frame_base(window, id) { }

/*void top_frame::dynamical_plane_on_paint(wxPaintEvent& event) {
  wxPaintDC dc(dynamicalPlane);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(0,0,80, 80);
}*/

void top_frame::on_button_click_compile(wxCommandEvent& event) {
  double tMax = std::stod(std::string(tMaxValField->GetValue()));
  double tMin = std::stod(std::string(tMinValField->GetValue()));
  double xInit = std::stod(std::string(xInitValField->GetValue()));
  double yInit = std::stod(std::string(yInitValField->GetValue()));
  double zInit = std::stod(std::string(zInitValField->GetValue()));
  double tInit = std::stod(std::string(tInitValField->GetValue()));



  xFunc =
    dr.compile_as_function<driver::var4_double_func_t>(std::string(xFuncField->GetValue().mb_str()));
  yFunc =
    dr.compile_as_function<driver::var4_double_func_t>(std::string(yFuncField->GetValue().mb_str()));
  zFunc =
    dr.compile_as_function<driver::var4_double_func_t>(std::string(zFuncField->GetValue().mb_str()));


  solutions = std::vector<solution_t>();
  dynamicalPlane->Refresh();
}

void top_frame::on_left_down_dynamical_plane(wxMouseEvent& evt) {
  std::cout << "AA" << std::endl;
  /*double inc = 0.1;
  math::vector<double, 4> vec;
  vec[0] = std::stod(std::string(tInitValField->GetValue()));
  vec[1] = std::stod(std::string(xInitValField->GetValue()));
  vec[2] = std::stod(std::string(yInitValField->GetValue()));
  vec[3] = std::stod(std::string(zInitValField->GetValue()));
  std::list<math::vector<double, 4> > points = std::list<math::vector<double, 4> >();
  points.push_back(vec);
  while(vec[0] < tMax) {
    vec = math::euler(vec, xFunc, yFunc, zFunc, inc);
    points.push_back(vec);
  }
  vec = *points.begin();
  while(vec[0] > tMin) {
    vec = math::euler(vec, xFunc, yFunc, zFunc, -inc);
    points.push_front(vec);
    std::cout << vec << std::endl;
  }

  solutions.push_back(points);*/
}

void top_frame::on_motion_dynamical_plane(wxMouseEvent& evt) {

}

void top_frame::on_choice_dimension(wxCommandEvent& evt) {
  int dim = std::stoi(std::string(evt.GetString()));
  if(dim == 1) {

  } else if(dim == 2) {

  } else if(dim == 3) {

  }
}

void top_frame::on_choice_axes(wxCommandEvent& evt) {

}

void top_frame::on_paint_dynamical_plane(wxPaintEvent& evt) {
  wxPaintDC dc(dynamicalPlane);
  math::vector<int, 2> canvasSize;
  dynamicalPlane->GetSize(&canvasSize[0], &canvasSize[1]);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(0,0,canvasSize[0], canvasSize[1]);
  dc.SetBrush(*wxRED_BRUSH);

  math::vector<int, 2> tmp;
  /*Paint the axes*/
  for(int xInc = 20; xInc < canvasSize[0]; xInc+=40) {
    tmp[0] = xInc;
    tmp[1] = 0;
    math::vector<int, 2> val = math::pixel_to_value(tmp,
    canvasSize, valueBoundaryTopLeft, valueBoundaryBottomRight);
    dc.DrawText(std::to_string(val[0]), xInc, canvasSize[1] - 20);
    dc.DrawLine(xInc, canvasSize[1], xInc, canvasSize[1] - 10);
  }
  for(int yInc = 20; yInc < canvasSize[0]; yInc+=40) {
    tmp[0] = 0;
    tmp[1] = yInc;
    math::vector<int, 2> val = math::pixel_to_value(tmp,
    canvasSize, valueBoundaryTopLeft, valueBoundaryBottomRight);
    dc.DrawText(std::to_string(val[1]), 20, yInc);
    dc.DrawLine(0, yInc, 10, yInc);
  }

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
      if(vec1[0] < valueBoundaryTopLeft[0] && vec1[0] > valueBoundaryBottomRight[0]
      && vec1[1] > valueBoundaryTopLeft[1] && vec1[1] < valueBoundaryBottomRight[1]
      && vec2[0] < valueBoundaryTopLeft[0] && vec2[0] > valueBoundaryBottomRight[0]
      && vec2[1] > valueBoundaryTopLeft[1] && vec2[1] < valueBoundaryBottomRight[1]) {
        vec1 = vec2;
        continue;
      }
      math::vector<double,2> tmp;
      tmp[0] = vec1[0];
      tmp[1] = vec1[1];
      math::vector<int, 2> pixel1 = math::value_to_pixel(tmp,
        canvasSize, valueBoundaryTopLeft, valueBoundaryBottomRight);

      tmp[0] = vec2[0];
      tmp[1] = vec2[1];
      math::vector<int, 2> pixel2 = math::value_to_pixel(tmp,
        canvasSize, valueBoundaryTopLeft, valueBoundaryBottomRight);


      dc.DrawLine(pixel1[0], pixel1[1], pixel2[0], pixel2[1]);
      vec1 = vec2;
    }
  }
}

void top_frame::on_menu_selection_vector_field(wxCommandEvent&) {

}
