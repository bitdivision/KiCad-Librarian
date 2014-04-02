///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "libmngr_gui_base.h"

#include "res/ArrowLeft.xpm"
#include "res/ArrowRight.xpm"
#include "res/Information.xpm"
#include "res/Measure.xpm"
#include "res/ZoomIn.xpm"
#include "res/ZoomOut.xpm"

///////////////////////////////////////////////////////////////////////////

AppFrame::AppFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	m_menubar = new wxMenuBar( 0 );
	m_mnuFile = new wxMenu();
	wxMenuItem* m_mnuNew;
	m_mnuNew = new wxMenuItem( m_mnuFile, wxID_NEW, wxString( _("New &Library...") ) , _("Create a new (empty) module library"), wxITEM_NORMAL );
	m_mnuFile->Append( m_mnuNew );
	
	wxMenuItem* m_mnuNewFootprint;
	m_mnuNewFootprint = new wxMenuItem( m_mnuFile, IDM_NEWFOOTPRINT, wxString( _("New &Footprint..") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuFile->Append( m_mnuNewFootprint );
	
	wxMenuItem* m_mnuNewSymbol;
	m_mnuNewSymbol = new wxMenuItem( m_mnuFile, IDM_NEWSYMBOL, wxString( _("New &symbol...") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuFile->Append( m_mnuNewSymbol );
	
	m_mnuFile->AppendSeparator();
	
	wxMenuItem* m_mnuFootprintSheet;
	m_mnuFootprintSheet = new wxMenuItem( m_mnuFile, IDM_REPORTFOOTPRINT, wxString( _("Create footprint &report") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuFile->Append( m_mnuFootprintSheet );
	
	wxMenuItem* m_mnuSymbolSheet;
	m_mnuSymbolSheet = new wxMenuItem( m_mnuFile, IDM_REPORTSYMBOL, wxString( _("Create s&ymbol report") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuFile->Append( m_mnuSymbolSheet );
	
	m_mnuFile->AppendSeparator();
	
	wxMenuItem* m_mnuQuit;
	m_mnuQuit = new wxMenuItem( m_mnuFile, wxID_EXIT, wxString( _("&Quit") ) + wxT('\t') + wxT("Alt+F4"), _("Exit the application"), wxITEM_NORMAL );
	m_mnuFile->Append( m_mnuQuit );
	
	m_menubar->Append( m_mnuFile, _("&File") ); 
	
	m_mnuView = new wxMenu();
	wxMenuItem* m_mnuFootprintMode;
	m_mnuFootprintMode = new wxMenuItem( m_mnuView, IDM_FOOTPRINTMODE, wxString( _("&Footprint mode") ) , _("In footprint mode, you can view, adjust, move and copy footprints bewteen libraries"), wxITEM_RADIO );
	m_mnuView->Append( m_mnuFootprintMode );
	m_mnuFootprintMode->Check( true );
	
	wxMenuItem* m_mnuSymbolMode;
	m_mnuSymbolMode = new wxMenuItem( m_mnuView, IDM_SCHEMATICMODE, wxString( _("&Schematic mode") ) , _("In schematic mode, you can view, move and copy symbols between schematic libraries"), wxITEM_RADIO );
	m_mnuView->Append( m_mnuSymbolMode );
	
	m_mnuView->AppendSeparator();
	
	wxMenuItem* m_mnuCompareMode;
	m_mnuCompareMode = new wxMenuItem( m_mnuView, IDM_COMPAREMODE, wxString( _("&Compare mode") ) + wxT('\t') + wxT("Ctrl+C"), _("In compare mode, you can compare two footprints"), wxITEM_CHECK );
	m_mnuView->Append( m_mnuCompareMode );
	
	wxMenuItem* m_mnuDetailsPanel;
	m_mnuDetailsPanel = new wxMenuItem( m_mnuView, IDM_DETAILSPANEL, wxString( _("Details panel") ) + wxT('\t') + wxT("Ctrl+D"), _("The details panel allows you to see properties of the footprint, and adjust them"), wxITEM_CHECK );
	m_mnuView->Append( m_mnuDetailsPanel );
	
	m_menubar->Append( m_mnuView, _("&View") ); 
	
	m_mnuPreferences = new wxMenu();
	wxMenuItem* m_mnuSearchPaths;
	m_mnuSearchPaths = new wxMenuItem( m_mnuPreferences, wxID_PREFERENCES, wxString( _("Search &Paths...") ) , _("Add or remove paths to search for footprint or symbol libraries"), wxITEM_NORMAL );
	m_mnuPreferences->Append( m_mnuSearchPaths );
	
	wxMenuItem* m_mnuRemoteLink;
	m_mnuRemoteLink = new wxMenuItem( m_mnuPreferences, IDM_DLGREMOTE, wxString( _("Remote Repository...") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuPreferences->Append( m_mnuRemoteLink );
	
	wxMenuItem* m_mnuReport;
	m_mnuReport = new wxMenuItem( m_mnuPreferences, IDM_DLGREPORT, wxString( _("&Report options...") ) , _("Configuration of the footprint report"), wxITEM_NORMAL );
	m_mnuPreferences->Append( m_mnuReport );
	
	wxMenuItem* m_mnuUI;
	m_mnuUI = new wxMenuItem( m_mnuPreferences, IDM_DLGOPTIONS, wxString( _("&User interface...") ) , _("User interface settings"), wxITEM_NORMAL );
	m_mnuPreferences->Append( m_mnuUI );
	
	wxMenuItem* m_mnuTemplate;
	m_mnuTemplate = new wxMenuItem( m_mnuPreferences, IDM_TEMPLATEOPTIONS, wxString( _("&Template variables...") ) , _("Change parameters for templates for new footprints"), wxITEM_NORMAL );
	m_mnuPreferences->Append( m_mnuTemplate );
	
	m_menubar->Append( m_mnuPreferences, _("&Preferences") ); 
	
	m_mnuHelp = new wxMenu();
	wxMenuItem* m_mnuHelpManual;
	m_mnuHelpManual = new wxMenuItem( m_mnuHelp, wxID_HELP, wxString( _("&Manual") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_mnuHelp->Append( m_mnuHelpManual );
	
	wxMenuItem* m_mnuAbout;
	m_mnuAbout = new wxMenuItem( m_mnuHelp, wxID_ABOUT, wxString( _("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_mnuHelp->Append( m_mnuAbout );
	
	m_menubar->Append( m_mnuHelp, _("&Help") ); 
	
	this->SetMenuBar( m_menubar );
	
	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( AppFrame::m_splitterOnIdle ), NULL, this );
	m_splitter->SetMinimumPaneSize( 5 );
	
	m_panelTop = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bHorSplitter1;
	bHorSplitter1 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerLeft;
	bSizerLeft = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_choiceModuleLeftChoices;
	m_choiceModuleLeft = new wxChoice( m_panelTop, IDM_LEFT_LIB, wxDefaultPosition, wxDefaultSize, m_choiceModuleLeftChoices, 0 );
	m_choiceModuleLeft->SetSelection( 0 );
	bSizerLeft->Add( m_choiceModuleLeft, 0, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_listModulesLeft = new wxListCtrl( m_panelTop, IDM_LEFT_MOD, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_SORT_ASCENDING );
	bSizerLeft->Add( m_listModulesLeft, 1, wxEXPAND|wxBOTTOM|wxLEFT, 5 );
	
	
	bHorSplitter1->Add( bSizerLeft, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerMid;
	bSizerMid = new wxBoxSizer( wxVERTICAL );
	
	
	bSizerMid->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnMove = new wxButton( m_panelTop, IDM_MOVEMODULE, _("&Move"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_btnMove->SetToolTip( _("Move the footprint / symbol to the other library.") );
	
	bSizerMid->Add( m_btnMove, 0, wxALL, 5 );
	
	m_btnCopy = new wxButton( m_panelTop, IDM_COPYMODULE, _("&Copy"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_btnCopy->SetToolTip( _("Copy the footprint / symbol to the other library.") );
	
	bSizerMid->Add( m_btnCopy, 0, wxALL, 5 );
	
	m_btnDelete = new wxButton( m_panelTop, IDM_DELETEMODULE, _("&Delete"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_btnDelete->SetToolTip( _("Remove the selected footprint / symbol.") );
	
	bSizerMid->Add( m_btnDelete, 0, wxALL, 5 );
	
	m_btnRename = new wxButton( m_panelTop, IDM_RENAMEMODULE, _("&Rename"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_btnRename->SetToolTip( _("Rename the selected footprint / symbol.") );
	
	bSizerMid->Add( m_btnRename, 0, wxALL, 5 );
	
	m_btnDuplicate = new wxButton( m_panelTop, IDM_RENAMEMODULE, _("D&uplicate"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_btnDuplicate->SetToolTip( _("Copy the selected footprint / symbol to the same library under a different name.") );
	
	bSizerMid->Add( m_btnDuplicate, 0, wxALL, 5 );
	
	
	bSizerMid->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bHorSplitter1->Add( bSizerMid, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerRight;
	bSizerRight = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_choiceModuleRightChoices;
	m_choiceModuleRight = new wxChoice( m_panelTop, IDM_RIGHT_LIB, wxDefaultPosition, wxDefaultSize, m_choiceModuleRightChoices, 0 );
	m_choiceModuleRight->SetSelection( 0 );
	bSizerRight->Add( m_choiceModuleRight, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_listModulesRight = new wxListCtrl( m_panelTop, IDM_RIGHT_MOD, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_SORT_ASCENDING );
	bSizerRight->Add( m_listModulesRight, 1, wxBOTTOM|wxRIGHT|wxEXPAND, 5 );
	
	
	bHorSplitter1->Add( bSizerRight, 1, wxEXPAND, 5 );
	
	
	m_panelTop->SetSizer( bHorSplitter1 );
	m_panelTop->Layout();
	bHorSplitter1->Fit( m_panelTop );
	m_panelBottom = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerBottom;
	bSizerBottom = new wxBoxSizer( wxHORIZONTAL );
	
	m_toolBar = new wxToolBar( m_panelBottom, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_VERTICAL );
	m_toolBar->SetToolBitmapSize( wxSize( 24,24 ) );
	m_toolBar->AddTool( IDT_ZOOMIN, _("Zoom in"), wxBitmap( ZoomIn_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Zoom in (Ctrl +)."), wxEmptyString, NULL ); 
	
	m_toolBar->AddTool( IDT_ZOOMOUT, _("Zoom out"), wxBitmap( ZoomOut_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Zoom out (Ctrl -)."), wxEmptyString, NULL ); 
	
	m_toolBar->AddTool( IDT_MEASUREMENTS, _("Measure"), wxBitmap( Measure_xpm ), wxNullBitmap, wxITEM_CHECK, _("Show pitch and pad measurements."), wxEmptyString, NULL ); 
	
	m_toolBar->AddTool( IDT_DETAILSPANEL, _("Details panel"), wxBitmap( Information_xpm ), wxNullBitmap, wxITEM_CHECK, _("Show/hide the side panel with parameters."), wxEmptyString, NULL ); 
	
	m_toolBar->AddTool( IDT_LEFTFOOTPRINT, _("Left"), wxBitmap( ArrowLeft_xpm ), wxNullBitmap, wxITEM_CHECK, _("Show left footprint."), wxEmptyString, NULL ); 
	
	m_toolBar->AddTool( IDT_RIGHTFOOTPRINT, _("Right"), wxBitmap( ArrowRight_xpm ), wxNullBitmap, wxITEM_CHECK, _("Show right footprint."), wxEmptyString, NULL ); 
	
	m_toolBar->Realize(); 
	
	bSizerBottom->Add( m_toolBar, 0, wxEXPAND, 5 );
	
	m_splitterViewPanel = new wxSplitterWindow( m_panelBottom, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH );
	m_splitterViewPanel->SetSashGravity( 1 );
	m_splitterViewPanel->Connect( wxEVT_IDLE, wxIdleEventHandler( AppFrame::m_splitterViewPanelOnIdle ), NULL, this );
	
	m_panelView = new wxPanel( m_splitterViewPanel, IDM_PANELVIEW, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelView->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	m_panelSettings = new wxScrolledWindow( m_splitterViewPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxVSCROLL );
	m_panelSettings->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSidePanel;
	fgSidePanel = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSidePanel->AddGrowableCol( 1 );
	fgSidePanel->SetFlexibleDirection( wxBOTH );
	fgSidePanel->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblViewSide = new wxStaticText( m_panelSettings, wxID_ANY, _("View"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblViewSide->Wrap( -1 );
	fgSidePanel->Add( m_lblViewSide, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerViewSide;
	bSizerViewSide = new wxBoxSizer( wxHORIZONTAL );
	
	m_radioViewLeft = new wxRadioButton( m_panelSettings, wxID_ANY, _("Left"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioViewLeft->SetToolTip( _("Show the symbol from the right symbol/footprint list.") );
	
	bSizerViewSide->Add( m_radioViewLeft, 0, wxALL, 5 );
	
	m_radioViewRight = new wxRadioButton( m_panelSettings, wxID_ANY, _("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	m_radioViewRight->SetToolTip( _("Show the symbol from the right symbol/footprint list.") );
	
	bSizerViewSide->Add( m_radioViewRight, 0, wxALL, 5 );
	
	
	fgSidePanel->Add( bSizerViewSide, 1, 0, 5 );
	
	m_lblDescription = new wxStaticText( m_panelSettings, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDescription->Wrap( -1 );
	fgSidePanel->Add( m_lblDescription, 0, wxALL, 5 );
	
	m_txtDescription = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtDescription->SetToolTip( _("A general description of the part.") );
	
	fgSidePanel->Add( m_txtDescription, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 3 );
	
	m_lblAlias = new wxStaticText( m_panelSettings, wxID_ANY, _("Alias"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAlias->Wrap( -1 );
	m_lblAlias->Hide();
	
	fgSidePanel->Add( m_lblAlias, 0, wxALL, 5 );
	
	m_txtAlias = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtAlias->Hide();
	m_txtAlias->SetToolTip( _("Other names for the symbol, separated with spaces.") );
	
	fgSidePanel->Add( m_txtAlias, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 3 );
	
	m_lblFootprintFilter = new wxStaticText( m_panelSettings, wxID_ANY, _("Footprints"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFootprintFilter->Wrap( -1 );
	m_lblFootprintFilter->Hide();
	
	fgSidePanel->Add( m_lblFootprintFilter, 0, wxALL, 5 );
	
	m_txtFootprintFilter = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtFootprintFilter->Hide();
	m_txtFootprintFilter->SetToolTip( _("Applicable footprints, separated by spaces; the footprint names may have wildcard characters.") );
	
	fgSidePanel->Add( m_txtFootprintFilter, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 3 );
	
	m_lblPadCount = new wxStaticText( m_panelSettings, wxID_ANY, _("Pad count"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPadCount->Wrap( -1 );
	fgSidePanel->Add( m_lblPadCount, 0, wxALL, 5 );
	
	m_txtPadCount = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtPadCount->SetToolTip( _("The number of pads or pins of the footprint or symbol.\nIf this field is read-only, either\n1) the part was not based on a template\n2)  rebuilding the part from the template is disabled in the preferences\n3) this part only exists with one pin count.") );
	m_txtPadCount->SetMinSize( wxSize( 55,-1 ) );
	
	fgSidePanel->Add( m_txtPadCount, 0, wxBOTTOM|wxRIGHT|wxLEFT, 3 );
	
	m_lblPinNames = new wxStaticText( m_panelSettings, wxID_ANY, _("Pins"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPinNames->Wrap( -1 );
	m_lblPinNames->Hide();
	
	fgSidePanel->Add( m_lblPinNames, 0, wxALL, 5 );
	
	m_gridPinNames = new wxGrid( m_panelSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
	
	// Grid
	m_gridPinNames->CreateGrid( 2, 5 );
	m_gridPinNames->EnableEditing( true );
	m_gridPinNames->EnableGridLines( true );
	m_gridPinNames->EnableDragGridSize( false );
	m_gridPinNames->SetMargins( 0, 0 );
	
	// Columns
	m_gridPinNames->AutoSizeColumns();
	m_gridPinNames->EnableDragColMove( false );
	m_gridPinNames->EnableDragColSize( true );
	m_gridPinNames->SetColLabelSize( 16 );
	m_gridPinNames->SetColLabelValue( 0, _("pin") );
	m_gridPinNames->SetColLabelValue( 1, _("label") );
	m_gridPinNames->SetColLabelValue( 2, _("type") );
	m_gridPinNames->SetColLabelValue( 3, _("style") );
	m_gridPinNames->SetColLabelValue( 4, _("pos.") );
	m_gridPinNames->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridPinNames->EnableDragRowSize( false );
	m_gridPinNames->SetRowLabelSize( 0 );
	m_gridPinNames->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridPinNames->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_gridPinNames->Hide();
	m_gridPinNames->SetToolTip( _("Pin numbers, labels and properties.\nThe \"type\" is the electrical type.\nThe \"style\" is the graphic shape.\nThe position can only be changed for symbols that are based on a template.") );
	
	fgSidePanel->Add( m_gridPinNames, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 3 );
	
	m_lblPadShape = new wxStaticText( m_panelSettings, wxID_ANY, _("Pad shape"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPadShape->Wrap( -1 );
	fgSidePanel->Add( m_lblPadShape, 0, wxALL, 5 );
	
	wxString m_choicePadShapeChoices[] = { _("(varies)"), _("Rectangular"), _("Round"), _("Round + square"), _("Stadium"), _("Trapezoid") };
	int m_choicePadShapeNChoices = sizeof( m_choicePadShapeChoices ) / sizeof( wxString );
	m_choicePadShape = new wxChoice( m_panelSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePadShapeNChoices, m_choicePadShapeChoices, 0 );
	m_choicePadShape->SetSelection( 0 );
	m_choicePadShape->SetToolTip( _("The shape of the pads\n(except any secondary pad, such as a thermal pad).") );
	
	fgSidePanel->Add( m_choicePadShape, 0, wxBOTTOM|wxRIGHT|wxLEFT, 3 );
	
	m_lblPadSize = new wxStaticText( m_panelSettings, wxID_ANY, _("Pad size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPadSize->Wrap( -1 );
	fgSidePanel->Add( m_lblPadSize, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerPadSize;
	bSizerPadSize = new wxBoxSizer( wxHORIZONTAL );
	
	m_txtPadWidth = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtPadWidth->SetToolTip( _("The width of the pads, in mm.") );
	m_txtPadWidth->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerPadSize->Add( m_txtPadWidth, 0, wxBOTTOM|wxLEFT, 3 );
	
	m_txtPadLength = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtPadLength->SetToolTip( _("The length of the pads, in mm.") );
	m_txtPadLength->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerPadSize->Add( m_txtPadLength, 0, wxBOTTOM|wxRIGHT, 3 );
	
	
	fgSidePanel->Add( bSizerPadSize, 1, wxEXPAND, 5 );
	
	m_lblPitch = new wxStaticText( m_panelSettings, wxID_ANY, _("Pitch"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPitch->Wrap( -1 );
	fgSidePanel->Add( m_lblPitch, 0, wxALL, 5 );
	
	m_txtPitch = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtPitch->SetToolTip( _("Pad pitch, in mm.") );
	m_txtPitch->SetMinSize( wxSize( 55,-1 ) );
	
	fgSidePanel->Add( m_txtPitch, 0, wxBOTTOM|wxRIGHT|wxLEFT, 3 );
	
	m_lblPadSpan = new wxStaticText( m_panelSettings, wxID_ANY, _("Span"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPadSpan->Wrap( -1 );
	fgSidePanel->Add( m_lblPadSpan, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerPadSpan;
	bSizerPadSpan = new wxBoxSizer( wxHORIZONTAL );
	
	m_txtPadSpanX = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtPadSpanX->SetToolTip( _("The distance between two rows of pads, in the horizontal direction, in mm.") );
	m_txtPadSpanX->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerPadSpan->Add( m_txtPadSpanX, 0, wxBOTTOM|wxLEFT, 3 );
	
	m_txtPadSpanY = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtPadSpanY->SetToolTip( _("The distance between two rows of pads, in the vertical direction, in mm.") );
	m_txtPadSpanY->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerPadSpan->Add( m_txtPadSpanY, 0, wxBOTTOM|wxRIGHT, 3 );
	
	
	fgSidePanel->Add( bSizerPadSpan, 1, wxEXPAND, 5 );
	
	m_lblDrillSize = new wxStaticText( m_panelSettings, wxID_ANY, _("Drill size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDrillSize->Wrap( -1 );
	fgSidePanel->Add( m_lblDrillSize, 0, wxALL, 5 );
	
	m_txtDrillSize = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtDrillSize->SetToolTip( _("The drill size in mm, or zero for no hole.") );
	m_txtDrillSize->SetMinSize( wxSize( 55,-1 ) );
	
	fgSidePanel->Add( m_txtDrillSize, 0, wxBOTTOM|wxRIGHT|wxLEFT, 3 );
	
	m_lblAuxPad = new wxStaticText( m_panelSettings, wxID_ANY, _("Aux. pad"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAuxPad->Wrap( -1 );
	fgSidePanel->Add( m_lblAuxPad, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerAuxPad;
	bSizerAuxPad = new wxBoxSizer( wxHORIZONTAL );
	
	m_txtAuxPadWidth = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtAuxPadWidth->SetToolTip( _("The width of an auxiliary pad, for example, for a thermal pad.") );
	m_txtAuxPadWidth->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerAuxPad->Add( m_txtAuxPadWidth, 0, wxBOTTOM|wxLEFT, 3 );
	
	m_txtAuxPadLength = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtAuxPadLength->SetToolTip( _("The length of an auxiliary pad, for example, for a thermal pad.") );
	m_txtAuxPadLength->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerAuxPad->Add( m_txtAuxPadLength, 0, wxBOTTOM|wxRIGHT, 3 );
	
	
	fgSidePanel->Add( bSizerAuxPad, 1, wxEXPAND, 5 );
	
	m_lblBodySize = new wxStaticText( m_panelSettings, wxID_ANY, _("Body size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBodySize->Wrap( -1 );
	fgSidePanel->Add( m_lblBodySize, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerBodySize;
	bSizerBodySize = new wxBoxSizer( wxHORIZONTAL );
	
	m_txtBodyWidth = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtBodyWidth->SetToolTip( _("The width of the body of the component, in mm.\nIf this field is disabled, either the footprint/symbol is not based on a template,\nor rebuilding the footprint/symbol from the template is disabled in the preferences.") );
	m_txtBodyWidth->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerBodySize->Add( m_txtBodyWidth, 0, wxBOTTOM|wxLEFT, 3 );
	
	m_txtBodyLength = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtBodyLength->SetToolTip( _("The length of the body of the component, in mm.\nIf this field is disabled, either the footprint/symbol is not based on a template,\nor rebuilding the footprint/symbol from the template is disabled in the preferences.") );
	m_txtBodyLength->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerBodySize->Add( m_txtBodyLength, 0, wxBOTTOM|wxRIGHT, 3 );
	
	
	fgSidePanel->Add( bSizerBodySize, 1, wxEXPAND, 5 );
	
	m_lblRefLabel = new wxStaticText( m_panelSettings, wxID_ANY, _("Ref. label"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblRefLabel->Wrap( -1 );
	fgSidePanel->Add( m_lblRefLabel, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerRefLabel;
	bSizerRefLabel = new wxBoxSizer( wxHORIZONTAL );
	
	m_txtRefLabel = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtRefLabel->SetToolTip( _("The text size, in mm.") );
	m_txtRefLabel->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerRefLabel->Add( m_txtRefLabel, 0, wxBOTTOM|wxLEFT, 3 );
	
	m_chkRefLabelVisible = new wxCheckBox( m_panelSettings, wxID_ANY, _("visible"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkRefLabelVisible->SetToolTip( _("Whether the reference label is visible by default.") );
	
	bSizerRefLabel->Add( m_chkRefLabelVisible, 0, wxALL, 5 );
	
	
	fgSidePanel->Add( bSizerRefLabel, 1, wxEXPAND, 5 );
	
	m_lblValueLabel = new wxStaticText( m_panelSettings, wxID_ANY, _("Value label"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblValueLabel->Wrap( -1 );
	fgSidePanel->Add( m_lblValueLabel, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerValueLabel;
	bSizerValueLabel = new wxBoxSizer( wxHORIZONTAL );
	
	m_txtValueLabel = new wxTextCtrl( m_panelSettings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_txtValueLabel->SetToolTip( _("The text size, in mm.") );
	m_txtValueLabel->SetMinSize( wxSize( 55,-1 ) );
	
	bSizerValueLabel->Add( m_txtValueLabel, 0, wxBOTTOM|wxLEFT, 3 );
	
	m_chkValueLabelVisible = new wxCheckBox( m_panelSettings, wxID_ANY, _("visible"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkValueLabelVisible->SetToolTip( _("Whether the value label is visible by default.") );
	
	bSizerValueLabel->Add( m_chkValueLabelVisible, 0, wxALL, 5 );
	
	
	fgSidePanel->Add( bSizerValueLabel, 1, wxEXPAND, 5 );
	
	
	fgSidePanel->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerSaveRevert;
	bSizerSaveRevert = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnSavePart = new wxButton( m_panelSettings, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_btnSavePart->SetToolTip( _("Save the modified data.") );
	m_btnSavePart->SetMinSize( wxSize( 54,-1 ) );
	
	bSizerSaveRevert->Add( m_btnSavePart, 0, wxTOP|wxBOTTOM|wxLEFT, 3 );
	
	m_btnRevertPart = new wxButton( m_panelSettings, wxID_ANY, _("Revert"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_btnRevertPart->SetToolTip( _("Undo modifications and restore to the last saved state.") );
	m_btnRevertPart->SetMinSize( wxSize( 54,-1 ) );
	
	bSizerSaveRevert->Add( m_btnRevertPart, 0, wxALL, 3 );
	
	
	fgSidePanel->Add( bSizerSaveRevert, 1, wxEXPAND, 5 );
	
	
	m_panelSettings->SetSizer( fgSidePanel );
	m_panelSettings->Layout();
	fgSidePanel->Fit( m_panelSettings );
	m_splitterViewPanel->SplitVertically( m_panelView, m_panelSettings, -200 );
	bSizerBottom->Add( m_splitterViewPanel, 1, wxEXPAND, 5 );
	
	
	m_panelBottom->SetSizer( bSizerBottom );
	m_panelBottom->Layout();
	bSizerBottom->Fit( m_panelBottom );
	m_splitter->SplitHorizontally( m_panelTop, m_panelBottom, 185 );
	bMainSizer->Add( m_splitter, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bMainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AppFrame::OnCloseApp ) );
	this->Connect( m_mnuNew->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnNewLibrary ) );
	this->Connect( m_mnuNewFootprint->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnNewFootprint ) );
	this->Connect( m_mnuNewSymbol->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnNewSymbol ) );
	this->Connect( m_mnuFootprintSheet->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnFootprintReport ) );
	this->Connect( m_mnuSymbolSheet->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnSymbolReport ) );
	this->Connect( m_mnuQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnQuit ) );
	this->Connect( m_mnuFootprintMode->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnFootprintMode ) );
	this->Connect( m_mnuSymbolMode->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnSymbolMode ) );
	this->Connect( m_mnuCompareMode->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnCompareMode ) );
	this->Connect( m_mnuDetailsPanel->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnDetailsPanel ) );
	this->Connect( m_mnuSearchPaths->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnSearchPaths ) );
	this->Connect( m_mnuRemoteLink->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnRemoteLink ) );
	this->Connect( m_mnuReport->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnReportSettings ) );
	this->Connect( m_mnuUI->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnUIOptions ) );
	this->Connect( m_mnuTemplate->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnTemplateOptions ) );
	this->Connect( m_mnuHelpManual->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnHelp ) );
	this->Connect( m_mnuAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnAbout ) );
	m_choiceModuleLeft->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppFrame::OnLeftLibSelect ), NULL, this );
	m_listModulesLeft->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AppFrame::OnLeftModSelect ), NULL, this );
	m_btnMove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnMovePart ), NULL, this );
	m_btnCopy->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnCopyPart ), NULL, this );
	m_btnDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnDeletePart ), NULL, this );
	m_btnRename->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnRenamePart ), NULL, this );
	m_btnDuplicate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnDuplicatePart ), NULL, this );
	m_choiceModuleRight->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppFrame::OnRightLibSelect ), NULL, this );
	m_listModulesRight->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AppFrame::OnRightModSelect ), NULL, this );
	this->Connect( IDT_ZOOMIN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnZoomIn ) );
	this->Connect( IDT_ZOOMOUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnZoomOut ) );
	this->Connect( IDT_MEASUREMENTS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowMeasurements ) );
	this->Connect( IDT_DETAILSPANEL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowDetails ) );
	this->Connect( IDT_LEFTFOOTPRINT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowLeftFootprint ) );
	this->Connect( IDT_RIGHTFOOTPRINT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowRightFootprint ) );
	m_splitterViewPanel->Connect( wxEVT_COMMAND_SPLITTER_UNSPLIT, wxSplitterEventHandler( AppFrame::OnDetailsPanelUnsplit ), NULL, this );
	m_panelView->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( AppFrame::OnViewCentre ), NULL, this );
	m_panelView->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( AppFrame::OnViewStartDrag ), NULL, this );
	m_panelView->Connect( wxEVT_MOTION, wxMouseEventHandler( AppFrame::OnViewDrag ), NULL, this );
	m_panelView->Connect( wxEVT_PAINT, wxPaintEventHandler( AppFrame::OnPaintViewport ), NULL, this );
	m_panelView->Connect( wxEVT_SIZE, wxSizeEventHandler( AppFrame::OnSizeViewport ), NULL, this );
	m_radioViewLeft->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AppFrame::OnShowLeftDetails ), NULL, this );
	m_radioViewRight->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AppFrame::OnShowRightDetails ), NULL, this );
	m_txtDescription->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusTextInfo ), NULL, this );
	m_txtDescription->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtDescription->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterTextInfo ), NULL, this );
	m_txtAlias->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusTextInfo ), NULL, this );
	m_txtAlias->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtAlias->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterTextInfo ), NULL, this );
	m_txtFootprintFilter->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusTextInfo ), NULL, this );
	m_txtFootprintFilter->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtFootprintFilter->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterTextInfo ), NULL, this );
	m_txtPadCount->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadCount ), NULL, this );
	m_txtPadCount->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadCount->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadCount ), NULL, this );
	m_gridPinNames->Connect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( AppFrame::OnPinNameChanged ), NULL, this );
	m_gridPinNames->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( AppFrame::OnPinNameRearrange ), NULL, this );
	m_choicePadShape->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppFrame::OnChoiceFieldChange ), NULL, this );
	m_txtPadWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtPadWidth->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtPadLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtPadLength->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadLength->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtPitch->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPitchInfo ), NULL, this );
	m_txtPitch->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPitch->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPitchInfo ), NULL, this );
	m_txtPadSpanX->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusSpanInfo ), NULL, this );
	m_txtPadSpanX->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadSpanX->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterSpanInfo ), NULL, this );
	m_txtPadSpanY->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusSpanInfo ), NULL, this );
	m_txtPadSpanY->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadSpanY->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterSpanInfo ), NULL, this );
	m_txtDrillSize->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtDrillSize->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtDrillSize->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtAuxPadWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtAuxPadWidth->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtAuxPadWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtAuxPadLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtAuxPadLength->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtAuxPadLength->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtBodyWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusBodyInfo ), NULL, this );
	m_txtBodyWidth->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtBodyWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterBodyInfo ), NULL, this );
	m_txtBodyLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusBodyInfo ), NULL, this );
	m_txtBodyLength->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtBodyLength->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterBodyInfo ), NULL, this );
	m_txtRefLabel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusLabelField ), NULL, this );
	m_txtRefLabel->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtRefLabel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterLabelField ), NULL, this );
	m_chkRefLabelVisible->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppFrame::OnLabelShowHide ), NULL, this );
	m_txtValueLabel->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusLabelField ), NULL, this );
	m_txtValueLabel->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtValueLabel->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterLabelField ), NULL, this );
	m_chkValueLabelVisible->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppFrame::OnLabelShowHide ), NULL, this );
	m_btnSavePart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnSavePart ), NULL, this );
	m_btnRevertPart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnRevertPart ), NULL, this );
}

