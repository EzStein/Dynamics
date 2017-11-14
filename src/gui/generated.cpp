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
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook21 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_notebook21->SetMaxSize( wxSize( 300,-1 ) );
	
	m_panel3 = new wxPanel( m_notebook21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( m_panel3, wxID_ANY, wxT("Dimension:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer3->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	dimensionField = new wxTextCtrl( m_panel3, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( dimensionField, 0, wxALL, 5 );
	
	m_button21 = new wxButton( m_panel3, wxID_ANY, wxT("Set"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button21, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText151 = new wxStaticText( m_panel3, wxID_ANY, wxT("Parameters:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText151->Wrap( -1 );
	bSizer41->Add( m_staticText151, 0, wxALL, 5 );
	
	parametersField = new wxTextCtrl( m_panel3, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( parametersField, 0, wxALL, 5 );
	
	m_button3 = new wxButton( m_panel3, wxID_ANY, wxT("Set"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button3, 0, wxALL, 5 );
	
	
	bSizer2->Add( bSizer41, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("System:") ), wxVERTICAL );
	
	functionsListCtrl = new wxDataViewListCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( functionsListCtrl, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Parameters") ), wxVERTICAL );
	
	parametersListCtrl = new wxDataViewListCtrl( sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( parametersListCtrl, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	m_button2 = new wxButton( m_panel3, wxID_ANY, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button2, 0, wxALL|wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( bSizer2 );
	m_panel3->Layout();
	bSizer2->Fit( m_panel3 );
	m_notebook21->AddPage( m_panel3, wxT("System"), true );
	m_panel6 = new wxPanel( m_notebook21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel6, wxID_ANY, wxT("Initial Conditions") ), wxVERTICAL );
	
	initialValuesListCtrl = new wxDataViewListCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( initialValuesListCtrl, 1, wxEXPAND, 5 );
	
	
	bSizer10->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panel6, wxID_ANY, wxT("Time") ), wxVERTICAL );
	
	m_staticText11 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, wxT("T Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	sbSizer9->Add( m_staticText11, 0, wxALL, 5 );
	
	tMinValField = new wxTextCtrl( sbSizer9->GetStaticBox(), wxID_ANY, wxT("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( tMinValField, 0, wxALL, 5 );
	
	m_staticText121 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, wxT("T Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	sbSizer9->Add( m_staticText121, 0, wxALL, 5 );
	
	tMaxValField = new wxTextCtrl( sbSizer9->GetStaticBox(), wxID_ANY, wxT("40"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( tMaxValField, 0, wxALL, 5 );
	
	m_staticText141 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, wxT("T Increment"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	sbSizer9->Add( m_staticText141, 0, wxALL, 5 );
	
	tIncrementField = new wxTextCtrl( sbSizer9->GetStaticBox(), wxID_ANY, wxT("0.001"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer9->Add( tIncrementField, 0, wxALL, 5 );
	
	
	bSizer10->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer10 );
	m_panel6->Layout();
	bSizer10->Fit( m_panel6 );
	m_notebook21->AddPage( m_panel6, wxT("Trajectory"), false );
	m_panel4 = new wxPanel( m_notebook21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Dynamical Plane") ), wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText15 = new wxStaticText( sbSizer12->GetStaticBox(), wxID_ANY, wxT("1st-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer11->Add( m_staticText15, 1, wxALL, 5 );
	
	wxString axisVariableChoice1Choices[] = { wxT("t"), wxT("x"), wxT("y"), wxT("z") };
	int axisVariableChoice1NChoices = sizeof( axisVariableChoice1Choices ) / sizeof( wxString );
	axisVariableChoice1 = new wxChoice( sbSizer12->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, axisVariableChoice1NChoices, axisVariableChoice1Choices, 0 );
	axisVariableChoice1->SetSelection( 1 );
	bSizer11->Add( axisVariableChoice1, 1, wxALL, 5 );
	
	
	sbSizer12->Add( bSizer11, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( sbSizer12->GetStaticBox(), wxID_ANY, wxT("2nd-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer14->Add( m_staticText16, 1, wxALL, 5 );
	
	wxString axisVariableChoice2Choices[] = { wxT("t"), wxT("x"), wxT("y"), wxT("z") };
	int axisVariableChoice2NChoices = sizeof( axisVariableChoice2Choices ) / sizeof( wxString );
	axisVariableChoice2 = new wxChoice( sbSizer12->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, axisVariableChoice2NChoices, axisVariableChoice2Choices, 0 );
	axisVariableChoice2->SetSelection( 2 );
	bSizer14->Add( axisVariableChoice2, 1, wxALL, 5 );
	
	
	sbSizer12->Add( bSizer14, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText18 = new wxStaticText( sbSizer12->GetStaticBox(), wxID_ANY, wxT("3rd-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	bSizer15->Add( m_staticText18, 1, wxALL, 5 );
	
	wxString axisVariableChoice3Choices[] = { wxT("t"), wxT("x"), wxT("y"), wxT("z") };
	int axisVariableChoice3NChoices = sizeof( axisVariableChoice3Choices ) / sizeof( wxString );
	axisVariableChoice3 = new wxChoice( sbSizer12->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, axisVariableChoice3NChoices, axisVariableChoice3Choices, 0 );
	axisVariableChoice3->SetSelection( 3 );
	bSizer15->Add( axisVariableChoice3, 1, wxALL, 5 );
	
	
	sbSizer12->Add( bSizer15, 1, wxEXPAND, 5 );
	
	
	bSizer7->Add( sbSizer12, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Parameter Plane") ), wxVERTICAL );
	
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText152 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, wxT("1st-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText152->Wrap( -1 );
	bSizer111->Add( m_staticText152, 1, wxALL, 5 );
	
	wxString parameterAxisVariableChoice1Choices[] = { wxT("a"), wxT("b"), wxT("c") };
	int parameterAxisVariableChoice1NChoices = sizeof( parameterAxisVariableChoice1Choices ) / sizeof( wxString );
	parameterAxisVariableChoice1 = new wxChoice( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, parameterAxisVariableChoice1NChoices, parameterAxisVariableChoice1Choices, 0 );
	parameterAxisVariableChoice1->SetSelection( 0 );
	bSizer111->Add( parameterAxisVariableChoice1, 1, wxALL, 5 );
	
	
	sbSizer10->Add( bSizer111, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer112;
	bSizer112 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText153 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, wxT("1nd-axis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText153->Wrap( -1 );
	bSizer112->Add( m_staticText153, 1, wxALL, 5 );
	
	wxString parameterAxisVariableChoice2Choices[] = { wxT("a"), wxT("b"), wxT("c") };
	int parameterAxisVariableChoice2NChoices = sizeof( parameterAxisVariableChoice2Choices ) / sizeof( wxString );
	parameterAxisVariableChoice2 = new wxChoice( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, parameterAxisVariableChoice2NChoices, parameterAxisVariableChoice2Choices, 0 );
	parameterAxisVariableChoice2->SetSelection( 1 );
	bSizer112->Add( parameterAxisVariableChoice2, 1, wxALL, 5 );
	
	
	sbSizer10->Add( bSizer112, 1, wxEXPAND, 5 );
	
	
	bSizer7->Add( sbSizer10, 1, wxEXPAND, 5 );
	
	renderCheckBox = new wxCheckBox( m_panel4, wxID_ANY, wxT("3d Render!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( renderCheckBox, 0, wxALL, 5 );
	
	
	m_panel4->SetSizer( bSizer7 );
	m_panel4->Layout();
	bSizer7->Fit( m_panel4 );
	m_notebook21->AddPage( m_panel4, wxT("View"), false );
	
	bSizer9->Add( m_notebook21, 1, wxEXPAND | wxALL, 5 );
	
	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer9->Add( m_panel5, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer4->Add( bSizer9, 0, wxEXPAND, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_top_frame ) );
	this->Connect( menuItemVectorField->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( top_frame_base::on_menu_selection_vector_field ) );
	m_button21->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_text_enter_dimension ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_text_enter_parameters ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_button_click_compile ), NULL, this );
	axisVariableChoice1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariableChoice2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariableChoice3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	parameterAxisVariableChoice1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_parameter_axis_choice ), NULL, this );
	parameterAxisVariableChoice2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_parameter_axis_choice ), NULL, this );
	renderCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( top_frame_base::on_3d_render_check ), NULL, this );
}

top_frame_base::~top_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( top_frame_base::on_size_top_frame ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( top_frame_base::on_menu_selection_vector_field ) );
	m_button21->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_text_enter_dimension ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_text_enter_parameters ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( top_frame_base::on_button_click_compile ), NULL, this );
	axisVariableChoice1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariableChoice2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	axisVariableChoice3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_axis_choice ), NULL, this );
	parameterAxisVariableChoice1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_parameter_axis_choice ), NULL, this );
	parameterAxisVariableChoice2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( top_frame_base::on_parameter_axis_choice ), NULL, this );
	renderCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( top_frame_base::on_3d_render_check ), NULL, this );
	
}
