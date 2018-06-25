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
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Open...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	wxMenuItem* m_menuItem41;
	m_menuItem41 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem41 );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Export...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem19;
	m_menuItem19 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Preferences...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem19 );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem38;
	m_menuItem38 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Help...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem38 );
	
	m_menubar1->Append( m_menu1, wxT("File") ); 
	
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Isoclines...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem15 );
	
	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Separatrices...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem16 );
	
	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Initial Value Solution...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem17 );
	
	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Periodic Solution...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem18 );
	
	wxMenuItem* m_menuItem39;
	m_menuItem39 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Critical Points...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem39 );
	
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
	
	parametersTableBox = new wxPanel( sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer5->Add( parametersTableBox, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer18->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	
	bSizer4->Add( bSizer18, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Objects") ), wxVERTICAL );
	
	objectsTableBox = new wxPanel( sbSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer4->Add( objectsTableBox, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer4->Add( sbSizer4, 2, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( console_frame_base::console_frame_on_close ) );
	this->Connect( newDynamicalWindowMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::new_dynamical_window_menu_item_on_selection ) );
	this->Connect( closeDynamicalWindowsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_dynamical_windows_menu_item_on_selection ) );
	this->Connect( lorenzExampleMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::lorenz_example_menu_item_on_menu_selection ) );
	compileButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
	variablesComboBox->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( console_frame_base::variables_combo_box_on_combo_box ), NULL, this );
	variablesComboBox->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( console_frame_base::variables_combo_box_on_kill_focus ), NULL, this );
	variablesComboBox->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::variables_combo_box_on_text_enter ), NULL, this );
}

console_frame_base::~console_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( console_frame_base::console_frame_on_close ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::new_dynamical_window_menu_item_on_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_dynamical_windows_menu_item_on_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::lorenz_example_menu_item_on_menu_selection ) );
	compileButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
	variablesComboBox->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( console_frame_base::variables_combo_box_on_combo_box ), NULL, this );
	variablesComboBox->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( console_frame_base::variables_combo_box_on_kill_focus ), NULL, this );
	variablesComboBox->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::variables_combo_box_on_text_enter ), NULL, this );
	
}

dynamical_frame_base::dynamical_frame_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	dynamicalWindowBox = new wxBoxSizer( wxVERTICAL );
	
	
	this->SetSizer( dynamicalWindowBox );
	this->Layout();
	m_statusBar2 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	popupMenu = new wxMenu();
	solutionMenu = new wxMenuItem( popupMenu, wxID_ANY, wxString( wxT("Initial Value Solution...") ) , wxEmptyString, wxITEM_NORMAL );
	popupMenu->Append( solutionMenu );
	
	isoclineMenu = new wxMenuItem( popupMenu, wxID_ANY, wxString( wxT("Isocline...") ) , wxEmptyString, wxITEM_NORMAL );
	popupMenu->Append( isoclineMenu );
	
	singularPointMenu = new wxMenuItem( popupMenu, wxID_ANY, wxString( wxT("Singular Point...") ) , wxEmptyString, wxITEM_NORMAL );
	popupMenu->Append( singularPointMenu );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::dynamical_frame_baseOnContextMenu ), NULL, this ); 
	
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
	this->Connect( solutionMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::solution_menu_on_menu_selection ) );
	this->Connect( isoclineMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::isocline_menu_on_menu_selection ) );
	this->Connect( singularPointMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::singular_point_menu_on_menu_selection ) );
	this->Connect( editMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::edit_menu_item_on_menu_selection ) );
}

dynamical_frame_base::~dynamical_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dynamical_frame_base::dynamical_frame_on_close ) );
	this->Disconnect( wxEVT_ICONIZE, wxIconizeEventHandler( dynamical_frame_base::dynamical_frame_on_iconize ) );
	this->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( dynamical_frame_base::dynamical_frame_on_set_focus ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::solution_menu_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::isocline_menu_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::singular_point_menu_on_menu_selection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::edit_menu_item_on_menu_selection ) );
	
	delete popupMenu; 
}

