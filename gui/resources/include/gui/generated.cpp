///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
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

	isoclineMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Isocline...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( isoclineMenuItem );

	separatricesMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Separatrices...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( separatricesMenuItem );

	m_menuItem33 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Draw All Separatrices") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem33 );

	m_menu2->AppendSeparator();

	hopfBifurcationMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Hopf Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( hopfBifurcationMenuItem );

	saddleNodeBifurcationMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Saddle Node Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( saddleNodeBifurcationMenuItem );

	limitCycleBifurcationMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Semi-stable Limit Cycle Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( limitCycleBifurcationMenuItem );

	saddleConnectionBifurcationMenuItem = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Saddle Connection Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( saddleConnectionBifurcationMenuItem );

	m_menubar1->Append( m_menu2, wxT("Objects") );

	m_menu4 = new wxMenu();
	newDynamicalMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("New Dynamical...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( newDynamicalMenuItem );

	newParameterMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("New Parameter...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( newParameterMenuItem );

	m_menu4->AppendSeparator();

	closeDynamicalMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Close Dynamical Windows...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( closeDynamicalMenuItem );

	closeParameterMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Close Parameter Windows...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( closeParameterMenuItem );

	m_menubar1->Append( m_menu4, wxT("Windows") );

	m_menu8 = new wxMenu();
	wxMenuItem* lorenzExampleMenuItem;
	lorenzExampleMenuItem = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Lorenz System") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( lorenzExampleMenuItem );

	wxMenuItem* vanDerPolExampleMenuItem;
	vanDerPolExampleMenuItem = new wxMenuItem( m_menu8, wxID_ANY, wxString( wxT("Van der Pol Oscillator") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu8->Append( vanDerPolExampleMenuItem );

	m_menubar1->Append( m_menu8, wxT("Examples") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_notebook4 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer183;
	bSizer183 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer41;
	sbSizer41 = new wxStaticBoxSizer( new wxStaticBox( bSizer18, wxID_ANY, wxT("Equations") ), wxVERTICAL );

	equationsDataViewCtrl = new wxDataViewListCtrl( sbSizer41->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn16 = equationsDataViewCtrl->AppendTextColumn( wxT("Variable"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(0), 0 );
	sbSizer41->Add( equationsDataViewCtrl, 1, wxEXPAND, 5 );


	bSizer183->Add( sbSizer41, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer42;
	sbSizer42 = new wxStaticBoxSizer( new wxStaticBox( bSizer18, wxID_ANY, wxT("Variables") ), wxVERTICAL );

	variablesTextCtrl = new wxTextCtrl( sbSizer42->GetStaticBox(), wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	sbSizer42->Add( variablesTextCtrl, 0, wxALL, 5 );

	variableNamesDataViewCtrl = new wxDataViewListCtrl( sbSizer42->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn5 = variableNamesDataViewCtrl->AppendTextColumn( wxT("Id"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(0), 0 );
	sbSizer42->Add( variableNamesDataViewCtrl, 1, wxEXPAND, 5 );


	bSizer183->Add( sbSizer42, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( bSizer18, wxID_ANY, wxT("Parameters") ), wxVERTICAL );

	parametersTextCtrl = new wxTextCtrl( sbSizer6->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	sbSizer6->Add( parametersTextCtrl, 0, wxALL, 5 );

	parameterNamesDataViewCtrl = new wxDataViewListCtrl( sbSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn51 = parameterNamesDataViewCtrl->AppendTextColumn( wxT("Id"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(0), 0 );
	sbSizer6->Add( parameterNamesDataViewCtrl, 1, wxEXPAND, 5 );


	bSizer183->Add( sbSizer6, 1, wxEXPAND, 5 );

	compileButton = new wxButton( bSizer18, wxID_ANY, wxT("Compile"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer183->Add( compileButton, 0, wxALL, 5 );

	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( bSizer18, wxID_ANY, wxT("Variable of Differentiation: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer33->Add( m_staticText3, 0, wxALL, 5 );

	varDiffTextCtrl = new wxTextCtrl( bSizer18, wxID_ANY, wxT("t"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer33->Add( varDiffTextCtrl, 0, wxALL, 5 );


	bSizer183->Add( bSizer33, 0, wxEXPAND, 5 );


	bSizer18->SetSizer( bSizer183 );
	bSizer18->Layout();
	bSizer183->Fit( bSizer18 );
	m_notebook4->AddPage( bSizer18, wxT("Staging"), true );
	m_panel13 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel13, wxID_ANY, wxT("Parameters") ), wxVERTICAL );

	compiledParametersDataViewCtrl = new wxDataViewListCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn3 = compiledParametersDataViewCtrl->AppendTextColumn( wxT("Parameter"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	sbSizer4->Add( compiledParametersDataViewCtrl, 1, wxEXPAND, 5 );


	bSizer34->Add( sbSizer4, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panel13, wxID_ANY, wxT("Equations") ), wxVERTICAL );

	compiledEquationsDataViewCtrl = new wxDataViewListCtrl( sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn1 = compiledEquationsDataViewCtrl->AppendTextColumn( wxT("Variable"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	m_dataViewListColumn17 = compiledEquationsDataViewCtrl->AppendTextColumn( wxT("Expression"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	sbSizer5->Add( compiledEquationsDataViewCtrl, 1, wxEXPAND, 5 );


	bSizer34->Add( sbSizer5, 1, wxEXPAND, 5 );


	m_panel13->SetSizer( bSizer34 );
	m_panel13->Layout();
	bSizer34->Fit( m_panel13 );
	m_notebook4->AddPage( m_panel13, wxT("Compiled"), false );

	bSizer4->Add( m_notebook4, 1, wxEXPAND | wxALL, 5 );

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

	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( m_panel5, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid1 = new wxGrid( sbSizer61->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid1->CreateGrid( 0, 0 );
	m_grid1->EnableEditing( false );
	m_grid1->EnableGridLines( true );
	m_grid1->EnableDragGridSize( false );
	m_grid1->SetMargins( 0, 0 );

	// Columns
	m_grid1->EnableDragColMove( false );
	m_grid1->EnableDragColSize( true );
	m_grid1->SetColLabelSize( 30 );
	m_grid1->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid1->EnableDragRowSize( true );
	m_grid1->SetRowLabelSize( 80 );
	m_grid1->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid1->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer61->Add( m_grid1, 1, wxEXPAND, 5 );


	bSizer182->Add( sbSizer61, 1, wxEXPAND, 5 );


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

	m_button19 = new wxButton( m_panel6, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_button19, 0, wxALL, 5 );

	singularPointsDeleteButton = new wxButton( m_panel6, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( singularPointsDeleteButton, 0, wxALL, 5 );


	bSizer20->Add( bSizer19, 0, wxEXPAND, 5 );


	m_panel6->SetSizer( bSizer20 );
	m_panel6->Layout();
	bSizer20->Fit( m_panel6 );
	m_notebook2->AddPage( m_panel6, wxT("Singular Points"), false );
	m_panel14 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );

	separatricesDataViewCtrl = new wxDataViewListCtrl( m_panel14, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_dataViewListColumn7 = separatricesDataViewCtrl->AppendTextColumn( wxT("Id"), wxDATAVIEW_CELL_INERT, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE );
	bSizer37->Add( separatricesDataViewCtrl, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer371;
	bSizer371 = new wxBoxSizer( wxHORIZONTAL );

	m_button20 = new wxButton( m_panel14, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer371->Add( m_button20, 0, wxALL, 5 );

	m_button21 = new wxButton( m_panel14, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer371->Add( m_button21, 0, wxALL, 5 );


	bSizer37->Add( bSizer371, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer611;
	sbSizer611 = new wxStaticBoxSizer( new wxStaticBox( m_panel14, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid11 = new wxGrid( sbSizer611->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid11->CreateGrid( 0, 0 );
	m_grid11->EnableEditing( false );
	m_grid11->EnableGridLines( true );
	m_grid11->EnableDragGridSize( false );
	m_grid11->SetMargins( 0, 0 );

	// Columns
	m_grid11->EnableDragColMove( false );
	m_grid11->EnableDragColSize( true );
	m_grid11->SetColLabelSize( 30 );
	m_grid11->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid11->EnableDragRowSize( true );
	m_grid11->SetRowLabelSize( 80 );
	m_grid11->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid11->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer611->Add( m_grid11, 1, wxEXPAND, 5 );


	bSizer37->Add( sbSizer611, 1, wxEXPAND, 5 );


	m_panel14->SetSizer( bSizer37 );
	m_panel14->Layout();
	bSizer37->Fit( m_panel14 );
	m_notebook2->AddPage( m_panel14, wxT("Separatrices"), false );
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

	wxStaticBoxSizer* sbSizer612;
	sbSizer612 = new wxStaticBoxSizer( new wxStaticBox( m_panel7, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid12 = new wxGrid( sbSizer612->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid12->CreateGrid( 0, 0 );
	m_grid12->EnableEditing( false );
	m_grid12->EnableGridLines( true );
	m_grid12->EnableDragGridSize( false );
	m_grid12->SetMargins( 0, 0 );

	// Columns
	m_grid12->EnableDragColMove( false );
	m_grid12->EnableDragColSize( true );
	m_grid12->SetColLabelSize( 30 );
	m_grid12->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid12->EnableDragRowSize( true );
	m_grid12->SetRowLabelSize( 80 );
	m_grid12->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid12->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer612->Add( m_grid12, 1, wxEXPAND, 5 );


	bSizer191->Add( sbSizer612, 1, wxEXPAND, 5 );


	m_panel7->SetSizer( bSizer191 );
	m_panel7->Layout();
	bSizer191->Fit( m_panel7 );
	m_notebook2->AddPage( m_panel7, wxT("Isoclines"), false );
	m_panel15 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxVERTICAL );

	hopfBifurcationsDataViewCtrl = new wxDataViewListCtrl( m_panel15, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( hopfBifurcationsDataViewCtrl, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxHORIZONTAL );

	m_button22 = new wxButton( m_panel15, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( m_button22, 0, wxALL, 5 );

	m_button23 = new wxButton( m_panel15, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer39->Add( m_button23, 0, wxALL, 5 );


	bSizer38->Add( bSizer39, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer613;
	sbSizer613 = new wxStaticBoxSizer( new wxStaticBox( m_panel15, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid13 = new wxGrid( sbSizer613->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid13->CreateGrid( 0, 0 );
	m_grid13->EnableEditing( false );
	m_grid13->EnableGridLines( true );
	m_grid13->EnableDragGridSize( false );
	m_grid13->SetMargins( 0, 0 );

	// Columns
	m_grid13->EnableDragColMove( false );
	m_grid13->EnableDragColSize( true );
	m_grid13->SetColLabelSize( 30 );
	m_grid13->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid13->EnableDragRowSize( true );
	m_grid13->SetRowLabelSize( 80 );
	m_grid13->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid13->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer613->Add( m_grid13, 1, wxEXPAND, 5 );


	bSizer38->Add( sbSizer613, 1, wxEXPAND, 5 );


	m_panel15->SetSizer( bSizer38 );
	m_panel15->Layout();
	bSizer38->Fit( m_panel15 );
	m_notebook2->AddPage( m_panel15, wxT("Hopf Bifurcations"), false );
	m_panel16 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxVERTICAL );

	saddleNodeBifurcationsDataViewCtrl = new wxDataViewListCtrl( m_panel16, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer40->Add( saddleNodeBifurcationsDataViewCtrl, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxHORIZONTAL );

	m_button24 = new wxButton( m_panel16, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button24, 0, wxALL, 5 );

	m_button25 = new wxButton( m_panel16, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer41->Add( m_button25, 0, wxALL, 5 );


	bSizer40->Add( bSizer41, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer614;
	sbSizer614 = new wxStaticBoxSizer( new wxStaticBox( m_panel16, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid14 = new wxGrid( sbSizer614->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid14->CreateGrid( 0, 0 );
	m_grid14->EnableEditing( false );
	m_grid14->EnableGridLines( true );
	m_grid14->EnableDragGridSize( false );
	m_grid14->SetMargins( 0, 0 );

	// Columns
	m_grid14->EnableDragColMove( false );
	m_grid14->EnableDragColSize( true );
	m_grid14->SetColLabelSize( 30 );
	m_grid14->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid14->EnableDragRowSize( true );
	m_grid14->SetRowLabelSize( 80 );
	m_grid14->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid14->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer614->Add( m_grid14, 1, wxEXPAND, 5 );


	bSizer40->Add( sbSizer614, 1, wxEXPAND, 5 );


	m_panel16->SetSizer( bSizer40 );
	m_panel16->Layout();
	bSizer40->Fit( m_panel16 );
	m_notebook2->AddPage( m_panel16, wxT("Saddle-Node Bifurcations"), true );
	m_panel161 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer401;
	bSizer401 = new wxBoxSizer( wxVERTICAL );

	limitCycleBifurcationsDataViewCtrl = new wxDataViewListCtrl( m_panel161, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer401->Add( limitCycleBifurcationsDataViewCtrl, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer411;
	bSizer411 = new wxBoxSizer( wxHORIZONTAL );

	m_button241 = new wxButton( m_panel161, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer411->Add( m_button241, 0, wxALL, 5 );

	m_button251 = new wxButton( m_panel161, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer411->Add( m_button251, 0, wxALL, 5 );


	bSizer401->Add( bSizer411, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer6141;
	sbSizer6141 = new wxStaticBoxSizer( new wxStaticBox( m_panel161, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid141 = new wxGrid( sbSizer6141->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid141->CreateGrid( 0, 0 );
	m_grid141->EnableEditing( false );
	m_grid141->EnableGridLines( true );
	m_grid141->EnableDragGridSize( false );
	m_grid141->SetMargins( 0, 0 );

	// Columns
	m_grid141->EnableDragColMove( false );
	m_grid141->EnableDragColSize( true );
	m_grid141->SetColLabelSize( 30 );
	m_grid141->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid141->EnableDragRowSize( true );
	m_grid141->SetRowLabelSize( 80 );
	m_grid141->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid141->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer6141->Add( m_grid141, 1, wxEXPAND, 5 );


	bSizer401->Add( sbSizer6141, 1, wxEXPAND, 5 );


	m_panel161->SetSizer( bSizer401 );
	m_panel161->Layout();
	bSizer401->Fit( m_panel161 );
	m_notebook2->AddPage( m_panel161, wxT("Limit Cycle Bifurcations"), false );
	m_panel162 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer402;
	bSizer402 = new wxBoxSizer( wxVERTICAL );

	saddleConnectionBifurcationsDataViewCtrl = new wxDataViewListCtrl( m_panel162, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer402->Add( saddleConnectionBifurcationsDataViewCtrl, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer412;
	bSizer412 = new wxBoxSizer( wxHORIZONTAL );

	m_button242 = new wxButton( m_panel162, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer412->Add( m_button242, 0, wxALL, 5 );

	m_button252 = new wxButton( m_panel162, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer412->Add( m_button252, 0, wxALL, 5 );


	bSizer402->Add( bSizer412, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer6142;
	sbSizer6142 = new wxStaticBoxSizer( new wxStaticBox( m_panel162, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid142 = new wxGrid( sbSizer6142->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid142->CreateGrid( 0, 0 );
	m_grid142->EnableEditing( false );
	m_grid142->EnableGridLines( true );
	m_grid142->EnableDragGridSize( false );
	m_grid142->SetMargins( 0, 0 );

	// Columns
	m_grid142->EnableDragColMove( false );
	m_grid142->EnableDragColSize( true );
	m_grid142->SetColLabelSize( 30 );
	m_grid142->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid142->EnableDragRowSize( true );
	m_grid142->SetRowLabelSize( 80 );
	m_grid142->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid142->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer6142->Add( m_grid142, 1, wxEXPAND, 5 );


	bSizer402->Add( sbSizer6142, 1, wxEXPAND, 5 );


	m_panel162->SetSizer( bSizer402 );
	m_panel162->Layout();
	bSizer402->Fit( m_panel162 );
	m_notebook2->AddPage( m_panel162, wxT("Saddle Connection Bifurcations"), false );
	m_panel163 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer403;
	bSizer403 = new wxBoxSizer( wxVERTICAL );

	periodicSolutionBifurcationsDataViewCtrl = new wxDataViewListCtrl( m_panel163, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer403->Add( periodicSolutionBifurcationsDataViewCtrl, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer413;
	bSizer413 = new wxBoxSizer( wxHORIZONTAL );

	m_button243 = new wxButton( m_panel163, wxID_ANY, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer413->Add( m_button243, 0, wxALL, 5 );

	m_button253 = new wxButton( m_panel163, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer413->Add( m_button253, 0, wxALL, 5 );


	bSizer403->Add( bSizer413, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer6143;
	sbSizer6143 = new wxStaticBoxSizer( new wxStaticBox( m_panel163, wxID_ANY, wxT("Data") ), wxVERTICAL );

	m_grid143 = new wxGrid( sbSizer6143->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid143->CreateGrid( 0, 0 );
	m_grid143->EnableEditing( false );
	m_grid143->EnableGridLines( true );
	m_grid143->EnableDragGridSize( false );
	m_grid143->SetMargins( 0, 0 );

	// Columns
	m_grid143->EnableDragColMove( false );
	m_grid143->EnableDragColSize( true );
	m_grid143->SetColLabelSize( 30 );
	m_grid143->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid143->EnableDragRowSize( true );
	m_grid143->SetRowLabelSize( 80 );
	m_grid143->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid143->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	sbSizer6143->Add( m_grid143, 1, wxEXPAND, 5 );


	bSizer403->Add( sbSizer6143, 1, wxEXPAND, 5 );


	m_panel163->SetSizer( bSizer403 );
	m_panel163->Layout();
	bSizer403->Fit( m_panel163 );
	m_notebook2->AddPage( m_panel163, wxT("Periodic Solution"), false );

	bSizer4->Add( m_notebook2, 2, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer4 );
	this->Layout();
	m_menu22 = new wxMenu();
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( console_frame_base::console_frame_baseOnContextMenu ), NULL, this );


	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( console_frame_base::console_frame_on_close ) );
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_menu_item_on_menu_selection ), this, closeMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::solution_menu_item_on_menu_selection ), this, solutionMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::singular_point_menu_item_on_menu_selection ), this, singularPointMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::isocline_menu_item_on_menu_selection ), this, isoclineMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::separatrices_menu_item_on_menu_selection ), this, separatricesMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::draw_all_separatrices_menu_item_on_menu_selection ), this, m_menuItem33->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::hopf_bifurcation_menu_item_on_selection ), this, hopfBifurcationMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::saddle_node_bifurcation_menu_item_on_selection ), this, saddleNodeBifurcationMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::limit_cycle_bifurcation_menu_item_on_selection ), this, limitCycleBifurcationMenuItem->GetId());
	m_menu2->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::saddle_connection_bifurcation_menu_item_on_selection ), this, saddleConnectionBifurcationMenuItem->GetId());
	m_menu4->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::new_dynamical_menu_item_on_selection ), this, newDynamicalMenuItem->GetId());
	m_menu4->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::new_parameter_menu_item_on_selection ), this, newParameterMenuItem->GetId());
	m_menu4->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_dynamical_menu_item_on_selection ), this, closeDynamicalMenuItem->GetId());
	m_menu4->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::close_parameter_menu_item_on_selection ), this, closeParameterMenuItem->GetId());
	m_menu8->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::lorenz_example_menu_item_on_menu_selection ), this, lorenzExampleMenuItem->GetId());
	m_menu8->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( console_frame_base::van_der_pol_example_on_menu_selection ), this, vanDerPolExampleMenuItem->GetId());
	equationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemActivated ), NULL, this );
	equationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemEditingDone ), NULL, this );
	equationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_STARTED, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemEditingStarted ), NULL, this );
	equationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_START_EDITING, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemStartEditing ), NULL, this );
	equationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlSelectionChanged ), NULL, this );
	variablesTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::variables_text_ctrl_on_text_enter ), NULL, this );
	variableNamesDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( console_frame_base::variable_names_data_view_ctrl_on_value_changed ), NULL, this );
	parametersTextCtrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::parameters_text_ctrl_on_text_enter ), NULL, this );
	compileButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
	solutionsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::solutions_data_view_ctrl_on_selection_changed ), NULL, this );
	solutionsEditButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_edit_button_on_button_click ), NULL, this );
	solutionsDeleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_delete_button_on_button_click ), NULL, this );
	singularPointsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::singular_points_data_view_ctrl_on_selection_changed ), NULL, this );
	singularPointsDeleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::singular_points_delete_button_on_button_click ), NULL, this );
	separatricesDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::separatrices_data_view_ctrl_on_selection_changed ), NULL, this );
	isoclinesDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::isoclines_data_view_ctrl_on_selection_changed ), NULL, this );
	isoclinesEditButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_edit_button_on_button_click ), NULL, this );
	isoclinesDeleteButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_delete_button_on_button_click ), NULL, this );
	hopfBifurcationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::hopf_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	saddleNodeBifurcationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	limitCycleBifurcationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	saddleConnectionBifurcationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	periodicSolutionBifurcationsDataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
}

console_frame_base::~console_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( console_frame_base::console_frame_on_close ) );
	equationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemActivated ), NULL, this );
	equationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemEditingDone ), NULL, this );
	equationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_STARTED, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemEditingStarted ), NULL, this );
	equationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_START_EDITING, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlItemStartEditing ), NULL, this );
	equationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::equationsDataViewCtrlOnDataViewListCtrlSelectionChanged ), NULL, this );
	variablesTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::variables_text_ctrl_on_text_enter ), NULL, this );
	variableNamesDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED, wxDataViewEventHandler( console_frame_base::variable_names_data_view_ctrl_on_value_changed ), NULL, this );
	parametersTextCtrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( console_frame_base::parameters_text_ctrl_on_text_enter ), NULL, this );
	compileButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::compile_button_on_button_click ), NULL, this );
	solutionsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::solutions_data_view_ctrl_on_selection_changed ), NULL, this );
	solutionsEditButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_edit_button_on_button_click ), NULL, this );
	solutionsDeleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::solutions_delete_button_on_button_click ), NULL, this );
	singularPointsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::singular_points_data_view_ctrl_on_selection_changed ), NULL, this );
	singularPointsDeleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::singular_points_delete_button_on_button_click ), NULL, this );
	separatricesDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::separatrices_data_view_ctrl_on_selection_changed ), NULL, this );
	isoclinesDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::isoclines_data_view_ctrl_on_selection_changed ), NULL, this );
	isoclinesEditButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_edit_button_on_button_click ), NULL, this );
	isoclinesDeleteButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( console_frame_base::isoclines_delete_button_on_button_click ), NULL, this );
	hopfBifurcationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::hopf_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	saddleNodeBifurcationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	limitCycleBifurcationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	saddleConnectionBifurcationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );
	periodicSolutionBifurcationsDataViewCtrl->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( console_frame_base::saddle_node_bifurcations_data_view_ctrl_on_selection_changed ), NULL, this );

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

	canvas = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer34s->Add( canvas, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer34s );
	this->Layout();
	m_statusBar2 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_menu4 = new wxMenu();
	editMenuItem = new wxMenuItem( m_menu4, wxID_ANY, wxString( wxT("Edit...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( editMenuItem );

	m_menubar1->Append( m_menu4, wxT("View...") );

	this->SetMenuBar( m_menubar1 );


	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dynamical_frame_base::dynamical_frame_on_close ) );
	this->Connect( wxEVT_ICONIZE, wxIconizeEventHandler( dynamical_frame_base::dynamical_frame_on_iconize ) );
	this->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( dynamical_frame_base::dynamical_frame_on_set_focus ) );
	selectionPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_select_menu_item_on_menu_selection ), this, selectionSelectMenuItem->GetId());
	selectionPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_delete_menu_item_on_menu_selection ), this, selectionDeleteMenuItem->GetId());
	selectionPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::selection_edit_menu_item_on_menu_selection ), this, selectionEditMenuItem->GetId());
	objectsPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::solution_menu_item_on_menu_selection ), this, solutionMenuItem->GetId());
	objectsPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::singular_point_menu_item_on_menu_selection ), this, singularPointMenuItem->GetId());
	objectsPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::isocline_menu_item_on_menu_selection ), this, isoclineMenuItem->GetId());
	canvas->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( dynamical_frame_base::canvas_on_key_down ), NULL, this );
	canvas->Connect( wxEVT_KEY_UP, wxKeyEventHandler( dynamical_frame_base::canvas_on_key_up ), NULL, this );
	canvas->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( dynamical_frame_base::canvas_on_left_down ), NULL, this );
	canvas->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( dynamical_frame_base::canvas_on_left_up ), NULL, this );
	canvas->Connect( wxEVT_MOTION, wxMouseEventHandler( dynamical_frame_base::canvas_on_motion ), NULL, this );
	canvas->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( dynamical_frame_base::canvas_on_mouse_wheel ), NULL, this );
	canvas->Connect( wxEVT_PAINT, wxPaintEventHandler( dynamical_frame_base::canvas_on_paint ), NULL, this );
	canvas->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::canvas_on_right_down ), NULL, this );
	canvas->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( dynamical_frame_base::canvas_on_right_up ), NULL, this );
	canvas->Connect( wxEVT_SIZE, wxSizeEventHandler( dynamical_frame_base::canvas_on_size ), NULL, this );
	m_menu4->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dynamical_frame_base::edit_menu_item_on_menu_selection ), this, editMenuItem->GetId());
}

