#ifndef TOP_FRAME_GUARD
#define TOP_FRAME_GUARD


#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include "../app/app_state.h"
#include "render_panel.h"

class app_state;
class render_panel;
/*This class is the top level frame and contains all the children that will be drawn to the screen*/
class top_frame : public wxFrame {
public:
  top_frame(const app_state *);
  render_panel * renderPanel;

private:
  const app_state * state;
  void on_help_menu_click(wxCommandEvent&);
  void on_open_menu_click(wxCommandEvent&);
  void on_close_menu_click(wxCommandEvent&);
  void on_save_menu_click(wxCommandEvent&);
  void on_save_as_menu_click(wxCommandEvent&);
  void on_about_menu_click(wxCommandEvent&);
  void on_preferences_menu_click(wxCommandEvent&);
  void on_quit_menu_click(wxCommandEvent&);
  void on_close(wxCloseEvent&);
};

#endif
