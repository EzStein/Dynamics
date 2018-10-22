#ifndef DYNSOLVER_DEBUG_APP_H_
#define DYNSOLVER_DEBUG_APP_H_

#include <wx/app.h>
#include <wx/dataview.h>

class wxGLCanvas;
class wxFrame;
class wxGLContext;

namespace dynsolver {
namespace debug {

class app : public wxApp {
 private:
  wxGLCanvas* canvas;
  wxFrame* frame;
  
  wxGLContext* glContext;

  wxFrame* dataViewFrame;
  wxDataViewListCtrl* dataView;
  
 public:
  virtual bool OnInit() override;
  ~app();
};
}
}

#endif
