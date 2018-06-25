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
#include <wx/dataview.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class console_frame_base
///////////////////////////////////////////////////////////////////////////////
class console_frame_base : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxMenu* m_menu4;
		wxMenuItem* newDynamicalWindowMenuItem;
		wxMenuItem* closeDynamicalWindowsMenuItem;
		wxMenu* m_menu8;
		wxDataViewListCtrl* equationsDataViewCtrl;
		wxButton* compileButton;
		wxStaticText* m_staticText1;
		wxComboBox* variablesComboBox;
		wxPanel* parametersTableBox;
		wxPanel* objectsTableBox;
		wxStatusBar* statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void console_frame_on_close( wxCloseEvent& event ) = 0;
		virtual void new_dynamical_window_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void close_dynamical_windows_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void lorenz_example_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void compile_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void variables_combo_box_on_combo_box( wxCommandEvent& event ) = 0;
		virtual void variables_combo_box_on_kill_focus( wxFocusEvent& event ) = 0;
		virtual void variables_combo_box_on_text_enter( wxCommandEvent& event ) = 0;
		
	
	public:
		
		console_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Console"), const wxPoint& pos = wxPoint( -1,-1 ), const wxSize& size = wxSize( 904,785 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~console_frame_base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dynamical_frame_base
///////////////////////////////////////////////////////////////////////////////
class dynamical_frame_base : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* dynamicalWindowBox;
		wxStatusBar* m_statusBar2;
		wxMenu* popupMenu;
		wxMenuItem* solutionMenu;
		wxMenuItem* isoclineMenu;
		wxMenuItem* singularPointMenu;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu4;
		wxMenuItem* editMenuItem;
		
		// Virtual event handlers, overide them in your derived class
		virtual void dynamical_frame_on_close( wxCloseEvent& event ) = 0;
		virtual void dynamical_frame_on_iconize( wxIconizeEvent& event ) = 0;
		virtual void dynamical_frame_on_set_focus( wxFocusEvent& event ) = 0;
		virtual void solution_menu_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void isocline_menu_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void singular_point_menu_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void edit_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		
	
	public:
		
		dynamical_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Dynamical Space"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~dynamical_frame_base();
		
		void dynamical_frame_baseOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( popupMenu, event.GetPosition() );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class parameter_frame_base
///////////////////////////////////////////////////////////////////////////////
class parameter_frame_base : public wxFrame 
{
	private:
	
	protected:
		wxPanel* parameterWindowBox;
		wxStatusBar* m_statusBar3;
		wxMenu* m_menu22;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu4;
	
	public:
		
		parameter_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Parameter Space"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~parameter_frame_base();
		
		void parameter_frame_baseOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( m_menu22, event.GetPosition() );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class solution_dialog_base
///////////////////////////////////////////////////////////////////////////////
class solution_dialog_base : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook2;
		wxPanel* m_panel11;
		wxDataViewListCtrl* initialValueDataViewCtrl;
		wxPanel* m_panel10;
		wxPropertyGrid* solutionPropertyGrid;
		wxPGProperty* tMinPropertyGridItem;
		wxPGProperty* tMaxPropertyGridItem;
		wxPGProperty* incrementPropertyGridItem;
		wxButton* addButton;
		wxButton* cancelButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void add_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) = 0;
		
	
	public:
		
		solution_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Initial Value Solution"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 596,367 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~solution_dialog_base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class preferences_dialog_base
///////////////////////////////////////////////////////////////////////////////
class preferences_dialog_base : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook3;
		wxPanel* m_panel11;
		wxPanel* m_panel9;
		wxPanel* m_panel10;
		wxPanel* m_panel12;
	
	public:
		
		preferences_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 569,392 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~preferences_dialog_base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class parameter_dialog_base
///////////////////////////////////////////////////////////////////////////////
class parameter_dialog_base : public wxDialog 
{
	private:
	
	protected:
		wxPropertyGrid* m_propertyGrid1;
		wxPGProperty* m_propertyGridItem5;
		wxPGProperty* m_propertyGridItem7;
		wxPGProperty* m_propertyGridItem91;
		wxPGProperty* m_propertyGridItem92;
		wxPGProperty* m_propertyGridItem6;
		wxPGProperty* m_propertyGridItem8;
		wxPGProperty* m_propertyGridItem9;
		wxPGProperty* m_propertyGridItem10;
		wxButton* m_button8;
		wxButton* m_button9;
	
	public:
		
		parameter_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 509,312 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~parameter_dialog_base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dynamical_dialog_base
///////////////////////////////////////////////////////////////////////////////
class dynamical_dialog_base : public wxDialog 
{
	private:
	
	protected:
		wxPropertyGrid* dynamicalPropertyGrid;
		wxButton* okButton;
		wxButton* cancelButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) = 0;
		
	
	public:
		
		dynamical_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 661,300 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~dynamical_dialog_base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class singular_point_dialog_base
///////////////////////////////////////////////////////////////////////////////
class singular_point_dialog_base : public wxDialog 
{
	private:
	
	protected:
		wxDataViewListCtrl* initialValueDataViewCtrl;
		wxButton* addButton;
		wxButton* cancelButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void add_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) = 0;
		
	
	public:
		
		singular_point_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Singular Point"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,349 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~singular_point_dialog_base();
	
};

#endif //__GENERATED_H__
