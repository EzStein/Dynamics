///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

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
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>
#include <wx/combobox.h>

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
		wxMenuItem* isoclineMenuItem;
		wxMenuItem* separatricesMenuItem;
		wxMenuItem* m_menuItem33;
		wxMenuItem* hopfBifurcationMenuItem;
		wxMenuItem* saddleNodeBifurcationMenuItem;
		wxMenuItem* limitCycleBifurcationMenuItem;
		wxMenuItem* saddleConnectionBifurcationMenuItem;
		wxMenu* m_menu4;
		wxMenuItem* newDynamicalMenuItem;
		wxMenuItem* newParameterMenuItem;
		wxMenuItem* closeDynamicalMenuItem;
		wxMenuItem* closeParameterMenuItem;
		wxMenu* m_menu8;
		wxNotebook* m_notebook4;
		wxPanel* bSizer18;
		wxDataViewListCtrl* equationsDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn16;
		wxTextCtrl* variablesTextCtrl;
		wxDataViewListCtrl* variableNamesDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn5;
		wxTextCtrl* parametersTextCtrl;
		wxDataViewListCtrl* parameterNamesDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn51;
		wxButton* compileButton;
		wxStaticText* m_staticText3;
		wxTextCtrl* varDiffTextCtrl;
		wxPanel* m_panel13;
		wxDataViewListCtrl* compiledParametersDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn3;
		wxDataViewListCtrl* compiledEquationsDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn1;
		wxDataViewColumn* m_dataViewListColumn17;
		wxNotebook* m_notebook2;
		wxPanel* m_panel5;
		wxDataViewListCtrl* solutionsDataViewCtrl;
		wxButton* solutionsEditButton;
		wxButton* solutionsDeleteButton;
		wxGrid* m_grid1;
		wxPanel* m_panel6;
		wxDataViewListCtrl* singularPointsDataViewCtrl;
		wxButton* m_button19;
		wxButton* singularPointsDeleteButton;
		wxPanel* m_panel14;
		wxDataViewListCtrl* separatricesDataViewCtrl;
		wxDataViewColumn* m_dataViewListColumn7;
		wxButton* m_button20;
		wxButton* m_button21;
		wxGrid* m_grid11;
		wxPanel* m_panel7;
		wxDataViewListCtrl* isoclinesDataViewCtrl;
		wxButton* isoclinesEditButton;
		wxButton* isoclinesDeleteButton;
		wxGrid* m_grid12;
		wxPanel* m_panel15;
		wxDataViewListCtrl* hopfBifurcationsDataViewCtrl;
		wxButton* m_button22;
		wxButton* m_button23;
		wxGrid* m_grid13;
		wxPanel* m_panel16;
		wxDataViewListCtrl* saddleNodeBifurcationsDataViewCtrl;
		wxButton* m_button24;
		wxButton* m_button25;
		wxGrid* m_grid14;
		wxPanel* m_panel161;
		wxDataViewListCtrl* limitCycleBifurcationsDataViewCtrl;
		wxButton* m_button241;
		wxButton* m_button251;
		wxGrid* m_grid141;
		wxPanel* m_panel162;
		wxDataViewListCtrl* saddleConnectionBifurcationsDataViewCtrl;
		wxButton* m_button242;
		wxButton* m_button252;
		wxGrid* m_grid142;
		wxPanel* m_panel163;
		wxDataViewListCtrl* periodicSolutionBifurcationsDataViewCtrl;
		wxButton* m_button243;
		wxButton* m_button253;
		wxGrid* m_grid143;
		wxMenu* m_menu22;

		// Virtual event handlers, overide them in your derived class
		virtual void console_frame_on_close( wxCloseEvent& event ) = 0;
		virtual void close_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void solution_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void singular_point_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void isocline_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void separatrices_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void draw_all_separatrices_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void hopf_bifurcation_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void saddle_node_bifurcation_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void limit_cycle_bifurcation_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void saddle_connection_bifurcation_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void new_dynamical_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void new_parameter_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void close_dynamical_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void close_parameter_menu_item_on_selection( wxCommandEvent& event ) = 0;
		virtual void lorenz_example_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void van_der_pol_example_on_menu_selection( wxCommandEvent& event ) = 0;
		virtual void equationsDataViewCtrlOnDataViewListCtrlItemActivated( wxDataViewEvent& event ) = 0;
		virtual void equationsDataViewCtrlOnDataViewListCtrlItemEditingDone( wxDataViewEvent& event ) = 0;
		virtual void equationsDataViewCtrlOnDataViewListCtrlItemEditingStarted( wxDataViewEvent& event ) = 0;
		virtual void equationsDataViewCtrlOnDataViewListCtrlItemStartEditing( wxDataViewEvent& event ) = 0;
		virtual void equationsDataViewCtrlOnDataViewListCtrlSelectionChanged( wxDataViewEvent& event ) = 0;
		virtual void variables_text_ctrl_on_text_enter( wxCommandEvent& event ) = 0;
		virtual void variable_names_data_view_ctrl_on_value_changed( wxDataViewEvent& event ) = 0;
		virtual void parameters_text_ctrl_on_text_enter( wxCommandEvent& event ) = 0;
		virtual void compile_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void solutions_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;
		virtual void solutions_edit_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void solutions_delete_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void singular_points_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;
		virtual void singular_points_delete_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void separatrices_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;
		virtual void isoclines_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;
		virtual void isoclines_edit_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void isoclines_delete_button_on_button_click( wxCommandEvent& event ) = 0;
		virtual void hopf_bifurcations_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;
		virtual void saddle_node_bifurcations_data_view_ctrl_on_selection_changed( wxDataViewEvent& event ) = 0;


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
		virtual void isocline_menu_item_on_menu_selection( wxCommandEvent& event ) = 0;
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
/// Class parameter_frame_base
///////////////////////////////////////////////////////////////////////////////
class parameter_frame_base : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar3;
		wxMenu* m_menu9;
		wxMenuItem* editMenuItem;
		wxStatusBar* m_statusBar3;
		wxPanel* m_panel11;
		wxMenu* parameterPopupMenu;
		wxBoxSizer* parameterWindowBox;

		// Virtual event handlers, overide them in your derived class
		virtual void parameter_frame_on_close( wxCloseEvent& event ) { event.Skip(); }
		virtual void edit_menu_item_on_menu_selection( wxCommandEvent& event ) { event.Skip(); }
		virtual void hopf_bifurcation_menu_item_on_selection( wxCommandEvent& event ) { event.Skip(); }
		virtual void saddle_node_bifurcation_menu_item_on_selection( wxCommandEvent& event ) { event.Skip(); }
		virtual void limit_cycle_bifurcation_menu_item_on_selection( wxCommandEvent& event ) { event.Skip(); }
		virtual void saddle_connection_bifurcation_menu_item_on_selection( wxCommandEvent& event ) { event.Skip(); }


	public:

		parameter_frame_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Parameter Space"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 845,713 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~parameter_frame_base();

		void m_panel11OnContextMenu( wxMouseEvent &event )
		{
			m_panel11->PopupMenu( parameterPopupMenu, event.GetPosition() );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class parameter_dialog_base
///////////////////////////////////////////////////////////////////////////////
class parameter_dialog_base : public wxDialog
{
	private:

	protected:
		wxPropertyGrid* parameterPropertyGrid;
		wxButton* okButton;
		wxButton* cancelButton;

		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		parameter_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Parameter"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 627,357 ), long style = wxDEFAULT_DIALOG_STYLE );
		~parameter_dialog_base();

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

		dynamical_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Dynamical"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 661,300 ), long style = wxDEFAULT_DIALOG_STYLE );
		~dynamical_dialog_base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class isocline_dialog_base
///////////////////////////////////////////////////////////////////////////////
class isocline_dialog_base : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook3;
		wxPanel* m_panel9;
		wxDataViewListCtrl* directionDataViewCtrl;
		wxPanel* m_panel10;
		wxDataViewListCtrl* initialDataViewCtrl;
		wxPanel* m_panel101;
		wxPropertyGrid* m_propertyGrid5;
		wxPGProperty* incrementPGItem;
		wxPGProperty* spanPGItem;
		wxPGProperty* searchRadiusPGItem;
		wxPGProperty* searchIncrementPGItem;
		wxButton* m_button14;
		wxButton* m_button13;

		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		isocline_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Isocline"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 663,392 ), long style = wxDEFAULT_DIALOG_STYLE );
		~isocline_dialog_base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class hopf_bifurcation_dialog_base
///////////////////////////////////////////////////////////////////////////////
class hopf_bifurcation_dialog_base : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook5;
		wxPanel* m_panel17;
		wxDataViewListCtrl* initialVarsDataViewCtrl;
		wxDataViewListCtrl* initialParametersDataViewCtrl;
		wxPanel* m_panel18;
		wxPropertyGrid* m_propertyGrid7;
		wxPGProperty* incrementPGItem;
		wxPGProperty* spanPGItem;
		wxPGProperty* searchRadiusPGItem;
		wxPGProperty* searchIncrementPGItem;
		wxButton* m_button26;
		wxButton* m_button27;

		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		hopf_bifurcation_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 648,386 ), long style = wxDEFAULT_DIALOG_STYLE );
		~hopf_bifurcation_dialog_base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class saddle_connection_bifurcation_dialog_base