AppFrame::~AppFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AppFrame::OnCloseApp ) );
	this->Disconnect( wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnNewLibrary ) );
	this->Disconnect( IDM_NEWFOOTPRINT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnNewFootprint ) );
	this->Disconnect( IDM_NEWSYMBOL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnNewSymbol ) );
	this->Disconnect( IDM_REPORTFOOTPRINT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnFootprintReport ) );
	this->Disconnect( IDM_REPORTSYMBOL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnSymbolReport ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnQuit ) );
	this->Disconnect( IDM_FOOTPRINTMODE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnFootprintMode ) );
	this->Disconnect( IDM_SCHEMATICMODE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnSymbolMode ) );
	this->Disconnect( IDM_COMPAREMODE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnCompareMode ) );
	this->Disconnect( IDM_DETAILSPANEL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnDetailsPanel ) );
	this->Disconnect( wxID_PREFERENCES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnSearchPaths ) );
	this->Disconnect( IDM_DLGREMOTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnRemoteLink ) );
	this->Disconnect( IDM_DLGREPORT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnReportSettings ) );
	this->Disconnect( IDM_DLGOPTIONS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnUIOptions ) );
	this->Disconnect( IDM_TEMPLATEOPTIONS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnTemplateOptions ) );
	this->Disconnect( wxID_HELP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnHelp ) );
	this->Disconnect( wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( AppFrame::OnAbout ) );
	m_choiceModuleLeft->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppFrame::OnLeftLibSelect ), NULL, this );
	m_listModulesLeft->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AppFrame::OnLeftModSelect ), NULL, this );
	m_btnMove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnMovePart ), NULL, this );
	m_btnCopy->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnCopyPart ), NULL, this );
	m_btnDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnDeletePart ), NULL, this );
	m_btnRename->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnRenamePart ), NULL, this );
	m_btnDuplicate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnDuplicatePart ), NULL, this );
	m_choiceModuleRight->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppFrame::OnRightLibSelect ), NULL, this );
	m_listModulesRight->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( AppFrame::OnRightModSelect ), NULL, this );
	this->Disconnect( IDT_ZOOMIN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnZoomIn ) );
	this->Disconnect( IDT_ZOOMOUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnZoomOut ) );
	this->Disconnect( IDT_MEASUREMENTS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowMeasurements ) );
	this->Disconnect( IDT_DETAILSPANEL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowDetails ) );
	this->Disconnect( IDT_LEFTFOOTPRINT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowLeftFootprint ) );
	this->Disconnect( IDT_RIGHTFOOTPRINT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( AppFrame::OnShowRightFootprint ) );
	m_splitterViewPanel->Disconnect( wxEVT_COMMAND_SPLITTER_UNSPLIT, wxSplitterEventHandler( AppFrame::OnDetailsPanelUnsplit ), NULL, this );
	m_panelView->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( AppFrame::OnViewCentre ), NULL, this );
	m_panelView->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( AppFrame::OnViewStartDrag ), NULL, this );
	m_panelView->Disconnect( wxEVT_MOTION, wxMouseEventHandler( AppFrame::OnViewDrag ), NULL, this );
	m_panelView->Disconnect( wxEVT_PAINT, wxPaintEventHandler( AppFrame::OnPaintViewport ), NULL, this );
	m_panelView->Disconnect( wxEVT_SIZE, wxSizeEventHandler( AppFrame::OnSizeViewport ), NULL, this );
	m_radioViewLeft->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AppFrame::OnShowLeftDetails ), NULL, this );
	m_radioViewRight->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AppFrame::OnShowRightDetails ), NULL, this );
	m_txtDescription->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusTextInfo ), NULL, this );
	m_txtDescription->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtDescription->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterTextInfo ), NULL, this );
	m_txtAlias->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusTextInfo ), NULL, this );
	m_txtAlias->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtAlias->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterTextInfo ), NULL, this );
	m_txtFootprintFilter->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusTextInfo ), NULL, this );
	m_txtFootprintFilter->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtFootprintFilter->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterTextInfo ), NULL, this );
	m_txtPadCount->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadCount ), NULL, this );
	m_txtPadCount->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadCount->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadCount ), NULL, this );
	m_gridPinNames->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( AppFrame::OnPinNameChanged ), NULL, this );
	m_gridPinNames->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( AppFrame::OnPinNameRearrange ), NULL, this );
	m_choicePadShape->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AppFrame::OnChoiceFieldChange ), NULL, this );
	m_txtPadWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtPadWidth->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtPadLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtPadLength->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadLength->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtPitch->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPitchInfo ), NULL, this );
	m_txtPitch->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPitch->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPitchInfo ), NULL, this );
	m_txtPadSpanX->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusSpanInfo ), NULL, this );
	m_txtPadSpanX->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadSpanX->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterSpanInfo ), NULL, this );
	m_txtPadSpanY->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusSpanInfo ), NULL, this );
	m_txtPadSpanY->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtPadSpanY->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterSpanInfo ), NULL, this );
	m_txtDrillSize->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtDrillSize->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtDrillSize->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtAuxPadWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtAuxPadWidth->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtAuxPadWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtAuxPadLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusPadInfo ), NULL, this );
	m_txtAuxPadLength->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtAuxPadLength->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterPadInfo ), NULL, this );
	m_txtBodyWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusBodyInfo ), NULL, this );
	m_txtBodyWidth->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtBodyWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterBodyInfo ), NULL, this );
	m_txtBodyLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusBodyInfo ), NULL, this );
	m_txtBodyLength->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtBodyLength->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterBodyInfo ), NULL, this );
	m_txtRefLabel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusLabelField ), NULL, this );
	m_txtRefLabel->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtRefLabel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterLabelField ), NULL, this );
	m_chkRefLabelVisible->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppFrame::OnLabelShowHide ), NULL, this );
	m_txtValueLabel->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( AppFrame::OnKillFocusLabelField ), NULL, this );
	m_txtValueLabel->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AppFrame::OnTextFieldChange ), NULL, this );
	m_txtValueLabel->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( AppFrame::OnEnterLabelField ), NULL, this );
	m_chkValueLabelVisible->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AppFrame::OnLabelShowHide ), NULL, this );
	m_btnSavePart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnSavePart ), NULL, this );
	m_btnRevertPart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AppFrame::OnRevertPart ), NULL, this );
	
}

