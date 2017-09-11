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
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class top_frame_base
///////////////////////////////////////////////////////////////////////////////
class top_frame_base : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxMenu* m_menu3;
		wxMenu* m_menu4;
		wxStaticText* m_staticText8;
		wxChoice* m_choice1;
		wxStaticText* m_staticText9;
		wxTextCtrl* xFuncField;
		wxStaticText* yEqnLabel;
		wxTextCtrl* yFuncField;
		wxStaticText* zEqnLabel;
		wxTextCtrl* zFuncField;
		wxStaticText* m_staticText91;
		wxStaticText* m_staticText101;
		wxTextCtrl* tInitValField;
		wxStaticText* m_staticText4;
		wxTextCtrl* xInitValField;
		wxStaticText* m_staticText5;
		wxTextCtrl* yInitValField;
		wxStaticText* m_staticText6;
		wxTextCtrl* zInitValField;
		wxStaticText* m_staticText102;
		wxStaticText* m_staticText11;
		wxTextCtrl* tMinValField;
		wxStaticText* m_staticText121;
		wxTextCtrl* tMaxValField;
		wxStaticText* m_staticText14;
		wxChoice* axesChoice;
		wxButton* m_button2;
		wxNotebook* m_notebook2;
		wxPanel* dynamicalPlane;
		wxPanel* parameterPlane;
		wxStatusBar* statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void on_menu_selection_vector_field( wxCommandEvent& event ) = 0;
		virtual void on_choice_dimension( wxCommandEvent& event ) = 0;
		virtual void on_choice_axes( wxCommandEvent& event ) = 0;
		virtual void on_button_click_compile( wxCommandEvent& event ) = 0;
		virtual void on_left_down_dynamical_plane( wxMouseEvent& event ) = 0;
		virtual void on_motion_dynamical_plane( wxMouseEvent& event ) = 0;
		virtual void on_paint_dynamical_plane( wxPaintEvent& event ) = 0;
		virtual void on_size_dynamical_plane( wxSizeEvent& event ) = 0;
		
	
	public:
		
		top_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 907,727 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~top_frame_base();
	
};

#endif //__GENERATED_H__
