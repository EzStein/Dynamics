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
#include <wx/dataview.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
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
		wxBoxSizer* bSizer2;
		wxStaticText* m_staticText8;
		wxTextCtrl* dimensionField;
		wxButton* m_button21;
		wxDataViewListCtrl* m_dataViewListCtrl2;
		wxButton* m_button2;
		wxStaticText* m_staticText91;
		wxDataViewListCtrl* m_dataViewListCtrl1;
		wxStaticText* m_staticText102;
		wxStaticText* m_staticText11;
		wxTextCtrl* tMinValField;
		wxStaticText* m_staticText121;
		wxTextCtrl* tMaxValField;
		wxStaticText* m_staticText141;
		wxTextCtrl* tIncrementField;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText15;
		wxChoice* axisVariable1;
		wxStaticText* m_staticText16;
		wxChoice* axisVariable2;
		wxStaticText* m_staticText18;
		wxChoice* axisVariable3;
		wxCheckBox* renderCheckBox;
		wxNotebook* m_notebook2;
		wxPanel* parameterPlane;
		wxStatusBar* statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void on_size_top_frame( wxSizeEvent& event ) = 0;
		virtual void on_menu_selection_vector_field( wxCommandEvent& event ) = 0;
		virtual void on_text_enter_dimension( wxCommandEvent& event ) = 0;
		virtual void on_button_click_compile( wxCommandEvent& event ) = 0;
		virtual void on_axis_choice( wxCommandEvent& event ) = 0;
		virtual void on_3d_render_check( wxCommandEvent& event ) = 0;
		
	
	public:
		
		top_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,800 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~top_frame_base();
	
};

#endif //__GENERATED_H__
