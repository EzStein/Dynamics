#ifndef APP_GUARD
#define APP_GUARD

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif
#include "app_state.h"

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
    wxFrame * frame;

    /*A pointer to the application's state*/
    app_state * state;
};

#endif