///////////////////////////////////////////////////////////////////////////////
class saddle_connection_bifurcation_dialog_base : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook5;
		wxPanel* m_panel17;
		wxDataViewListCtrl* initialParametersDataViewCtrl;
		wxPanel* m_panel18;
		wxPropertyGrid* saddleConnectionPG;
		wxPGProperty* incrementPGItem;
		wxPGProperty* spanPGItem;
		wxPGProperty* searchRadiusPGItem;
		wxPGProperty* searchIncrementPGItem;
		wxStaticText* m_staticText10;
		wxComboBox* separatrix1ComboBox;
		wxStaticText* m_staticText11;
		wxComboBox* separatrix2ComboBox;
		wxButton* m_button26;
		wxButton* m_button27;

		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		saddle_connection_bifurcation_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 513,372 ), long style = wxDEFAULT_DIALOG_STYLE );
		~saddle_connection_bifurcation_dialog_base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class limit_cycle_bifurcation_dialog_base
///////////////////////////////////////////////////////////////////////////////
class limit_cycle_bifurcation_dialog_base : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook5;
		wxPanel* m_panel17;
		wxDataViewListCtrl* initialParametersDataViewCtrl;
		wxPanel* m_panel18;
		wxPropertyGrid* limitCyclePG;
		wxPGProperty* incrementPGItem;
		wxPGProperty* spanPGItem;
		wxPGProperty* searchRadiusPGItem;
		wxPGProperty* searchIncrementPGItem;
		wxStaticText* m_staticText10;
		wxComboBox* limitCycle1ComboBox;
		wxStaticText* m_staticText11;
		wxComboBox* limitCycle2ComboBox;
		wxButton* m_button26;
		wxButton* m_button27;

		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		limit_cycle_bifurcation_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 584,395 ), long style = wxDEFAULT_DIALOG_STYLE );
		~limit_cycle_bifurcation_dialog_base();

};