parameter_frame_base::parameter_frame_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxVERTICAL );
	
	parameterWindowBox = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer35->Add( parameterWindowBox, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer35 );
	this->Layout();
	m_statusBar3 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menu22 = new wxMenu();
	wxMenuItem* m_menuItem52;
	m_menuItem52 = new wxMenuItem( m_menu22, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu22->Append( m_menuItem52 );
	
	wxMenuItem* m_menuItem53;
	m_menuItem53 = new wxMenuItem( m_menu22, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu22->Append( m_menuItem53 );
	
	wxMenuItem* m_menuItem54;
	m_menuItem54 = new wxMenuItem( m_menu22, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu22->Append( m_menuItem54 );
	
	wxMenuItem* m_menuItem55;
	m_menuItem55 = new wxMenuItem( m_menu22, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu22->Append( m_menuItem55 );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( parameter_frame_base::parameter_frame_baseOnContextMenu ), NULL, this ); 
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Edit...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem7 );
	
	m_menubar1->Append( m_menu4, wxT("View") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
}

parameter_frame_base::~parameter_frame_base()
{
	delete m_menu22; 
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
	
	addButton = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
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

preferences_dialog_base::preferences_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook3 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel11 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook3->AddPage( m_panel11, wxT("General"), false );
	m_panel9 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook3->AddPage( m_panel9, wxT("Parameter Plane"), false );
	m_panel10 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook3->AddPage( m_panel10, wxT("Dynamical Plane"), false );
	m_panel12 = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook3->AddPage( m_panel12, wxT("Advanced"), true );
	
	bSizer31->Add( m_notebook3, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer31 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

preferences_dialog_base::~preferences_dialog_base()
{
}

parameter_dialog_base::parameter_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyGrid1 = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER);
	m_propertyGridItem5 = m_propertyGrid1->Append( new wxPropertyCategory( wxT("X Axis"), wxT("X Axis") ) ); 
	m_propertyGridItem7 = m_propertyGrid1->Append( new wxEnumProperty( wxT("Variable"), wxT("Variable") ) ); 
	m_propertyGridItem91 = m_propertyGrid1->Append( new wxFloatProperty( wxT("Minimum"), wxT("Minimum") ) ); 
	m_propertyGridItem92 = m_propertyGrid1->Append( new wxFloatProperty( wxT("Maximum"), wxT("Maximum") ) ); 
	m_propertyGridItem6 = m_propertyGrid1->Append( new wxPropertyCategory( wxT("Y Axis"), wxT("Y Axis") ) ); 
	m_propertyGridItem8 = m_propertyGrid1->Append( new wxEnumProperty( wxT("Variable"), wxT("Variable") ) ); 
	m_propertyGridItem9 = m_propertyGrid1->Append( new wxFloatProperty( wxT("Minimum"), wxT("Minimum") ) ); 
	m_propertyGridItem10 = m_propertyGrid1->Append( new wxFloatProperty( wxT("Maximum"), wxT("Maximum") ) ); 
	bSizer7->Add( m_propertyGrid1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button8 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_button8, 0, wxALL, 5 );
	
	m_button9 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( m_button9, 0, wxALL, 5 );
	
	
	bSizer7->Add( bSizer17, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

parameter_dialog_base::~parameter_dialog_base()
{
}

dynamical_dialog_base::dynamical_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	dynamicalPropertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE|wxPG_SPLITTER_AUTO_CENTER);
	bSizer7->Add( dynamicalPropertyGrid, 1, wxEXPAND, 5 );
	
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
	okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::cancel_button_on_button_click ), NULL, this );
}

dynamical_dialog_base::~dynamical_dialog_base()
{
	// Disconnect Events
	okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dynamical_dialog_base::cancel_button_on_button_click ), NULL, this );
	
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
