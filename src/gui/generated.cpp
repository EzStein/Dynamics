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
	
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Dimension:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer3->Add( m_staticText8, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	dimensionField = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( dimensionField, 0, wxALL, 5 );
	
	m_button21 = new wxButton( this, wxID_ANY, wxT("Set"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button21, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	m_dataViewListCtrl2 = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_dataViewListCtrl2, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button2, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText91 = new wxStaticText( this, wxID_ANY, wxT("Initial Values"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	bSizer2->Add( m_staticText91, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_dataViewListCtrl1 = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_dataViewListCtrl1, 0, wxALL, 5 );
	
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
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Project:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer31->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer31->Add( m_staticText19, 0, wxALL, 5 );
	
	m_staticText15 = new wxStaticText( this, wxID_ANY, wxT("1st-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer31->Add( m_staticText15, 0, wxALL, 5 );
	
	wxArrayString axisVariable1Choices;
	axisVariable1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, axisVariable1Choices, 0 );
	axisVariable1->SetSelection( 0 );
	fgSizer31->Add( axisVariable1, 0, wxALL, 5 );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("2nd-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer31->Add( m_staticText16, 0, wxALL, 5 );
	
	wxArrayString axisVariable2Choices;
	axisVariable2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, axisVariable2Choices, 0 );
	axisVariable2->SetSelection( 0 );
	fgSizer31->Add( axisVariable2, 0, wxALL, 5 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("3rd-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer31->Add( m_staticText18, 0, wxALL, 5 );
	
	wxArrayString axisVariable3Choices;
	axisVariable3 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, axisVariable3Choices, 0 );
	axisVariable3->SetSelection( 0 );
	fgSizer31->Add( axisVariable3, 0, wxALL, 5 );
	
	
	bSizer2->Add( fgSizer31, 0, wxEXPAND, 5 );
	
	renderCheckBox = new wxCheckBox( this, wxID_ANY, wxT("3d Render!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( renderCheckBox, 0, wxALL, 5 );
	
	
	bSizer4->Add( bSizer2, 0, wxALL, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	parameterPlane = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook2->AddPage( parameterPlane, wxT("Parameter Plane"), false );
	
	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_top_frame ) );
	this->Connect( menuItemVectorField->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( top_frame_base::on_menu_selection_vector_field ) );
	m_button21->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_text_enter_dimension ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_button_click_compile ), NULL, this );
	axisVariable1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariable2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariable3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	renderCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( top_frame_base::on_3d_render_check ), NULL, this );
}

top_frame_base::~top_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_top_frame ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( top_frame_base::on_menu_selection_vector_field ) );
	m_button21->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_text_enter_dimension ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_button_click_compile ), NULL, this );
	axisVariable1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariable2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariable3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	renderCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( top_frame_base::on_3d_render_check ), NULL, this );
	
}
