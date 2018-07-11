///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "generated.h"

///////////////////////////////////////////////////////////////////////////

console_frame_base::console_frame_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Open...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	closeMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( closeMenuItem );
	
	m_menu1->AppendSeparator();
	
	saveMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Save...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( saveMenuItem );
	
	saveAsMenuItem = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Save As...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( saveAsMenuItem );
	
	m_menu1->AppendSeparator();
	
	m_menuItem19 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Preferences...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem19 );
	
	m_menu1->AppendSeparator();
	
	m_menuItem38 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Help...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem38 );
	
	m_menubar1->Append( m_menu1, wxT("File") ); 
	
	m_menu2 = new wxMenu();
	solutionMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Solution...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( solutionMenuItem );
	
	singularPointMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Singular Point...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( singularPointMenuItem );
	
	wxMenuItem* isoclineMenuItem;
	isoclineMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Isocline...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( isoclineMenuItem );
	
	m_menubar1->Append( m_menu2, wxT("Objects") ); 
	
	m_menu4 = new wxMenu();
	newDynamicalWindowMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("New Dynamical...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( newDynamicalWindowMenuItem );
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("New Parameter...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem8 );
	
	m_menu4->AppendSeparator();
	
	closeDynamicalWindowsMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Close Dynamical Windows...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( closeDynamicalWindowsMenuItem );
	
	wxMenuItem* m_menuItem23;
	m_menuItem23 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Close Parameter Windows...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem23 );
	
	m_menubar1->Append( m_menu4, wxT("Window") ); 
	
	m_menu8 = new wxMenu();
	wxMenuItem* lorenzExampleMenuItem;
	lorenzExampleMenuItem = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Lorenz System") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( lorenzExampleMenuItem );
	
	wxMenuItem* m_menuItem25;
	m_menuItem25 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Damped Harmonic Motion") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem25 );
	
	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Saddle Node Bifurcation") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( m_menuItem26 );
	
	m_menubar1->Append( m_menu8, wxT("Examples") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer41;
	sbSizer41 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Equations") ), wxVERTICAL );
	
	equationsDataViewCtrl = new wxDataViewListCtrl( sbSizer41->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer41->Add( equationsDataViewCtrl, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	compileButton = new wxButton( sbSizer41->GetStaticBox(), wxID_ANY, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( compileButton, 0, wxALL, 5 );
	
	m_staticText1 = new wxStaticText( sbSizer41->GetStaticBox(), wxID_ANY, wxT("Variables: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer16->Add( m_staticText1, 0, wxALL, 10 );
	
	variablesComboBox = new wxComboBox( sbSizer41->GetStaticBox(), wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER );
	variablesComboBox->Append( wxT("1") );
	variablesComboBox->Append( wxT("2") );
	variablesComboBox->Append( wxT("3") );
	variablesComboBox->Append( wxT("4") );
	variablesComboBox->SetSelection( 0 );
	bSizer16->Add( variablesComboBox, 1, wxALL, 5 );
	
	
	sbSizer41->Add( bSizer16, 0, wxEXPAND, 5 );
	
	
	bSizer18->Add( sbSizer41, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Parameters") ), wxVERTICAL );
	
	m_dataViewListCtrl6 = new wxDataViewListCtrl( sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer5->Add( m_dataViewListCtrl6, 1, wxEXPAND, 5 );
	
	
	bSizer18->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	
	bSizer4->Add( bSizer18, 1, wxEXPAND, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel5 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer182;
	bSizer182 = new wxBoxSizer( wxVERTICAL );
	
	solutionsDataViewCtrl = new wxDataViewListCtrl( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer182->Add( solutionsDataViewCtrl, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxHORIZONTAL );
	
	solutionsEditButton = new wxButton( m_panel5, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( solutionsEditButton, 0, wxALL, 5 );
	
	solutionsDeleteButton = new wxButton( m_panel5, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer181->Add( solutionsDeleteButton, 0, wxALL, 5 );
	
	
	bSizer182->Add( bSizer181, 0, wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( bSizer182 );
	m_panel5->Layout();
	bSizer182->Fit( m_panel5 );
	m_notebook2->AddPage( m_panel5, wxT("Solutions"), false );
	m_panel6 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	singularPointsDataViewCtrl = new wxDataViewListCtrl( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( singularPointsDataViewCtrl, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	singularPointsDeleteButton = new wxButton( m_panel6, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( singularPointsDeleteButton, 0, wxALL, 5 );
	
	
	bSizer20->Add( bSizer19, 0, wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer20 );
	m_panel6->Layout();
	bSizer20->Fit( m_panel6 );
	m_notebook2->AddPage( m_panel6, wxT("Singular Points"), false );
	m_panel7 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxVERTICAL );
	
	isoclinesDataViewCtrl = new wxDataViewListCtrl( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer191->Add( isoclinesDataViewCtrl, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxHORIZONTAL );
	
	isoclinesEditButton = new wxButton( m_panel7, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( isoclinesEditButton, 0, wxALL, 5 );
	
	isoclinesDeleteButton = new wxButton( m_panel7, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( isoclinesDeleteButton, 0, wxALL, 5 );
	
	
	bSizer191->Add( bSizer201, 0, wxEXPAND, 5 );
	
	
	m_panel7->SetSizer( bSizer191 );
	m_panel7->Layout();
	bSizer191->Fit( m_panel7 );
	m_notebook2->AddPage( m_panel7, wxT("Isoclines"), true );
	
	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menu22 = new wxMenu();
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( console_frame_base::console_frame_baseOnContextMenu ), NULL, this ); 
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( console_frame_base::console_frame_on_close ) );
	this->Connect( closeMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_menu_item_on_menu_selection ) );
	this->Connect( solutionMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::solution_menu_item_on_menu_selection ) );
	this->Connect( singularPointMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::singular_point_menu_item_on_menu_selection ) );
	this->Connect( isoclineMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::isocline_menu_item_on_menu_selection ) );
	this->Connect( newDynamicalWindowMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::new_dynamical_window_menu_item_on_selection ) );
	this->Connect( closeDynamicalWindowsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_dynamical_windows_menu_item_on_selection ) );
	this->Connect( lorenzExampleMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::lorenz_example_menu_item_on_menu_selection ) );
	compileButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
	variablesComboBox->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( console_frame_base::variables_combo_box_on_combo_box ), NULL, this );
	variablesComboBox->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( console_frame_base::variables_combo_box_on_kill_focus ), NULL, this );
	variablesComboBox->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::variables_combo_box_on_text_enter ), NULL, this );
	solutionsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::solutions_data_view_ctrl_on_selection_changed ), NULL, this );
	solutionsEditButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_edit_button_on_button_click ), NULL, this );
	solutionsDeleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_delete_button_on_button_click ), NULL, this );
	singularPointsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::singular_points_data_view_ctrl_on_selection_changed ), NULL, this );
	singularPointsDeleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::singular_points_delete_button_on_button_click ), NULL, this );
	isoclinesDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::isoclines_data_view_ctrl_on_selection_changed ), NULL, this );
	isoclinesEditButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_edit_button_on_button_click ), NULL, this );
	isoclinesDeleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_delete_button_on_button_click ), NULL, this );
}

