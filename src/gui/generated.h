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
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>

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
		wxMenuItem* m_menuItem1;
		wxMenuItem* closeMenuItem;
		wxMenuItem* saveMenuItem;
		wxMenuItem* saveAsMenuItem;
		wxMenuItem* m_menuItem19;
		wxMenuItem* m_menuItem38;
		wxMenu* m_menu2;
		wxMenuItem* solutionMenuItem;
		wxMenuItem* singularPointMenuItem;
		wxMenu* m_menu4;
		wxMenuItem* newDynamicalWindowMenuItem;
		wxMenuItem* closeDynamicalWindowsMenuItem;
		wxMenu* m_menu8;
		wxDataViewListCtrl* equationsDataViewCtrl;
		wxButton* compileButton;
		wxStaticText* m_staticText1;
		wxComboBox* variablesComboBox;
		wxDataViewListCtrl* m_dataViewListCtrl6;
		wxNotebook* m_notebook2;
		wxPanel* m_panel5;
		wxDataViewListCtrl* solutionsDataViewCtrl;
		wxButton* solutionsEditButton;
		wxButton* solutionsDeleteButton;
		wxPanel* m_panel6;
		wxDataViewListCtrl* singularPointsDataViewCtrl;
		wxButton* singularPointsDeleteButton;
		wxStatusBar* statusBar;
		wxMenu* m_menu22;
		
		// Virtual event handlers, overide them in your derived class
		virtual void console_frame_on_close( wxCloseEvent& event ) = 0;
		virtual void close_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void solution_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void singular_point_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void new_dynamical_window_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void close_dynamical_windows_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void lorenz_example_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void compile_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void variables_combo_box_on_combo_box( wxCommandEvent& event ) = 0;
		virtual void variables_combo_box_on_kill_focus( wxFocusEvent& event ) = 0;
		virtual void variables_combo_box_on_text_enter( wxCommandEvent& event ) = 0;
		virtual void solutions_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;
		virtual void solutions_edit_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void solutions_delete_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void singular_points_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;
		virtual void singular_points_delete_button_on_button_click( wxCommandEvent& event ) = 0;
		
	
	public:
		
		console_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Console"), const wxPoint& pos = wxPoint( -1,-1 ), const wxSize& size = wxSize( 904,785 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~console_frame_base();
		
		void console_frame_baseOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( m_menu22, event.GetPosition() );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class dynamical_frame_base
///////////////////////////////////////////////////////////////////////////////
class dynamical_frame_base : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* bSizer34s;
		wxPanel* m_panel11;
		wxMenu* selectionPopupMenu;
		wxPanel* m_panel15;
		wxMenu* objectsPopupMenu;
		wxMenuItem* solutionMenuItem;
		wxMenuItem* singularPointMenuItem;
		wxBoxSizer* dynamicalWindowBox;
		wxStatusBar* m_statusBar2;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu4;
		wxMenuItem* editMenuItem;
		
		// Virtual event handlers, overide them in your derived class
		virtual void dynamical_frame_on_close( wxCloseEvent& event ) = 0;
		virtual void dynamical_frame_on_iconize( wxIconizeEvent& event ) = 0;
		virtual void dynamical_frame_on_set_focus( wxFocusEvent& event ) = 0;
		virtual void selection_select_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void selection_delete_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void selection_edit_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void solution_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void singular_point_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void edit_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		
	
	public:
		
		dynamical_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Dynamical Space"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~dynamical_frame_base();
		
		void m_panel11OnContextMenu( wxMouseEvent &event )
		{
			m_panel11->PopupMenu( selectionPopupMenu, event.GetPosition() );
		}
		
		void m_panel15OnContextMenu( wxMouseEvent &event )
		{
			m_panel15->PopupMenu( objectsPopupMenu, event.GetPosition() );
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
/// Class dynamical_dialog_base
///////////////////////////////////////////////////////////////////////////////
class dynamical_dialog_base : public wxDialog 
{
	private:
	
	protected:
		wxRadioBox* viewportRadioBox;
		wxPropertyGrid* dynamical2dPropertyGrid;
		wxPropertyGrid* dynamical3dPropertyGrid;
		wxButton* okButton;
		wxButton* cancelButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void viewport_radio_box_on_radio_box( wxCommandEvent& event ) = 0;
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
