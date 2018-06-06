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
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Dynamical...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem7 );
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Parameter...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem8 );
	
	m_menubar1->Append( m_menu4, wxT("Window") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer41;
	sbSizer41 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Equations") ), wxVERTICAL );
	
	equationsTableBox = new wxPanel( sbSizer41->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer41->Add( equationsTableBox, 1, wxEXPAND | wxALL, 5 );
	
	m_button7 = new wxButton( sbSizer41->GetStaticBox(), wxID_ANY, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer41->Add( m_button7, 0, wxALL, 5 );
	
	
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
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
}

console_frame_base::~console_frame_base()
{
	// Disconnect Events
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
	
}

dynamical_frame_base::dynamical_frame_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxVERTICAL );
	
	dynamicalWindowBox = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer34->Add( dynamicalWindowBox, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer34 );
	this->Layout();
	m_statusBar2 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menu21 = new wxMenu();
	wxMenuItem* m_menuItem48;
	m_menuItem48 = new wxMenuItem( m_menu21, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem48 );
	
	wxMenuItem* m_menuItem49;
	m_menuItem49 = new wxMenuItem( m_menu21, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem49 );
	
	wxMenuItem* m_menuItem50;
	m_menuItem50 = new wxMenuItem( m_menu21, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem50 );
	
	wxMenuItem* m_menuItem51;
	m_menuItem51 = new wxMenuItem( m_menu21, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem51 );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::dynamical_frame_baseOnContextMenu ), NULL, this ); 
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Edit...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem7 );
	
	m_menubar1->Append( m_menu4, wxT("View") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( dynamical_frame_base::dynamical_frame_on_set_focus ) );
	dynamicalWindowBox->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( dynamical_frame_base::dynamical_window_box_on_key_down ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_KEY_UP, wxKeyEventHandler( dynamical_frame_base::dynamical_window_box_on_key_up ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_left_down ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_left_up ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_MOTION, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_motion ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_mouse_wheel ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_PAINT, wxPaintEventHandler( dynamical_frame_base::dynamical_window_box_on_paint ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_right_down ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_right_up ), NULL, this );
	dynamicalWindowBox->Connect( wxEVT_SIZE, wxSizeEventHandler( dynamical_frame_base::dynamical_window_box_on_size ), NULL, this );
}

dynamical_frame_base::~dynamical_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( dynamical_frame_base::dynamical_frame_on_set_focus ) );
	dynamicalWindowBox->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( dynamical_frame_base::dynamical_window_box_on_key_down ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( dynamical_frame_base::dynamical_window_box_on_key_up ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_left_down ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_left_up ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_MOTION, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_motion ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_mouse_wheel ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_PAINT, wxPaintEventHandler( dynamical_frame_base::dynamical_window_box_on_paint ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_right_down ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( dynamical_frame_base::dynamical_window_box_on_right_up ), NULL, this );
	dynamicalWindowBox->Disconnect( wxEVT_SIZE, wxSizeEventHandler( dynamical_frame_base::dynamical_window_box_on_size ), NULL, this );
	
	delete m_menu21; 
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
	m_panel9 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyGrid8 = new wxPropertyGrid(m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer31->Add( m_propertyGrid8, 1, wxEXPAND, 5 );
	
	
	m_panel9->SetSizer( bSizer31 );
	m_panel9->Layout();
	bSizer31->Fit( m_panel9 );
	m_notebook2->AddPage( m_panel9, wxT("Initial Value"), false );
	m_panel10 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer321;
	bSizer321 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyGrid9 = new wxPropertyGrid(m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	m_propertyGridItem4 = m_propertyGrid9->Append( new wxIntProperty( wxT("T Min"), wxT("T Min") ) ); 
	m_propertyGridItem5 = m_propertyGrid9->Append( new wxIntProperty( wxT("T Max"), wxT("T Max") ) ); 
	m_propertyGridItem6 = m_propertyGrid9->Append( new wxColourProperty( wxT("Color"), wxT("Color") ) ); 
	m_propertyGridItem7 = m_propertyGrid9->Append( new wxEnumProperty( wxT("Method"), wxT("Method") ) ); 
	bSizer321->Add( m_propertyGrid9, 1, wxEXPAND, 5 );
	
	
	m_panel10->SetSizer( bSizer321 );
	m_panel10->Layout();
	bSizer321->Fit( m_panel10 );
	m_notebook2->AddPage( m_panel10, wxT("Properties"), true );
	
	bSizer10->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button4 = new wxButton( this, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( m_button4, 0, wxALL, 5 );
	
	m_button5 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer32->Add( m_button5, 0, wxALL, 5 );
	
	
	bSizer10->Add( bSizer32, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer10 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

solution_dialog_base::~solution_dialog_base()
{
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

dynamical_dialog_base::~dynamical_dialog_base()
{
}

singular_point_dialog_base::singular_point_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Start Position") ), wxVERTICAL );
	
	m_propertyGrid10 = new wxPropertyGrid(sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	sbSizer6->Add( m_propertyGrid10, 1, wxEXPAND, 5 );
	
	
	bSizer33->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button20 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer34->Add( m_button20, 0, wxALL, 5 );
	
	m_button21 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer34->Add( m_button21, 0, wxALL, 5 );
	
	
	bSizer33->Add( bSizer34, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer33 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

singular_point_dialog_base::~singular_point_dialog_base()
{
}
