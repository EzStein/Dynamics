#ifndef APP_GUARD
#define APP_GUARD

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "app_state.h"
#include "../gui/top_frame.h"

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
    /*The top level frame*/
    top_frame * frame;

    /*A pointer to the application's state*/
    app_state * state;

  public:
    void render_panel_on_mouse_motion(wxMouseEvent&) const;

    void render_panel_on_mouse_left_up(wxMouseEvent&) const;

    void render_panel_on_mouse_left_down(wxMouseEvent&) const;

    void compute_image() const;
};

#endif