dynamical_frame_base::~dynamical_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dynamical_frame_base::dynamical_frame_on_close ) );
	this->Disconnect( wxEVT_ICONIZE, wxIconizeEventHandler( dynamical_frame_base::dynamical_frame_on_iconize ) );
	this->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( dynamical_frame_base::dynamical_frame_on_set_focus ) );
	canvas->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( dynamical_frame_base::canvas_on_key_down ), NULL, this );
	canvas->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( dynamical_frame_base::canvas_on_key_up ), NULL, this );
	canvas->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( dynamical_frame_base::canvas_on_left_down ), NULL, this );
	canvas->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( dynamical_frame_base::canvas_on_left_up ), NULL, this );
	canvas->Disconnect( wxEVT_MOTION, wxMouseEventHandler( dynamical_frame_base::canvas_on_motion ), NULL, this );
	canvas->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( dynamical_frame_base::canvas_on_mouse_wheel ), NULL, this );
	canvas->Disconnect( wxEVT_PAINT, wxPaintEventHandler( dynamical_frame_base::canvas_on_paint ), NULL, this );
	canvas->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dynamical_frame_base::canvas_on_right_down ), NULL, this );
	canvas->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( dynamical_frame_base::canvas_on_right_up ), NULL, this );
	canvas->Disconnect( wxEVT_SIZE, wxSizeEventHandler( dynamical_frame_base::canvas_on_size ), NULL, this );

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

	m_menubar3->Append( m_menu9, wxT("View...") );

	this->SetMenuBar( m_menubar3 );

	m_statusBar3 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );

	m_panel11 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel11->Hide();

	parameterPopupMenu = new wxMenu();
	wxMenuItem* hopfBifurcationMenuItem;
	hopfBifurcationMenuItem = new wxMenuItem( parameterPopupMenu, wxID_ANY, wxString( wxT("Hopf Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	parameterPopupMenu->Append( hopfBifurcationMenuItem );

	wxMenuItem* saddleNodeBifurcationMenuItem;
	saddleNodeBifurcationMenuItem = new wxMenuItem( parameterPopupMenu, wxID_ANY, wxString( wxT("Saddle Node Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	parameterPopupMenu->Append( saddleNodeBifurcationMenuItem );

	wxMenuItem* limitCycleBifurcationMenuItem;
	limitCycleBifurcationMenuItem = new wxMenuItem( parameterPopupMenu, wxID_ANY, wxString( wxT("Semi-stable Limit Cycle Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	parameterPopupMenu->Append( limitCycleBifurcationMenuItem );

	wxMenuItem* saddleConnectionBifurcationMenuItem;
	saddleConnectionBifurcationMenuItem = new wxMenuItem( parameterPopupMenu, wxID_ANY, wxString( wxT("Saddle Connection Bifurcation...") ) , wxEmptyString, wxITEM_NORMAL );
	parameterPopupMenu->Append( saddleConnectionBifurcationMenuItem );

	m_panel11->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( parameter_frame_base::m_panel11OnContextMenu ), NULL, this );

	bSizer27->Add( m_panel11, 1, wxEXPAND | wxALL, 5 );


	bSizer26->Add( bSizer27, 1, wxEXPAND, 5 );

	canvas = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer26->Add( canvas, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer26 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( parameter_frame_base::parameter_frame_on_close ) );
	m_menu9->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( parameter_frame_base::edit_menu_item_on_menu_selection ), this, editMenuItem->GetId());
	parameterPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( parameter_frame_base::hopf_bifurcation_menu_item_on_selection ), this, hopfBifurcationMenuItem->GetId());
	parameterPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( parameter_frame_base::saddle_node_bifurcation_menu_item_on_selection ), this, saddleNodeBifurcationMenuItem->GetId());
	parameterPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( parameter_frame_base::limit_cycle_bifurcation_menu_item_on_selection ), this, limitCycleBifurcationMenuItem->GetId());
	parameterPopupMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( parameter_frame_base::saddle_connection_bifurcation_menu_item_on_selection ), this, saddleConnectionBifurcationMenuItem->GetId());
	canvas->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( parameter_frame_base::canvas_on_key_down ), NULL, this );
	canvas->Connect( wxEVT_KEY_UP, wxKeyEventHandler( parameter_frame_base::canvas_on_key_up ), NULL, this );
	canvas->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( parameter_frame_base::canvas_on_left_down ), NULL, this );
	canvas->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( parameter_frame_base::canvas_on_left_up ), NULL, this );
	canvas->Connect( wxEVT_MOTION, wxMouseEventHandler( parameter_frame_base::canvas_on_motion ), NULL, this );
	canvas->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( parameter_frame_base::canvas_on_mouse_wheel ), NULL, this );
	canvas->Connect( wxEVT_PAINT, wxPaintEventHandler( parameter_frame_base::canvas_on_paint ), NULL, this );
	canvas->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( parameter_frame_base::canvas_on_right_down ), NULL, this );
	canvas->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( parameter_frame_base::canvas_on_right_up ), NULL, this );
	canvas->Connect( wxEVT_SIZE, wxSizeEventHandler( parameter_frame_base::canvas_on_size ), NULL, this );
}

