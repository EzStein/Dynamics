#ifndef APP_GUARD
#define APP_GUARD
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

/*The class from which the gui is initialized and where execution begins*/
class app : public wxApp {
public:
  virtual bool OnInit() override;
  virtual int OnExit() override;
};

#endif