DlgPaths::DlgPaths( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	m_lblFootprintPaths = new wxStaticText( this, wxID_ANY, _("Footprint libraries search paths"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFootprintPaths->Wrap( -1 );
	bSizerMain->Add( m_lblFootprintPaths, 0, wxTOP|wxRIGHT|wxLEFT, 8 );
	
	wxBoxSizer* bSizerFootprints;
	bSizerFootprints = new wxBoxSizer( wxHORIZONTAL );
	
	m_lstFootprints = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_SINGLE|wxLB_SORT ); 
	m_lstFootprints->SetToolTip( _("The librarian searches for footprint libraries in these paths.\n(Note that the librarian does not recurse into sub-paths.)") );
	m_lstFootprints->SetMinSize( wxSize( 200,-1 ) );
	
	bSizerFootprints->Add( m_lstFootprints, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerButtonsFootprint;
	bSizerButtonsFootprint = new wxBoxSizer( wxVERTICAL );
	
	m_btnAddFootprint = new wxButton( this, wxID_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnAddFootprint->SetToolTip( _("Add a search path for footprint libraries.") );
	
	bSizerButtonsFootprint->Add( m_btnAddFootprint, 0, wxALL, 5 );
	
	m_btnRemoveFootprint = new wxButton( this, wxID_REMOVE, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnRemoveFootprint->SetToolTip( _("Remove the currently selected path.") );
	
	bSizerButtonsFootprint->Add( m_btnRemoveFootprint, 0, wxALL, 5 );
	
	
	bSizerFootprints->Add( bSizerButtonsFootprint, 0, wxEXPAND, 5 );
	
	
	bSizerMain->Add( bSizerFootprints, 1, wxEXPAND, 5 );
	
	m_lblSchematicPath = new wxStaticText( this, wxID_ANY, _("Schematic symbol libraries search paths"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblSchematicPath->Wrap( -1 );
	bSizerMain->Add( m_lblSchematicPath, 0, wxTOP|wxRIGHT|wxLEFT, 8 );
	
	wxBoxSizer* bSizerSymbol;
	bSizerSymbol = new wxBoxSizer( wxHORIZONTAL );
	
	m_lstSymbols = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_SINGLE|wxLB_SORT ); 
	m_lstSymbols->SetToolTip( _("The librarian searches for symbol libraries in these paths.\n(Note that the librarian does not recurse into sub-paths.)") );
	
	bSizerSymbol->Add( m_lstSymbols, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerButtonsSchematic;
	bSizerButtonsSchematic = new wxBoxSizer( wxVERTICAL );
	
	m_btnAddSymbol = new wxButton( this, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnAddSymbol->SetToolTip( _("Add a search path for symbol libraries.") );
	
	bSizerButtonsSchematic->Add( m_btnAddSymbol, 0, wxALL, 5 );
	
	m_btnRemoveSymbol = new wxButton( this, wxID_ANY, _("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnRemoveSymbol->SetToolTip( _("Remove the currently selected path.") );
	
	bSizerButtonsSchematic->Add( m_btnRemoveSymbol, 0, wxALL, 5 );
	
	
	bSizerSymbol->Add( bSizerButtonsSchematic, 0, wxEXPAND, 5 );
	
	
	bSizerMain->Add( bSizerSymbol, 1, wxEXPAND, 5 );
	
	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();
	
	bSizerMain->Add( m_sdbSizer, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_lstFootprints->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgPaths::OnFootprintPathSelect ), NULL, this );
	m_btnAddFootprint->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnAddFootprintPath ), NULL, this );
	m_btnRemoveFootprint->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnRemoveFootprintPath ), NULL, this );
	m_lstSymbols->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgPaths::OnSymbolPathSelect ), NULL, this );
	m_btnAddSymbol->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnAddSymbolPath ), NULL, this );
	m_btnRemoveSymbol->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnRemoveSymbolPath ), NULL, this );
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnOK ), NULL, this );
}