parameter_frame_base::~parameter_frame_base()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( parameter_frame_base::parameter_frame_on_close ) );
	canvas->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( parameter_frame_base::canvas_on_key_down ), NULL, this );
	canvas->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( parameter_frame_base::canvas_on_key_up ), NULL, this );
	canvas->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( parameter_frame_base::canvas_on_left_down ), NULL, this );
	canvas->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( parameter_frame_base::canvas_on_left_up ), NULL, this );
	canvas->Disconnect( wxEVT_MOTION, wxMouseEventHandler( parameter_frame_base::canvas_on_motion ), NULL, this );
	canvas->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( parameter_frame_base::canvas_on_mouse_wheel ), NULL, this );
	canvas->Disconnect( wxEVT_PAINT, wxPaintEventHandler( parameter_frame_base::canvas_on_paint ), NULL, this );
	canvas->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( parameter_frame_base::canvas_on_right_down ), NULL, this );
	canvas->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( parameter_frame_base::canvas_on_right_up ), NULL, this );
	canvas->Disconnect( wxEVT_SIZE, wxSizeEventHandler( parameter_frame_base::canvas_on_size ), NULL, this );

	delete parameterPopupMenu;
}

parameter_dialog_base::parameter_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );

	parameterPropertyGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer30->Add( parameterPropertyGrid, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );

	okButton = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( okButton, 0, wxALL, 5 );

	cancelButton = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( cancelButton, 0, wxALL, 5 );


	bSizer30->Add( bSizer31, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer30 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( parameter_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( parameter_dialog_base::cancel_button_on_button_click ), NULL, this );
}

