///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GENERATED_H__
#define __GENERATED_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxMenu* m_menu3;
		wxMenu* m_menu4;
		wxStaticText* m_staticText9;
		wxTextCtrl* m_textCtrl4;
		wxStaticText* m_staticText10;
		wxTextCtrl* m_textCtrl3;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_textCtrl5;
		wxStaticText* m_staticText12;
		wxButton* m_button2;
		wxNotebook* m_notebook2;
		wxPanel* m_panel10;
		wxPanel* m_panel11;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1001,719 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MyFrame1();
	
};

#endif //__GENERATED_H__
