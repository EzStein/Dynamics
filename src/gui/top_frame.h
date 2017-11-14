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

  /*A list of the functions. There should be one fewer functions than initVals*/
  std::vector<driver::double_func_t> functions;

  /*These values are updated at each mouse down event*/
  math::static_vector<int, 2> initMousePos;
  math::static_vector<int, 2> initViewportCenter;

  math::static_vector<float, 3> initCameraDirection;

  math::static_vector<double, 2> initZoomViewportCenter;
  math::static_vector<double, 2> initZoomCenter;
  math::static_vector<double, 2> initViewportSpan;
  double totalMag;

  GLuint vao2d, vao3d, vbo,
  axes3dVbo, axes3dVao;

  void initialize_gl();
  void set_nullclines();
  void on_paint_dynamical_plane(wxPaintEvent&);
  void on_left_down_dynamical_plane(wxMouseEvent&);
  void on_right_down_dynamical_plane(wxMouseEvent&);
  void on_motion_dynamical_plane(wxMouseEvent&);
  void on_size_dynamical_plane( wxSizeEvent& );
  void on_key_down_dynamical_plane( wxKeyEvent& );
  void on_key_up_dynamical_plane( wxKeyEvent& );
  void on_mouse_wheel_dynamical_plane(wxMouseEvent&);
  void on_magnify_dynamical_plane(wxMouseEvent&);
  math::static_vector<double, 2> get_real_coordinates(int x, int y);

  virtual void on_button_click_compile(wxCommandEvent&) override;
  virtual void on_size_top_frame( wxSizeEvent& event ) override;
  virtual void on_menu_selection_vector_field(wxCommandEvent&) override;
  virtual void on_axis_choice(wxCommandEvent&) override;
  virtual void on_text_enter_dimension(wxCommandEvent& evt) override;
  virtual void on_3d_render_check(wxCommandEvent& evt) override;
  virtual void on_text_enter_parameters(wxCommandEvent& evt) override;
  virtual void on_parameter_axis_choice(wxCommandEvent& evt) override;
};
