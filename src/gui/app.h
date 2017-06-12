#ifndef APP_GUARD
#define APP_GUARD

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

/*This class inherits from the underlying wxApp class that implements the application
* the virtual OnInit function is overriden and called by the wx framework
*/
class app : public wxApp {
  public:
    virtual bool OnInit();
  private:
    wxFrame * frame;
};

#endif
