#ifndef APP_GUARD
#define APP_GUARD

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "app_state.h"
#include "../gui/top_frame.h"
#include <mutex>
#include <thread>

class top_frame;
class app_state;
/*This class inherits from the underlying wxApp class that implements the application
* the virtual OnInit function is overriden and called by the wx framework
* This class also implements the controller aspect of the MVC pattern
*/
class app : public wxApp {
  public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
  private:
    void thread_compute_region(const vector_2d<int>&, const vector_2d<int>&, const vector_2d<int>&, unsigned long *) const;
    void compute_image() const;

    /*This fills the given array with "rough data." That is, it skips over most imformation
    using the roughness factor and the previousRoughness factor*/
    void calculate_rough_image(const vector_2d<int>&, const vector_2d<int>&,const vector_2d<int>&, unsigned long *, int, int) const;

    /*Uses the data array to render that imformation to the screen continuously*/
    void render_thread(const vector_2d<int>& size, unsigned long * data) const;

    void calculate_boundary(const vector_2d<int>& size, unsigned long * data) const;

    /*Data will have a valid bit as its most significant bit. If it is 0, it is valid. If it is 1, it is invalid*/
    bool is_boundary_point(const vector_2d<int>& point, const vector_2d<int>& size, unsigned long * data, unsigned char * metaData) const;

    void traverse_boundary(const vector_2d<int>& point, const vector_2d<int>& previousPointChange,
      const vector_2d<int>& size, unsigned long * data, unsigned char * metaData) const;

    /*The top level frame*/
    top_frame * frame;

    /*A pointer to the application's state*/
    app_state * state;

  public:
    void render_panel_on_mouse_motion(wxMouseEvent&) const;

    void render_panel_on_mouse_left_up(wxMouseEvent&) const;

    void render_panel_on_mouse_left_down(wxMouseEvent&) const;




};

#endif
