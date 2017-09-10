#include "generated.h"
#include "compiler/front/driver.h"
#include "math/vector.h"
#include <list>

/*The top level frame that inherets from the generated class top_frame_base*/
class top_frame : public top_frame_base {
public:
  top_frame(wxWindow*, wxWindowID);
private:
  driver dr;
  typedef std::list<math::vector<double, 4> > solution_t;
  std::vector<solution_t> solutions;
  math::vector<double, 2> valueBoundaryTopLeft, valueBoundaryBottomRight;
  double tMax, tMin, xInit, yInit, zInit, tInit;
  driver::var4_double_func_t xFunc, yFunc, zFunc;

  virtual void on_paint_dynamical_plane(wxPaintEvent&) override;

  virtual void on_button_click_compile(wxCommandEvent&) override;

  virtual void on_choice_dimension(wxCommandEvent&) override;
  virtual void on_choice_axes(wxCommandEvent&) override;
  virtual void on_menu_selection_vector_field(wxCommandEvent&) override;
  virtual void on_left_down_dynamical_plane(wxMouseEvent&) override;
  virtual void on_motion_dynamical_plane(wxMouseEvent&) override;
};