parameter_dialog_base::~parameter_dialog_base()
{
	// Disconnect Events
	okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( parameter_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( parameter_dialog_base::cancel_button_on_button_click ), NULL, this );

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
	incrementPGItem = m_propertyGrid5->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) );
	spanPGItem = m_propertyGrid5->Append( new wxIntProperty( wxT("Span"), wxT("Span") ) );
	searchRadiusPGItem = m_propertyGrid5->Append( new wxFloatProperty( wxT("Search Radius"), wxT("Search Radius") ) );
	searchIncrementPGItem = m_propertyGrid5->Append( new wxFloatProperty( wxT("Search Increment"), wxT("Search Increment") ) );
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

hopf_bifurcation_dialog_base::hopf_bifurcation_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );

	m_notebook5 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel17 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );

	initialVarsDataViewCtrl = new wxDataViewListCtrl( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( initialVarsDataViewCtrl, 1, wxEXPAND, 5 );

	initialParametersDataViewCtrl = new wxDataViewListCtrl( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( initialParametersDataViewCtrl, 1, wxEXPAND, 5 );


	m_panel17->SetSizer( bSizer43 );
	m_panel17->Layout();
	bSizer43->Fit( m_panel17 );
	m_notebook5->AddPage( m_panel17, wxT("Initial"), false );
	m_panel18 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid7 = new wxPropertyGrid(m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER);
	incrementPGItem = m_propertyGrid7->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) );
	spanPGItem = m_propertyGrid7->Append( new wxIntProperty( wxT("Span"), wxT("Span") ) );
	searchRadiusPGItem = m_propertyGrid7->Append( new wxFloatProperty( wxT("Search Radius"), wxT("Search Radius") ) );
	searchIncrementPGItem = m_propertyGrid7->Append( new wxFloatProperty( wxT("Search Increment"), wxT("Search Increment") ) );
	bSizer44->Add( m_propertyGrid7, 1, wxEXPAND, 5 );


	m_panel18->SetSizer( bSizer44 );
	m_panel18->Layout();
	bSizer44->Fit( m_panel18 );
	m_notebook5->AddPage( m_panel18, wxT("Properties"), false );

	bSizer42->Add( m_notebook5, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );

	m_button26 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button26, 0, wxALL, 5 );

	m_button27 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button27, 0, wxALL, 5 );


	bSizer42->Add( bSizer45, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer42 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_button26->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( hopf_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( hopf_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );
}

hopf_bifurcation_dialog_base::~hopf_bifurcation_dialog_base()
{
	// Disconnect Events
	m_button26->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( hopf_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( hopf_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );

}

saddle_connection_bifurcation_dialog_base::saddle_connection_bifurcation_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );

	m_notebook5 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel17 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );

	initialParametersDataViewCtrl = new wxDataViewListCtrl( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( initialParametersDataViewCtrl, 1, wxEXPAND, 5 );


	m_panel17->SetSizer( bSizer43 );
	m_panel17->Layout();
	bSizer43->Fit( m_panel17 );
	m_notebook5->AddPage( m_panel17, wxT("Initial"), false );
	m_panel18 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );

	saddleConnectionPG = new wxPropertyGrid(m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER);
	incrementPGItem = saddleConnectionPG->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) );
	spanPGItem = saddleConnectionPG->Append( new wxIntProperty( wxT("Span"), wxT("Span") ) );
	searchRadiusPGItem = saddleConnectionPG->Append( new wxFloatProperty( wxT("Search Radius"), wxT("Search Radius") ) );
	searchIncrementPGItem = saddleConnectionPG->Append( new wxFloatProperty( wxT("Search Increment"), wxT("Search Increment") ) );
	bSizer44->Add( saddleConnectionPG, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer84;
	bSizer84 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText10 = new wxStaticText( m_panel18, wxID_ANY, wxT("Separatrix 1:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer84->Add( m_staticText10, 0, wxALL, 5 );

	separatrix1ComboBox = new wxComboBox( m_panel18, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	bSizer84->Add( separatrix1ComboBox, 0, wxALL, 5 );


	bSizer44->Add( bSizer84, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer86;
	bSizer86 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText11 = new wxStaticText( m_panel18, wxID_ANY, wxT("Separatrix 2:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer86->Add( m_staticText11, 0, wxALL, 5 );

	separatrix2ComboBox = new wxComboBox( m_panel18, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	bSizer86->Add( separatrix2ComboBox, 0, wxALL, 5 );


	bSizer44->Add( bSizer86, 1, wxEXPAND, 5 );


	m_panel18->SetSizer( bSizer44 );
	m_panel18->Layout();
	bSizer44->Fit( m_panel18 );
	m_notebook5->AddPage( m_panel18, wxT("Properties"), true );

	bSizer42->Add( m_notebook5, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );

	m_button26 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button26, 0, wxALL, 5 );

	m_button27 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button27, 0, wxALL, 5 );


	bSizer42->Add( bSizer45, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer42 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_button26->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_connection_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_connection_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );
}

saddle_connection_bifurcation_dialog_base::~saddle_connection_bifurcation_dialog_base()
{
	// Disconnect Events
	m_button26->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_connection_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_connection_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );

}

limit_cycle_bifurcation_dialog_base::limit_cycle_bifurcation_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );

	m_notebook5 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel17 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );

	initialParametersDataViewCtrl = new wxDataViewListCtrl( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( initialParametersDataViewCtrl, 1, wxEXPAND, 5 );


	m_panel17->SetSizer( bSizer43 );
	m_panel17->Layout();
	bSizer43->Fit( m_panel17 );
	m_notebook5->AddPage( m_panel17, wxT("Initial"), false );
	m_panel18 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );

	limitCyclePG = new wxPropertyGrid(m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER);
	incrementPGItem = limitCyclePG->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) );
	spanPGItem = limitCyclePG->Append( new wxIntProperty( wxT("Span"), wxT("Span") ) );
	searchRadiusPGItem = limitCyclePG->Append( new wxFloatProperty( wxT("Search Radius"), wxT("Search Radius") ) );
	searchIncrementPGItem = limitCyclePG->Append( new wxFloatProperty( wxT("Search Increment"), wxT("Search Increment") ) );
	bSizer44->Add( limitCyclePG, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer84;
	bSizer84 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText10 = new wxStaticText( m_panel18, wxID_ANY, wxT("Limit Cycle 1:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer84->Add( m_staticText10, 0, wxALL, 5 );

	limitCycle1ComboBox = new wxComboBox( m_panel18, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bSizer84->Add( limitCycle1ComboBox, 0, wxALL, 5 );


	bSizer44->Add( bSizer84, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer86;
	bSizer86 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText11 = new wxStaticText( m_panel18, wxID_ANY, wxT("Limit Cycle 2:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer86->Add( m_staticText11, 0, wxALL, 5 );

	limitCycle2ComboBox = new wxComboBox( m_panel18, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bSizer86->Add( limitCycle2ComboBox, 0, wxALL, 5 );


	bSizer44->Add( bSizer86, 1, wxEXPAND, 5 );


	m_panel18->SetSizer( bSizer44 );
	m_panel18->Layout();
	bSizer44->Fit( m_panel18 );
	m_notebook5->AddPage( m_panel18, wxT("Properties"), false );

	bSizer42->Add( m_notebook5, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );

	m_button26 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button26, 0, wxALL, 5 );

	m_button27 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button27, 0, wxALL, 5 );


	bSizer42->Add( bSizer45, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer42 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_button26->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( limit_cycle_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( limit_cycle_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );
}

limit_cycle_bifurcation_dialog_base::~limit_cycle_bifurcation_dialog_base()
{
	// Disconnect Events
	m_button26->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( limit_cycle_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( limit_cycle_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );

}

saddle_node_bifurcation_dialog_base::saddle_node_bifurcation_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );

	m_notebook5 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel17 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );

	initialVarsDataViewCtrl = new wxDataViewListCtrl( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( initialVarsDataViewCtrl, 1, wxEXPAND, 5 );

	initialParametersDataViewCtrl = new wxDataViewListCtrl( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer43->Add( initialParametersDataViewCtrl, 1, wxEXPAND, 5 );


	m_panel17->SetSizer( bSizer43 );
	m_panel17->Layout();
	bSizer43->Fit( m_panel17 );
	m_notebook5->AddPage( m_panel17, wxT("Initial"), true );
	m_panel18 = new wxPanel( m_notebook5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer44;
	bSizer44 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid7 = new wxPropertyGrid(m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER);
	incrementPGItem = m_propertyGrid7->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) );
	spanPGItem = m_propertyGrid7->Append( new wxIntProperty( wxT("Span"), wxT("Span") ) );
	searchRadiusPGItem = m_propertyGrid7->Append( new wxFloatProperty( wxT("Search Radius"), wxT("Search Radius") ) );
	searchIncrementPGItem = m_propertyGrid7->Append( new wxFloatProperty( wxT("Search Increment"), wxT("Search Increment") ) );
	bSizer44->Add( m_propertyGrid7, 1, wxEXPAND, 5 );


	m_panel18->SetSizer( bSizer44 );
	m_panel18->Layout();
	bSizer44->Fit( m_panel18 );
	m_notebook5->AddPage( m_panel18, wxT("Properties"), false );

	bSizer42->Add( m_notebook5, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxHORIZONTAL );

	m_button26 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button26, 0, wxALL, 5 );

	m_button27 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer45->Add( m_button27, 0, wxALL, 5 );


	bSizer42->Add( bSizer45, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer42 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_button26->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_node_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_node_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );
}

saddle_node_bifurcation_dialog_base::~saddle_node_bifurcation_dialog_base()
{
	// Disconnect Events
	m_button26->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_node_bifurcation_dialog_base::ok_button_on_button_click ), NULL, this );
	m_button27->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( saddle_node_bifurcation_dialog_base::cancel_button_on_button_click ), NULL, this );

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

separatrix_dialog_base::separatrix_dialog_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );

	m_propertyGrid6 = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	incrementPGItem = m_propertyGrid6->Append( new wxFloatProperty( wxT("Increment"), wxT("Increment") ) );
	timePGItem = m_propertyGrid6->Append( new wxFloatProperty( wxT("Time"), wxT("Time") ) );
	bSizer32->Add( m_propertyGrid6, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Singular Point:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer34->Add( m_staticText2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	singularPointComboBox = new wxComboBox( this, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	bSizer34->Add( singularPointComboBox, 0, wxALL, 5 );


	bSizer32->Add( bSizer34, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer36->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	typeComboBox = new wxComboBox( this, wxID_ANY, wxT("STABLE"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	typeComboBox->Append( wxT("STABLE") );
	typeComboBox->Append( wxT("UNSTABLE") );
	typeComboBox->SetSelection( 0 );
	bSizer36->Add( typeComboBox, 0, wxALL, 5 );


	bSizer32->Add( bSizer36, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );

	okButton = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer33->Add( okButton, 0, wxALL, 5 );

	cancelButton = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer33->Add( cancelButton, 0, wxALL, 5 );


	bSizer32->Add( bSizer33, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer32 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( separatrix_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( separatrix_dialog_base::cancel_button_on_button_click ), NULL, this );
}

separatrix_dialog_base::~separatrix_dialog_base()
{
	// Disconnect Events
	okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( separatrix_dialog_base::ok_button_on_button_click ), NULL, this );
	cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( separatrix_dialog_base::cancel_button_on_button_click ), NULL, this );

}
