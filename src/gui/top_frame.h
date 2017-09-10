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
  std::vector<std::list<math::vector<double, 4> > > solutions;
  math::vector<double, 2> 

  virtual void on_paint_dynamical_plane(wxPaintEvent&) override;

  virtual void on_button_click_compile(wxCommandEvent&) override;

  virtual void on_choice_dimension(wxCommandEvent&) override;
  virtual void on_choice_axes(wxCommandEvent&) override;
  virtual void on_menu_selection_vector_field(wxCommandEvent&) override;
};
