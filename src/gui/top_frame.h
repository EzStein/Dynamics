#include "generated.h"
#include "compiler/front/driver.h"
#include "math/static_vector.h"
#include "math/vector.h"
#include <wx/glcanvas.h>
#include <list>
#include <algorithm>
#include <string>
#include <set>

/*The top level frame that inherets from the generated class top_frame_base*/
class top_frame : public top_frame_base {
public:
  top_frame(wxWindow*, wxWindowID);
  ~top_frame();
private:
  wxGLCanvas* glPanel;
  wxGLContext* glContext;
  driver dr;
  typedef std::list<std::vector<double> > solution_t;

  /*A list of solutions to draw*/
  std::vector<solution_t> solutions;

  /*A list to draw*/
  std::vector<solution_t> isoclines;

  /*An array of the initValues*/
  std::vector<double> initVals;
  /*A list of the functions. There should be one fewer functions than initVals*/
  std::vector<driver::double_func_t> functions;

  /*Info for converting coordinate systems*/
  math::static_vector<double, 2> valueBoundaryTopLeft;
  /*Measures the number of pixels per unit
  in the x and y directions*/
  math::static_vector<double, 2> pixelToValueRatio;
  double tMax = 20, tMin = -20;
  double tInc = 0.01;

  GLuint sqrVAO, triVAO;

  void initialize_gl();
  void set_nullclines();
  virtual void on_paint_dynamical_plane(wxPaintEvent&) override;
  virtual void on_button_click_compile(wxCommandEvent&) override;
  virtual void on_size_top_frame( wxSizeEvent& event ) override;
  virtual void on_choice_dimension(wxCommandEvent&) override;
  virtual void on_choice_axes(wxCommandEvent&) override;
  virtual void on_menu_selection_vector_field(wxCommandEvent&) override;
  virtual void on_left_down_dynamical_plane(wxMouseEvent&) override;
  virtual void on_motion_dynamical_plane(wxMouseEvent&) override;
  virtual void on_size_dynamical_plane( wxSizeEvent& event ) override;
  void on_paint_gl_renderer(wxPaintEvent&);
  void update_vals();
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

math::static_vector<int, 2> get_axes_choice_indices(axes_choice axesChoice);
axes_choice get_axes_choice_from_string(std::string str);