console_frame_base::~console_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( console_frame_base::console_frame_on_close ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::solution_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::singular_point_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::isocline_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::new_dynamical_window_menu_item_on_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_dynamical_windows_menu_item_on_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::lorenz_example_menu_item_on_menu_selection ) );
	compileButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
	variablesComboBox->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( console_frame_base::variables_combo_box_on_combo_box ), NULL, this );
	variablesComboBox->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( console_frame_base::variables_combo_box_on_kill_focus ), NULL, this );
	variablesComboBox->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::variables_combo_box_on_text_enter ), NULL, this );
	solutionsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::solutions_data_view_ctrl_on_selection_changed ), NULL, this );
	solutionsEditButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_edit_button_on_button_click ), NULL, this );
	solutionsDeleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_delete_button_on_button_click ), NULL, this );
	singularPointsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::singular_points_data_view_ctrl_on_selection_changed ), NULL, this );
	singularPointsDeleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::singular_points_delete_button_on_button_click ), NULL, this );
	isoclinesDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::isoclines_data_view_ctrl_on_selection_changed ), NULL, this );
	isoclinesEditButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_edit_button_on_button_click ), NULL, this );
	isoclinesDeleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_delete_button_on_button_click ), NULL, this );
	
	delete m_menu22; 
}

dynamical_frame_base::dynamical_frame_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	bSizer34s = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_panel11 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel11->Hide();
	
	selectionPopupMenu = new wxMenu();
	wxMenuItem* selectionSelectMenuItem;
	selectionSelectMenuItem = new wxMenuItem( selectionPopupMenu, wxID_ANY, wxString( wxT("Select") ) , wxEmptyString, wxITEM_NORMAL );
	selectionPopupMenu->Append( selectionSelectMenuItem );
	
	wxMenuItem* selectionDeleteMenuItem;
	selectionDeleteMenuItem = new wxMenuItem( selectionPopupMenu, wxID_ANY, wxString( wxT("Delete...") ) , wxEmptyString, wxITEM_NORMAL );
	selectionPopupMenu->Append( selectionDeleteMenuItem );
	
	wxMenuItem* selectionEditMenuItem;
	selectionEditMenuItem = new wxMenuItem( selectionPopupMenu, wxID_ANY, wxString( wxT("Edit...") ) , wxEmptyString, wxITEM_NORMAL );
	selectionPopupMenu->Append( selectionEditMenuItem );
	
	m_panel11->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::m_panel11OnContextMenu ), NULL, this ); 
	
	bSizer20->Add( m_panel11, 1, wxEXPAND | wxALL, 5 );
	
	m_panel15 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel15->Hide();
	
	objectsPopupMenu = new wxMenu();
	solutionMenuItem = new wxMenuItem( objectsPopupMenu, wxID_ANY, wxString( wxT("Initial Value Solution...") ) , wxEmptyString, wxITEM_NORMAL );
	objectsPopupMenu->Append( solutionMenuItem );
	
	singularPointMenuItem = new wxMenuItem( objectsPopupMenu, wxID_ANY, wxString( wxT("Singular Point...") ) , wxEmptyString, wxITEM_NORMAL );
	objectsPopupMenu->Append( singularPointMenuItem );
	
	wxMenuItem* isoclineMenuItem;
	isoclineMenuItem = new wxMenuItem( objectsPopupMenu, wxID_ANY, wxString( wxT("Isocline...") ) , wxEmptyString, wxITEM_NORMAL );
	objectsPopupMenu->Append( isoclineMenuItem );
	
	m_panel15->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::m_panel15OnContextMenu ), NULL, this ); 
	
	bSizer20->Add( m_panel15, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer34s->Add( bSizer20, 0, wxALL, 5 );
	
	dynamicalWindowBox = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer34s->Add( dynamicalWindowBox, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer34s );
	this->Layout();
	m_statusBar2 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_menu4 = new wxMenu();
	editMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Edit...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( editMenuItem );
	
	m_menubar1->Append( m_menu4, wxT("View") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dynamical_frame_base::dynamical_frame_on_close ) );
	this->Connect( wxEVT_ICONIZE, wxIconizeEventHandler( dynamical_frame_base::dynamical_frame_on_iconize ) );
	this->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( dynamical_frame_base::dynamical_frame_on_set_focus ) );
	this->Connect( selectionSelectMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_select_menu_item_on_menu_selection ) );
	this->Connect( selectionDeleteMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_delete_menu_item_on_menu_selection ) );
	this->Connect( selectionEditMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_edit_menu_item_on_menu_selection ) );
	this->Connect( solutionMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::solution_menu_item_on_menu_selection ) );
	this->Connect( singularPointMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::singular_point_menu_item_on_menu_selection ) );
	this->Connect( isoclineMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::isocline_menu_item_on_menu_selection ) );
	this->Connect( editMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::edit_menu_item_on_menu_selection ) );
}

