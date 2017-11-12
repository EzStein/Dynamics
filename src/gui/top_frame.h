#include "glad/glad.h"
#include "generated.h"
#include "compiler/front/driver.h"
#include "math/static_vector.h"
#include "math/vector.h"
#include <wx/glcanvas.h>
#include <list>
#include <algorithm>
#include <string>
#include <set>
#include "gui/render_data.h"
#include "gl_program.h"

/*The top level frame that inherets from the generated class top_frame_base*/
class top_frame : public top_frame_base {
public:
  top_frame(wxWindow*, wxWindowID);
  ~top_frame();
private:
  wxGLCanvas* dynamicalPlane;
  wxGLContext* glContext;
  driver dr;
  gl_program program2d, program3d;

  render_data data;

  /*An array of the initValues*/
  std::vector<double> initVals;
  /*A list of the functions. There should be one fewer functions than initVals*/
  std::vector<driver::double_func_t> functions;

  double tMax = 20, tMin = -20;
  double tInc = 0.01;

  GLuint vao, vbo;

  void initialize_gl();
  void set_nullclines();
  virtual void on_paint_dynamical_plane(wxPaintEvent&);
  virtual void on_left_down_dynamical_plane(wxMouseEvent&);
  virtual void on_motion_dynamical_plane(wxMouseEvent&);
  virtual void on_size_dynamical_plane( wxSizeEvent& );

  virtual void on_button_click_compile(wxCommandEvent&) override;
  virtual void on_size_top_frame( wxSizeEvent& event ) override;
  virtual void on_choice_dimension(wxCommandEvent&) override;
  virtual void on_choice_axes(wxCommandEvent&) override;
  virtual void on_menu_selection_vector_field(wxCommandEvent&) override;

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