///////////////////////////////////////////////////////////////////////////////
/// Class saddle_node_bifurcation_dialog_base
///////////////////////////////////////////////////////////////////////////////
class saddle_node_bifurcation_dialog_base : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook5;
		wxPanel* m_panel17;
		wxDataViewListCtrl* initialVarsDataViewCtrl;
		wxDataViewListCtrl* initialParametersDataViewCtrl;
		wxPanel* m_panel18;
		wxPropertyGrid* m_propertyGrid7;
		wxPGProperty* incrementPGItem;
		wxPGProperty* spanPGItem;
		wxPGProperty* searchRadiusPGItem;
		wxPGProperty* searchIncrementPGItem;
		wxButton* m_button26;
		wxButton* m_button27;

		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		saddle_node_bifurcation_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 648,386 ), long style = wxDEFAULT_DIALOG_STYLE );
		~saddle_node_bifurcation_dialog_base();

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

///////////////////////////////////////////////////////////////////////////////
/// Class separatrix_dialog_base
///////////////////////////////////////////////////////////////////////////////
class separatrix_dialog_base : public wxDialog
{
	private:

	protected:
		wxPropertyGrid* m_propertyGrid6;
		wxPGProperty* incrementPGItem;
		wxPGProperty* timePGItem;
		wxStaticText* m_staticText2;
		wxComboBox* singularPointComboBox;
		wxStaticText* m_staticText3;
		wxComboBox* typeComboBox;
		wxButton* okButton;
		wxButton* cancelButton;

		// Virtual event handlers, overide them in your derived class
		virtual void ok_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancel_button_on_button_click( wxCommandEvent& event ) { event.Skip(); }


	public:

		separatrix_dialog_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Separatrix"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,400 ), long style = wxDEFAULT_DIALOG_STYLE );
		~separatrix_dialog_base();

};