DlgPaths::~DlgPaths()
{
	// Disconnect Events
	m_lstFootprints->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgPaths::OnFootprintPathSelect ), NULL, this );
	m_btnAddFootprint->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnAddFootprintPath ), NULL, this );
	m_btnRemoveFootprint->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnRemoveFootprintPath ), NULL, this );
	m_lstSymbols->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgPaths::OnSymbolPathSelect ), NULL, this );
	m_btnAddSymbol->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnAddSymbolPath ), NULL, this );
	m_btnRemoveSymbol->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnRemoveSymbolPath ), NULL, this );
	m_sdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgPaths::OnOK ), NULL, this );
	
}

DlgReport::DlgReport( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerColumns;
	bSizerColumns = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerPage;
	bSizerPage = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerPage;
	fgSizerPage = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerPage->SetFlexibleDirection( wxBOTH );
	fgSizerPage->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblPageSize = new wxStaticText( this, wxID_ANY, _("Page size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPageSize->Wrap( -1 );
	fgSizerPage->Add( m_lblPageSize, 0, wxALL, 5 );
	
	wxString m_choicePageSizeChoices[] = { _("A3"), _("A4"), _("Executive"), _("Legal"), _("Letter") };
	int m_choicePageSizeNChoices = sizeof( m_choicePageSizeChoices ) / sizeof( wxString );
	m_choicePageSize = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePageSizeNChoices, m_choicePageSizeChoices, 0 );
	m_choicePageSize->SetSelection( 0 );
	m_choicePageSize->SetToolTip( _("The paper size for the PDF report.") );
	
	fgSizerPage->Add( m_choicePageSize, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizerPage->Add( fgSizerPage, 0, wxTOP|wxEXPAND, 5 );
	
	wxString m_radioLayoutChoices[] = { _("Portrait"), _("Landscape") };
	int m_radioLayoutNChoices = sizeof( m_radioLayoutChoices ) / sizeof( wxString );
	m_radioLayout = new wxRadioBox( this, wxID_ANY, _("Lay-out"), wxDefaultPosition, wxDefaultSize, m_radioLayoutNChoices, m_radioLayoutChoices, 2, wxRA_SPECIFY_COLS );
	m_radioLayout->SetSelection( 0 );
	m_radioLayout->SetToolTip( _("Page lay-out, portrait or landscape.") );
	
	bSizerPage->Add( m_radioLayout, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizerFont;
	fgSizerFont = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizerFont->SetFlexibleDirection( wxBOTH );
	fgSizerFont->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblFontSize = new wxStaticText( this, wxID_ANY, _("Font size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontSize->Wrap( -1 );
	fgSizerFont->Add( m_lblFontSize, 0, wxTOP|wxLEFT, 5 );
	
	m_spinFontSize = new wxSpinCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 8, 32, 8 );
	m_spinFontSize->SetToolTip( _("The font size of the dimensions, in points.") );
	m_spinFontSize->SetMinSize( wxSize( 50,-1 ) );
	m_spinFontSize->SetMaxSize( wxSize( 100,-1 ) );
	
	fgSizerFont->Add( m_spinFontSize, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_lblFontUnit = new wxStaticText( this, wxID_ANY, _("point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontUnit->Wrap( -1 );
	fgSizerFont->Add( m_lblFontUnit, 0, wxTOP, 5 );
	
	
	bSizerPage->Add( fgSizerFont, 0, wxEXPAND|wxBOTTOM, 5 );
	
	
	bSizerColumns->Add( bSizerPage, 1, wxEXPAND|wxBOTTOM|wxRIGHT, 5 );
	
	wxBoxSizer* bSizerOptions;
	bSizerOptions = new wxBoxSizer( wxVERTICAL );
	
	m_chkDescription = new wxCheckBox( this, wxID_ANY, _("Include des&cription"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkDescription->SetToolTip( _("Include the description of the component in the report.") );
	
	bSizerOptions->Add( m_chkDescription, 0, wxALL, 5 );
	
	m_chkValueLabels = new wxCheckBox( this, wxID_ANY, _("Draw  &labels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkValueLabels->SetToolTip( _("Draw the reference and value labels in the footprint.\n(Footprint reports only.)") );
	
	bSizerOptions->Add( m_chkValueLabels, 0, wxALL, 5 );
	
	m_chkPadInfo = new wxCheckBox( this, wxID_ANY, _("Include &pad information"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkPadInfo->SetToolTip( _("Include pad pitch and size information in the report.\n(Footprint reports only.)") );
	
	bSizerOptions->Add( m_chkPadInfo, 0, wxALL, 5 );
	
	m_chkFPList = new wxCheckBox( this, wxID_ANY, _("Include footprint list"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkFPList->SetToolTip( _("Include the list of suitable footprints in the report.\n(Symbol reports only.)") );
	
	bSizerOptions->Add( m_chkFPList, 0, wxALL, 5 );
	
	
	bSizerColumns->Add( bSizerOptions, 1, wxEXPAND|wxBOTTOM|wxLEFT, 5 );
	
	
	bSizerMain->Add( bSizerColumns, 1, wxEXPAND, 5 );
	
	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();
	
	bSizerMain->Add( m_sdbSizer, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgReport::OnOK ), NULL, this );
}

DlgReport::~DlgReport()
{
	// Disconnect Events
	m_sdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgReport::OnOK ), NULL, this );
	
}

DlgOptions::DlgOptions( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerOptionsMain;
	bSizerOptionsMain = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerFont;
	fgSizerFont = new wxFlexGridSizer( 3, 3, 0, 0 );
	fgSizerFont->SetFlexibleDirection( wxBOTH );
	fgSizerFont->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblFontSize = new wxStaticText( this, wxID_ANY, _("Font size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontSize->Wrap( -1 );
	m_lblFontSize->SetToolTip( _("Font size used in the viewport for dimensions, pin numbers and other miscellaneous labels") );
	
	fgSizerFont->Add( m_lblFontSize, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_spinFontSize = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 5, 20, 8 );
	m_spinFontSize->SetToolTip( _("The font size of the dimensions and pin numbers, in point.") );
	m_spinFontSize->SetMinSize( wxSize( 50,-1 ) );
	m_spinFontSize->SetMaxSize( wxSize( 100,-1 ) );
	
	fgSizerFont->Add( m_spinFontSize, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_lblFontUnit = new wxStaticText( this, wxID_ANY, _("point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFontUnit->Wrap( -1 );
	fgSizerFont->Add( m_lblFontUnit, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_lblDimOffset = new wxStaticText( this, wxID_ANY, _("Dimension offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDimOffset->Wrap( -1 );
	m_lblDimOffset->SetToolTip( _("The offset (in pixels) of the dimensions from the pads in a footprint.") );
	
	fgSizerFont->Add( m_lblDimOffset, 0, wxALL, 5 );
	
	m_spinDimOffset = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 200, 49 );
	m_spinDimOffset->SetToolTip( _("The (minumum) offset of the dimensions from the pads, in pixels.") );
	m_spinDimOffset->SetMinSize( wxSize( 50,-1 ) );
	m_spinDimOffset->SetMaxSize( wxSize( 100,-1 ) );
	
	fgSizerFont->Add( m_spinDimOffset, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_lblDimUnit = new wxStaticText( this, wxID_ANY, _("pixels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDimUnit->Wrap( -1 );
	fgSizerFont->Add( m_lblDimUnit, 0, wxALL, 5 );
	
	
	bSizerOptionsMain->Add( fgSizerFont, 0, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizerOptions;
	bSizerOptions = new wxBoxSizer( wxVERTICAL );
	
	m_chkDrawLabels = new wxCheckBox( this, wxID_ANY, _("Draw footprint/symbol labels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkDrawLabels->SetToolTip( _("Draw the reference and value labels in the footprint or schematic symbol.") );
	
	bSizerOptions->Add( m_chkDrawLabels, 0, wxALL, 5 );
	
	m_chkFullPaths = new wxCheckBox( this, wxID_ANY, _("Show full library paths"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkFullPaths->SetToolTip( _("If checked. shows the full path to each library in the footprint/symbol lists.\nIf not checked, only shows the base filenames.") );
	
	bSizerOptions->Add( m_chkFullPaths, 0, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerOptions->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_chkCopyVRML = new wxCheckBox( this, wxID_ANY, _("Copy 3D models"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkCopyVRML->SetToolTip( _("Copy the VRML models when moving/copying the  footprints to other libraries.\nVRML models are not deleted when deleting footprints, and not renamed.") );
	
	bSizerOptions->Add( m_chkCopyVRML, 0, wxALL, 5 );
	
	m_chkDisableTemplates = new wxCheckBox( this, wxID_ANY, _("Disable edits via templates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_chkDisableTemplates->SetToolTip( _("By default, if you edit the parameters of a footprint or symbol and that footprint/symbol\nis based on a template, the Librarian rebuilds the part from the template.\nHowever, this looses changes that you may have made in the KiCad module editor.\nPutting a checkmark in this option disables rebuilding from a template.") );
	
	bSizerOptions->Add( m_chkDisableTemplates, 0, wxALL, 5 );
	
	
	bSizerOptions->Add( 0, 5, 1, wxEXPAND, 5 );
	
	
	bSizerOptionsMain->Add( bSizerOptions, 1, wxEXPAND|wxBOTTOM, 5 );
	
	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();
	
	bSizerOptionsMain->Add( m_sdbSizer, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizerOptionsMain );
	this->Layout();
	bSizerOptionsMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgOptions::OnOK ), NULL, this );
}

DlgOptions::~DlgOptions()
{
	// Disconnect Events
	m_sdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgOptions::OnOK ), NULL, this );
	
}

DlgTemplateOpts::DlgTemplateOpts( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,300 ), wxDefaultSize );
	
	wxBoxSizer* bSizerDlg;
	bSizerDlg = new wxBoxSizer( wxVERTICAL );
	
	m_lblVariables = new wxStaticText( this, wxID_ANY, _("Template variables"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblVariables->Wrap( -1 );
	m_lblVariables->SetToolTip( _("Template variables, used in footprints that are created from a template") );
	
	bSizerDlg->Add( m_lblVariables, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	m_gridTemplateVars = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gridTemplateVars->CreateGrid( 5, 3 );
	m_gridTemplateVars->EnableEditing( true );
	m_gridTemplateVars->EnableGridLines( true );
	m_gridTemplateVars->EnableDragGridSize( false );
	m_gridTemplateVars->SetMargins( 0, 0 );
	
	// Columns
	m_gridTemplateVars->AutoSizeColumns();
	m_gridTemplateVars->EnableDragColMove( false );
	m_gridTemplateVars->EnableDragColSize( true );
	m_gridTemplateVars->SetColLabelSize( 20 );
	m_gridTemplateVars->SetColLabelValue( 0, _("Description") );
	m_gridTemplateVars->SetColLabelValue( 1, _("Name") );
	m_gridTemplateVars->SetColLabelValue( 2, _("Value") );
	m_gridTemplateVars->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridTemplateVars->EnableDragRowSize( true );
	m_gridTemplateVars->SetRowLabelSize( 0 );
	m_gridTemplateVars->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridTemplateVars->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_gridTemplateVars->SetToolTip( _("Dimensions used in templates. Dimensions are in mm, except the Solder Paste Reduction (in percent).") );
	
	bSizerDlg->Add( m_gridTemplateVars, 1, wxEXPAND|wxALL, 5 );
	
	m_txtInfo = new wxTextCtrl( this, wxID_ANY, _("You can only change the values of the predefined variables in the list. When a value is set, it overrides the value that the template specifies. When a value is empty, the default from the template is used."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY|wxTE_WORDWRAP );
	m_txtInfo->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	bSizerDlg->Add( m_txtInfo, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();
	
	bSizerDlg->Add( m_sdbSizer, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizerDlg );
	this->Layout();
	bSizerDlg->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgTemplateOpts::OnOK ), NULL, this );
}

DlgTemplateOpts::~DlgTemplateOpts()
{
	// Disconnect Events
	m_sdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgTemplateOpts::OnOK ), NULL, this );
	
}

DlgNewFootprint::DlgNewFootprint( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerListInfo;
	bSizerListInfo = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerListName;
	bSizerListName = new wxBoxSizer( wxVERTICAL );
	
	m_lstTemplates = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SINGLE ); 
	m_lstTemplates->SetToolTip( _("These are template files.\nChoose one that best matches the footprint that you need to make.") );
	
	bSizerListName->Add( m_lstTemplates, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerNameInfo;
	bSizerNameInfo = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblName = new wxStaticText( this, wxID_ANY, _("Footprint name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblName->Wrap( -1 );
	bSizerNameInfo->Add( m_lblName, 0, wxALL, 5 );
	
	m_txtName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtName->SetToolTip( _("Enter the name of the new footprint.") );
	
	bSizerNameInfo->Add( m_txtName, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizerListName->Add( bSizerNameInfo, 0, wxEXPAND, 5 );
	
	
	bSizerListInfo->Add( bSizerListName, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerExample;
	bSizerExample = new wxBoxSizer( wxVERTICAL );
	
	m_bmpExample = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 128,128 ), wxSTATIC_BORDER );
	m_bmpExample->SetToolTip( _("Preview of the currently selected template (with default parameters).") );
	
	bSizerExample->Add( m_bmpExample, 0, wxALL, 5 );
	
	m_txtDescription = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_txtDescription->SetToolTip( _("Description and elements of the currently selected template.") );
	m_txtDescription->SetMinSize( wxSize( -1,50 ) );
	
	bSizerExample->Add( m_txtDescription, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizerListInfo->Add( bSizerExample, 0, wxEXPAND, 5 );
	
	
	bSizerMain->Add( bSizerListInfo, 1, wxEXPAND, 5 );
	
	m_sdbSizerOkCancel = new wxStdDialogButtonSizer();
	m_sdbSizerOkCancelOK = new wxButton( this, wxID_OK );
	m_sdbSizerOkCancel->AddButton( m_sdbSizerOkCancelOK );
	m_sdbSizerOkCancelCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerOkCancel->AddButton( m_sdbSizerOkCancelCancel );
	m_sdbSizerOkCancel->Realize();
	
	bSizerMain->Add( m_sdbSizerOkCancel, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_lstTemplates->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgNewFootprint::OnTemplateSelect ), NULL, this );
	m_sdbSizerOkCancelOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgNewFootprint::OnOk ), NULL, this );
}

DlgNewFootprint::~DlgNewFootprint()
{
	// Disconnect Events
	m_lstTemplates->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgNewFootprint::OnTemplateSelect ), NULL, this );
	m_sdbSizerOkCancelOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgNewFootprint::OnOk ), NULL, this );
	
}

DlgRemoteLink::DlgRemoteLink( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblURL = new wxStaticText( this, wxID_ANY, _("&URL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblURL->Wrap( -1 );
	fgSizer->Add( m_lblURL, 0, wxALL, 5 );
	
	m_txtURL = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtURL->SetToolTip( _("The complete URL to the repository. (Mandatory)") );
	m_txtURL->SetMinSize( wxSize( 250,-1 ) );
	
	fgSizer->Add( m_txtURL, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_lblUserName = new wxStaticText( this, wxID_ANY, _("User &name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblUserName->Wrap( -1 );
	fgSizer->Add( m_lblUserName, 0, wxALL, 5 );
	
	m_txtUserName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtUserName->SetToolTip( _("The user name to access the repository. (Mandatory)") );
	
	fgSizer->Add( m_txtUserName, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_lblPassword = new wxStaticText( this, wxID_ANY, _("&Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPassword->Wrap( -1 );
	fgSizer->Add( m_lblPassword, 0, wxALL, 5 );
	
	m_txtPassword = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_txtPassword->SetToolTip( _("The password to access the repository. (Mandatory)") );
	
	fgSizer->Add( m_txtPassword, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_lblAuthentication = new wxStaticText( this, wxID_ANY, _("Authentication"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAuthentication->Wrap( -1 );
	fgSizer->Add( m_lblAuthentication, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerAuth;
	bSizerAuth = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerAuthUser;
	bSizerAuthUser = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblAuthUser = new wxStaticText( this, wxID_ANY, _("user"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAuthUser->Wrap( -1 );
	bSizerAuthUser->Add( m_lblAuthUser, 0, wxALL, 5 );
	
	m_txtAuthUser = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtAuthUser->SetToolTip( _("A user name for access to the server; this need not be the same as the user name for the repository access. (Optional)") );
	
	bSizerAuthUser->Add( m_txtAuthUser, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_lblAuthPwd = new wxStaticText( this, wxID_ANY, _("password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAuthPwd->Wrap( -1 );
	bSizerAuthUser->Add( m_lblAuthPwd, 0, wxALL, 5 );
	
	m_txtAuthPWD = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_txtAuthPWD->SetToolTip( _("The password for access to the server, in the case of protected connections. (Optional)") );
	
	bSizerAuthUser->Add( m_txtAuthPWD, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizerAuth->Add( bSizerAuthUser, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerAuthOpt;
	bSizerAuthOpt = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkVerifyPeer = new wxCheckBox( this, wxID_ANY, _("Verify certificate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkVerifyPeer->SetToolTip( _("Verify that the certificate is issued by a CA in the list of trusted root CAs (and that it has not expired).") );
	
	bSizerAuthOpt->Add( m_checkVerifyPeer, 0, wxALL, 5 );
	
	m_checkVerifyHost = new wxCheckBox( this, wxID_ANY, _("Verify host name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkVerifyHost->SetToolTip( _("Verify that the host name matches the name in the certificate.") );
	
	bSizerAuthOpt->Add( m_checkVerifyHost, 0, wxALL, 5 );
	
	
	bSizerAuth->Add( bSizerAuthOpt, 1, wxEXPAND, 5 );
	
	
	fgSizer->Add( bSizerAuth, 1, wxEXPAND, 5 );
	
	
	fgSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_lblHelp = new wxStaticText( this, wxID_ANY, _("Fill in the above fields. If you do not have an account yet, click on the button \"Sign up\"."), wxDefaultPosition, wxDefaultSize, 0|wxSTATIC_BORDER );
	m_lblHelp->Wrap( 300 );
	m_lblHelp->SetMinSize( wxSize( 300,-1 ) );
	
	fgSizer->Add( m_lblHelp, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizerMain->Add( fgSizer, 1, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizerButtons;
	bSizerButtons = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizerButtons->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btnSignUp = new wxButton( this, wxID_ANY, _("Sign up"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_btnSignUp, 0, wxALL, 3 );
	
	m_btnOK = new wxButton( this, wxID_ANY, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_btnOK, 0, wxALL, 3 );
	
	m_btnCancel = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerButtons->Add( m_btnCancel, 0, wxALL, 3 );
	
	
	bSizerMain->Add( bSizerButtons, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_btnSignUp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteLink::OnSignUp ), NULL, this );
	m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteLink::OnOK ), NULL, this );
	m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteLink::OnCancel ), NULL, this );
}

DlgRemoteLink::~DlgRemoteLink()
{
	// Disconnect Events
	m_btnSignUp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteLink::OnSignUp ), NULL, this );
	m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteLink::OnOK ), NULL, this );
	m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteLink::OnCancel ), NULL, this );
	
}

DlgRemoteSignUp::DlgRemoteSignUp( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblURL = new wxStaticText( this, wxID_ANY, _("&URL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblURL->Wrap( -1 );
	fgSizer->Add( m_lblURL, 0, wxALL, 5 );
	
	m_txtURL = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtURL->SetToolTip( _("The complete URL to the repository. (Mandatory)") );
	m_txtURL->SetMinSize( wxSize( 250,-1 ) );
	
	fgSizer->Add( m_txtURL, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_lblUserName = new wxStaticText( this, wxID_ANY, _("User &name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblUserName->Wrap( -1 );
	fgSizer->Add( m_lblUserName, 0, wxALL, 5 );
	
	m_txtUserName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtUserName->SetToolTip( _("The user name to access the repository. (Mandatory)") );
	
	fgSizer->Add( m_txtUserName, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_lbEmail = new wxStaticText( this, wxID_ANY, _("&Email"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbEmail->Wrap( -1 );
	fgSizer->Add( m_lbEmail, 0, wxALL, 5 );
	
	m_txtEmail = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtEmail->SetToolTip( _("The email address linked to the account. (Mandatory)") );
	
	fgSizer->Add( m_txtEmail, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_lblAuthentication = new wxStaticText( this, wxID_ANY, _("Authentication"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAuthentication->Wrap( -1 );
	fgSizer->Add( m_lblAuthentication, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerAuth;
	bSizerAuth = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerAuthUser;
	bSizerAuthUser = new wxBoxSizer( wxHORIZONTAL );
	
	m_lblAuthUser = new wxStaticText( this, wxID_ANY, _("user"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAuthUser->Wrap( -1 );
	bSizerAuthUser->Add( m_lblAuthUser, 0, wxALL, 5 );
	
	m_txtAuthUser = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtAuthUser->SetToolTip( _("A user name for access to the server; this need not be the same as the user name for the repository access. (Optional)") );
	
	bSizerAuthUser->Add( m_txtAuthUser, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_lblAuthPwd = new wxStaticText( this, wxID_ANY, _("password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAuthPwd->Wrap( -1 );
	bSizerAuthUser->Add( m_lblAuthPwd, 0, wxALL, 5 );
	
	m_txtAuthPWD = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_txtAuthPWD->SetToolTip( _("The password for access to the server, in the case of protected connections. (Optional)") );
	
	bSizerAuthUser->Add( m_txtAuthPWD, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizerAuth->Add( bSizerAuthUser, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerAuthOpt;
	bSizerAuthOpt = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkVerifyPeer = new wxCheckBox( this, wxID_ANY, _("Verify certificate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkVerifyPeer->SetToolTip( _("Verify that the certificate is issued by a CA in the list of trusted root CAs (and that it has not expired).") );
	
	bSizerAuthOpt->Add( m_checkVerifyPeer, 0, wxALL, 5 );
	
	m_checkVerifyHost = new wxCheckBox( this, wxID_ANY, _("Verify host name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkVerifyHost->SetToolTip( _("Verify that the host name matches the name in the certificate.") );
	
	bSizerAuthOpt->Add( m_checkVerifyHost, 0, wxALL, 5 );
	
	
	bSizerAuth->Add( bSizerAuthOpt, 1, wxEXPAND, 5 );
	
	
	fgSizer->Add( bSizerAuth, 1, wxEXPAND, 5 );
	
	
	fgSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_lblHelp = new wxStaticText( this, wxID_ANY, _("Fill in the above fields. You will receive a password by email."), wxDefaultPosition, wxDefaultSize, 0|wxSTATIC_BORDER );
	m_lblHelp->Wrap( 300 );
	m_lblHelp->SetMinSize( wxSize( 300,-1 ) );
	
	fgSizer->Add( m_lblHelp, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizerMain->Add( fgSizer, 1, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();
	
	bSizerMain->Add( m_sdbSizer, 0, wxEXPAND|wxALL, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteSignUp::OnOK ), NULL, this );
}

DlgRemoteSignUp::~DlgRemoteSignUp()
{
	// Disconnect Events
	m_sdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgRemoteSignUp::OnOK ), NULL, this );
	
}

DlgNewSymbol::DlgNewSymbol( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerListInfo;
	bSizerListInfo = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerListName;
	bSizerListName = new wxBoxSizer( wxVERTICAL );
	
	m_lstTemplates = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_NEEDED_SB|wxLB_SINGLE ); 
	m_lstTemplates->SetToolTip( _("These are template files.\nChoose one that best matches the symbol that you need to make.") );
	
	bSizerListName->Add( m_lstTemplates, 1, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizerNameRef;
	fgSizerNameRef = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerNameRef->SetFlexibleDirection( wxHORIZONTAL );
	fgSizerNameRef->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lblName = new wxStaticText( this, wxID_ANY, _("Symbol name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblName->Wrap( -1 );
	fgSizerNameRef->Add( m_lblName, 0, wxALL, 5 );
	
	m_txtName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtName->SetToolTip( _("Enter the name of the new symbol.") );
	
	fgSizerNameRef->Add( m_txtName, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_lblPrefix = new wxStaticText( this, wxID_ANY, _("Prefix"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPrefix->Wrap( -1 );
	fgSizerNameRef->Add( m_lblPrefix, 0, wxALL, 5 );
	
	m_txtPrefix = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtPrefix->SetToolTip( _("Enter the prefix for the reference.\nFor example: U for a integrated circuit, R for a resistor, ...") );
	
	fgSizerNameRef->Add( m_txtPrefix, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizerListName->Add( fgSizerNameRef, 0, wxEXPAND, 5 );
	
	
	bSizerListInfo->Add( bSizerListName, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerExample;
	bSizerExample = new wxBoxSizer( wxVERTICAL );
	
	m_bmpExample = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 128,128 ), wxSTATIC_BORDER );
	m_bmpExample->SetToolTip( _("Preview of the currently selected template (with default parameters).") );
	
	bSizerExample->Add( m_bmpExample, 0, wxALL, 5 );
	
	m_txtDescription = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	m_txtDescription->SetToolTip( _("Description and elements of the currently selected template.") );
	m_txtDescription->SetMinSize( wxSize( -1,50 ) );
	
	bSizerExample->Add( m_txtDescription, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizerListInfo->Add( bSizerExample, 0, wxEXPAND, 5 );
	
	
	bSizerMain->Add( bSizerListInfo, 1, wxEXPAND, 5 );
	
	m_sdbSizerOkCancel = new wxStdDialogButtonSizer();
	m_sdbSizerOkCancelOK = new wxButton( this, wxID_OK );
	m_sdbSizerOkCancel->AddButton( m_sdbSizerOkCancelOK );
	m_sdbSizerOkCancelCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerOkCancel->AddButton( m_sdbSizerOkCancelCancel );
	m_sdbSizerOkCancel->Realize();
	
	bSizerMain->Add( m_sdbSizerOkCancel, 0, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_lstTemplates->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgNewSymbol::OnTemplateSelect ), NULL, this );
	m_sdbSizerOkCancelOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgNewSymbol::OnOk ), NULL, this );
}

DlgNewSymbol::~DlgNewSymbol()
{
	// Disconnect Events
	m_lstTemplates->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( DlgNewSymbol::OnTemplateSelect ), NULL, this );
	m_sdbSizerOkCancelOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgNewSymbol::OnOk ), NULL, this );
	
}