dynamical_frame_base::~dynamical_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dynamical_frame_base::dynamical_frame_on_close ) );
	this->Disconnect( wxEVT_ICONIZE, wxIconizeEventHandler( dynamical_frame_base::dynamical_frame_on_iconize ) );
	this->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( dynamical_frame_base::dynamical_frame_on_set_focus ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_select_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_delete_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_edit_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::solution_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::singular_point_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::isocline_menu_item_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::edit_menu_item_on_menu_selection ) );
	
	delete selectionPopupMenu; 
	delete objectsPopupMenu; 
}

parameter_frame_base::parameter_frame_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar3 = new wxMenuBar( 0 );
	m_menu9 = new wxMenu();
	editMenuItem = new wxMenuItem( m_menu9, wxID_ANY, wxString( wxT("Edit...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu9->Append( editMenuItem );
	
	m_menubar3->Append( m_menu9, wxT("View") ); 
	
	this->SetMenuBar( m_menubar3 );
	
	m_statusBar3 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );
	
	m_panel11 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel11->Hide();
	
	m_menu10 = new wxMenu();
	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_menu10, wxID_ANY, wxString( wxT("Hopf Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu10->Append( m_menuItem26 );
	
	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( m_menu10, wxID_ANY, wxString( wxT("Saddle Node Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu10->Append( m_menuItem27 );
	
	m_panel11->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( parameter_frame_base::m_panel11OnContextMenu ), NULL, this ); 
	
	bSizer27->Add( m_panel11, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer26->Add( bSizer27, 1, wxEXPAND, 5 );
	
	parameterWindowBox = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer26->Add( parameterWindowBox, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer26 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( editMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( parameter_frame_base::edit_menu_item_on_menu_selection ) );
}

parameter_frame_base::~parameter_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( parameter_frame_base::edit_menu_item_on_menu_selection ) );
	
	delete m_menu10; 
}

parameter_dialog_base::parameter_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyGrid6 = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer30->Add( m_propertyGrid6, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button15 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button15, 0, wxALL, 5 );
	
	m_button16 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( m_button16, 0, wxALL, 5 );
	
	
	bSizer30->Add( bSizer31, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer30 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

parameter_dialog_base::~parameter_dialog_base()
{
}

solution_dialog_base::solution_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel11 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	initialValueDataViewCtrl = new wxDataViewListCtrl( m_panel11, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( initialValueDataViewCtrl, 1, wxEXPAND, 5 );
	
	
	m_panel11->SetSizer( bSizer17 );
	m_panel11->Layout();
	bSizer17->Fit( m_panel11 );
	m_notebook2->AddPage( m_panel11, wxT("Initial Value"), true );
	m_panel10 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer321;
	bSizer321 = new wxBoxSizer( wxVERTICAL );
	
	solutionPropertyGrid = new wxPropertyGrid(m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER);
	tMinPropertyGridItem = solutionPropertyGrid->Append( new wxFloatProperty( wxT("T Min"), wxT("T Min") ) ); 
	tMaxPropertyGridItem = solutionPropertyGrid->Append( new wxFloatProperty( wxT("T Max"), wxT("T Max") ) ); 
	incrementPropertyGridItem = solutionPropertyGrid->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) ); 
	bSizer321->Add( solutionPropertyGrid, 1, wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( bSizer321 );
	m_panel10->Layout();
	bSizer321->Fit( m_panel10 );
	m_notebook2->AddPage( m_panel10, wxT("Properties"), false );
	
	bSizer10->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxHORIZONTAL );
	
	addButton = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( addButton, 0, wxALL, 5 );
	
	cancelButton = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( cancelButton, 0, wxALL, 5 );
	
	
	bSizer10->Add( bSizer32, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer10 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	addButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( solution_dialog_base::add_button_on_button_click ), NULL, this );
	cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( solution_dialog_base::cancel_button_on_button_click ), NULL, this );
}

solution_dialog_base::~solution_dialog_base()
{
	// Disconnect Events
	addButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( solution_dialog_base::add_button_on_button_click ), NULL, this );
	cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( solution_dialog_base::cancel_button_on_button_click ), NULL, this );
	
}

dynamical_dialog_base::dynamical_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxString viewportRadioBoxChoices[] = { wxT("2D"), wxT("3D") };
	int viewportRadioBoxNChoices = sizeof( viewportRadioBoxChoices ) / sizeof( wxString );
	viewportRadioBox = new wxRadioBox( this, wxID_ANY, wxT("Viewport"), wxDefaultPosition, wxDefaultSize, viewportRadioBoxNChoices, viewportRadioBoxChoices, 1, wxRA_SPECIFY_ROWS );
	viewportRadioBox->SetSelection( 0 );
	bSizer7->Add( viewportRadioBox, 0, wxEXPAND, 5 );
	
	dynamical2dPropertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER);
	bSizer7->Add( dynamical2dPropertyGrid, 1, wxEXPAND, 5 );
	
	dynamical3dPropertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER);
	dynamical3dPropertyGrid->Hide();
	
	bSizer7->Add( dynamical3dPropertyGrid, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	okButton = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( okButton, 0, wxALL, 5 );
	
	cancelButton = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( cancelButton, 0, wxALL, 5 );
	
	
	bSizer7->Add( bSizer17, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	viewportRadioBox->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( dynamical_dialog_base::viewport_radio_box_on_radio_box ), NULL, this );
	okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::cancel_button_on_button_click ), NULL, this );
}

