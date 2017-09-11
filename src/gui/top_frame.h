#include "generated.h"
#include "compiler/front/driver.h"
#include "math/vector.h"
#include <list>
#include <algorithm>
#include <string>

/*The top level frame that inherets from the generated class top_frame_base*/
class top_frame : public top_frame_base {
public:
  top_frame(wxWindow*, wxWindowID);
private:
  driver dr;
  typedef std::list<math::vector<double, 4> > solution_t;
  std::vector<solution_t> solutions;
  math::vector<double, 2> valueBoundaryTopLeft;
  /*Measures the number of pixels per unit
  in the x and y directions*/
  math::vector<double, 2> pixelToValueRatio;
  math::vector<double, 4> initVals;
  double tMax = 20, tMin = -20;
  driver::var4_double_func_t xFunc = nullptr, yFunc = nullptr, zFunc = nullptr;

  virtual void on_paint_dynamical_plane(wxPaintEvent&) override;

  virtual void on_button_click_compile(wxCommandEvent&) override;

  virtual void on_choice_dimension(wxCommandEvent&) override;
  virtual void on_choice_axes(wxCommandEvent&) override;
  virtual void on_menu_selection_vector_field(wxCommandEvent&) override;
  virtual void on_left_down_dynamical_plane(wxMouseEvent&) override;
  virtual void on_motion_dynamical_plane(wxMouseEvent&) override;
  virtual void on_size_dynamical_plane( wxSizeEvent& event ) override;
};

/*All permutations are listed
here. The first variable is the the horizontal axis, and the second is the vertical axis*/
enum class axes_choice {
  TX,
  XT,
  TY,
  YT,
  TZ,
  ZT,
  XY,
  YX,
  XZ,
  ZX,
  YZ,
  ZY,
  NONE
};

math::vector<int, 2> get_axes_choice_indices(axes_choice axesChoice);
axes_choice get_axes_choice_from_string(std::string str);
