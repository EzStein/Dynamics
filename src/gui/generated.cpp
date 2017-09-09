///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "generated.h"

///////////////////////////////////////////////////////////////////////////

MyFrame1::MyFrame1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
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
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem3 );
	
	m_menu2->AppendSeparator();
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("MyMenuItem") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem4 );
	
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
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("x = "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALL, 5 );
	
	m_textCtrl4 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl4, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("y = "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer1->Add( m_staticText10, 0, wxALL, 5 );
	
	m_textCtrl3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl3, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("z = "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALL, 5 );
	
	m_textCtrl5 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl5, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Render"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer1->Add( m_staticText12, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Render"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_button2, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer4->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel10 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook2->AddPage( m_panel10, wxT("Differential Plane"), true );
	m_panel11 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebook2->AddPage( m_panel11, wxT("Parameter Plane"), false );
	
	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame1::onMenuSelection ) );
}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame1::onMenuSelection ) );
	
}
