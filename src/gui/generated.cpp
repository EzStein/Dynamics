///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "generated.h"

///////////////////////////////////////////////////////////////////////////

top_frame_base::top_frame_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );
	
	m_menubar1->Append( m_menu1, wxT("File") ); 
	
	m_menu2 = new wxMenu();
	wxMenuItem* menuItemVectorField;
	menuItemVectorField = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Vector Field") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( menuItemVectorField );
	
	m_menubar1->Append( m_menu2, wxT("Edit") ); 
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem5 );
	
	m_menu3->AppendSeparator();
	
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem6 );
	
	m_menubar1->Append( m_menu3, wxT("View") ); 
	
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem7 );
	
	m_menu4->AppendSeparator();
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem8 );
	
	m_menubar1->Append( m_menu4, wxT("Window") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Dimension:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer3->Add( m_staticText8, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_choice1Choices[] = { wxT("1"), wxT("2"), wxT("3") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 2 );
	bSizer3->Add( m_choice1, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer200;
	fgSizer200 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer200->AddGrowableCol( 1 );
	fgSizer200->SetFlexibleDirection( wxBOTH );
	fgSizer200->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("x' = "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer200->Add( m_staticText9, 0, wxALL, 5 );
	
	xFuncField = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer200->Add( xFuncField, 0, wxALL|wxEXPAND, 5 );
	
	yEqnLabel = new wxStaticText( this, wxID_ANY, wxT("y' = "), wxDefaultPosition, wxDefaultSize, 0 );
	yEqnLabel->Wrap( -1 );
	fgSizer200->Add( yEqnLabel, 0, wxALL, 5 );
	
	yFuncField = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer200->Add( yFuncField, 1, wxALL|wxEXPAND, 5 );
	
	zEqnLabel = new wxStaticText( this, wxID_ANY, wxT("z' ="), wxDefaultPosition, wxDefaultSize, 0 );
	zEqnLabel->Wrap( -1 );
	fgSizer200->Add( zEqnLabel, 0, wxALL, 5 );
	
	zFuncField = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer200->Add( zFuncField, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer2->Add( fgSizer200, 0, wxEXPAND, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button2, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText91 = new wxStaticText( this, wxID_ANY, wxT("Initial Values"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	bSizer2->Add( m_staticText91, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText101 = new wxStaticText( this, wxID_ANY, wxT("t ="), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	fgSizer3->Add( m_staticText101, 0, wxALL, 5 );
	
	tInitValField = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( tInitValField, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("x ="), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer3->Add( m_staticText4, 0, wxALL, 5 );
	
	xInitValField = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( xInitValField, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("y ="), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer3->Add( m_staticText5, 0, wxALL, 5 );
	
	yInitValField = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( yInitValField, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("z ="), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer3->Add( m_staticText6, 0, wxALL, 5 );
	
	zInitValField = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( zInitValField, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer2->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	m_staticText102 = new wxStaticText( this, wxID_ANY, wxT("Range"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	bSizer2->Add( m_staticText102, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer31->SetFlexibleDirection( wxBOTH );
	fgSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("T Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer31->Add( m_staticText11, 0, wxALL, 5 );
	
	tMinValField = new wxTextCtrl( this, wxID_ANY, wxT("-4"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer31->Add( tMinValField, 0, wxALL, 5 );
	
	m_staticText121 = new wxStaticText( this, wxID_ANY, wxT("T Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	fgSizer31->Add( m_staticText121, 0, wxALL, 5 );
	
	tMaxValField = new wxTextCtrl( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer31->Add( tMaxValField, 0, wxALL, 5 );
	
	m_staticText141 = new wxStaticText( this, wxID_ANY, wxT("T Increment"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	fgSizer31->Add( m_staticText141, 0, wxALL, 5 );
	
	tIncrementField = new wxTextCtrl( this, wxID_ANY, wxT("0.01"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer31->Add( tIncrementField, 0, wxALL, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Axes:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer31->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString axesChoiceChoices[] = { wxT("tx"), wxT("xy"), wxT("ty"), wxT("tz"), wxT("xt"), wxT("yt"), wxT("zt"), wxT("yz"), wxT("xz"), wxT("yx"), wxT("zy"), wxT("zx") };
	int axesChoiceNChoices = sizeof( axesChoiceChoices ) / sizeof( wxString );
	axesChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, axesChoiceNChoices, axesChoiceChoices, 0 );
	axesChoice->SetSelection( 0 );
	fgSizer31->Add( axesChoice, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer2->Add( fgSizer31, 0, 0, 5 );
	
	
	bSizer4->Add( bSizer2, 0, 0, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	dynamicalPlane = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_notebook2->AddPage( dynamicalPlane, wxT("Dynamical Plane"), true );
	parameterPlane = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook2->AddPage( parameterPlane, wxT("Parameter Plane"), false );
	glParent = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook2->AddPage( glParent, wxT("OpenGL"), false );
	
	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_top_frame ) );
	this->Connect( menuItemVectorField->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( top_frame_base::on_menu_selection_vector_field ) );
	m_choice1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_choice_dimension ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_button_click_compile ), NULL, this );
	axesChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_choice_axes ), NULL, this );
	dynamicalPlane->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( top_frame_base::on_left_down_dynamical_plane ), NULL, this );
	dynamicalPlane->Connect( wxEVT_MOTION, wxMouseEventHandler( top_frame_base::on_motion_dynamical_plane ), NULL, this );
	dynamicalPlane->Connect( wxEVT_PAINT, wxPaintEventHandler( top_frame_base::on_paint_dynamical_plane ), NULL, this );
	dynamicalPlane->Connect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_dynamical_plane ), NULL, this );
}

top_frame_base::~top_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_top_frame ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( top_frame_base::on_menu_selection_vector_field ) );
	m_choice1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_choice_dimension ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_button_click_compile ), NULL, this );
	axesChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_choice_axes ), NULL, this );
	dynamicalPlane->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( top_frame_base::on_left_down_dynamical_plane ), NULL, this );
	dynamicalPlane->Disconnect( wxEVT_MOTION, wxMouseEventHandler( top_frame_base::on_motion_dynamical_plane ), NULL, this );
	dynamicalPlane->Disconnect( wxEVT_PAINT, wxPaintEventHandler( top_frame_base::on_paint_dynamical_plane ), NULL, this );
	dynamicalPlane->Disconnect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_dynamical_plane ), NULL, this );
	
}