dynamical_dialog_base::~dynamical_dialog_base()
{
	// Disconnect Events
	viewportRadioBox->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( dynamical_dialog_base::viewport_radio_box_on_radio_box ), NULL, this );
	okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::cancel_button_on_button_click ), NULL, this );
	
}

isocline_dialog_base::isocline_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook3 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel9 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	directionDataViewCtrl = new wxDataViewListCtrl( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( directionDataViewCtrl, 1, wxEXPAND, 5 );
	
	
	m_panel9->SetSizer( bSizer23 );
	m_panel9->Layout();
	bSizer23->Fit( m_panel9 );
	m_notebook3->AddPage( m_panel9, wxT("Direction"), false );
	m_panel10 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	initialDataViewCtrl = new wxDataViewListCtrl( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer24->Add( initialDataViewCtrl, 1, wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( bSizer24 );
	m_panel10->Layout();
	bSizer24->Fit( m_panel10 );
	m_notebook3->AddPage( m_panel10, wxT("Initial Point"), false );
	m_panel101 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyGrid5 = new wxPropertyGrid(m_panel101, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER);
	m_propertyGridItem4 = m_propertyGrid5->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) ); 
	m_propertyGridItem5 = m_propertyGrid5->Append( new wxIntProperty( wxT("Span"), wxT("Span") ) ); 
	bSizer25->Add( m_propertyGrid5, 1, wxEXPAND, 5 );
	
	
	m_panel101->SetSizer( bSizer25 );
	m_panel101->Layout();
	bSizer25->Fit( m_panel101 );
	m_notebook3->AddPage( m_panel101, wxT("Properties"), true );
	
	bSizer21->Add( m_notebook3, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button14 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( m_button14, 0, wxALL, 5 );
	
	m_button13 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( m_button13, 0, wxALL, 5 );
	
	
	bSizer21->Add( bSizer22, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer21 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( isocline_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( isocline_dialog_base::cancel_button_on_button_click ), NULL, this );
}

isocline_dialog_base::~isocline_dialog_base()
{
	// Disconnect Events
	m_button14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( isocline_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( isocline_dialog_base::cancel_button_on_button_click ), NULL, this );
	
}

singular_point_dialog_base::singular_point_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );
	
	initialValueDataViewCtrl = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer33->Add( initialValueDataViewCtrl, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	addButton = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer34->Add( addButton, 0, wxALL, 5 );
	
	cancelButton = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer34->Add( cancelButton, 0, wxALL, 5 );
	
	
	bSizer33->Add( bSizer34, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer33 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	addButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( singular_point_dialog_base::add_button_on_button_click ), NULL, this );
	cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( singular_point_dialog_base::cancel_button_on_button_click ), NULL, this );
}

singular_point_dialog_base::~singular_point_dialog_base()
{
	// Disconnect Events
	addButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( singular_point_dialog_base::add_button_on_button_click ), NULL, this );
	cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( singular_point_dialog_base::cancel_button_on_button_click ), NULL, this );
	
}
