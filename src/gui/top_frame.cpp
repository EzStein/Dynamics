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
  double inc = 0.1;
  std::cout << "Hello" << std::endl;
  std::string str();
  driver::var4_double_func_t xFunc =
    dr.compile_as_function<driver::var4_double_func_t>(std::string(xFuncField->GetValue().mb_str()));
  driver::var4_double_func_t yFunc =
    dr.compile_as_function<driver::var4_double_func_t>(std::string(yFuncField->GetValue().mb_str()));
  driver::var4_double_func_t zFunc =
    dr.compile_as_function<driver::var4_double_func_t>(std::string(zFuncField->GetValue().mb_str()));
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

  solutions.push_back(points);
  dynamicalPlane->Refresh();
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

  std::cout << "!!!" << std::endl;
  wxPaintDC dc(dynamicalPlane);
  int width, height;
  dynamicalPlane->GetSize(&width, &height);
  dc.SetBrush(*wxWHITE_BRUSH);
  dc.DrawRectangle(0,0,width, height);
  dc.SetBrush(*wxRED_BRUSH);

  std::vector<std::list<math::vector<double, 4> > >::const_iterator solution;
  for(solution = solutions.begin(); solution != solutions.end(); ++solution) {
    if(solution->size() < 2) continue;
    std::list<math::vector<double, 4> >::const_iterator iter;
    math::vector<double, 4> vec1 = *solution->begin(), vec2;
    for(iter = solution->begin()++; iter != solution->end(); ++iter) {
      vec2 = *iter;
      vector_2d<int> v1 = math::value_to_pixel(vector_2d<double>(vec1[0], vec1[1]),
        vector_2d<int>(width, height),
        vector_2d<double>(-5, 5), vector_2d<double>(5, -5));

    vector_2d<int> v2 = math::value_to_pixel(vector_2d<double>(vec2[0], vec2[1]),
      vector_2d<int>(width, height),
      vector_2d<double>(-5, 5), vector_2d<double>(5, -5));
      dc.DrawLine(v1.x, v1.y, v2.x, v2.y);

      vec1 = vec2;
    }
  }
}
void top_frame::on_menu_selection_vector_field(wxCommandEvent&) {

}
