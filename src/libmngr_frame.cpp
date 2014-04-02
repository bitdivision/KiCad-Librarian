/*
 *  Librarian for KiCad, a free EDA CAD application.
 *  This file contains the code for the main frame, which is almost all of the
 *  user-interface code.
 *
 *  Copyright (C) 2013-2014 CompuPhase
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not
 *  use this file except in compliance with the License. You may obtain a copy
 *  of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 *
 *  $Id: libmngr_frame.cpp 5030 2014-02-10 08:33:24Z thiadmer $
 */
#include "librarymanager.h"
#include "libmngr_frame.h"
#include "libmngr_dlgoptions.h"
#include "libmngr_dlgnewfootprint.h"
#include "libmngr_dlgnewsymbol.h"
#include "libmngr_paths.h"
#include "libmngr_dlgreport.h"
#include "libmngr_dlgtemplate.h"
#include "pdfreport.h"
#include "remotelink.h"
#include "rpn.h"
#include "svnrev.h"
#if !defined NO_CURL
	#include "libmngr_dlgremotelink.h"
#endif
#include <wx/aboutdlg.h>
#include <wx/accel.h>
#include <wx/choicdlg.h>
#include <wx/config.h>
#include <wx/cursor.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/mimetype.h>
#include <wx/textdlg.h>
#include <wx/textfile.h>
#include <wx/utils.h>
#include <math.h>

#include "res/logo32.xpm"
#include "res/logo64.xpm"


#if !defined M_PI
	#define M_PI 3.14159265358979323846
#endif

#if !defined sizearray
	#define sizearray(a)	(sizeof(a) / sizeof((a)[0]))
#endif

#define LINE_OFFSET		5	/* in pixels (offset of the dimension line from the centre of the pad) */
#define SCALE_MIN		0.8
#define SCALE_DEFAULT   8
#define SCALE_MAX		80
#if defined _WIN32
	#define PANEL_WIDTH	200	/* default width of the sidebar, in pixels */
#else
	#define PANEL_WIDTH	240
#endif


libmngrFrame::libmngrFrame( wxWindow* parent )
:
AppFrame( parent )
{
	#if defined _MSC_VER
		_CrtCheckMemory();
	#endif

	SetIcon(wxIcon(logo32_xpm));

	wxAcceleratorEntry entries[5];
	entries[0].Set(wxACCEL_CTRL, '+', IDT_ZOOMIN);		// Ctrl-+
	entries[1].Set(wxACCEL_CTRL, '=', IDT_ZOOMIN);		// On US keyboards, Ctrl-= is needed instead of Ctrl-+
	entries[2].Set(wxACCEL_CTRL, WXK_NUMPAD_ADD, IDT_ZOOMIN);
	entries[3].Set(wxACCEL_CTRL, '-', IDT_ZOOMOUT);
	entries[4].Set(wxACCEL_CTRL, WXK_NUMPAD_SUBTRACT, IDT_ZOOMOUT);
	wxAcceleratorTable accel(sizearray(entries), entries);
	SetAcceleratorTable(accel);

	/* restore application size */
	wxConfig *config = new wxConfig(APP_NAME);
	long w = 0, h = 0;
	if (config->Read(wxT("settings/framewidth"), &w) && config->Read(wxT("settings/frameheight"), &h))
		SetSize(w, h);
	if (!config->Read(wxT("settings/scale"), &Scale) || Scale < SCALE_MIN || Scale > SCALE_MAX)
		Scale = SCALE_DEFAULT;

	config->Read(wxT("settings/symbolmode"), &SymbolMode, false);
	m_menubar->Check(IDM_SCHEMATICMODE, SymbolMode);
	config->Read(wxT("settings/comparemode"), &CompareMode, false);
	m_menubar->Check(IDM_COMPAREMODE, CompareMode);
	m_toolBar->EnableTool(IDT_LEFTFOOTPRINT, CompareMode);
	m_toolBar->EnableTool(IDT_RIGHTFOOTPRINT, CompareMode);
	if (CompareMode) {
		m_toolBar->ToggleTool(IDT_LEFTFOOTPRINT, true);
		m_toolBar->ToggleTool(IDT_RIGHTFOOTPRINT, true);
	}
	m_radioViewLeft->Enable(CompareMode);
	m_radioViewRight->Enable(CompareMode);
	m_radioViewLeft->SetValue(true);

	config->Read(wxT("display/showlabels"), &ShowLabels, true);
	config->Read(wxT("display/fullpath"), &ShowFullPaths, false);
	config->Read(wxT("settings/copyvrml"), &CopyVRML, true);
	config->Read(wxT("settings/disabletemplate"), &DontRebuildTemplate, false);

	bool measurements = true;
	config->Read(wxT("display/measurements"), &measurements, true);
	m_toolBar->ToggleTool(IDT_MEASUREMENTS, measurements);

	long pos;
	if (!config->Read(wxT("settings/splitterpanel"), &pos))
		pos = -1;
	if (pos <= 0)
		m_splitterViewPanel->Unsplit();
	m_menubar->Check(IDM_DETAILSPANEL, pos > 0);
	m_toolBar->ToggleTool(IDT_DETAILSPANEL, pos > 0);

	FontSizeLegend = config->Read(wxT("display/fontsize"), 8L);
	DimensionOffset = config->Read(wxT("display/dimoffset"), 50L);

	delete config;

	/* configure the list controls (columns) */
	m_listModulesLeft->InsertColumn(0, wxT("Part"), wxLIST_FORMAT_LEFT);
	m_listModulesLeft->InsertColumn(1, wxT("Library"), wxLIST_FORMAT_LEFT);
	m_listModulesLeft->InsertColumn(2, wxT("Path"), wxLIST_FORMAT_LEFT);
	m_listModulesRight->InsertColumn(0, wxT("Part"), wxLIST_FORMAT_LEFT);
	m_listModulesRight->InsertColumn(1, wxT("Library"), wxLIST_FORMAT_LEFT);
	m_listModulesRight->InsertColumn(2, wxT("Path"), wxLIST_FORMAT_LEFT);

	PinData[0] = PinData[1] = NULL;
	PinDataCount[0] = PinDataCount[1] = 0;

	/* set starting state and labels for the buttons (no footprint/symbol selected yet) */
	ToggleMode(SymbolMode);

	#if defined NO_CURL
		/* disable the repository functions in the menu */
		m_menubar->Enable(IDM_DLGREMOTE, false);
	#endif

	/* set a single-shot timer to set the initial splitter positions (this action
	   must be delayed, because of the internals of wxWidgets) */
	m_Timer = new wxTimer(this, IDM_TIMER);
	Connect(IDM_TIMER, wxEVT_TIMER, wxTimerEventHandler(libmngrFrame::OnTimer));
	m_Timer->Start(100, true);

	wxString path = theApp->GetFontFile();
	VFont.Read(path.mb_str(wxConvFile));
}

void libmngrFrame::OnTimer(wxTimerEvent& /*event*/)
{
	wxConfig *config = new wxConfig(APP_NAME);
	long pos;
	if (config->Read(wxT("settings/splitter"), &pos))
		m_splitter->SetSashPosition(pos);
	if (config->Read(wxT("settings/splitterpanel"), &pos) && pos > 0)
		m_splitterViewPanel->SetSashPosition(pos);
	delete config;
}

void libmngrFrame::OnCloseApp( wxCloseEvent& event )
{
	for (int fp = 0; fp < 2; fp++)
		PartData[fp].Clear();

	wxSize size = GetSize();
	wxConfig *config = new wxConfig(APP_NAME);
	config->Write(wxT("settings/framewidth"), size.GetWidth());
	config->Write(wxT("settings/frameheight"), size.GetHeight());

	config->Write(wxT("settings/scale"), Scale);
	config->Write(wxT("settings/symbolmode"), SymbolMode);
	config->Write(wxT("settings/comparemode"), CompareMode);

	config->Write(wxT("settings/splitter"), m_splitter->GetSashPosition());
	if (m_splitterViewPanel->IsSplit() && m_toolBar->GetToolState(IDT_DETAILSPANEL))
		config->Write(wxT("settings/splitterpanel"), m_splitterViewPanel->GetSashPosition());
	else
		config->Write(wxT("settings/splitterpanel"), -1);

	config->Write(wxT("display/measurements"), m_toolBar->GetToolState(IDT_MEASUREMENTS));

	delete config;
	event.Skip();
}

void libmngrFrame::OnNewLibrary( wxCommandEvent& /*event*/ )
{
	wxString filter;
	if (SymbolMode)
		filter = wxT("Symbol libraries (*.lib)|*.lib");
	else
		filter = wxT("Legacy (*.mod)|*.mod|Legacy (mm) (*.mod)|*.mod|s-expression|*.pretty");
	wxFileDialog* dlg = new wxFileDialog(this, wxT("New library..."),
                                         wxEmptyString, wxEmptyString,
                                         filter, wxFD_SAVE);
	if (!SymbolMode)
		dlg->SetFilterIndex(1);	/* by default, create libraries in legacy version 2 format */
	if (dlg->ShowModal() != wxID_OK)
		return;
	/* set default extension */
	wxFileName fname(dlg->GetPath());
	if (fname.GetExt().length() == 0) {
		if (SymbolMode)
			fname.SetExt(wxT("lib"));
		else if (dlg->GetFilterIndex() <= 1)
			fname.SetExt(wxT("mod"));
		else
			fname.SetExt(wxT("pretty"));
	}

	if (dlg->GetFilterIndex() <= 1) {
		wxTextFile file;
		if (!file.Create(fname.GetFullPath())) {
			if (!file.Open(fname.GetFullPath())) {
				wxMessageBox(wxT("Failed to create library ") + fname.GetFullPath());
				return;
			}
		}
		file.Clear();
		if (SymbolMode) {
			file.AddLine(wxT("EESchema-LIBRARY Version 2.3  Date: ") + wxNow());
			file.AddLine(wxT("#encoding utf-8"));
			file.AddLine(wxT("#"));
			file.AddLine(wxT("#End Library"));
		} else {
			file.AddLine(wxT("PCBNEW-LibModule-V1 ") + wxNow());
			file.AddLine(wxT("# encoding utf-8"));
			if (dlg->GetFilterIndex() == 1)
				file.AddLine(wxT("Units mm"));
			file.AddLine(wxT("$INDEX"));
			file.AddLine(wxT("$EndINDEX"));
			file.AddLine(wxT("$EndLIBRARY"));
		}
		file.Write();
		file.Close();
	} else {
		wxASSERT(!SymbolMode);
		if (!wxMkdir(fname.GetFullPath())) {
			wxMessageBox(wxT("Failed to create library ") + fname.GetFullPath());
			return;
		}
	}

	/* get current selections in the left and right combo-boxes */
	wxString leftlib;
	int idx = m_choiceModuleLeft->GetCurrentSelection();
	if (idx < 0 || idx >= (int)m_choiceModuleLeft->GetCount())
		leftlib = LIB_NONE;		/* assume "none" is selected when there is no selection */
	else
		leftlib = m_choiceModuleLeft->GetString(idx);
	wxASSERT(leftlib.length() > 0);
	wxString rightlib;
	idx = m_choiceModuleRight->GetCurrentSelection();
	if (idx < 0 || idx >= (int)m_choiceModuleRight->GetCount())
		rightlib = LIB_NONE;	/* assume "none" is selected when there is no selection */
	else
		rightlib = m_choiceModuleRight->GetString(idx);
	wxASSERT(rightlib.length() > 0);

	/* refresh comboboxes */
	CollectAllLibraries(false);
	EnableButtons(0);

	/* first try to restore the previous selections */
	idx = m_choiceModuleLeft->FindString(leftlib);
	if (idx >= 0)
		m_choiceModuleLeft->SetSelection(idx);
	idx = m_choiceModuleRight->FindString(rightlib);
	if (idx >= 0)
		m_choiceModuleRight->SetSelection(idx);

	/* check whether to auto-select the new library
	 * if one of the comboboxes is set to "(None)", open it there
	 * if one of the comboboxes is set to "(All)" or "(Repository)", open it in the other
	 */
	int refresh = 0;
	/* first try left box */
	if (leftlib.CmpNoCase(LIB_NONE) == 0) {
		idx = m_choiceModuleLeft->FindString(fname.GetFullPath());
		if (idx >= 0) {
			m_choiceModuleLeft->SetSelection(idx);
			refresh = -1;
		}
	}
	/* then try the right box */
	if (refresh == 0) {
		if (rightlib.CmpNoCase(LIB_NONE) == 0) {
			idx = m_choiceModuleRight->FindString(fname.GetFullPath());
			if (idx >= 0) {
				m_choiceModuleRight->SetSelection(idx);
				refresh = 1;
			}
		}
	}
	/* neither has "(None)" selected, see if either has "(All)" or "(Repository)"
	   selected, left box first */
	if (refresh == 0) {
		if (leftlib.CmpNoCase(LIB_ALL) == 0 || leftlib.CmpNoCase(LIB_REPOS) == 0) {
			idx = m_choiceModuleRight->FindString(fname.GetFullPath());
			if (idx >= 0) {
				m_choiceModuleRight->SetSelection(idx);
				refresh = 1;
			}
		}
	}
	if (refresh == 0) {
		if (rightlib.CmpNoCase(LIB_ALL) == 0 || rightlib.CmpNoCase(LIB_REPOS) == 0) {
			idx = m_choiceModuleLeft->FindString(fname.GetFullPath());
			if (idx >= 0) {
				m_choiceModuleLeft->SetSelection(idx);
				refresh = -1;
			}
		}
	}
	if (refresh == 0) {
		/* both sides have a library selected, then just vacate the right box */
		idx = m_choiceModuleRight->FindString(fname.GetFullPath());
		if (idx >= 0) {
			m_choiceModuleRight->SetSelection(idx);
			refresh = 1;
		}
	}
	if (refresh == -1)
		HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
	else if (refresh == 1)
		HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
	m_panelView->Refresh();
}

bool libmngrFrame::GetSelectedLibrary(wxString* library, int *side)
{
	/* first check which library to add the footprint/symbol to */
	wxString leftname, rightname;
	int idx = m_choiceModuleLeft->GetCurrentSelection();
	if (idx >= 0 && idx < (int)m_choiceModuleLeft->GetCount()) {
		leftname = m_choiceModuleLeft->GetString(idx);
		if (leftname[0] == wxT('('))
			leftname = wxEmptyString;
	}
	idx = m_choiceModuleRight->GetCurrentSelection();
	if (idx >= 0 && idx < (int)m_choiceModuleRight->GetCount()) {
		rightname = m_choiceModuleRight->GetString(idx);
		if (rightname[0] == wxT('('))
			rightname = wxEmptyString;
	}
	wxString filename;
	if (leftname.length() > 0 && rightname.length() > 0 && leftname.CmpNoCase(rightname) != 0) {
		wxArrayString choices;
		choices.Add(leftname);
		choices.Add(rightname);
		wxSingleChoiceDialog dlg(this, wxT("Select library"), wxT("Library"), choices);
		if (dlg.ShowModal() != wxID_OK)
			return false;
		filename = dlg.GetStringSelection();
	} else if (leftname.length() > 0) {
		filename = leftname;
	} else if (rightname.length() > 0) {
		filename = rightname;
	} else {
		wxASSERT(leftname.length() == 0 && rightname.length() == 0);
		wxMessageBox(wxT("No local library is selected."));
		return false;
	}

	wxASSERT(side != NULL);
	*side = 0;
	if (filename.Cmp(leftname) == 0)
		*side -= 1;
	if (filename.Cmp(rightname) == 0)
		*side += 1;
	/* if at the end of this procedure, side == 0, it is equal to both left and right */

	wxASSERT(library != NULL);
	*library = filename;
	return true;
}

void libmngrFrame::OnNewFootprint( wxCommandEvent& /*event*/ )
{
	if (CompareMode) {
		wxMessageBox(wxT("Compare mode must be switched off."));
		return;
	}
	if (SymbolMode) {
		wxMessageBox(wxT("Footprint mode must be selected (instead of Schematic mode)."));
		return;
	}

	/* first check which library to add the footprint to */
	int side;
	wxString filename;
	if (!GetSelectedLibrary(&filename, &side))
		return;

	/* set the dialog for the template */
	libmngrDlgNewFootprint dlg(this);
	dlg.SetLibraryName(filename);
	if (dlg.ShowModal() == wxID_OK) {
		/* load the template */
		wxArrayString templat;
		LoadTemplate(dlg.GetTemplateName(), &templat, false);
		/* run over the expressions to create the initial shape */
		RPNexpression rpn;
		wxString description = GetTemplateHeaderField(dlg.GetTemplateName(), wxT("brief"), SymbolMode);
		SetVarDefaults(&rpn, dlg.GetTemplateName(), dlg.GetFootprintName(), description);
		/* create a footprint from the template */
		wxArrayString module;
		if (!FootprintFromTemplate(&module, templat, rpn, false))
			return;	/* error message(s) already given */
		/* save the footprint with initial settings (may need to erase the footprint
			 first) */
		if (ExistFootprint(filename, dlg.GetFootprintName()))
			RemoveFootprint(filename, dlg.GetFootprintName());
		if (!InsertFootprint(filename, dlg.GetFootprintName(), module, true)) {
			wxMessageBox(wxT("Operation failed."));
			return;
		}
		wxString vrmlpath = GetVRMLPath(filename, module);
		if (vrmlpath.Length() > 0) {
			wxString modelname = GetTemplateHeaderField(dlg.GetTemplateName(), wxT("model"), SymbolMode);
			if (modelname.length() == 0) {
				modelname = dlg.GetTemplateName();
			} else {
				int idx = modelname.Find(wxT(' '));
				if (idx >= 0)
					modelname = modelname.Left(idx);
			}
			VRMLFromTemplate(vrmlpath, modelname, rpn);
		}
		/* refresh the library (or possibly both, if both lists contain the same library) */
		if (side <= 0)
			HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
		if (side >= 0)
			HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
		/* load the newly created part */
		RemoveSelection(m_listModulesLeft);
		RemoveSelection(m_listModulesRight);
		OffsetX = OffsetY = 0;
		wxListCtrl* list = (side <= 0) ? m_listModulesLeft : m_listModulesRight;
		wxBusyCursor cursor;
		/* get the index of the new part in the footprint list */
		int idx;
		for (idx = 0; idx < list->GetItemCount(); idx++) {
			wxString symbol = list->GetItemText(idx);
			if (symbol.Cmp(dlg.GetFootprintName()) == 0)
				break;
		}
		wxASSERT(idx < list->GetItemCount());	/* it was just inserted, so it must be found */
		list->SetItemState(idx, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
		list->EnsureVisible(idx);
		LoadPart(idx, list, 0, 0);
		UpdateDetails(0);
		m_panelView->Refresh();
		m_statusBar->SetStatusText(wxT("New footprint"));
		/* toggle the details panel on, if it was off */
		bool details = m_menubar->IsChecked(IDM_DETAILSPANEL);
		if (!details) {
			m_menubar->Check(IDM_DETAILSPANEL, true);
			m_toolBar->ToggleTool(IDT_DETAILSPANEL, true);
			ToggleDetailsPanel(true);
		}
	}
}

void libmngrFrame::OnNewSymbol( wxCommandEvent& /*event*/ )
{
	if (CompareMode) {
		wxMessageBox(wxT("Compare mode must be switched off."));
		return;
	}
	if (!SymbolMode) {
		wxMessageBox(wxT("Schematic mode must be selected (instead of Footprint mode)."));
		return;
	}

	/* first check which library to add the footprint to */
	wxString filename;
	int side;
	if (!GetSelectedLibrary(&filename, &side))
		return;

	/* set the dialog for the template */
	libmngrDlgNewSymbol dlg(this);
	dlg.SetLibraryName(filename);
	if (dlg.ShowModal() == wxID_OK) {
		/* load the template */
		wxArrayString templat;
		LoadTemplate(dlg.GetTemplateName(), &templat, true);
		/* run over the expressions to create the initial shape */
		RPNexpression rpn;
		wxString description = GetTemplateHeaderField(dlg.GetTemplateName(), wxT("brief"), SymbolMode);
		SetVarDefaults(&rpn, dlg.GetTemplateName(), dlg.GetSymbolName(), description,
									 dlg.GetSymbolRef());
		/* create default pin info structure from the pin count */
		int pins = 0;
		rpn.Set("PT");
		if (rpn.Parse() == RPN_OK) {
			RPNvalue v = rpn.Value();
			pins = (int)(v.Double() + 0.001);
		}
		if (pins == 0)
			pins = 2;	/* this is actually a template error; PT should be set */
		PinInfo* info = new PinInfo[pins];
		if (info == NULL)
			return;
		int totals[PinInfo::Sections];
		int counts[PinInfo::Sections];
		for (int s = 0; s < PinInfo::Sections; s++) {
			totals[s] = 0;
			counts[s] = 0;
			char str[20];
			sprintf(str, "PT:%d", s);
			rpn.Set(str);
			if (rpn.Parse() == RPN_OK) {
				RPNvalue v = rpn.Value();
				totals[s] = (int)(v.Double() + 0.001);
			}
		}
		int cursec = PinInfo::Left;
		for (int i = 0; i < pins; i++) {
			while (cursec < PinInfo::Sections && counts[cursec] >= totals[cursec])
				cursec++;
			if (cursec >= PinInfo::Sections)
				cursec = PinInfo::Left;	/* on error, map remaining pins to left side */
			counts[cursec] += 1;
			info[i].seq = i;
			info[i].number = wxString::Format(wxT("%d"), i + 1);
			info[i].name = wxT("~");
			info[i].type = PinInfo::Passive;
			info[i].shape = PinInfo::Line;
			info[i].section = cursec;
		}
		/* create a footprint from the template */
		wxArrayString symbol;
		bool result = SymbolFromTemplate(&symbol, templat, rpn, info, pins);
		delete[] info;
		if (!result)
				return;	/* error message(s) already given */
		/* save the symbol with initial settings (may need to erase the symbol first) */
		if (ExistSymbol(filename, dlg.GetSymbolName()))
			RemoveSymbol(filename, dlg.GetSymbolName());
		if (!InsertSymbol(filename, dlg.GetSymbolName(), symbol)) {
			wxMessageBox(wxT("Operation failed."));
			return;
		}
		/* refresh the library (or possibly both, if both lists contain the same library) */
		if (side <= 0)
			HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
		if (side >= 0)
			HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
		/* load the newly created part */
		RemoveSelection(m_listModulesLeft);
		RemoveSelection(m_listModulesRight);
		OffsetX = OffsetY = 0;
		wxListCtrl* list = (side <= 0) ? m_listModulesLeft : m_listModulesRight;
		wxBusyCursor cursor;
		/* get the index of the new part in the footprint list */
		int idx;
		for (idx = 0; idx < list->GetItemCount(); idx++) {
			wxString symbol = list->GetItemText(idx);
			if (symbol.Cmp(dlg.GetSymbolName()) == 0)
				break;
		}
		wxASSERT(idx < list->GetItemCount());	/* it was just inserted, so it must be found */
		list->SetItemState(idx, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
		list->EnsureVisible(idx);
		LoadPart(idx, list, 0, 0);
		UpdateDetails(0);
		m_panelView->Refresh();
		m_statusBar->SetStatusText(wxT("New symbol"));
		/* toggle the details panel on, if it was off */
		bool details = m_menubar->IsChecked(IDM_DETAILSPANEL);
		if (!details) {
			m_menubar->Check(IDM_DETAILSPANEL, true);
			m_toolBar->ToggleTool(IDT_DETAILSPANEL, true);
			ToggleDetailsPanel(true);
		}
	}
}

bool libmngrFrame::GetReportNameList(wxString* reportfile, wxString* library, wxArrayString* namelist)
{
	/* find which library to print */
	wxString leftname, rightname;
	int idx = m_choiceModuleLeft->GetCurrentSelection();
	if (idx >= 0 && idx < (int)m_choiceModuleLeft->GetCount()) {
		leftname = m_choiceModuleLeft->GetString(idx);
		if (leftname[0] == wxT('('))
			leftname = wxEmptyString;
	}
	idx = m_choiceModuleRight->GetCurrentSelection();
	if (idx >= 0 && idx < (int)m_choiceModuleRight->GetCount()) {
		rightname = m_choiceModuleRight->GetString(idx);
		if (rightname[0] == wxT('('))
			rightname = wxEmptyString;
	}
	wxASSERT(library != NULL);
	if (leftname.length() > 0 && rightname.length() > 0) {
		wxArrayString choices;
		choices.Add(leftname);
		choices.Add(rightname);
		wxSingleChoiceDialog dlg(this, wxT("Select library"), wxT("Library"), choices);
		if (dlg.ShowModal() != wxID_OK)
			return false;
		*library = dlg.GetStringSelection();
	} else if (leftname.length() > 0) {
		*library = leftname;
	} else if (rightname.length() > 0) {
		*library = rightname;
	} else {
		wxASSERT(leftname.length() == 0 && rightname.length() == 0);
		wxMessageBox(wxT("No local library is selected."));
		return false;
	}

	/* get the name of the report file */
	wxFileName fname(*library);
	fname.SetExt(wxT("pdf"));
	wxFileDialog* dlg = new wxFileDialog(this, wxT("Choose report filename..."),
																				 wxEmptyString, fname.GetFullPath(),
																				 wxT("Acrobat PDF (*.pdf)|*.pdf"),
																				 wxFD_SAVE);
	if (dlg->ShowModal() != wxID_OK)
		return false;
	/* set default extension */
	fname.Assign(dlg->GetPath());
	if (fname.GetExt().length() == 0)
		fname.SetExt(wxT("pdf"));
	wxASSERT(reportfile != NULL);
	*reportfile = fname.GetFullPath();

	/* collect the list of footprint names to print */
	wxListCtrl* list;
	if (library->Cmp(leftname) == 0)
		list = m_listModulesLeft;
	else
		list = m_listModulesRight;
	wxASSERT(namelist != NULL);
	namelist->Clear();
	for (long idx = 0; idx < list->GetItemCount(); idx++)
		namelist->Add(list->GetItemText(idx));

	return true;
}

void libmngrFrame::OnFootprintReport( wxCommandEvent& /*event*/ )
{
	wxString reportfile;
	wxString library;
	wxArrayString modules;
	if (!GetReportNameList(&reportfile, &library, &modules))
		return;

	/* print the library, then show the report */
	wxString format;
	wxConfig *config = new wxConfig(APP_NAME);
	format = config->Read(wxT("report/paper"), wxT("Letter"));
	long landscape = config->Read(wxT("report/layout"), 0L);
	long opt_labels = config->Read(wxT("report/drawlabels"), 0L);
	long opt_description = config->Read(wxT("report/includedescription"), 1L);
	long opt_padinfo = config->Read(wxT("report/includepadinfo"), 1L);
	long fontsize = config->Read(wxT("report/fontsize"), 8L);
	delete config;
	PdfReport report;
	report.SetPage(format, (landscape != 0));
	report.FootprintOptions(opt_description != 0, opt_padinfo != 0, opt_labels != 0);
	report.SetFont(fontsize);
	report.FootprintReport(this, library, modules, reportfile);
	m_statusBar->SetStatusText(wxT("Finished report"));

	wxTheMimeTypesManager->ReadMailcap(wxT("/etc/mailcap"));
	wxFileType *FileType = wxTheMimeTypesManager->GetFileTypeFromMimeType(wxT("application/pdf"));
	wxString Command = FileType->GetOpenCommand(reportfile);
	/* sometimes wildcards remain in the command */
	int pos;
	while ((pos = Command.Find(wxT('*'))) >= 0)
		Command.Remove(pos, 1);
	Command.Trim();
	/* finally, run it */
	wxExecute(Command);
}

void libmngrFrame::OnSymbolReport( wxCommandEvent& /*event*/ )
{
	wxString reportfile;
	wxString library;
	wxArrayString symbols;
	if (!GetReportNameList(&reportfile, &library, &symbols))
		return;

	/* print the library, then show the report */
	wxString format;
	wxConfig *config = new wxConfig(APP_NAME);
	format = config->Read(wxT("report/paper"), wxT("Letter"));
	long landscape = config->Read(wxT("report/layout"), 0L);
	long fontsize = config->Read(wxT("report/fontsize"), 8L);
	long opt_description = config->Read(wxT("report/includedescription"), 1L);
	long opt_fplist = config->Read(wxT("report/fplist"), 1L);
	delete config;
	PdfReport report;
	report.SetPage(format, (landscape != 0));
	report.SymbolOptions(opt_description != 0, opt_fplist != 0);
	report.SetFont(fontsize);
	report.SymbolReport(this, library, symbols, reportfile);
	m_statusBar->SetStatusText(wxT("Finished report"));

	wxTheMimeTypesManager->ReadMailcap(wxT("/etc/mailcap"));
	wxFileType *FileType = wxTheMimeTypesManager->GetFileTypeFromMimeType(wxT("application/pdf"));
	wxString Command = FileType->GetOpenCommand(reportfile);
	/* sometimes wildcards remain in the command */
	int pos;
	while ((pos = Command.Find(wxT('*'))) >= 0)
		Command.Remove(pos, 1);
	Command.Trim();
	/* finally, run it */
	wxExecute(Command);
}

void libmngrFrame::OnQuit( wxCommandEvent& /*event*/ )
{
	Close(true);
}

void libmngrFrame::ToggleMode(bool symbolmode)
{
	SymbolMode = symbolmode;

	/* hide or show the fields in the side panel */
	wxBoxSizer* bsizer;
	wxFlexGridSizer* fgSidePanel = dynamic_cast<wxFlexGridSizer*>(m_panelSettings->GetSizer());
	wxASSERT(fgSidePanel != 0);
	fgSidePanel->Show(m_lblAlias, SymbolMode);
	fgSidePanel->Show(m_txtAlias, SymbolMode);
	fgSidePanel->Show(m_lblFootprintFilter, SymbolMode);
	fgSidePanel->Show(m_txtFootprintFilter, SymbolMode);
	fgSidePanel->Show(m_lblPinNames, SymbolMode);
	fgSidePanel->Show(m_gridPinNames, SymbolMode);

	fgSidePanel->Show(m_lblPadShape, !SymbolMode);
	fgSidePanel->Show(m_choicePadShape, !SymbolMode);
	fgSidePanel->Show(m_lblPadSize, !SymbolMode);
	bsizer = dynamic_cast<wxBoxSizer*>(m_txtPadWidth->GetContainingSizer());
	wxASSERT(bsizer != 0);
	fgSidePanel->Show(bsizer, !SymbolMode, true);
	fgSidePanel->Show(m_lblPitch, !SymbolMode);
	fgSidePanel->Show(m_txtPitch, !SymbolMode);
	fgSidePanel->Show(m_lblPadSpan, !SymbolMode);
	bsizer = dynamic_cast<wxBoxSizer*>(m_txtPadSpanX->GetContainingSizer());
	wxASSERT(bsizer != 0);
	fgSidePanel->Show(bsizer, !SymbolMode, true);
	fgSidePanel->Show(m_lblDrillSize, !SymbolMode);
	fgSidePanel->Show(m_txtDrillSize, !SymbolMode);
	fgSidePanel->Show(m_lblAuxPad, !SymbolMode);
	bsizer = dynamic_cast<wxBoxSizer*>(m_txtAuxPadWidth->GetContainingSizer());
	wxASSERT(bsizer != 0);
	fgSidePanel->Show(bsizer, !SymbolMode, true);
	fgSidePanel->Layout();
	m_panelSettings->Refresh();

	/* "pins" is for symbols what "pads" is for footprints */
	if (SymbolMode)
		m_lblPadCount->SetLabel(wxT("Pin count"));
	else
		m_lblPadCount->SetLabel(wxT("Pad count"));

	/* disable options not relevant for schematic mode, or re-enable them */
	m_menubar->Enable(IDM_NEWFOOTPRINT, !SymbolMode);
	m_menubar->Enable(IDM_NEWSYMBOL, SymbolMode);
	m_menubar->Enable(IDM_REPORTFOOTPRINT, !SymbolMode);
	m_menubar->Enable(IDM_REPORTSYMBOL, SymbolMode);
	m_toolBar->EnableTool(IDT_MEASUREMENTS, !SymbolMode);

	/* restore starting state, and refresh */
	OffsetX = OffsetY = 0;
	EnableButtons(0);
	CollectAllLibraries();
	UpdateDetails(0);
	m_panelView->Refresh();
}

void libmngrFrame::OnFootprintMode( wxCommandEvent& event )
{
	ToggleMode(false);
	event.Skip();
}

void libmngrFrame::OnSymbolMode( wxCommandEvent& event )
{
	ToggleMode(true);
	event.Skip();
}

void libmngrFrame::OnSearchPaths( wxCommandEvent& /*event*/ )
{
	libmngrDlgPaths dlg(this);
	if (dlg.ShowModal() == wxID_OK) {
		m_statusBar->SetStatusText(wxT("Settings changed"));
		CollectAllLibraries();
	}
}

void libmngrFrame::OnRemoteLink( wxCommandEvent& /*event*/ )
{
	#if !defined NO_CURL
		libmngrDlgRemoteLink dlg(this);
		if (dlg.ShowModal() == wxID_OK) {
			m_statusBar->SetStatusText(wxT("Settings changed"));
			CollectAllLibraries();
		}
	#endif
}

void libmngrFrame::OnReportSettings( wxCommandEvent& /*event*/ )
{
	libmngrDlgReport dlg(this);
	if (dlg.ShowModal() == wxID_OK)
		m_statusBar->SetStatusText(wxT("Settings changed"));
}

void libmngrFrame::OnUIOptions( wxCommandEvent& /*event*/ )
{
	libmngrDlgOptions dlg(this);
	if (dlg.ShowModal() == wxID_OK) {
		m_statusBar->SetStatusText(wxT("Settings changed"));
		/* refresh, with the new settings */
		wxConfig *config = new wxConfig(APP_NAME);
		FontSizeLegend = config->Read(wxT("display/fontsize"), 8L);
		DimensionOffset = config->Read(wxT("display/dimoffset"), 50L);
		config->Read(wxT("display/showlabels"), &ShowLabels, true);
		config->Read(wxT("display/fullpath"), &ShowFullPaths, false);
		config->Read(wxT("settings/copyvrml"), &CopyVRML, true);
		config->Read(wxT("settings/disabletemplate"), &DontRebuildTemplate, false);
		delete config;
		m_panelView->Refresh();
	}
}

void libmngrFrame::OnTemplateOptions( wxCommandEvent& /*event*/ )
{
	libmngrDlgTemplateOpts dlg(this);
	if (dlg.ShowModal() == wxID_OK)
		m_statusBar->SetStatusText(wxT("Settings changed"));
}

void libmngrFrame::OnCompareMode( wxCommandEvent& /*event*/ )
{
	wxBusyCursor cursor;
	CompareMode = m_menubar->IsChecked(IDM_COMPAREMODE);
	m_toolBar->EnableTool(IDT_LEFTFOOTPRINT, CompareMode);
	m_toolBar->EnableTool(IDT_RIGHTFOOTPRINT, CompareMode);
	m_toolBar->ToggleTool(IDT_LEFTFOOTPRINT, CompareMode);
	m_toolBar->ToggleTool(IDT_RIGHTFOOTPRINT, CompareMode);
	m_radioViewLeft->Enable(CompareMode);
	m_radioViewRight->Enable(CompareMode);
	m_radioViewLeft->SetValue(true);
	if (CompareMode) {
	  EnableButtons(0);
		/* reload the footprints, for both listboxes */
		long idx = m_listModulesLeft->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (idx >= 0)
			LoadPart(idx, m_listModulesLeft, m_choiceModuleLeft, 0);
		idx = m_listModulesRight->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (idx >= 0)
			LoadPart(idx, m_listModulesRight, m_choiceModuleRight, 1);
	} else {
		/* remove the selection in the right listctrl (we only keep the left footprint) */
		RemoveSelection(m_listModulesRight);
		PartData[1].Clear();
		/* check whether there is a selection in the left listctrl, only enable
		   the buttons if so */
		long idx = m_listModulesRight->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (idx >= 0)
			EnableButtons(-1);
	}
	OffsetX = OffsetY = 0;
	UpdateDetails(0);	/* update details for the left component */
	m_panelView->Refresh();
}

void libmngrFrame::OnDetailsPanel( wxCommandEvent& /*event*/ )
{
	bool details = m_menubar->IsChecked(IDM_DETAILSPANEL);
	m_toolBar->ToggleTool(IDT_DETAILSPANEL, details);
	ToggleDetailsPanel(details);
}

void libmngrFrame::OnHelp( wxCommandEvent& /*event*/ )
{
	wxString filename = theApp->GetDocumentationPath() + wxT(DIRSEP_STR) wxT("kicadlibrarian.pdf");
	wxTheMimeTypesManager->ReadMailcap(wxT("/etc/mailcap"));
	wxFileType *FileType = wxTheMimeTypesManager->GetFileTypeFromMimeType(wxT("application/pdf"));
	wxString Command = FileType->GetOpenCommand(filename);
	wxExecute(Command);
}

void libmngrFrame::OnAbout( wxCommandEvent& /*event*/ )
{
	wxIcon icon(logo64_xpm);
	wxString description = wxT("A utility to view and manage footprint and symbol libraries.\n");
	#if defined NO_CURL
		description += wxT("Built without support for remote repositories.");
	#else
		description += wxT("Including support for remote repositories.");
	#endif

	wxAboutDialogInfo info;
	info.SetName(wxT("KiCad Librarian"));
	info.SetVersion(wxT(SVN_REVSTR));
	info.SetDescription(description);
	info.SetCopyright(wxT("(C) 2013-2014 ITB CompuPhase"));
	info.SetIcon(icon);
	info.SetWebSite(wxT("http://www.compuphase.com/"));
	info.AddDeveloper(wxT("KiCad Librarian uses Haru PDF for the reports"));
	info.AddDeveloper(wxT("KiCad Librarian uses the wxWidgets GUI library"));
	wxAboutBox(info);
}

void libmngrFrame::OnMovePart( wxCommandEvent& /*event*/ )
{
	wxString modname, source, target, author;
	wxString leftmod = GetSelection(m_listModulesLeft, m_choiceModuleLeft, &source, &author);
	wxString rightmod = GetSelection(m_listModulesRight, m_choiceModuleRight, &source, &author);
	wxASSERT(leftmod.IsEmpty() || rightmod.IsEmpty());

	if (leftmod.length() > 0) {
		modname = leftmod;
		long idx = m_choiceModuleRight->GetCurrentSelection();
		wxASSERT(idx >= 0 && idx < (long)m_choiceModuleRight->GetCount());
		target = m_choiceModuleRight->GetString(idx);
	} else if (rightmod.length() > 0) {
		modname = rightmod;
		long idx = m_choiceModuleLeft->GetCurrentSelection();
		wxASSERT(idx >= 0 && idx < (long)m_choiceModuleLeft->GetCount());
		target = m_choiceModuleLeft->GetString(idx);
	}
	wxASSERT(source.length() > 0);
	wxASSERT(target.length() > 0);

	if (SymbolMode) {
		/* first remove the symbol from the target library (if it exists) */
		if (ExistSymbol(target, modname)) {
			if (wxMessageBox(wxT("Overwrite \"") + modname + wxT("\"\nin ") + target + wxT("?"), wxT("Confirm overwrite"), wxYES_NO | wxICON_QUESTION) != wxYES)
				return;
			RemoveSymbol(target, modname);
		}

		wxASSERT(PartData[0].Count() > 0);
		if (InsertSymbol(target, modname, PartData[0])) {
			RemoveSymbol(source, modname);
			/* both libraries need to be refreshed */
			HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
			HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
			m_panelView->Refresh();
		} else {
			wxMessageBox(wxT("Operation failed."));
		}
	} else {
		/* get the types of the libraries (and check) */
		int sourcetype = LibraryType(source);
		int targettype = LibraryType(target);
		if (sourcetype == VER_INVALID) {
			wxMessageBox(wxT("Unsupported file format for the source library."));
			return;
		} else if (targettype == VER_INVALID) {
			wxMessageBox(wxT("Unsupported file format for the target library."));
			return;
		}

		if (ExistFootprint(target, modname)
			  && wxMessageBox(wxT("Overwrite \"") + modname + wxT("\"\nin ") + target + wxT("?"), wxT("Confirm overwrite"), wxYES_NO | wxICON_QUESTION) != wxYES)
			return;

		if (sourcetype == VER_S_EXPR && targettype == VER_S_EXPR) {
			/* create full names for source and target, then move the file */
			wxFileName old_fname(source, modname);
			old_fname.SetExt(wxT("kicad_mod"));
			wxFileName new_fname(target, modname);
			new_fname.SetExt(wxT("kicad_mod"));
			if (wxRenameFile(old_fname.GetFullPath(), new_fname.GetFullPath(), true)) {
				HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
				HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
				m_panelView->Refresh();
			} else {
				wxMessageBox(wxT("Operation failed."));
			}
		} else {
			/* make a copy of the name, because the user may need to adjust it */
			wxString newname = modname;
			wxArrayString sourcedata;	/* create a copy, to avoid converting the original */
			if (sourcetype == VER_S_EXPR) {
				wxASSERT(targettype != VER_S_EXPR);
				TranslateToLegacy(&sourcedata, PartData[0]);
				sourcetype = VER_MM;
			} else if (targettype == VER_S_EXPR) {
				wxASSERT(sourcetype != VER_S_EXPR);
				wxArrayString data_mm = PartData[0];
				if (sourcetype == VER_MIL)
					TranslateUnits(data_mm, false, true);
				TranslateToSexpr(&sourcedata, data_mm);
				sourcetype = VER_S_EXPR;
				/* verify that the name is valid */
				if (!ValidateFilename(&newname))
					return;
			} else {
				wxASSERT(sourcetype != VER_S_EXPR && targettype != VER_S_EXPR);
				sourcedata = PartData[0];
			}
			RemoveFootprint(target, newname);	/* remove the footprint from the target library */
			wxASSERT(sourcedata.Count() > 0);
			if (InsertFootprint(target, newname, sourcedata, sourcetype >= VER_MM)) {
				RemoveFootprint(source, modname);
				/* both libraries need to be refreshed */
				HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
				HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
				m_panelView->Refresh();
			} else {
				wxMessageBox(wxT("Operation failed."));
			}
		}
		if (CopyVRML)
			CopyVRMLfile(source, target, author, PartData[0]);
	}
}

void libmngrFrame::OnCopyPart( wxCommandEvent& /*event*/ )
{
	wxString modname, source, target, author;
	wxString leftmod = GetSelection(m_listModulesLeft, m_choiceModuleLeft, &source, &author);
	wxString rightmod = GetSelection(m_listModulesRight, m_choiceModuleRight, &source, &author);
	wxASSERT(leftmod.IsEmpty() || rightmod.IsEmpty());

	int refresh = 0;
	if (leftmod.length() > 0) {
		modname = leftmod;
		refresh = 1;	/* after copy, must refresh the right list */
		long idx = m_choiceModuleRight->GetCurrentSelection();
		wxASSERT(idx >= 0 && idx < (long)m_choiceModuleRight->GetCount());
		target = m_choiceModuleRight->GetString(idx);
	} else if (rightmod.length() > 0) {
		modname = rightmod;
		refresh = -1;	/* after copy, must refresh the left list */
		long idx = m_choiceModuleLeft->GetCurrentSelection();
		wxASSERT(idx >= 0 && idx < (long)m_choiceModuleLeft->GetCount());
		target = m_choiceModuleLeft->GetString(idx);
	}
	wxASSERT(source.length() > 0);
	wxASSERT(target.length() > 0);
	wxASSERT(target.CmpNoCase(LIB_ALL) != 0);

	if (SymbolMode) {
		/* first remove the symbol from the target library (if it exists) */
		if (ExistSymbol(target, modname)) {
			if (wxMessageBox(wxT("Overwrite \"") + modname + wxT("\"\nin ") + target + wxT("?"), wxT("Confirm overwrite"), wxYES_NO | wxICON_QUESTION) != wxYES)
				return;
			RemoveSymbol(target, modname);
		}

		wxASSERT(PartData[0].Count() > 0);
		if (InsertSymbol(target, modname, PartData[0])) {
			if (refresh == -1)
				HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
			else if (refresh == 1)
				HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
			m_panelView->Refresh();
		} else {
			wxMessageBox(wxT("Operation failed."));
		}
	} else {
		/* get the types of the libraries (and check) */
		int sourcetype = LibraryType(source);
		int targettype = LibraryType(target);
		if (sourcetype == VER_INVALID) {
			wxMessageBox(wxT("Unsupported file format for the source library."));
			return;
		} else if (targettype == VER_INVALID) {
			wxMessageBox(wxT("Unsupported file format for the target library."));
			return;
		}

		if (ExistFootprint(target, modname)
				&& wxMessageBox(wxT("Overwrite \"") + modname + wxT("\"\nin ") + target + wxT("?"), wxT("Confirm overwrite"), wxYES_NO | wxICON_QUESTION) != wxYES)
			return;

		if (sourcetype == VER_S_EXPR && targettype == VER_S_EXPR) {
			/* create full names for source and target, then copy the file */
			wxFileName old_fname(source, modname);
			old_fname.SetExt(wxT("kicad_mod"));
			wxFileName new_fname(target, modname);
			new_fname.SetExt(wxT("kicad_mod"));
			if (wxCopyFile(old_fname.GetFullPath(), new_fname.GetFullPath(), true)) {
				if (refresh == -1)
					HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
				else if (refresh == 1)
					HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
				m_panelView->Refresh();
			} else {
				wxMessageBox(wxT("Operation failed."));
			}
		} else {
			wxArrayString sourcedata;	/* create a copy, to avoid converting the original */
			if (sourcetype == VER_S_EXPR) {
				wxASSERT(targettype != VER_S_EXPR);
				TranslateToLegacy(&sourcedata, PartData[0]);
				sourcetype = VER_MM;
			} else if (targettype == VER_S_EXPR) {
				wxASSERT(sourcetype != VER_S_EXPR);
				wxArrayString data_mm = PartData[0];
				if (sourcetype == VER_MIL)
					TranslateUnits(data_mm, false, true);
				TranslateToSexpr(&sourcedata, data_mm);
				sourcetype = VER_S_EXPR;
				/* verify that the name is valid */
				if (!ValidateFilename(&modname))
					return;
			} else {
				wxASSERT(sourcetype != VER_S_EXPR && targettype != VER_S_EXPR);
				sourcedata = PartData[0];
			}
			RemoveFootprint(target, modname);	/* remove the footprint from the target library */
			wxASSERT(sourcedata.Count() > 0);
			if (InsertFootprint(target, modname, sourcedata, sourcetype >= VER_MM)) {
				if (refresh == -1)
					HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
				else if (refresh == 1)
					HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
				m_panelView->Refresh();
			} else {
				wxMessageBox(wxT("Operation failed."));
			}
		}
		if (CopyVRML)
			CopyVRMLfile(source, target, author, PartData[0]);
	}
}

void libmngrFrame::OnDeletePart( wxCommandEvent& /*event*/ )
{
	wxString modname, filename;
	wxString leftmod = GetSelection(m_listModulesLeft, m_choiceModuleLeft, &filename);
	wxString rightmod = GetSelection(m_listModulesRight, m_choiceModuleRight, &filename);
	wxASSERT(leftmod.IsEmpty() || rightmod.IsEmpty());

	int refresh = 0;
	if (leftmod.length() > 0) {
		modname = leftmod;
		refresh = -1;	/* after deletion, must refresh the left list */
	} else if (rightmod.length() > 0) {
		modname = rightmod;
		refresh = 1;	/* after deletion, must refresh the right list */
	}
	wxASSERT(filename.length() > 0);

	if (wxMessageBox(wxT("Delete \"") + modname + wxT("\"\nfrom ") + filename + wxT("?"), wxT("Confirm deletion"), wxYES_NO | wxICON_QUESTION) == wxYES) {
		bool result;
		if (SymbolMode)
			result = RemoveSymbol(filename, modname);
		else
			result = RemoveFootprint(filename, modname);
		if (result) {
			PartData[0].Clear();
			PartData[1].Clear();	/* should already be clear (delete operation is inactive in compare mode) */
			if (refresh == -1)
				HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
			else if (refresh == 1)
				HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
			m_panelView->Refresh();
			UpdateDetails(0);
		} else {
			wxMessageBox(wxT("Operation failed"));
		}
	}
}

void libmngrFrame::OnRenamePart( wxCommandEvent& /*event*/ )
{
	wxString modname, filename;
	wxString leftmod = GetSelection(m_listModulesLeft, m_choiceModuleLeft, &filename);
	wxString rightmod = GetSelection(m_listModulesRight, m_choiceModuleRight, &filename);
	wxASSERT(leftmod.IsEmpty() || rightmod.IsEmpty());

	int refresh = 0;
	if (leftmod.length() > 0) {
		modname = leftmod;
		refresh = -1;	/* after rename, must refresh the left list */
	} else if (rightmod.length() > 0) {
		modname = rightmod;
		refresh = 1;	/* after rename, must refresh the right list */
	}
	wxASSERT(filename.length() > 0);

	wxTextEntryDialog dlg(this, wxT("New name"), wxT("Rename") + modname, modname);
	if (dlg.ShowModal() == wxID_OK) {
		wxString newname = dlg.GetValue();
		if (!SymbolMode) {
			/* verify that the new name is a valid symbol (in s-expression libraries,
				 the footprint name is a filename) */
			if (!ValidateFilename(&newname))
				return;
		}
		if (newname.length() > 0) {
			bool result;
			if (SymbolMode)
				result = RenameSymbol(filename, modname, newname);
			else
				result = RenameFootprint(filename, modname, newname);
			if (result) {
				if (refresh == -1)
					HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
				else if (refresh == 1)
					HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
				m_panelView->Refresh();
			} else {
				wxMessageBox(wxT("Operation failed"));
			}
		}
	}
}

void libmngrFrame::OnDuplicatePart( wxCommandEvent& /*event*/ )
{
	wxString modname, filename, author;
	wxString leftmod = GetSelection(m_listModulesLeft, m_choiceModuleLeft, &filename, &author);
	wxString rightmod = GetSelection(m_listModulesRight, m_choiceModuleRight, &filename, &author);
	wxASSERT(leftmod.IsEmpty() || rightmod.IsEmpty());

	int refresh = 0;
	if (leftmod.length() > 0) {
		modname = leftmod;
		refresh = -1;	/* after duplicate, must refresh the left list */
	} else if (rightmod.length() > 0) {
		modname = rightmod;
		refresh = 1;	/* after duplicate, must refresh the right list */
	}
	wxASSERT(filename.length() > 0);
	/* check whether the same library is selected at both sides */
	wxString otherlib = wxEmptyString;
	if (refresh < 0) {
		long idx = m_choiceModuleRight->GetCurrentSelection();
		if (idx >= 0 && idx < (long)m_choiceModuleRight->GetCount())
			otherlib = m_choiceModuleRight->GetString(idx);
	} else if (refresh > 0) {
		long idx = m_choiceModuleLeft->GetCurrentSelection();
		if (idx >= 0 && idx < (long)m_choiceModuleLeft->GetCount())
			otherlib = m_choiceModuleLeft->GetString(idx);
	}
	bool refreshboth = (filename.Cmp(otherlib) == 0);

	wxTextEntryDialog dlg(this, wxT("New name"), wxT("Duplicate") + modname, modname);
	if (dlg.ShowModal() == wxID_OK) {
		wxString newname = dlg.GetValue();
		if (!SymbolMode) {
			/* verify that the new name is a valid symbol (in s-expression libraries,
				 the footprint name is a filename) */
			if (!ValidateFilename(&newname))
				return;
		}
		if (newname.length() > 0) {
			bool result = false;
			if (SymbolMode) {
				/* first remove the symbol from the library (if it exists) */
				if (ExistSymbol(filename, newname)) {
					if (wxMessageBox(wxT("Overwrite \"") + newname + wxT("\"\nin ") + filename + wxT("?"), wxT("Confirm overwrite"), wxYES_NO | wxICON_QUESTION) != wxYES)
						return;
					RemoveSymbol(filename, newname);
				}
				wxASSERT(PartData[0].Count() > 0);
				wxArrayString symbol = PartData[0];	/* create a copy */
				RenameSymbol(&symbol, modname, newname);
				result = InsertSymbol(filename, newname, symbol);
			} else {
				int type = LibraryType(filename);
				if (type == VER_INVALID) {
					wxMessageBox(wxT("Unsupported file format for the library."));
					return;
				}
				/* first remove the footprint from the library (if it exists) */
				if (ExistFootprint(filename, newname)
						&& wxMessageBox(wxT("Overwrite \"") + newname + wxT("\"\nin ") + filename + wxT("?"), wxT("Confirm overwrite"), wxYES_NO | wxICON_QUESTION) != wxYES)
					return;
				wxArrayString module;
				if (type == VER_S_EXPR) {
					/* create full names for source and target, then copy the file */
					wxFileName old_fname(filename, modname);
					old_fname.SetExt(wxT("kicad_mod"));
					wxFileName new_fname(filename, newname);
					new_fname.SetExt(wxT("kicad_mod"));
					result = wxCopyFile(old_fname.GetFullPath(), new_fname.GetFullPath(), true);
					/* adjust fields in the new file to newname */
					wxTextFile file;
					if (!file.Open(new_fname.GetFullPath()))
						return;
					/* find the index */
					for (unsigned idx = 0; idx < file.GetLineCount(); idx++) {
						wxString line = file.GetLine(idx);
						module.Add(line);
					}
					RenameFootprint(&module, modname, newname);
					file.Clear();
					for (unsigned idx = 0; idx < module.Count(); idx++)
						file.AddLine(module[idx]);
					file.Write();
					file.Close();
				} else {
					RemoveFootprint(filename, newname);	/* remove the footprint from the target library */
					wxASSERT(PartData[0].Count() > 0);
					module = PartData[0];	/* create a copy */
					RenameFootprint(&module, modname, newname);
					result = InsertFootprint(filename, newname, module, type >= VER_MM);
				}
				/* make a copy of the VRML file too */
				if (CopyVRML)
					CopyVRMLfile(modname, newname, author, module);
			}
			if (result) {
				wxBusyCursor cursor;
				if (refresh < 0 || refreshboth)
					HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
				if (refresh > 0 || refreshboth)
					HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
				/* get the index of the new part in the footprint/symbol list */
				wxASSERT(refresh == -1 || refresh == 1);
				wxListCtrl* list = (refresh == -1) ? m_listModulesLeft : m_listModulesRight;
				long idx;
				for (idx = 0; idx < list->GetItemCount(); idx++) {
					wxString symbol = list->GetItemText(idx);
					if (symbol.Cmp(newname) == 0)
						break;
				}
				wxASSERT(idx < list->GetItemCount());	/* it was just inserted, so it must be found */
				list->SetItemState(idx, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
				list->EnsureVisible(idx);
				LoadPart(idx, list, 0, 0);
				UpdateDetails(0);
				m_statusBar->SetStatusText(wxT("Duplicated symbol/footprint"));
				m_panelView->Refresh();
			} else {
				wxMessageBox(wxT("Operation failed"));
			}
		}
	}
}

bool libmngrFrame::ValidateFilename(wxString* name)
{
	/* verify that the new name is a valid symbol (in s-expression libraries,
		 the footprint name is a filename) */
	bool ok;
	do {
		ok = true;
		for (unsigned i = 0; ok && i < wxFileName::GetForbiddenChars().length(); i++) {
			if (name->Find(wxFileName::GetForbiddenChars().GetChar(i)) >= 0) {
				ok = false;
				wxString msg = wxString::Format(wxT("The character '%c' is invalid in a symbol name; please choose a different name"),
																	wxFileName::GetForbiddenChars().GetChar(i));
				wxTextEntryDialog dlg(this, msg, wxT("Rename ") + *name, *name);
				if (dlg.ShowModal() != wxID_OK)
					return false;
			}
		}
	} while (!ok);
	return true;
}

void libmngrFrame::OnLeftLibSelect( wxCommandEvent& /*event*/ )
{
	wxString leftmod = GetSelection(m_listModulesLeft);
	if (leftmod.length() > 0) {
		PartData[0].Clear();
		UpdateDetails(0);
		m_panelView->Refresh();
	}
	WarnNoRepository(m_choiceModuleLeft);
	HandleLibrarieSelect(m_choiceModuleLeft, m_listModulesLeft);
}

void libmngrFrame::OnRightLibSelect( wxCommandEvent& /*event*/ )
{
	wxString rightmod = GetSelection(m_listModulesRight);
	if (rightmod.length() > 0) {
		if (CompareMode) {
			PartData[1].Clear();
			UpdateDetails(1);
		} else {
			PartData[0].Clear();
			UpdateDetails(0);
		}
		m_panelView->Refresh();
	}
	WarnNoRepository(m_choiceModuleRight);
	HandleLibrarieSelect(m_choiceModuleRight, m_listModulesRight);
}

void libmngrFrame::OnLeftModSelect( wxListEvent& event )
{
	wxBusyCursor cursor;
	if (!CompareMode) {
		EnableButtons(event.GetIndex() >= 0 ? -1 : 0);
		RemoveSelection(m_listModulesRight);
		OffsetX = OffsetY = 0;
	}
	LoadPart(event.GetIndex(), m_listModulesLeft, m_choiceModuleLeft, 0);
	UpdateDetails(0);
	m_panelView->Refresh();
}

void libmngrFrame::OnRightModSelect( wxListEvent& event )
{
	wxBusyCursor cursor;
	if (!CompareMode) {
		EnableButtons(event.GetIndex() >= 0 ? 1 : 0);
		RemoveSelection(m_listModulesLeft);
		OffsetX = OffsetY = 0;
	}
	LoadPart(event.GetIndex(), m_listModulesRight, m_choiceModuleRight, CompareMode ? 1 : 0);
	UpdateDetails(CompareMode ? 1 : 0);
	m_panelView->Refresh();
}

void libmngrFrame::OnDetailsPanelUnsplit( wxSplitterEvent& event )
{
	m_menubar->Check(IDM_DETAILSPANEL, false);
	event.Skip();
}

void libmngrFrame::OnViewStartDrag( wxMouseEvent& event )
{
	DragOrgX = event.GetX() - OffsetX;
	DragOrgY = event.GetY() - OffsetY;
}

void libmngrFrame::OnViewDrag( wxMouseEvent& event )
{
	if (event.Dragging()) {
		OffsetX = event.GetX() - DragOrgX;
		OffsetY = event.GetY() - DragOrgY;
		m_panelView->Refresh();
	}
}

void libmngrFrame::OnViewCentre( wxMouseEvent& /*event*/ )
{
	OffsetX = OffsetY = 0;
	m_panelView->Refresh();
}

/** Draws a string using the plotter font, using the current pen. The brush is
 *  always reset to an empty brush.
 */
void libmngrFrame::DrawStrokeText(wxGraphicsContext *gc, float x, float y,
																	const wxString& text)
{
	/* create a stroke array for the text (applying scale and rotation) */
	std::vector<CXFPolyLine> strokes;
	VFont.DrawText(text.wc_str(wxConvLibc), &strokes);

	gc->SetBrush(*wxTRANSPARENT_BRUSH);
	for (size_t sidx = 0; sidx < strokes.size(); sidx++) {
		const CXFPolyLine *stroke = &strokes[sidx];
		wxPoint2DDouble *points = new wxPoint2DDouble[stroke->GetCount()];
		const CXFPoint* pt;
		for (size_t pidx = 0; (pt = stroke->GetPoint(pidx)) != 0; pidx++) {
			points[pidx].m_x = x + pt->m_x;
			points[pidx].m_y = y - pt->m_y;
		}
		gc->DrawLines(stroke->GetCount(), points);
		delete[] points;
	}
}

void libmngrFrame::DrawSymbols(wxGraphicsContext *gc, int midx, int midy, int transp[])
{
	#define DEFAULTPEN	1
	#define PINSHAPE_SZ	40
	wxBrush brush;
	wxColour clrForeground, clrBackground, clrText, clrHiddenText;
	double scale = Scale * 0.0254 * 0.25;
	double size_pinshape = PINSHAPE_SZ * scale;

	for (int fp = 0; fp < 2; fp++) {
		/* check whether the symbol is visible */
		if (fp == 0 && CompareMode && !m_toolBar->GetToolState(IDT_LEFTFOOTPRINT))
			continue;
		if (fp == 1 && (!CompareMode || !m_toolBar->GetToolState(IDT_RIGHTFOOTPRINT)))
			continue;
		if (PartData[fp].Count() == 0)
			continue;
		/* Draw the outline, plus optionally the texts */
		if (fp == 0) {
			clrForeground.Set(255, 64, 64, transp[fp]);
			clrBackground.Set(192, 192, 64, transp[fp]);
			clrText.Set(255, 240, 128, transp[fp]);
			clrHiddenText.Set(170, 160, 80, transp[fp]);
		} else {
			clrForeground.Set(64, 240, 64, transp[fp]);
			clrBackground.Set(64, 192, 192, transp[fp]);
			clrText.Set(224, 96, 224, transp[fp]);
			clrHiddenText.Set(150, 70, 150, transp[fp]);
		}
		bool indraw = false;
		double pinname_offset = size_pinshape;
		bool show_pinnr = true, show_pinname = true;
		for (int idx = 0; idx < (int)PartData[fp].Count(); idx++) {
			wxString line = PartData[fp][idx];
			if (line[0] == wxT('#'))
				continue;
			wxString token = GetToken(&line);
			if (token.CmpNoCase(wxT("DEF")) == 0) {
				GetToken(&line);	/* ignore name */
				GetToken(&line);	/* ignore designator prefix */
				GetToken(&line);	/* ignore reserved field */
				pinname_offset = GetTokenLong(&line) * scale;
				show_pinnr = GetToken(&line) == wxT('Y');
				show_pinname = GetToken(&line) == wxT('Y');
			} else if (token[0] == wxT('F') && isdigit(token[1])) {
				if (ShowLabels) {
					wxString name = GetToken(&line);
					double x = midx + GetTokenLong(&line) * scale;
					double y = midy - GetTokenLong(&line) * scale;
					long rawsize = GetTokenLong(&line);
					double size = rawsize * scale;
					wxString orient = GetToken(&line);
					int angle = (orient == wxT('V')) ? 90 : 0;
					wxString visflag = GetToken(&line);
					bool visible = (visflag == wxT('V'));
					wxString align = GetToken(&line);
					int horalign = CXF_ALIGNCENTRE;
					switch (align[0]) {
					case 'L':
						horalign = CXF_ALIGNLEFT;
						break;
					case 'R':
						horalign = CXF_ALIGNRIGHT;
						break;
					}
					align = GetToken(&line);
					int veralign = CXF_ALIGNCENTRE;
					switch (align[0]) {
					case 'T':
						veralign = CXF_ALIGNTOP;
						break;
					case 'B':
						veralign = CXF_ALIGNBOTTOM;
						break;
					}
					bool bold = (align.Length() >= 3 && align[2] == wxT('B'));
					double penwidth = bold ? (size * 0.25) : (size * 0.15);
					if (penwidth < DEFAULTPEN)
						penwidth = DEFAULTPEN;
					if (rawsize > 5) {
						wxColour penclr = visible ? clrText : clrHiddenText;
						wxPen pen(penclr, penwidth);
						gc->SetPen(pen);
						if (name[0] == '~')
							name = name.Mid(1);
						VFont.SetScale(size / CXF_CAPSHEIGHT, size / CXF_CAPSHEIGHT);
						VFont.SetOverbar(false);
						VFont.SetRotation(angle);
						VFont.SetAlign(horalign, veralign);
						DrawStrokeText(gc, x, y, name);
					}
				}
			} else if (token.CmpNoCase(wxT("DRAW")) == 0) {
				indraw = true;
			} else if (token.CmpNoCase(wxT("ENDDRAW")) == 0) {
				indraw = false;
			} else if (indraw) {
				//??? should draw all filled shapes before all non-filled shapes
				double x, y, w, h, penwidth, length, size_nr, size_name, angle, endangle;
				long count, orientation, bold, halign, valign;
				bool visible;
				wxPoint2DDouble *points;
				wxGraphicsPath path;
				wxString name, pin, field;
				wxPen pen(clrForeground);
				switch (token[0]) {
				case 'A':
					x = midx + GetTokenLong(&line) * scale;
					y = midy - GetTokenLong(&line) * scale;
					w = GetTokenLong(&line) * scale;
					angle = GetTokenLong(&line) * M_PI / 1800.0;
					endangle = GetTokenLong(&line) * M_PI / 1800.0;
					if (GetTokenLong(&line) > 1)
						break;			/* ignore parts other than part 1 */
					if (GetTokenLong(&line) > 1)
						break;			/* ignore De Morgan converted shape */
					if ((penwidth = GetTokenLong(&line) * scale) < DEFAULTPEN)
						penwidth = DEFAULTPEN;
					field = GetToken(&line);	/* fill parameter (save, analyze later) */
					pen.SetWidth(penwidth);
					gc->SetPen(pen);
					if (field == wxT('f'))
						gc->SetBrush(clrBackground);
					else if (field == wxT('F'))
						gc->SetBrush(clrForeground);
					else
						gc->SetBrush(*wxTRANSPARENT_BRUSH);
					path = gc->CreatePath();
					/* calculate the angle of rotation */
					while (angle > M_PI)
						angle -= 2*M_PI;
					while (angle < -M_PI)
						angle += 2*M_PI;
					while (endangle > M_PI)
						endangle -= 2*M_PI;
					while (endangle < angle)
						endangle += 2*M_PI;
					h = endangle - angle;
					wxASSERT(h > -EPSILON);
					wxASSERT(h < 2*M_PI + EPSILON);
					path.AddArc(x, y, w, -angle, -endangle, (h > M_PI));
					gc->DrawPath(path);
					break;
				case 'B':
					//??? Bezier curves apparently not yet handled by the KiCad Symbol Editor
					break;
				case 'C':
					x = midx + GetTokenLong(&line) * scale;
					y = midy - GetTokenLong(&line) * scale;
					w = GetTokenLong(&line) * scale;
					if (GetTokenLong(&line) > 1)
						break;	/* ignore parts other than part 1 */
					if (GetTokenLong(&line) > 1)
						break;	/* ignore De Morgan converted shape */
					if ((penwidth = GetTokenLong(&line) * scale) < DEFAULTPEN)
						penwidth = DEFAULTPEN;
					field = GetToken(&line);
					pen.SetWidth(penwidth);
					gc->SetPen(pen);
					if (field == wxT('f'))
						gc->SetBrush(clrBackground);
					else if (field == wxT('F'))
						gc->SetBrush(clrForeground);
					else
						gc->SetBrush(*wxTRANSPARENT_BRUSH);
					gc->DrawEllipse(x - w, y - w, 2 * w, 2 * w);
					break;
				case 'P':
					count = (int)GetTokenLong(&line);
					if (GetTokenLong(&line) > 1)
						break;	/* ignore parts other than part 1 */
					if (GetTokenLong(&line) > 1)
						break;	/* ignore De Morgan converted shape */
					if ((penwidth = GetTokenLong(&line) * scale) < DEFAULTPEN)
						penwidth = DEFAULTPEN;
					wxASSERT(count > 0);
					points = new wxPoint2DDouble[count + 1];	/* reserve 1 extra for filled polygons */
					wxASSERT(points != NULL);
					for (int p = 0; p < count; p++) {
						points[p].m_x = midx + GetTokenLong(&line) * scale;
						points[p].m_y = midy - GetTokenLong(&line) * scale;
					}
					field = GetToken(&line);
					if (field == wxT('F') || field == wxT('f')) {
						/* filled polygons are implicitly closed */
						points[count] = points[0];
						count += 1;
					}
					pen.SetWidth(penwidth);
					gc->SetPen(pen);
					if (field == wxT('f'))
						gc->SetBrush(clrBackground);
					else if (field == wxT('F'))
						gc->SetBrush(clrForeground);
					else
						gc->SetBrush(*wxTRANSPARENT_BRUSH);
					gc->DrawLines(count, points);
					delete[] points;
					break;
				case 'S':
					x = GetTokenLong(&line) * scale;
					y = GetTokenLong(&line) * scale;
					w = GetTokenLong(&line) * scale - x;
					h =  GetTokenLong(&line) * scale - y;
					if (GetTokenLong(&line) > 1)
						break;	/* ignore parts other than part 1 */
					if (GetTokenLong(&line) > 1)
						break;	/* ignore De Morgan converted shape */
					if ((penwidth = GetTokenLong(&line) * scale) < DEFAULTPEN)
						penwidth = DEFAULTPEN;
					field = GetToken(&line);
					if (w < 0) {
						x += w;
						w = -w;
					}
					if (h < 0) {
						y += h;
						h = -h;
					}
					pen.SetWidth(penwidth);
					gc->SetPen(pen);
					if (field == wxT('f'))
						gc->SetBrush(clrBackground);
					else if (field == wxT('F'))
						gc->SetBrush(clrForeground);
					else
						gc->SetBrush(*wxTRANSPARENT_BRUSH);
					gc->DrawRectangle(midx + x, midy - (y + h), w, h);
					break;
				case 'T':
					angle = GetTokenLong(&line) / 10.0;
					x = midx + GetTokenLong(&line) * scale;
					y = midy - GetTokenLong(&line) * scale;
					h = GetTokenLong(&line) * scale;	/* text size */
					visible = GetTokenLong(&line) == 0;
					if (GetTokenLong(&line) > 1)
						break;	/* ignore parts other than part 1 */
					if (GetTokenLong(&line) > 1)
						break;	/* ignore De Morgan converted shape */
					name = GetToken(&line);
					GetToken(&line);	//??? ignore Italic/Normal, because the CXF font currently does not support slanted text
					bold = GetTokenLong(&line);
					field = GetToken(&line);
					if (field == wxT('L'))
						halign = CXF_ALIGNLEFT;
					else if (field == wxT('R'))
						halign = CXF_ALIGNRIGHT;
					else
						halign = CXF_ALIGNCENTRE;
					field = GetToken(&line);
					if (field == wxT('T'))
						valign = CXF_ALIGNTOP;
					else if (field == wxT('B'))
						valign = CXF_ALIGNBOTTOM;
					else
						valign = CXF_ALIGNCENTRE;
					pen.SetWidth(bold ? h * 0.25 : h * 0.15);
					pen.SetColour(visible ? clrText : clrHiddenText);
					gc->SetPen(pen);
					VFont.SetScale(h / CXF_CAPSHEIGHT, h / CXF_CAPSHEIGHT);
					VFont.SetOverbar(false);
					VFont.SetRotation((int)angle);
					VFont.SetAlign(halign, valign);
					DrawStrokeText(gc, x, y, name);
					break;
				case 'X':
					name = GetToken(&line);
					if (name == wxT('~'))
						name = wxEmptyString;
					pin = GetToken(&line);
					if (pin == wxT('~'))
						pin = wxEmptyString;
					points = new wxPoint2DDouble[2];
					wxASSERT(points != NULL);
					points[0].m_x = midx + GetTokenLong(&line) * scale;
					points[0].m_y = midy - GetTokenLong(&line) * scale;
					length = GetTokenLong(&line) * scale;
					field = GetToken(&line);
					orientation = field[0];
					size_nr = GetTokenLong(&line) * scale;
					size_name = GetTokenLong(&line) * scale;
					if (GetTokenLong(&line) > 1)
						break;	/* ignore parts other than part 1 */
					if (GetTokenLong(&line) > 1)
						break;	/* ignore De Morgan converted shape */
					GetToken(&line);	/* ignore type */
					field = GetToken(&line);	/* pin shape */
					points[1] = points[0];
					switch (orientation) {
					case 'L':
						points[1].m_x = points[0].m_x - length;
						angle = 0;
						break;
					case 'R':
						points[1].m_x = points[0].m_x + length;
						angle = 0;
						break;
					case 'U':
						points[1].m_y = points[0].m_y - length;
						angle = 90;
						break;
					case 'D':
						points[1].m_y = points[0].m_y + length;
						angle = 90;
						break;
					default:
						wxASSERT(false);
						angle = 0;		/* just to avoid a compiler warning */
					}
					pen.SetWidth(DEFAULTPEN);
					gc->SetPen(pen);
					gc->DrawLines(2, points);
					if (field.Length() > 0) {
						gc->SetBrush(clrBackground);
						if (field == wxT('I') || field == wxT("CI")) {
							/* inverted or inverted clock */
							wxPoint2DDouble ptShape = points[1];
							if (!Equal(points[0].m_x, ptShape.m_x)) {
								int sign = (points[0].m_x < ptShape.m_x) ? -1 : 1;
								ptShape.m_x += sign * (size_pinshape + 1) / 2;
							} else if (!Equal(points[0].m_y, ptShape.m_y)) {
								int sign = (points[0].m_y < ptShape.m_y) ? -1 : 1;
								ptShape.m_y += sign * (size_pinshape + 1) / 2;
							}
							gc->DrawEllipse(ptShape.m_x - size_pinshape / 2, ptShape.m_y - size_pinshape / 2, size_pinshape, size_pinshape);
						}
						if (field == wxT('C') || field == wxT("CI")) {
							/* clock or inverted clock */
							wxPoint2DDouble ptShape[3];
							for (int i = 0; i < 3; i++)
								ptShape[i] = points[1];
							if (!Equal(points[0].m_x, ptShape[1].m_x)) {
								int sign = (points[0].m_x < ptShape[1].m_x) ? 1 : -1;
								ptShape[1].m_x += sign * (size_pinshape + 1) / 2;
								ptShape[0].m_y -= (size_pinshape + 1) / 2;
								ptShape[2].m_y += (size_pinshape + 1) / 2;
							} else if (!Equal(points[0].m_y, ptShape[1].m_y)) {
								int sign = (points[0].m_y < ptShape[1].m_y) ? 1 : -1;
								ptShape[1].m_y += sign * (size_pinshape + 1) / 2;
								ptShape[0].m_x -= (size_pinshape + 1) / 2;
								ptShape[2].m_x += (size_pinshape + 1) / 2;
							}
							gc->DrawLines(3, ptShape);
						}
					}
					gc->SetBrush(clrForeground);
					gc->DrawEllipse(points[0].m_x - 2, points[0].m_y - 2, 4, 4);	/* cicle at the endpoint */
					/* pin name and number */
					pen.SetColour(clrText);
					if (show_pinnr) {
						pen.SetWidth(size_nr * 0.125);
						gc->SetPen(pen);
						VFont.SetScale(size_nr / CXF_CAPSHEIGHT, size_nr / CXF_CAPSHEIGHT);
						VFont.SetOverbar(false);
						VFont.SetRotation((int)angle);
						VFont.SetAlign(CXF_ALIGNCENTRE, CXF_ALIGNBOTTOM);
						if (angle > EPSILON) {
							x = points[1].m_x;
							y = (points[0].m_y + points[1].m_y) / 2;
						} else {
							x = (points[0].m_x + points[1].m_x) / 2;
							y = points[1].m_y;
						}
						DrawStrokeText(gc, x, y, pin);
					}
					if (show_pinname) {
						pen.SetWidth(size_name * 0.15);
						gc->SetPen(pen);
						VFont.SetScale(size_name / CXF_CAPSHEIGHT, size_name / CXF_CAPSHEIGHT);
						if (name[0] == wxT('~')) {
							name = name.Mid(1);
							VFont.SetOverbar(true);
						}
						if (pinname_offset < EPSILON) {
							/* pin name is outside the shape */
							VFont.SetAlign(CXF_ALIGNCENTRE, CXF_ALIGNTOP);
							if (angle > EPSILON) {
								x = points[1].m_x;
								y = (points[0].m_y + points[1].m_y) / 2;
							} else {
								x = (points[0].m_x + points[1].m_x) / 2;
								y = points[1].m_y;
							}
						} else {
							/* pin name is inside the shape */
							if (pinname_offset < size_name / 2)
								pinname_offset = size_name / 2;
							if (angle > EPSILON) {
								x = points[1].m_x;
								if (points[0].m_y < points[1].m_y) {
									y = points[1].m_y + pinname_offset;
									VFont.SetAlign(CXF_ALIGNRIGHT, CXF_ALIGNCENTRE);
								} else {
									y = points[1].m_y - pinname_offset;
									VFont.SetAlign(CXF_ALIGNLEFT, CXF_ALIGNCENTRE);
								}
							} else {
								if (points[0].m_x < points[1].m_x) {
									x = points[1].m_x + pinname_offset;
									VFont.SetAlign(CXF_ALIGNLEFT, CXF_ALIGNCENTRE);
								} else {
									x = points[1].m_x - pinname_offset;
									VFont.SetAlign(CXF_ALIGNRIGHT, CXF_ALIGNCENTRE);
								}
								y = points[1].m_y;
							}
						}
						DrawStrokeText(gc, x, y, name);
					}
					delete[] points;
					break;
				}
			}
		}
	}
}

void libmngrFrame::DrawFootprints(wxGraphicsContext *gc, int midx, int midy, int transp[])
{
	wxColour clrBody, clrPad, clrPadFill, clrText, clrHiddenText;
	wxPoint2DDouble points[5];
	wxPen pen;

	for (int fp = 0; fp < 2; fp++) {
		/* check whether the footprint is visible */
		if (fp == 0 && CompareMode && !m_toolBar->GetToolState(IDT_LEFTFOOTPRINT))
			continue;
		if (fp == 1 && (!CompareMode || !m_toolBar->GetToolState(IDT_RIGHTFOOTPRINT)))
			continue;
		if (PartData[fp].Count() == 0)
			continue;
		if (fp == 0) {
			clrBody.Set(192, 192, 96, transp[fp]);
			clrPad.Set(160, 0, 0, transp[fp]);
			clrPadFill.Set(160, 48, 48, transp[fp]);
			clrText.Set(240, 240, 64, transp[fp]);
			clrHiddenText.Set(160, 160, 50, transp[fp]);
		} else {
			clrBody.Set(192, 96, 192, transp[fp]);
			clrPad.Set(0, 160, 0, transp[fp]);
			clrPadFill.Set(48, 160, 48, transp[fp]);
			clrText.Set(240, 64, 240, transp[fp]);
			clrHiddenText.Set(160, 50, 160, transp[fp]);
		}
		/* Draw the outline, plus optionally the texts */
		pen.SetColour(clrBody);
		gc->SetPen(pen);
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		bool unit_mm = Footprint[fp].Type >= VER_MM;
		double module_angle = 0;	/* all angles should be corrected with the footprint angle */
		for (int idx = 0; idx < (int)PartData[fp].Count(); idx++) {
			wxString line = PartData[fp][idx];
			wxString token = GetToken(&line);
			if (token.CmpNoCase(wxT("Po")) == 0) {
				GetToken(&line);	/* ignore X position */
				GetToken(&line);	/* ignore Y position */
				if (line.length() > 0)
					module_angle = GetTokenLong(&line) / 10.0;
			} else if (token.CmpNoCase(wxT("DS")) == 0) {
				double x1 = GetTokenDim(&line, unit_mm);
				double y1 = GetTokenDim(&line, unit_mm);
				double x2 = GetTokenDim(&line, unit_mm);
				double y2 = GetTokenDim(&line, unit_mm);
				double penwidth = GetTokenDim(&line, unit_mm);
				/* ignore layer */
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				points[0].m_x = x1 * Scale + midx;
				points[0].m_y = y1 * Scale + midy;
				points[1].m_x = x2 * Scale + midx;
				points[1].m_y = y2 * Scale + midy;
				gc->DrawLines(2, points);
			} else if (token.CmpNoCase(wxT("DC")) == 0) {
				double x = GetTokenDim(&line, unit_mm);
				double y = GetTokenDim(&line, unit_mm);
				double dx = GetTokenDim(&line, unit_mm);
				double dy = GetTokenDim(&line, unit_mm);
				double penwidth = GetTokenDim(&line, unit_mm);
				/* ignore layer */
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				dx -= x;
				dy -= y;
				double radius = sqrt(dx * dx + dy * dy) * Scale;
				gc->DrawEllipse(x * Scale - radius + midx, y * Scale - radius + midy, 2 * radius, 2 * radius);
			} else if (token.CmpNoCase(wxT("DA")) == 0) {
				double x = GetTokenDim(&line, unit_mm);
				double y = GetTokenDim(&line, unit_mm);
				double dx = GetTokenDim(&line, unit_mm);
				double dy = GetTokenDim(&line, unit_mm);
				double angle = GetTokenLong(&line) / 10.0;
				double penwidth = GetTokenDim(&line, unit_mm);
				/* ignore layer */
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				dx -= x;
				dy -= y;
				double radius = sqrt(dx * dx + dy * dy) * Scale;
				double startangle = atan2(dy, dx);
				double endangle = startangle + (double)angle * M_PI / 180.0;
				if (endangle > 2 * M_PI)
					endangle -= 2 * M_PI;
				wxGraphicsPath path = gc->CreatePath();
				path.AddArc(x * Scale + midx, y * Scale + midy, radius, startangle, endangle, true);
				gc->DrawPath(path);
			} else if (token.CmpNoCase(wxT("DP")) == 0) {
				GetToken(&line);	/* ignore first four unknown values */
				GetToken(&line);
				GetToken(&line);
				GetToken(&line);
				long count = GetTokenLong(&line);
				wxASSERT(count > 0);
				double penwidth = GetTokenDim(&line, unit_mm);
				/* ignore layer */
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				wxBrush brush(clrBody);
				gc->SetBrush(brush);
				wxPoint2DDouble *pt = new wxPoint2DDouble[count];
				wxASSERT(pt != NULL);
				for (long p = 0; p < count; p++) {
					idx++;
					line = PartData[fp][idx];
					token = GetToken(&line);
					wxASSERT(token.CmpNoCase(wxT("Dl")) == 0);
					double x = GetTokenDim(&line, unit_mm);
					double y = GetTokenDim(&line, unit_mm);
					pt[p].m_x = x * Scale + midx;
					pt[p].m_y = y * Scale + midy;
				}
				gc->DrawLines(count, pt);
				gc->SetBrush(*wxTRANSPARENT_BRUSH);
				delete[] pt;
			} else if (toupper(token[0]) == 'T' && isdigit(token[1])) {
				long field;
				if (ShowLabels || (token.Mid(1).ToLong(&field) && field >= 2)) {
					double x = GetTokenDim(&line, unit_mm);
					double y = GetTokenDim(&line, unit_mm);
					double cy = GetTokenDim(&line, unit_mm);
					double cx = GetTokenDim(&line, unit_mm);
					double rot = NormalizeAngle(GetTokenLong(&line) / 10.0 - module_angle);
					double penwidth = GetTokenDim(&line, unit_mm);
					GetToken(&line);	/* ignore flags */
					wxString visflag = GetToken(&line);
					bool visible = (visflag == wxT('V'));
					GetToken(&line);	/* ignore layer */
					GetToken(&line);	/* ignore unknown flags */
					token = GetToken(&line);
					penwidth *= Scale;
					if (penwidth < DEFAULTPEN)
						penwidth = DEFAULTPEN;
					wxPen tpen;
					tpen.SetWidth(penwidth);
					tpen.SetColour(visible ? clrText : clrHiddenText);
					gc->SetPen(tpen);
					VFont.SetScale(cx / CXF_CAPSHEIGHT * Scale, cy / CXF_CAPSHEIGHT * Scale);
					VFont.SetOverbar(false);
					VFont.SetRotation((int)rot);
					VFont.SetAlign(CXF_ALIGNCENTRE, CXF_ALIGNCENTRE);
					DrawStrokeText(gc, x * Scale + midx, y * Scale + midy, token);
				}
			} else if (token.Cmp(wxT("(at")) == 0) {
				GetToken(&line);	/* ignore X */
				GetToken(&line);	/* ignore Y */
				if (line.length() > 0)
					module_angle = GetTokenDouble(&line);
			} else if (token.Cmp(wxT("(fp_line")) == 0) {
				double x1=0, y1=0, x2=0, y2=0, penwidth=0;
				wxString section = GetSection(line, wxT("start"));
				if (section.length() > 0) {
					x1 = GetTokenDim(&section, true);
					y1 = GetTokenDim(&section, true);
				}
				section = GetSection(line, wxT("end"));
				if (section.length() > 0) {
					x2 = GetTokenDim(&section, true);
					y2 = GetTokenDim(&section, true);
				}
				section = GetSection(line, wxT("width"));
				if (section.length() > 0)
					penwidth = GetTokenDim(&section, true);
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				points[0].m_x = x1 * Scale + midx;
				points[0].m_y = y1 * Scale + midy;
				points[1].m_x = x2 * Scale + midx;
				points[1].m_y = y2 * Scale + midy;
				gc->DrawLines(2, points);
			} else if (token.Cmp(wxT("(fp_circle")) == 0) {
				double x=0, y=0, dx=0, dy=0, penwidth=0;
				wxString section = GetSection(line, wxT("center"));
				if (section.length() > 0) {
					x = GetTokenDim(&section, true);
					y = GetTokenDim(&section, true);
				}
				section = GetSection(line, wxT("end"));
				if (section.length() > 0) {
					dx = GetTokenDim(&section, true);
					dy = GetTokenDim(&section, true);
				}
				section = GetSection(line, wxT("width"));
				if (section.length() > 0)
					penwidth = GetTokenDim(&section, true);
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				dx -= x;
				dy -= y;
				double size = sqrt(dx * dx + dy * dy) * Scale;
				gc->DrawEllipse(x * Scale - size + midx, y * Scale - size + midy, 2 * size, 2 * size);
			} else if (token.Cmp(wxT("(fp_arc")) == 0) {
				double x=0, y=0, dx=0, dy=0, penwidth=0;
				double angle = 0;
				wxString section = GetSection(line, wxT("start"));
				if (section.length() > 0) {
					x = GetTokenDim(&section, true);
					y = GetTokenDim(&section, true);
				}
				section = GetSection(line, wxT("end"));
				if (section.length() > 0) {
					dx = GetTokenDim(&section, true);
					dy = GetTokenDim(&section, true);
				}
				section = GetSection(line, wxT("angle"));
				if (section.length() > 0)
					angle = GetTokenDouble(&section);
				section = GetSection(line, wxT("width"));
				if (section.length() > 0)
					penwidth = GetTokenDim(&section, true);
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				dx -= x;
				dy -= y;
				double radius = sqrt(dx * dx + dy * dy) * Scale;
				double startangle = atan2(dy, dx);
				double endangle = startangle + (double)angle * M_PI / 180.0;
				if (endangle > 2 * M_PI)
					endangle -= 2 * M_PI;
				wxGraphicsPath path = gc->CreatePath();
				path.AddArc(x * Scale + midx, y * Scale + midy, radius, startangle, endangle, true);
				gc->DrawPath(path);
			} else if (token.Cmp(wxT("(fp_poly")) == 0) {
				double penwidth = 0;
				wxString section = GetSection(line, wxT("width"));
				if (section.length() > 0)
					penwidth = GetTokenDim(&section, true);
				penwidth *= Scale;
				if (penwidth < DEFAULTPEN)
					penwidth = DEFAULTPEN;
				pen.SetWidth(penwidth);
				gc->SetPen(pen);
				wxBrush brush(clrBody);
				gc->SetBrush(brush);
				/* first count the number of vertices */
				section = GetSection(line, wxT("pts"));
				long count = 0;
				while (GetSection(section, wxT("xy"), count).Length() > 0)
					count++;
				wxPoint2DDouble *pt = new wxPoint2DDouble[count];
				wxASSERT(pt != NULL);
				for (long p = 0; p < count; p++) {
					wxString subsect = GetSection(section, wxT("xy"), p);
					wxASSERT(subsect.length() > 0);
					double x = GetTokenDim(&subsect, true);
					double y = GetTokenDim(&subsect, true);
					pt[p].m_x = x * Scale + midx;
					pt[p].m_y = y * Scale + midy;
				}
				gc->DrawLines(count, pt);
				gc->SetBrush(*wxTRANSPARENT_BRUSH);
				delete[] pt;
			} else if (token.Cmp(wxT("(fp_text")) == 0) {
				wxString ident = GetToken(&line);
				if (ShowLabels || ident.Cmp(wxT("user")) == 0) {
					double x = 0, y = 0, cx = 0, cy = 0, penwidth = 1;
					double rot = 0;
					token = GetToken(&line);	/* get the text itself */
					bool visible = line.Find(wxT(" hide ")) > 0;
					wxString section = GetSection(line, wxT("at"));
					if (section.length() > 0) {
						x = GetTokenDim(&section, true);
						y = GetTokenDim(&section, true);
						if (section.length() > 0)
							rot = NormalizeAngle(GetTokenDouble(&section) - module_angle);
					}
					section = GetSection(line, wxT("effects"));
					if (section.length() > 0) {
						section = GetSection(section, wxT("font"));
						if (section.length() > 0) {
							wxString subsect = GetSection(section, wxT("size"));
							if (subsect.length() > 0) {
								cy = GetTokenDim(&subsect, true);
								cx = GetTokenDim(&subsect, true);
							}
							subsect = GetSection(section, wxT("thickness"));
							if (subsect.length() > 0)
								penwidth = GetTokenDim(&subsect, true);
						}
					}
					penwidth *= Scale;
					if (penwidth < DEFAULTPEN)
						penwidth = DEFAULTPEN;
					wxPen tpen;
					tpen.SetWidth(penwidth);
					tpen.SetColour(visible ? clrText : clrHiddenText);
					gc->SetPen(tpen);
					VFont.SetScale(cx / CXF_CAPSHEIGHT * Scale, cy / CXF_CAPSHEIGHT * Scale);
					VFont.SetOverbar(false);
					VFont.SetRotation((int)rot);
					VFont.SetAlign(CXF_ALIGNCENTRE, CXF_ALIGNCENTRE);
					DrawStrokeText(gc, x * Scale + midx, y * Scale + midy, token);
				}
			}
		}

		/* draw the pads */
		wxBrush brush(clrPadFill);
		pen.SetColour(clrPad);
		pen.SetWidth(1);
		gc->SetBrush(brush);
		gc->SetPen(pen);

		wxBrush brushHole;
		brushHole.SetColour(wxColour(224, 224, 0, transp[fp]));

		wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		font.SetPointSize(FontSizeLegend);
		gc->SetFont(font, wxColour(96, 192, 192));

		bool inpad = false;
		double padx = 0, pady = 0, padwidth = 0, padheight = 0, padrot = 0;
		double drillx = 0, drilly = 0, drillwidth = 0, drillheight = 0;
		wxString padpin, padshape;
		for (int idx = 0; idx < (int)PartData[fp].Count(); idx++) {
			wxString line = PartData[fp][idx];
			if (line[0] == wxT('$')) {
				if (line.CmpNoCase(wxT("$PAD")) == 0) {
					inpad = true;
					drillwidth = drillheight = 0;
				} else if (line.CmpNoCase(wxT("$EndPAD")) == 0) {
					/* draw the pad */
					points[0].m_x = -padwidth/2 * Scale;
					points[0].m_y = -padheight/2 * Scale;
					points[1].m_x =  padwidth/2 * Scale;
					points[1].m_y = -padheight/2 * Scale;
					points[2].m_x =  padwidth/2 * Scale;
					points[2].m_y =  padheight/2 * Scale;
					points[3].m_x = -padwidth/2 * Scale;
					points[3].m_y =  padheight/2 * Scale;
					points[4].m_x = -padwidth/2 * Scale;
					points[4].m_y = -padheight/2 * Scale;
					/* apply rotation */
					if (padrot > EPSILON) {
						double angle = (padrot * M_PI / 180.0);
						for (int idx = 0; idx < 5; idx++) {
							wxDouble nx = points[idx].m_x * cos(angle) - points[idx].m_y * sin(angle);
							wxDouble ny = points[idx].m_x * sin(angle) + points[idx].m_y * cos(angle);
							points[idx].m_x = nx;
							points[idx].m_y = ny;
						}
					}
					/* move pad relative to footprint origin */
					for (int idx = 0; idx < 5; idx++) {
						points[idx].m_x += padx * Scale + midx;
						points[idx].m_y += pady * Scale + midy;
					}
					gc->SetBrush(brush);
					/* avoid negative width/height for ellipses or stadiums */
					CoordSize cs(points[0].m_x, points[0].m_y, points[2].m_x - points[0].m_x, points[2].m_y - points[0].m_y);
					if (padshape.CmpNoCase(wxT("C")) == 0)
						gc->DrawEllipse(cs.GetX(), cs.GetY(), cs.GetWidth(), cs.GetHeight());
					else if (padshape.CmpNoCase(wxT("O")) == 0)
						gc->DrawRoundedRectangle(cs.GetX(), cs.GetY(), cs.GetWidth(), cs.GetHeight(),
												 ((cs.GetWidth() < cs.GetHeight()) ? cs.GetWidth() : cs.GetHeight()) / 2);
					else
						gc->DrawLines(5, points);
					/* optionally the hole in the pad */
					if (drillwidth > EPSILON) {
						if (padshape == wxT('C') && padwidth - drillwidth < 0.05)
							gc->SetBrush(brushHole);
						else
							gc->SetBrush(*wxBLACK_BRUSH);
						if (drillheight > EPSILON) {
							if ((padrot > 45 && padrot < 135) || (padrot > 225 && padrot < 315)) {
								double t = drillwidth;
								drillwidth = drillheight;
								drillheight = t;
							}
							CoordSize cs((padx + drillx - drillwidth/2) * Scale + midx,
													 (pady + drilly - drillheight/2) * Scale + midy,
													 drillwidth * Scale, drillheight * Scale);
							gc->DrawRoundedRectangle(cs.GetX(), cs.GetY(), cs.GetWidth(), cs.GetHeight(),
													 ((cs.GetWidth() < cs.GetHeight()) ? cs.GetWidth() : cs.GetHeight()) / 2);
						} else {
							gc->DrawEllipse((padx + drillx - drillwidth/2) * Scale + midx,
											(pady + drilly - drillwidth/2) * Scale + midy,
											drillwidth * Scale, drillwidth * Scale);
						}
					}
					/* draw the pin name inside the pad */
					wxDouble tw, th, td, tex;
					gc->GetTextExtent(padpin, &tw, &th, &td, &tex);
					gc->DrawText(padpin, padx * Scale + midx - tw/2, pady * Scale + midy - th/2);
					inpad = false;
				}
				continue;
			} else if (line[0] == wxT('(') && line.Left(4).Cmp(wxT("(pad")) == 0) {
				GetToken(&line);	/* ignore "(pad" */
				padpin = GetToken(&line);
				GetToken(&line);	/* ignore smd/thru_hole/np_thru_hole type */
				padshape = GetToken(&line);
				padrot = 0;			/* preset pad rotation (frequently omitted) */
				wxString section = GetSection(line, wxT("at"));
				if (section.length() > 0) {
					padx = GetTokenDim(&section, true);
					pady = GetTokenDim(&section, true);
					if (section.length() > 0)
						padrot = NormalizeAngle(GetTokenDouble(&section) - module_angle);
				}
				section = GetSection(line, wxT("size"));
				if (section.length() > 0) {
					padwidth = GetTokenDim(&section, true);
					padheight = GetTokenDim(&section, true);
				}
				drillx = drilly = 0;	/* preset drill parameters (as these are optional) */
				drillwidth = drillheight = 0;
				section = GetSection(line, wxT("drill"));
				if (section.length() > 0) {
					wxString sectoffset = GetSection(section, wxT("offset"));
					if (section.Left(4).Cmp(wxT("oval"))==0) {
						GetToken(&section);
						drillwidth = GetTokenDim(&section, true);
						drillheight = GetTokenDim(&section, true);
					} else {
						drillwidth = GetTokenDim(&section, true);
					}
					if (sectoffset.length() > 0) {
						drillx = GetTokenDim(&sectoffset, true);
						drilly = GetTokenDim(&sectoffset, true);
					}
				}
				/* draw the pad */
				points[0].m_x = -padwidth/2 * Scale;
				points[0].m_y = -padheight/2 * Scale;
				points[1].m_x =  padwidth/2 * Scale;
				points[1].m_y = -padheight/2 * Scale;
				points[2].m_x =  padwidth/2 * Scale;
				points[2].m_y =  padheight/2 * Scale;
				points[3].m_x = -padwidth/2 * Scale;
				points[3].m_y =  padheight/2 * Scale;
				points[4].m_x = -padwidth/2 * Scale;
				points[4].m_y = -padheight/2 * Scale;
				/* apply rotation */
				if (padrot != 0) {
					double angle = (padrot * M_PI / 180.0);
					for (int idx = 0; idx < 5; idx++) {
						wxDouble nx = points[idx].m_x * cos(angle) - points[idx].m_y * sin(angle);
						wxDouble ny = points[idx].m_x * sin(angle) + points[idx].m_y * cos(angle);
						points[idx].m_x = nx;
						points[idx].m_y = ny;
					}
				}
				/* move pad relative to footprint origin */
				for (int idx = 0; idx < 5; idx++) {
					points[idx].m_x += padx * Scale + midx;
					points[idx].m_y += pady * Scale + midy;
				}
				gc->SetBrush(brush);
				CoordSize cs(points[0].m_x, points[0].m_y, points[2].m_x - points[0].m_x, points[2].m_y - points[0].m_y);
				if (padshape.Cmp(wxT("circle")) == 0)
					gc->DrawEllipse(cs.GetX(), cs.GetY(), cs.GetWidth(), cs.GetHeight());
				else if (padshape.Cmp(wxT("oval")) == 0)
					gc->DrawRoundedRectangle(cs.GetX(), cs.GetY(), cs.GetWidth(), cs.GetHeight(),
											 ((cs.GetWidth() < cs.GetHeight()) ? cs.GetWidth() : cs.GetHeight()) / 2);
				else
					gc->DrawLines(5, points);
				/* optionally the hole in the pad */
				if (drillwidth > EPSILON) {
					if (padshape.Cmp(wxT("circle")) == 0 && padwidth - drillwidth < 0.05)
						gc->SetBrush(brushHole);
					else
						gc->SetBrush(*wxBLACK_BRUSH);
					if (drillheight > EPSILON) {
						if ((padrot > 45 && padrot < 135) || (padrot > 225 && padrot < 315)) {
							double t = drillwidth;
							drillwidth = drillheight;
							drillheight = t;
						}
						CoordSize cs((padx + drillx - drillwidth/2) * Scale + midx,
												 (pady + drilly - drillheight/2) * Scale + midy,
												 drillwidth * Scale, drillheight * Scale);
						gc->DrawRoundedRectangle(cs.GetX(), cs.GetY(), cs.GetWidth(), cs.GetHeight(),
												 ((cs.GetWidth() < cs.GetHeight()) ? cs.GetWidth() : cs.GetHeight()) / 2);
					} else {
						gc->DrawEllipse((padx + drillx - drillwidth/2) * Scale + midx,
										(pady + drilly -drillwidth/2) * Scale + midy,
										drillwidth * Scale, drillwidth * Scale);
					}
				}
				/* draw the pin name inside the pad */
				wxDouble tw, th, td, tex;
				gc->GetTextExtent(padpin, &tw, &th, &td, &tex);
				gc->DrawText(padpin, padx * Scale + midx - tw/2, pady * Scale + midy - th/2);
				continue;
			}
			if (!inpad)
				continue;
			wxString token = GetToken(&line);
			if (token.CmpNoCase(wxT("Po")) == 0) {
				padx = GetTokenDim(&line, unit_mm);	/* this is relative to the footprint position, but in a footprint file, the position is always 0 */
				pady = GetTokenDim(&line, unit_mm);
			} else if (token.CmpNoCase(wxT("Sh")) == 0) {
				padpin = GetToken(&line);
				padshape = GetToken(&line);
				padwidth = GetTokenDim(&line, unit_mm);
				padheight = GetTokenDim(&line, unit_mm);
				GetToken(&line);	/* ignore deltas for trapezoid shape */
				GetToken(&line);
				padrot = NormalizeAngle(GetTokenLong(&line) / 10.0 - module_angle);
			} else if (token.CmpNoCase(wxT("Dr")) == 0) {
				drillwidth = GetTokenDim(&line, unit_mm);
				drillx = GetTokenDim(&line, unit_mm);	/* this is relative to the pad position */
				drilly = GetTokenDim(&line, unit_mm);
				token = GetToken(&line);
				if (token == wxT('O')) {
					drillwidth = GetTokenDim(&line, unit_mm);
					drillheight = GetTokenDim(&line, unit_mm);
				}
			}
		}

		/* draw the measures */
		if (m_toolBar->GetToolState(IDT_MEASUREMENTS)) {
			pen.SetColour(0, 192, 192);
			pen.SetWidth(1);
			gc->SetPen(pen);

			font.SetPointSize(FontSizeLegend);
			font.SetWeight(wxFONTWEIGHT_BOLD);
			gc->SetFont(font, wxColour(0, 192, 192));

			if (Footprint[fp].Pitch > EPSILON) {
				if (Footprint[fp].PitchVertical) {
					/* decide where to put the pitch dimension: left or right (prefer left) */
					int sign = (Footprint[fp].PitchPins[0].GetX() > 0 && Footprint[fp].PitchPins[1].GetX() > 0) ? 1 : -1;
					double pos = Footprint[fp].PitchPins[0].GetX();
					if (sign < 0 && pos > Footprint[fp].PitchPins[1].GetX())
						pos = Footprint[fp].PitchPins[1].GetX();
					else if (sign > 0 && pos < Footprint[fp].PitchPins[1].GetX())
						pos = Footprint[fp].PitchPins[1].GetX();
					for (int idx = 0; idx < 2; idx++) {
						points[0].m_x = Footprint[fp].PitchPins[idx].GetX() * Scale + midx + sign * LINE_OFFSET;
						points[0].m_y = Footprint[fp].PitchPins[idx].GetY() * Scale + midy;
						points[1].m_x = pos * Scale + midx + sign * DimensionOffset;
						points[1].m_y = Footprint[fp].PitchPins[idx].GetY() * Scale + midy;
						gc->DrawLines(2, points);
					}
					int x = pos * Scale + midx + sign * DimensionOffset;
					int y = (Footprint[fp].PitchPins[0].GetY() + Footprint[fp].PitchPins[1].GetY()) / 2 * Scale + midy;
					wxString text = wxString::Format(wxT("%.2f (%.1f)"),
						 							 Footprint[fp].Pitch, Footprint[fp].Pitch / 0.0254);
					wxDouble tw, th, td, tex;
					gc->GetTextExtent(text, &tw, &th, &td, &tex);
					gc->DrawText(text, (sign >= 0) ? x : x - th, y + tw/2, M_PI / 2);
				} else {
					/* decide where to put the pitch dimension: top or bottom (prefer top) */
					int sign = (Footprint[fp].PitchPins[0].GetY() > 0 && Footprint[fp].PitchPins[1].GetY() > 0) ? 1 : -1;
					double pos = Footprint[fp].PitchPins[0].GetY();
					if (sign < 0 && pos > Footprint[fp].PitchPins[1].GetY())
						pos = Footprint[fp].PitchPins[1].GetY();
					else if (sign > 0 && pos < Footprint[fp].PitchPins[1].GetY())
						pos = Footprint[fp].PitchPins[1].GetY();
					for (int idx = 0; idx < 2; idx++) {
						points[0].m_x = Footprint[fp].PitchPins[idx].GetX() * Scale + midx;
						points[0].m_y = Footprint[fp].PitchPins[idx].GetY() * Scale + midy + sign * LINE_OFFSET;
						points[1].m_x = Footprint[fp].PitchPins[idx].GetX() * Scale + midx;
						points[1].m_y = pos * Scale + midy + sign * DimensionOffset;
						gc->DrawLines(2, points);
					}
					int x = (Footprint[fp].PitchPins[0].GetX() + Footprint[fp].PitchPins[1].GetX()) / 2 * Scale + midx;
					int y = pos * Scale + midy + sign * DimensionOffset;
					wxString text = wxString::Format(wxT("%.2f (%.1f)"),
													 Footprint[fp].Pitch, Footprint[fp].Pitch / 0.0254);
					wxDouble tw, th, td, tex;
					gc->GetTextExtent(text, &tw, &th, &td, &tex);
					gc->DrawText(text, x - tw/2, (sign >= 0) ? y : y - th);
				}
			}
			if (Footprint[fp].SpanVer > EPSILON) {
				/* decide where to put the vertical span: left or right (prefer right) */
				int sign = (Footprint[fp].SpanVerPins[0].GetX() < 0 && Footprint[fp].SpanVerPins[1].GetX() < 0) ? -1 : 1;
				double pos = Footprint[fp].SpanVerPins[0].GetX();
				if (sign < 0 && pos > Footprint[fp].SpanVerPins[1].GetX())
					pos = Footprint[fp].SpanVerPins[1].GetX();
				else if (sign > 0 && pos < Footprint[fp].SpanVerPins[1].GetX())
					pos = Footprint[fp].SpanVerPins[1].GetX();
				for (int idx = 0; idx < 2; idx++) {
					points[0].m_x = Footprint[fp].SpanVerPins[idx].GetX() * Scale + midx + sign * LINE_OFFSET;
					points[0].m_y = Footprint[fp].SpanVerPins[idx].GetY() * Scale + midy;
					points[1].m_x = pos * Scale + midx + sign * DimensionOffset;
					points[1].m_y = Footprint[fp].SpanVerPins[idx].GetY() * Scale + midy;
					gc->DrawLines(2, points);
				}
				int x = pos * Scale + midx + sign * DimensionOffset;
				int y = (Footprint[fp].SpanVerPins[0].GetY() + Footprint[fp].SpanVerPins[1].GetY()) / 2 * Scale + midy;
				wxString text = wxString::Format(wxT("%.2f (%.1f)"),
												 Footprint[fp].SpanVer, Footprint[fp].SpanVer / 0.0254);
				wxDouble tw, th, td, tex;
				gc->GetTextExtent(text, &tw, &th, &td, &tex);
				gc->DrawText(text, (sign >= 0) ? x : x - th, y + tw/2, M_PI / 2);
			}
			if (Footprint[fp].SpanHor > EPSILON) {
				/* decide where to put the horizontal span: top or bottom (prefer bottom) */
				int sign = (Footprint[fp].SpanHorPins[0].GetY() < 0 && Footprint[fp].SpanHorPins[1].GetY() < 0) ? -1 : 1;
				double pos = Footprint[fp].SpanHorPins[0].GetY();
				if (sign < 0 && pos > Footprint[fp].SpanHorPins[1].GetY())
					pos = Footprint[fp].SpanHorPins[1].GetY();
				else if (sign > 0 && pos < Footprint[fp].SpanHorPins[1].GetY())
					pos = Footprint[fp].SpanHorPins[1].GetY();
				for (int idx = 0; idx < 2; idx++) {
					points[0].m_x = Footprint[fp].SpanHorPins[idx].GetX() * Scale + midx;
					points[0].m_y = Footprint[fp].SpanHorPins[idx].GetY() * Scale + midy + sign * LINE_OFFSET;
					points[1].m_x = Footprint[fp].SpanHorPins[idx].GetX() * Scale + midx;
					points[1].m_y = pos * Scale + midy + sign * DimensionOffset;
					gc->DrawLines(2, points);
				}
				int x = (Footprint[fp].SpanHorPins[0].GetX() + Footprint[fp].SpanHorPins[1].GetX()) / 2 * Scale + midx;
				int y = pos * Scale + midy + sign * DimensionOffset;
				wxString text = wxString::Format(wxT("%.2f (%.1f)"),
												 Footprint[fp].SpanHor, Footprint[fp].SpanHor / 0.0254);
				wxDouble tw, th, td, tex;
				gc->GetTextExtent(text, &tw, &th, &td, &tex);
				gc->DrawText(text, x - tw/2, (sign >= 0) ? y : y - th);
			}

		} /* if (draw measures) */
	} /* for (fp) */

	/* draw cross for the centre point */
	pen.SetColour(192, 192, 192);
	pen.SetWidth(1);
	gc->SetPen(pen);
	#define CROSS_SIZE 12
	points[0].m_x = midx - CROSS_SIZE;
	points[0].m_y = midy;
	points[1].m_x = midx + CROSS_SIZE;
	points[1].m_y = midy;
	gc->DrawLines(2, points);
	points[0].m_x = midx;
	points[0].m_y = midy - CROSS_SIZE;
	points[1].m_x = midx;
	points[1].m_y = midy + CROSS_SIZE;
	gc->DrawLines(2, points);
}

void libmngrFrame::OnPaintViewport( wxPaintEvent& /*event*/ )
{
	wxPaintDC dc(m_panelView);
	wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
	if (PartData[0].Count() == 0 && PartData[1].Count() == 0)
		return;

	int transp[2] = { wxALPHA_OPAQUE, 0x80 };	/* this is the transparency for the overlay */
	if (!CompareMode || !m_toolBar->GetToolState(IDT_LEFTFOOTPRINT))
		transp[1] = wxALPHA_OPAQUE;

	wxSize sz = m_panelView->GetClientSize();
	long midx = sz.GetWidth() / 2 + OffsetX;
	long midy = sz.GetHeight() / 2 + OffsetY;

	if (SymbolMode)
		DrawSymbols(gc, midx, midy, transp);
	else
		DrawFootprints(gc, midx, midy, transp);
}

void libmngrFrame::OnSizeViewport( wxSizeEvent& /*event*/ )
{
	m_panelView->Refresh();
}

void libmngrFrame::OnZoomIn( wxCommandEvent& /*event*/ )
{
	if (Scale < SCALE_MAX) {
		Scale *= 1.1892;
		m_panelView->Refresh();
	} else {
		Scale = SCALE_MAX;
	}
}

void libmngrFrame::OnZoomOut( wxCommandEvent& /*event*/ )
{
	if (Scale > SCALE_MIN) {
		Scale /= 1.1892;
		m_panelView->Refresh();
	} else {
		Scale = SCALE_MIN;
	}
}

void libmngrFrame::OnShowMeasurements( wxCommandEvent& /*event*/ )
{
	m_panelView->Refresh();
}

void libmngrFrame::OnShowDetails( wxCommandEvent& /*event*/ )
{
	bool details = m_toolBar->GetToolState(IDT_DETAILSPANEL);
	m_menubar->Check(IDM_DETAILSPANEL, details);
	ToggleDetailsPanel(details);
}

void libmngrFrame::OnShowLabels( wxCommandEvent& /*event*/ )
{
	m_panelView->Refresh();
}

void libmngrFrame::OnShowLeftFootprint( wxCommandEvent& /*event*/ )
{
	m_panelView->Refresh();
}

void libmngrFrame::OnShowRightFootprint( wxCommandEvent& /*event*/ )
{
	m_panelView->Refresh();
}

void libmngrFrame::OnShowLeftDetails( wxCommandEvent& /*event*/ )
{
	UpdateDetails(0);
}

void libmngrFrame::OnShowRightDetails( wxCommandEvent& /*event*/ )
{
	UpdateDetails(1);
}

void libmngrFrame::ToggleDetailsPanel(bool onoff)
{
	if (onoff && !m_splitterViewPanel->IsSplit()) {
		m_panelSettings->Show();
		m_splitterViewPanel->SplitVertically(m_panelView, m_panelSettings, -PANEL_WIDTH);
	} else if (!onoff && m_splitterViewPanel->IsSplit()) {
		m_splitterViewPanel->Unsplit();	/* this implicitly hides the panel at the right */
	}
}

void libmngrFrame::EnableButtons(int side)
{
	bool valid = false;
	int idx;

	switch (side) {
	case -1:
		idx = m_choiceModuleRight->GetCurrentSelection();
		if (idx >= 0 && idx < (int)m_choiceModuleRight->GetCount()) {
			wxString target = m_choiceModuleRight->GetString(idx);
			valid = (target[0] != '(') || target.CmpNoCase(LIB_REPOS) == 0;
			idx = m_choiceModuleLeft->GetCurrentSelection();
			if (idx >= 0) {
				wxString source = m_choiceModuleLeft->GetString(idx);
				if (source.CmpNoCase(target) == 0)
					valid = false;
			}
		}
		m_btnMove->Enable(valid);
		m_btnCopy->Enable(valid);
		m_btnDelete->Enable(true);
		m_btnRename->Enable(true);
		m_btnDuplicate->Enable(true);
		m_btnMove->SetLabel(wxT("&Move >>>"));
		m_btnCopy->SetLabel(wxT("&Copy >>>"));
		break;
	case 0:
		m_btnMove->Enable(false);
		m_btnCopy->Enable(false);
		m_btnDelete->Enable(false);
		m_btnRename->Enable(false);
		m_btnDuplicate->Enable(false);
		m_btnMove->SetLabel(wxT("&Move"));
		m_btnCopy->SetLabel(wxT("&Copy"));
		break;
	case 1:
		idx = m_choiceModuleLeft->GetCurrentSelection();
		if (idx >= 0 && idx < (int)m_choiceModuleLeft->GetCount()) {
			wxString target = m_choiceModuleLeft->GetString(idx);
			valid = (target[0] != '(') || target.CmpNoCase(LIB_REPOS) == 0;
			idx = m_choiceModuleRight->GetCurrentSelection();
			if (idx >= 0) {
				wxString source = m_choiceModuleRight->GetString(idx);
				if (source.CmpNoCase(target) == 0)
					valid = false;
			}
		}
		m_btnMove->Enable(valid);
		m_btnCopy->Enable(valid);
		m_btnDelete->Enable(true);
		m_btnRename->Enable(true);
		m_btnDuplicate->Enable(true);
		m_btnMove->SetLabel(wxT("<<< &Move"));
		m_btnCopy->SetLabel(wxT("<<< &Copy"));
		break;
	default:
		wxASSERT(false);
	}
}

/** GetSelection() returns the symbol/footprint name of the first (and only)
 *  selected item in the given list control.
 *
 *  The library path is optionally returned (if both "choice" and "library" are
 *  passed in). The "author" name is only returned when the library path is also
 *  returned (and when the parameter is not NULL).
 *
 *  If no item is selected in the list control, the function returns
 *  wxEmptyString and the library path and author name are not altered.
 */
wxString libmngrFrame::GetSelection(wxListCtrl* list, wxChoice* choice, wxString* library, wxString* author)
{
	/* returns only the first selected item */
	long row = list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (row == -1)
		return wxEmptyString;
	wxString symbol = list->GetItemText(row);
	if (library) {
		wxASSERT(choice);	/* choice control must be known to retrieve the library */
		long idx = choice->GetCurrentSelection();
		wxASSERT(idx >= 0 && idx < (long)choice->GetCount());
		wxString source = choice->GetString(idx);
		if (source.CmpNoCase(LIB_REPOS) == 0) {
			*library = source;
			if (author) {
				wxListItem item;
				item.SetId(row);
				item.SetColumn(1);
				item.SetMask(wxLIST_MASK_TEXT);
				wxCHECK(list->GetItem(item), wxEmptyString);
				*author = item.GetText();
			}
		} else {
			wxListItem item;
			item.SetId(row);
			item.SetColumn(2);
			item.SetMask(wxLIST_MASK_TEXT);
			wxCHECK(list->GetItem(item), wxEmptyString);
			*library = item.GetText();
			if (author)
				*author = wxEmptyString;
		}
	}
	return symbol;
}

void libmngrFrame::RemoveSelection(wxListCtrl* list)
{
	long idx = -1;
	for (;;) {
		idx = list->GetNextItem(idx, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (idx == -1)
			break;
		#if defined _WIN32
			list->SetItemState(idx, 0, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
		#else
			list->SetItemState(idx, 0, wxLIST_STATE_SELECTED);
		#endif
	}
}

static int CompareStringNoCase(const wxString& first, const wxString& second)
{
	return first.CmpNoCase(second);
}

void libmngrFrame::CollectLibraries(const wxString &path, wxArrayString *list)
{
	wxDir dir(path);
	if (!dir.IsOpened())
		return;	/* error message already given */
	if (SymbolMode) {
		dir.GetAllFiles(path, list, wxT("*.lib"), wxDIR_FILES);
		dir.GetAllFiles(path, list, wxT("*.sym"), wxDIR_FILES);
	} else {
		dir.GetAllFiles(path, list, wxT("*.mod"), wxDIR_FILES);
		dir.GetAllFiles(path, list, wxT("*.emp"), wxDIR_FILES);
		/* collect s-expression libraries separately, because GetAllFiles() does not
			 return directories */
		wxString basename;
		bool cont = dir.GetFirst(&basename, wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
		while (cont) {
			wxFileName name(basename);
			if (name.GetExt().CmpNoCase(wxT("pretty")) == 0)
				list->Add(path + wxT(DIRSEP_STR) + basename);
			cont = dir.GetNext(&basename);
		}
	}
}

void libmngrFrame::CollectAllLibraries(bool eraselists)
{
	#if defined _MSC_VER
		_CrtCheckMemory();
	#endif

	wxArrayString list;

	wxConfig *config = new wxConfig(APP_NAME);
	wxString path;
	wxString key;
	int idx = 1;
	for ( ;; ) {
		if (SymbolMode)
			key = wxString::Format(wxT("paths/symbols%d"), idx);
		else
			key = wxString::Format(wxT("paths/footprints%d"), idx);
		if (!config->Read(key, &path))
			break;
		CollectLibraries(path, &list);
		idx++;
	}
	delete config;

	list.Sort(CompareStringNoCase);
	m_choiceModuleLeft->Clear();
	m_choiceModuleLeft->Append(LIB_NONE);
	if (list.Count() > 0)
		m_choiceModuleLeft->Append(LIB_ALL);
	#if !defined NO_CURL
		m_choiceModuleLeft->Append(LIB_REPOS);
	#endif
	if (list.Count() > 0)
		m_choiceModuleLeft->Append(list);
	m_choiceModuleLeft->SetSelection(0);
	m_choiceModuleRight->Clear();
	m_choiceModuleRight->Append(LIB_NONE);
	if (list.Count() > 0)
		m_choiceModuleRight->Append(LIB_ALL);
	#if !defined NO_CURL
		m_choiceModuleRight->Append(LIB_REPOS);
	#endif
	if (list.Count() > 0)
		m_choiceModuleRight->Append(list);
	m_choiceModuleRight->SetSelection(0);

	if (eraselists) {
		m_listModulesLeft->DeleteAllItems();
		m_listModulesRight->DeleteAllItems();
		PartData[0].Clear();
		PartData[1].Clear();
	}

	#if defined _MSC_VER
		_CrtCheckMemory();
	#endif
}

void libmngrFrame::WarnNoRepository(wxChoice* choice)
{
	#if defined _MSC_VER
		_CrtCheckMemory();
	#endif

	int idx = choice->GetCurrentSelection();
	if (idx < 0 || idx >= (int)choice->GetCount())
		return;
	wxString path = choice->GetString(idx);
	wxASSERT(path.length() > 0);
	if (path.CmpNoCase(LIB_REPOS) != 0)
		return;
	/* so the repository is selected, check whether it was defined */
	#if defined NO_CURL
		wxMessageBox(wxT("The repository functions are disabled in this version."));
		choice->SetSelection(0);
	#else
		wxConfig *config = new wxConfig(APP_NAME);
		path = config->Read(wxT("repository/url"));
		delete config;
		if (path.Length() == 0) {
			wxMessageBox(wxT("No repository is configured.\nPlease configure a repository before using it."));
			choice->SetSelection(0);
		}
	#endif
}

void libmngrFrame::HandleLibrarieSelect(wxChoice* choice, wxListCtrl* list)
{
	#if defined _MSC_VER
		_CrtCheckMemory();
	#endif

	int idx = choice->GetCurrentSelection();
	if (idx < 0 || idx >= (int)choice->GetCount())
		return;

	list->DeleteAllItems();
	RemoveSelection(m_listModulesLeft);
	RemoveSelection(m_listModulesRight);
	EnableButtons(0);
	wxString filename = choice->GetString(idx);
	wxASSERT(filename.length() > 0);
	if (filename.CmpNoCase(LIB_NONE) == 0)
		return;

	if (filename.CmpNoCase(LIB_ALL) == 0) {
		for (idx = 0; idx < (int)choice->GetCount(); idx++) {
			wxString filename = choice->GetString(idx);
			wxASSERT(filename.length() > 0);
			if (filename.CmpNoCase(LIB_NONE) == 0 || filename.CmpNoCase(LIB_ALL) == 0 || filename.CmpNoCase(LIB_REPOS) == 0)
				continue;
			if (SymbolMode)
				CollectSymbols(filename, list);
			else
				CollectFootprints(filename, list);
		}
	} else if (filename.CmpNoCase(LIB_REPOS) == 0) {
		#if defined NO_CURL
			wxMessageBox(wxT("The repository functions are disabled in this version."));
		#else
			wxArrayString data;
			wxString msg = curlList(&data, SymbolMode ? wxT("symbols") : wxT("footprints"));
			if (msg.length() == 0) {
				for (unsigned idx = 0; idx < data.Count(); idx++) {
					wxString line = data[idx];
					int pos = line.Find(wxT('\t'));
					if (pos <= 0)
						continue;
					long item = list->InsertItem(list->GetItemCount(), line.Left(pos));
					list->SetItem(item, 1, line.Mid(pos + 1));
					list->SetItem(item, 2, wxEmptyString);	/* no full path for the repository */
				}
			} else {
				wxMessageBox(wxT("Error listing repository\n\nRepository message: ") + msg);
			}
		#endif
	} else {
		if (SymbolMode)
			CollectSymbols(filename, list);
		else
			CollectFootprints(filename, list);
	}

	list->SetColumnWidth(0, wxLIST_AUTOSIZE);
	list->SetColumnWidth(1, wxLIST_AUTOSIZE);
	list->SetColumnWidth(2, 0);	/* add an invisible column for the full path to the library */

	wxString msg;
	if (SymbolMode)
		msg = wxString::Format(wxT("Loaded %d symbols"), list->GetItemCount());
	else
		msg = wxString::Format(wxT("Loaded %d footprints"), list->GetItemCount());
	m_statusBar->SetStatusText(msg);
}

void libmngrFrame::CollectSymbols(const wxString &path, wxListCtrl* list)
{
	wxString libname;
	if (ShowFullPaths) {
		libname = path;
	} else {
		wxFileName fname(path);
		libname = fname.GetFullName();
	}

	wxTextFile file;
	if (!file.Open(path)) {
		wxMessageBox(wxT("Failed to open symbol library ") + path);
		return;
	}

	/* verify the header */
	wxString line = file.GetLine(0);
	if (line.Left(16).CmpNoCase(wxT("EESchema-LIBRARY")) != 0
			&& line.Left(13).CmpNoCase(wxT("EESchema-LIB ")) != 0)
	{
		wxMessageBox(wxT("Symbol library ") + path + wxT(" has an unsupported format."));
		file.Close();
		return;
	}
	m_statusBar->SetStatusText(wxT("Scanning ") + path);

	/* collect the symbol definitions in the file */
	for (unsigned idx = 1; idx < file.GetLineCount(); idx++) {
		line = file.GetLine(idx);
		if (line[0] == wxT('D') && line.Left(4).Cmp(wxT("DEF ")) == 0) {
			line = line.Mid(4);
			wxString name = GetToken(&line);
			/* remove leading tilde, this is an "non-visible" flag */
			if (name[0] == wxT('~'))
				name = name.Mid(1);
			long item = list->InsertItem(list->GetItemCount(), name);
			list->SetItem(item, 1, libname);
			list->SetItem(item, 2, path);
		}
	}

	file.Close();
}

void libmngrFrame::CollectFootprints(const wxString &path, wxListCtrl* list)
{
	wxString libname;
	if (ShowFullPaths) {
		libname = path;
	} else {
		wxFileName fname(path);
		libname = fname.GetFullName();
	}

	if (wxFileName::DirExists(path)) {
		m_statusBar->SetStatusText(wxT("Scanning ") + path);
		wxDir dir(path);
		if (!dir.IsOpened())
			return;	/* error message already given */
		wxArrayString modlist;
		dir.GetAllFiles(path, &modlist, wxT("*.kicad_mod"), wxDIR_FILES);
		for (unsigned idx = 0; idx < modlist.Count(); idx++) {
			wxFileName modfile(modlist[idx]);
			long item = list->InsertItem(list->GetItemCount(), modfile.GetName());
			list->SetItem(item, 1, libname);
			list->SetItem(item, 2, path);
		}
	} else {
		wxTextFile file;
		if (!file.Open(path)) {
			wxMessageBox(wxT("Failed to open footprint library ") + path);
			return;
		}

		/* verify the header */
		wxString line = file.GetLine(0);
		line = line.Left(19);
		if (line.CmpNoCase(wxT("PCBNEW-LibModule-V1")) != 0) {
			wxMessageBox(wxT("Footprint library ") + path + wxT(" has an unsupported format."));
			file.Close();
			return;
		}
		m_statusBar->SetStatusText(wxT("Scanning ") + path);

		/* search the index */
		unsigned idx = 1;
		while (idx < file.GetLineCount()) {
			line = file.GetLine(idx);
			idx++;
			if (line.CmpNoCase(wxT("$INDEX")) == 0)
				break;
		}
		/* read the index */
		while (idx < file.GetLineCount()) {
			line = file.GetLine(idx);
			if (line.CmpNoCase(wxT("$EndINDEX")) == 0)
				break;
			long item = list->InsertItem(list->GetItemCount(), line);
			list->SetItem(item, 1, libname);
			list->SetItem(item, 2, path);
			idx++;
		}

		file.Close();
	}
}

void libmngrFrame::LoadPart(int index, wxListCtrl* list, wxChoice* choice, int fp)
{
	m_statusBar->SetStatusText(wxEmptyString);
	PartData[fp].Clear();

	/* get the name of the symbol and the library it is in */
	wxString symbol = list->GetItemText(index);
	wxASSERT(symbol.length() > 0);
	wxString author = wxEmptyString;
	wxString filename = wxEmptyString;
	/* also check the library selection, in the case of the repository,
	   the user name is in column 1 (instead of the filename) */
	if (choice) {
		long libidx = choice->GetSelection();
		wxASSERT(libidx >= 0 && libidx < (long)choice->GetCount());
		filename = choice->GetString(libidx);
	}
	if (filename.CmpNoCase(LIB_REPOS) == 0) {
		FromRepository[fp] = true;
		wxListItem info;
		info.SetId(index);
		info.SetColumn(1);
		info.SetMask(wxLIST_MASK_TEXT);
		list->GetItem(info);
		author = info.GetText();
	} else {
		FromRepository[fp] = false;
		wxListItem info;
		info.SetId(index);
		info.SetColumn(2);
		info.SetMask(wxLIST_MASK_TEXT);
		list->GetItem(info);
		filename = info.GetText();
	}

	int version = VER_INVALID;
	if (SymbolMode) {
		if (!LoadSymbol(filename, symbol, author, DontRebuildTemplate, &PartData[fp])) {
			wxTextFile file;
			if (!file.Open(filename))
				wxMessageBox(wxT("Failed to open library ") + filename);
			else
				wxMessageBox(wxT("Symbol ") + symbol + wxT(" is not present."));
		}
	} else {
		if (!LoadFootprint(filename, symbol, author, DontRebuildTemplate, &PartData[fp], &version)) {
			/* check what the error is */
			wxTextFile file;
			if (!file.Open(filename))
				wxMessageBox(wxT("Failed to open library ") + filename);
			else if (version == VER_INVALID)
				wxMessageBox(wxT("Library ") + filename + wxT(" has an unsupported format."));
			else
				wxMessageBox(wxT("Footprint ") + symbol + wxT(" is not present."));
			return;
		}
	}

	/* verify that the symbol/footprint was actually found */
	wxASSERT(PartData[fp].Count() > 0);

	Footprint[fp].Clear(version);
	BodySize[fp].Clear();
	LabelData[fp].Clear();
	if (PinData[fp] != NULL) {
		delete[] PinData[fp];
		PinData[fp] = 0;
		PinDataCount[fp] = 0;
	}
	if (SymbolMode) {
		/* extract pin names and order */
		GetPinNames(PartData[fp], NULL, &PinDataCount[fp]);
		if (PinDataCount[fp] > 0) {
			PinData[fp] = new PinInfo[PinDataCount[fp]];
			wxASSERT(PinData[fp] != NULL);
			GetPinNames(PartData[fp], PinData[fp], NULL);
		}
	} else {
		/* get the pin pitch and pad size, plus the body size */
		Footprint[fp].Type = version;
		TranslatePadInfo(&PartData[fp], &Footprint[fp]);
	}
	GetBodySize(PartData[fp], &BodySize[fp], SymbolMode, Footprint[fp].Type >= VER_MM);
	GetTextLabelSize(PartData[fp], &LabelData[fp], SymbolMode, Footprint[fp].Type >= VER_MM);
	FieldEdited = false;
	PartEdited = false;
	PinNamesEdited = false;
}


#define PROTECTED	wxColour(224,224,224)
#define ENABLED		*wxWHITE
#define CHANGED		wxColour(255,192,192)

void libmngrFrame::OnTextFieldChange( wxCommandEvent& event )
{
	wxTextCtrl* field = (wxTextCtrl*)event.GetEventObject();
	if (field->IsEditable())
		FieldEdited = true;
	event.Skip();
}

void libmngrFrame::OnChoiceFieldChange( wxCommandEvent& event )
{
	FieldEdited = true;
	ChangePadInfo((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::OnEnterTextInfo( wxCommandEvent& event )
{
	ChangeTextInfo((wxControl*)event.GetEventObject());
}

void libmngrFrame::OnKillFocusTextInfo( wxFocusEvent& event )
{
	ChangeTextInfo((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::ChangeTextInfo(wxControl* ctrl)
{
	if (FieldEdited) {
		ctrl->SetBackgroundColour(CHANGED);
		ctrl->Refresh();
		FieldEdited = false;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		/* adjust the footprint or symbol */
		wxASSERT(PartData[0].Count() > 0);
		wxString field = m_txtDescription->GetValue();
		field = field.Trim(true);
		field = field.Trim(false);
		SetDescription(PartData[0], field, SymbolMode);

		if (SymbolMode) {
			field = m_txtAlias->GetValue();
			field = field.Trim(true);
			field = field.Trim(false);
			SetAliases(PartData[0], field);

			field = m_txtFootprintFilter->GetValue();
			field = field.Trim(true);
			field = field.Trim(false);
			SetFootprints(PartData[0], field);
		}
	}
}

static const wxString PinTypeNames[] = { wxT("input"), wxT("output"), wxT("bi-dir"),
																			wxT("tristate"), wxT("passive"), wxT("open-collector"),
																			wxT("open-emitter"), wxT("non-connect"), wxT("unspecified"),
																			wxT("power-in"), wxT("power-out") };
static const wxString PinShapeNames[] = { wxT("line"), wxT("inverted"), wxT("clock"), wxT("inv.clock"), wxT("*") };
static const wxString PinSectionNames[] = { wxT("left"), wxT("right"), wxT("top"), wxT("bottom") };

void libmngrFrame::OnPinNameChanged( wxGridEvent& event )
{
	static bool reenter = false;
	if (!reenter) {
		reenter = true;
		m_gridPinNames->SetCellBackgroundColour(event.GetRow(), event.GetCol(), CHANGED);
		m_gridPinNames->AutoSizeColumn(event.GetCol());
		PinNamesEdited = true;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		bool refreshtemplate = false;

		wxASSERT(m_gridPinNames->GetNumberRows() == PinDataCount[0]);	/* pins cannot be added or removed in the table */
		for (int idx = 0; idx < m_gridPinNames->GetNumberRows(); idx++) {
			PinData[0][idx].number = m_gridPinNames->GetCellValue(idx, 0);
			PinData[0][idx].name = m_gridPinNames->GetCellValue(idx, 1);

			wxString field = m_gridPinNames->GetCellValue(idx, 2);
			for (int t = 0; t < sizearray(PinTypeNames); t++)
				if (field.CmpNoCase(PinTypeNames[t]) == 0)
					PinData[0][idx].type = t;

			field = m_gridPinNames->GetCellValue(idx, 3);
			for (int s = 0; s < sizearray(PinShapeNames); s++)
				if (field.CmpNoCase(PinShapeNames[s]) == 0)
					PinData[0][idx].shape = s;

			field = m_gridPinNames->GetCellValue(idx, 4);
			for (int s = 0; s < sizearray(PinSectionNames); s++) {
				if (field.CmpNoCase(PinSectionNames[s]) == 0) {
					if (PinData[0][idx].section != s) {
						refreshtemplate = true;
						PinData[0][idx].section = s;
					}
				}
			}
		}

		SetPinNames(PartData[0], PinData[0], PinDataCount[0]);
		if (refreshtemplate)
			RebuildTemplate();
		m_panelView->Refresh();

		reenter = false;
	}
	event.Skip();
}

void libmngrFrame::OnPinNameRearrange( wxKeyEvent& event )
{
	if (event.m_altDown && (event.m_keyCode == WXK_DOWN || event.m_keyCode == WXK_UP)
		&& !CompareMode && !DontRebuildTemplate && PinDataCount[0] > 0)
	{
		int row = m_gridPinNames->GetCursorRow();
		int total = m_gridPinNames->GetNumberRows();
		if ((event.m_keyCode == WXK_DOWN && row < total - 1) || (event.m_keyCode == WXK_UP && row > 0)) {
			wxASSERT(PartData[0].Count() > 0);
			wxString templatename = GetTemplateName(PartData[0]);
			if (templatename.length() > 0) {
				/* swap the rows, except the sequence numbers */
				int alt = (event.m_keyCode == WXK_DOWN) ? row + 1 : row - 1;
				wxASSERT(alt >= 0 && alt < total);
				wxASSERT(row < PinDataCount[0] && alt < PinDataCount[0]);
				int seq_row = PinData[0][row].seq;
				int seq_alt = PinData[0][alt].seq;
				PinInfo t = PinData[0][row];
				PinData[0][row] = PinData[0][alt];
				PinData[0][alt] = t;
				PinData[0][row].seq = seq_row;
				PinData[0][alt].seq = seq_alt;
				/* rebuild the template */
				RebuildTemplate();
				/* refresh the data in the grid */
				m_gridPinNames->SetCellValue(row, 0, PinData[0][row].number);
				m_gridPinNames->SetCellValue(row, 1, PinData[0][row].name);
				m_gridPinNames->SetCellValue(row, 2, PinTypeNames[PinData[0][row].type]);
				m_gridPinNames->SetCellValue(row, 3, PinShapeNames[PinData[0][row].shape]);
				m_gridPinNames->SetCellValue(row, 4, PinSectionNames[PinData[0][row].section]);
				m_gridPinNames->SetCellValue(alt, 0, PinData[0][alt].number);
				m_gridPinNames->SetCellValue(alt, 1, PinData[0][alt].name);
				m_gridPinNames->SetCellValue(alt, 2, PinTypeNames[PinData[0][alt].type]);
				m_gridPinNames->SetCellValue(alt, 3, PinShapeNames[PinData[0][alt].shape]);
				m_gridPinNames->SetCellValue(alt, 4, PinSectionNames[PinData[0][alt].section]);

				PartEdited = true;
				m_btnSavePart->Enable(true);
				m_btnRevertPart->Enable(true);
				m_panelView->Refresh();
			}
		}
	}
	event.Skip();
}

void libmngrFrame::OnEnterPadCount( wxCommandEvent& event )
{
	ChangePadCount((wxControl*)event.GetEventObject());
}

void libmngrFrame::OnKillFocusPadCount( wxFocusEvent& event )
{
	ChangePadCount((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::ChangePadCount(wxControl* ctrl)
{
	if (FieldEdited) {
		ctrl->SetBackgroundColour(CHANGED);
		ctrl->Refresh();
		FieldEdited = false;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		/* check whether the pin count is valid */
		wxString field = m_txtPadCount->GetValue();
		long pins;
		field.ToLong(&pins);
		if (pins < 0)
			return;

		wxASSERT(PartData[0].Count() > 0);
		wxString templatename = GetTemplateName(PartData[0]);
		wxASSERT(templatename.length() > 0);	/* if there is no template, the field for
										   the number of pins is read-only */
		if (templatename.length() == 0)
			return;
		field = GetTemplateHeaderField(templatename, wxT("pins"), SymbolMode);
		wxASSERT(SymbolMode || field.length() > 0);	/* if this header is absent, the field for
										   the number of pins is read-only */
		long cur = 0, prev = 0;
		int count = 0;
		wxString item;
		if (field.length() == 0) {
			wxASSERT(SymbolMode);
			cur = pins;	/* accept all pin counts */
		} else {
			while (field.length() > 0) {
				item = GetToken(&field);
				if (item.Cmp(wxT("...")) == 0) {
					if (count < 2)
						break;	/* this means that the template definition is incorrect */
					long dif = cur - prev;
					if (dif <= 0)
						break;	/* again, template definition is incorrect */
					if (cur % dif == pins % dif)
						cur = pins;
				} else {
					prev = cur;
					item.ToLong(&cur);
				}
				if (cur >= pins)
					break;
				count++;
			}
		}
		if (cur != pins) {
			wxString msg = wxString::Format(wxT("The template %s does not allow a pin count of %d."), templatename.c_str(), pins);
			/* get the notes for the template and add these to the error message */
			wxString note = GetTemplateHeaderField(templatename, wxT("pins"), SymbolMode);
			if (note.length() > 0)
				msg += wxT("\n\nNote: pin specification = ") + note;
			wxMessageBox(msg);
			return;
		}

		/* first, set all variables to the defaults of the template */
		wxString leftmod = GetSelection(m_listModulesLeft);
		wxString rightmod = GetSelection(m_listModulesRight);
		wxASSERT(leftmod.IsEmpty() || rightmod.IsEmpty());
		wxString footprintname = (leftmod.length() > 0) ? leftmod : rightmod;
		wxString description = m_txtDescription->GetValue();
		wxString prefix = wxEmptyString;
		if (SymbolMode)
			prefix = GetPrefix(PartData[0]);
		RPNexpression rpn;
		if (SymbolMode)
			rpn.SetVariable(RPNvariable("PT", pins));	/* other defaults depend on the correct pin count */
		SetVarDefaults(&rpn, templatename, footprintname, description, prefix, true);
		/* then, update the variables from the fields */
		SetVarsFromFields(&rpn, SymbolMode);

		/* refresh the footprint or symbol from the template (completely) */
		wxArrayString templatemodule;
		LoadTemplate(templatename, &templatemodule, SymbolMode);
		if (SymbolMode) {
			if (pins == 0)
				return;
			/* first rebuild the pininfo structure */
			PinInfo* info = new PinInfo[pins];
			if (info == NULL)
				return;
			int totals[PinInfo::Sections];
			int counts[PinInfo::Sections];
			for (int s = 0; s < PinInfo::Sections; s++) {
				totals[s] = 0;
				counts[s] = 0;
				char str[20];
				sprintf(str, "PT:%d", s);
				rpn.Set(str);
				if (rpn.Parse() == RPN_OK) {
					RPNvalue v = rpn.Value();
					totals[s] = (int)(v.Double() + 0.001);
				}
			}
			/* copy the existing pins (but only if they have a label) */
			for (int i = 0; i < pins; i++) {
				info[i].section = -1;
				if (i < PinDataCount[0]
					&& (PinData[0][i].name.Cmp(wxT("~")) != 0
						|| PinData[0][i].type != PinInfo::Passive
						|| PinData[0][i].shape != PinInfo::Line))
				{
					info[i] = PinData[0][i];
					int s = info[i].section;
					counts[s] += 1;
				}
			}
			/* complete with the new pins */
			int cursec = PinInfo::Left;
			for (int i = 0; i < pins; i++) {
				while (cursec < PinInfo::Sections && counts[cursec] >= totals[cursec])
					cursec++;
				if (cursec >= PinInfo::Sections)
					cursec = PinInfo::Left;	/* on error, map remaining pins to left side */
				info[i].seq = i;
				if (info[i].section == -1) {
					info[i].number = wxString::Format(wxT("%d"), i + 1);
					info[i].name = wxT("~");
					info[i].type = PinInfo::Passive;
					info[i].shape = PinInfo::Line;
					info[i].section = cursec;
					counts[cursec] += 1;
				}
			}
			/* create a footprint from the template */
			wxArrayString symbol;
			SymbolFromTemplate(&PartData[0], templatemodule, rpn, info, pins);
			/* now swap the PinInfo in the symbol data structure */
			delete[] PinData[0];
			PinDataCount[0] = pins;
			PinData[0] = info;
			GetPinNames(PartData[0], PinData[0], NULL);	/* reload pins, for updated information */
			/* grid must be refreshed / resized */
			int currows = m_gridPinNames->GetNumberRows();
			if (pins < currows) {
				m_gridPinNames->DeleteRows(pins, currows - pins);
			} else {
				m_gridPinNames->InsertRows(currows, pins - currows);
				m_gridPinNames->EnableEditing(true);
				for (int idx = currows; idx < pins; idx++) {
					wxString field = wxString::Format(wxT("%u"), idx + 1);
					m_gridPinNames->SetCellValue((int)idx, 0, PinData[0][idx].number);
					m_gridPinNames->SetCellValue((int)idx, 1, PinData[0][idx].name);
					m_gridPinNames->SetCellValue((int)idx, 2, PinTypeNames[PinData[0][idx].type]);
					m_gridPinNames->SetCellValue((int)idx, 3, PinShapeNames[PinData[0][idx].shape]);
					m_gridPinNames->SetCellValue((int)idx, 4, PinSectionNames[PinData[0][idx].section]);
					/* no cell needs to be set to read-only and no new cell needs to be coloured */
					m_gridPinNames->SetCellEditor(idx, 2, new wxGridCellChoiceEditor(sizearray(PinTypeNames), PinTypeNames));
					m_gridPinNames->SetCellEditor(idx, 3, new wxGridCellChoiceEditor(sizearray(PinShapeNames) - 1, PinShapeNames));
					m_gridPinNames->SetCellEditor(idx, 4, new wxGridCellChoiceEditor(sizearray(PinSectionNames), PinSectionNames));
				}
			}
			/* existing pins may have been implicitly assigned to a different section;
				 update all rows of the fields */
			for (int idx = 0; idx < pins; idx++)
				m_gridPinNames->SetCellValue((int)idx, 4, PinSectionNames[PinData[0][idx].section]);
		} else {
			FootprintFromTemplate(&PartData[0], templatemodule, rpn, false);
			Footprint[0].Type = VER_MM;	/* should already be set to this */
			TranslatePadInfo(&PartData[0], &Footprint[0]);
		}
		m_panelView->Refresh();
	}
}

void libmngrFrame::OnEnterPadInfo( wxCommandEvent& event )
{
	ChangePadInfo((wxControl*)event.GetEventObject());
}

void libmngrFrame::OnKillFocusPadInfo( wxFocusEvent& event )
{
	ChangePadInfo((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::ChangePadInfo(wxControl* ctrl)
{
	if (FieldEdited) {
		ctrl->SetBackgroundColour(CHANGED);
		ctrl->Refresh();
		FieldEdited = false;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		/* adjust the footprint */
		wxASSERT(PartData[0].Count() > 0);
		FootprintInfo adjusted = Footprint[0];
		long idx;
		wxString field;

		idx = m_choicePadShape->GetSelection();
		if (idx >= 0)
			field = m_choicePadShape->GetString(idx);
		if (field.CmpNoCase(wxT("Round")) == 0)
			adjusted.PadShape = 'C';
		else if (field.CmpNoCase(wxT("Stadium")) == 0)
			adjusted.PadShape = 'O';
		else if (field.CmpNoCase(wxT("Rectangular")) == 0)
			adjusted.PadShape = 'R';
		else if (field.CmpNoCase(wxT("Round + square")) == 0)
			adjusted.PadShape = 'S';
		else if (field.CmpNoCase(wxT("Trapezoid")) == 0)
			adjusted.PadShape = 'T';

		double dim;
		field = m_txtPadWidth->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			adjusted.PadSize[0].Set(dim, adjusted.PadSize[0].GetY());
		field = m_txtPadLength->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			adjusted.PadSize[0].Set(adjusted.PadSize[0].GetX(), dim);
		/* if the pad shape is round or round+square, force the width and height
		   of that pad to be equal */
		if (adjusted.PadShape == 'C' || adjusted.PadShape == 'S') {
			if (adjusted.PadSize[0].GetX() < adjusted.PadSize[0].GetY())
				dim = adjusted.PadSize[0].GetX();
			else
				dim = adjusted.PadSize[0].GetY();
			adjusted.PadSize[0].Set(dim, dim);
		}

		field = m_txtAuxPadWidth->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			adjusted.PadSize[1].Set(dim, adjusted.PadSize[1].GetY());
		field = m_txtAuxPadLength->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			adjusted.PadSize[1].Set(adjusted.PadSize[1].GetX(), dim);

		field = m_txtDrillSize->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			adjusted.DrillSize = dim;

		/* force the footprint to use mm (for convenience) */
		if (TranslateUnits(PartData[0], Footprint[0].Type >= VER_MM, true)) {
			Footprint[0].Type = VER_MM;
			adjusted.Type = VER_MM;	/* footprint is converted to mm as well */
		}
		AdjustPad(PartData[0], &Footprint[0], adjusted);

		/* if this footprint is based on a template and the template has the "STP"
		   variable set (Silk-To-Pad clearance), re-run the template */
		if (CheckTemplateVar(wxT("STP"))) {
			ChangeBodyInfo(NULL);
			TranslatePadInfo(&PartData[0], &Footprint[0]);
		}
		m_panelView->Refresh();
	}
}

void libmngrFrame::OnEnterPitchInfo( wxCommandEvent& event )
{
	ChangePitch((wxControl*)event.GetEventObject());
}

void libmngrFrame::OnKillFocusPitchInfo( wxFocusEvent& event )
{
	ChangePitch((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::ChangePitch(wxControl* ctrl)
{
	if (FieldEdited) {
		ctrl->SetBackgroundColour(CHANGED);
		ctrl->Refresh();
		FieldEdited = false;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		double pitch;
		wxString field = m_txtPitch->GetValue();
		if (field.length() == 0 || !field.ToDouble(&pitch) || pitch <= 0.02)
			return;
		if (Footprint[0].SOT23pitch)
			pitch *= 2;

		/* force the footprint to use mm (for convenience) */
		wxASSERT(PartData[0].Count() > 0);
		if (TranslateUnits(PartData[0], Footprint[0].Type >= VER_MM, true))
			Footprint[0].Type = VER_MM;

		/* check the number of pins in each line */
		wxASSERT(Footprint[0].PadLines > 0);
		int seq = Footprint[0].RegPadCount / Footprint[0].PadLines * Footprint[0].PadSequence;
		if (Footprint[0].SOT23pitch) {
			wxASSERT(Footprint[0].RegPadCount == 3);
			wxASSERT(Footprint[0].PadSequence == 1);
			seq -= 1;
		}
		/* find initial direction */
		CoordPair startpin[4];
		int direction;	/* 0=down, 1=right, 2=up, 3=left */
		if (Footprint[0].PitchVertical)
			direction = (Footprint[0].PitchPins[0].GetY() < Footprint[0].PitchPins[1].GetY()) ? 0 : 2;
		else
			direction = (Footprint[0].PitchPins[0].GetX() < Footprint[0].PitchPins[1].GetX()) ? 1 : 3;
		/* find start pins */
		startpin[direction] = Footprint[0].PitchPins[0];
		int opposing = (direction + 2) % 4;
		if (opposing % 2 == 0 && Footprint[0].SpanHor > EPSILON) {
			if (!Equal(startpin[direction].GetX(), Footprint[0].SpanHorPins[0].GetX(), TOLERANCE))
				startpin[opposing] = Footprint[0].SpanHorPins[0];
			else if (!Equal(startpin[direction].GetX(), Footprint[0].SpanHorPins[1].GetX(), TOLERANCE))
				startpin[opposing] = Footprint[0].SpanHorPins[1];
		}
		if (opposing % 2 == 1 && Footprint[0].SpanVer > EPSILON) {
			if (!Equal(startpin[direction].GetY(), Footprint[0].SpanVerPins[0].GetY(), TOLERANCE))
				startpin[opposing] = Footprint[0].SpanVerPins[0];
			else if (!Equal(startpin[direction].GetY(), Footprint[0].SpanVerPins[1].GetY(), TOLERANCE))
				startpin[opposing] = Footprint[0].SpanVerPins[1];
		}
		if (direction % 2 == 0 && Footprint[0].SpanVer > EPSILON) {
			/* positions 0 and 2 set, now set 1 and 3 */
			if (Footprint[0].SpanVerPins[0].GetY() < Footprint[0].SpanVerPins[1].GetY()) {
				startpin[3] = Footprint[0].SpanVerPins[0];
				startpin[1] = Footprint[0].SpanVerPins[1];
			} else {
				startpin[1] = Footprint[0].SpanVerPins[0];
				startpin[3] = Footprint[0].SpanVerPins[1];
			}
		}
		if (direction % 2 == 1 && Footprint[0].SpanHor > EPSILON) {
			/* positions 1 and 3 set, now set 0 and 2 */
			if (Footprint[0].SpanHorPins[0].GetX() < Footprint[0].SpanHorPins[1].GetX()) {
				startpin[0] = Footprint[0].SpanVerPins[0];
				startpin[2] = Footprint[0].SpanVerPins[1];
			} else {
				startpin[2] = Footprint[0].SpanVerPins[0];
				startpin[0] = Footprint[0].SpanVerPins[1];
			}
		}

		/* now modify the footprint */
		int base = 1;
		for (int line = 1; line <= Footprint[0].PadLines; line++) {
			switch (direction) {
			case 0:
				AdjustPitchVer(PartData[0], base, base + seq - 1, Footprint[0].PadSequence,
											 startpin[direction].GetX(), pitch);
				break;
			case 1:
				AdjustPitchHor(PartData[0], base, base + seq - 1, Footprint[0].PadSequence,
											 startpin[direction].GetY(), pitch);
				break;
			case 2:
				AdjustPitchVer(PartData[0], base, base + seq - 1, Footprint[0].PadSequence,
											 startpin[direction].GetX(), -pitch);
				break;
			case 3:
				AdjustPitchHor(PartData[0], base, base + seq - 1, Footprint[0].PadSequence,
											 startpin[direction].GetY(), -pitch);
				break;
			}
			if (Footprint[0].PadSequence == 1) {
				base += seq;
				if (Footprint[0].PadLines == 2)
					direction = (direction + 2) % 4;
				else
					direction = (direction + 1) % 4;
			} else {
				/* for zig-zag style of dual-row pin headers (or similar) */
				base += 1;
				/* keep the direction, but change the X or Y coordinate */
				int d = (Footprint[0].PadLines == 2) ? (direction + 2) % 4 : (direction + 1) % 4;
				startpin[direction] = startpin[d];
			}
		}

		/* if this footprint is based on a template and the template has the "STP"
		   variable set (Silk-To-Pad clearance), re-run the template */
		if (CheckTemplateVar(wxT("STP")))
			ChangeBodyInfo(NULL);

		TranslatePadInfo(&PartData[0], &Footprint[0]);
		m_panelView->Refresh();
	}
}

void libmngrFrame::OnEnterSpanInfo( wxCommandEvent& event )
{
	ChangeSpan((wxControl*)event.GetEventObject());
}

void libmngrFrame::OnKillFocusSpanInfo( wxFocusEvent& event )
{
	ChangeSpan((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::ChangeSpan(wxControl* ctrl)
{
	if (FieldEdited) {
		ctrl->SetBackgroundColour(CHANGED);
		ctrl->Refresh();
		FieldEdited = false;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		/* force the footprint to use mm (for convenience) */
		wxASSERT(PartData[0].Count() > 0);
		if (TranslateUnits(PartData[0], Footprint[0].Type >= VER_MM, true))
			Footprint[0].Type = VER_MM;

		/* adjust the footprint */
		wxString field;
		double dim, delta;
		CoordPair p1, p2;
		field = m_txtPadSpanX->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02 && !Equal(dim, Footprint[0].SpanHor, 0.01)) {
			if (Footprint[0].SpanHorPins[0].GetX() < Footprint[0].SpanHorPins[1].GetX()) {
				p1 = Footprint[0].SpanHorPins[0];
				p2 = Footprint[0].SpanHorPins[1];
			} else {
				p1 = Footprint[0].SpanHorPins[1];
				p2 = Footprint[0].SpanHorPins[0];
			}
			delta = (dim - Footprint[0].SpanHor) / 2;
			MovePadHor(PartData[0], p1.GetX(), p1.GetX() - delta);
			MovePadHor(PartData[0], p2.GetX(), p2.GetX() + delta);
			Footprint[0].SpanHor = dim;
			Footprint[0].SpanHorPins[0].Set(p1.GetX() - delta, p1.GetY());
			Footprint[0].SpanHorPins[1].Set(p2.GetX() + delta, p2.GetY());
		}
		field = m_txtPadSpanY->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02 && !Equal(dim, Footprint[0].SpanVer, 0.01)) {
			if (Footprint[0].SpanVerPins[0].GetY() < Footprint[0].SpanVerPins[1].GetY()) {
				p1 = Footprint[0].SpanVerPins[0];
				p2 = Footprint[0].SpanVerPins[1];
			} else {
				p1 = Footprint[0].SpanVerPins[1];
				p2 = Footprint[0].SpanVerPins[0];
			}
			delta = (dim - Footprint[0].SpanVer) / 2;
			MovePadVer(PartData[0], p1.GetY(), p1.GetY() - delta);
			MovePadVer(PartData[0], p2.GetY(), p2.GetY() + delta);
			Footprint[0].SpanVer = dim;
			Footprint[0].SpanVerPins[0].Set(p1.GetX(), p1.GetY() - delta);
			Footprint[0].SpanVerPins[1].Set(p2.GetX(), p2.GetY() + delta);
		}

		/* if this footprint is based on a template and the template has the "STP"
			 variable set (Silk-To-Pad clearance), re-run the template */
		if (CheckTemplateVar(wxT("STP")))
			ChangeBodyInfo(NULL);
		/* always re-translate the pad information, because the pitch anchor points
		   must remain correct */
		TranslatePadInfo(&PartData[0], &Footprint[0]);
		m_panelView->Refresh();
	}
}

void libmngrFrame::OnEnterBodyInfo( wxCommandEvent& event )
{
	ChangeBodyInfo((wxControl*)event.GetEventObject());
}

void libmngrFrame::OnKillFocusBodyInfo( wxFocusEvent& event )
{
	ChangeBodyInfo((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::ChangeBodyInfo(wxControl* ctrl)
{
	if (FieldEdited || !ctrl) {
		if (ctrl) {
			ctrl->SetBackgroundColour(CHANGED);
			ctrl->Refresh();
		}
		FieldEdited = false;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		RebuildTemplate();
		m_panelView->Refresh();
	}
}

void libmngrFrame::OnEnterLabelField( wxCommandEvent& event )
{
	ChangeLabelInfo((wxControl*)event.GetEventObject());
}

void libmngrFrame::OnKillFocusLabelField( wxFocusEvent& event )
{
	ChangeLabelInfo((wxControl*)event.GetEventObject());
	event.Skip();
}

void libmngrFrame::OnLabelShowHide( wxCommandEvent& event )
{
	FieldEdited = true;
	ChangeLabelInfo((wxControl*)event.GetEventObject());
}

void libmngrFrame::ChangeLabelInfo(wxControl* ctrl)
{
	if (FieldEdited) {
		ctrl->SetBackgroundColour(CHANGED);
		ctrl->Refresh();
		FieldEdited = false;
		PartEdited = true;
		m_btnSavePart->Enable(true);
		m_btnRevertPart->Enable(true);

		/* adjust the symbol or footprint */
		wxASSERT(PartData[0].Count() > 0);
		LabelInfo adjusted = LabelData[0];

		wxString field;
		double dim;

		field = m_txtRefLabel->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			adjusted.RefLabelSize = dim;
		adjusted.RefLabelVisible = m_chkRefLabelVisible->GetValue();
		field = m_txtValueLabel->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			adjusted.ValueLabelSize = dim;
		adjusted.ValueLabelVisible = m_chkValueLabelVisible->GetValue();

		/* force the footprint to use mm (for convenience) */
		if (TranslateUnits(PartData[0], Footprint[0].Type >= VER_MM, true))
			Footprint[0].Type = VER_MM;
		SetTextLabelSize(PartData[0], adjusted, SymbolMode, true);

		/* if this is a footprint that is based on a template, re-run the
		   template because the text position is often a function of the text
		   size */
		if (!SymbolMode && GetTemplateName(PartData[0]).Length() > 0) {
			ChangeBodyInfo(NULL);
			TranslatePadInfo(&PartData[0], &Footprint[0]);
		}

		m_panelView->Refresh();
	}
}

void libmngrFrame::OnSavePart( wxCommandEvent& /*event*/ )
{
	long idxleft = m_listModulesLeft->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	long idxright = m_listModulesRight->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxASSERT(idxleft == -1 || idxright == -1);
	long index;
	wxListCtrl* list;
	if (idxleft >= 0) {
		list = m_listModulesLeft;
		index = idxleft;
	} else {
		list = m_listModulesRight;
		index = idxright;
	}
	wxASSERT(index >= 0);
	/* get the name of the symbol and the library it is in */
	wxASSERT(!FromRepository[0]);
	wxString symbol = list->GetItemText(index);
	wxListItem info;
	info.SetId(index);
	info.SetColumn(1);
	info.SetMask(wxLIST_MASK_TEXT);
	list->GetItem(info);
	wxString filename = info.GetText();
	if (filename.CmpNoCase(LIB_REPOS) != 0) {
		info.SetColumn(2);
		list->GetItem(info);
		filename = info.GetText();
	}

	wxASSERT(PartData[0].Count() > 0);
	bool result;
	if (SymbolMode) {
		if (PinNamesEdited) {
			/* verify pin numbers */
			wxASSERT(m_gridPinNames->GetNumberRows() == PinDataCount[0]);
			for (int idx = 0; idx < m_gridPinNames->GetNumberRows(); idx++) {
				if (PinData[0][idx].number.length() == 0)
					wxMessageBox(wxT("No pin number for label: ") + PinData[0][idx].name);
				for (int i = 0; i < idx; i++) {
					if (PinData[0][i].number.CmpNoCase(PinData[0][idx].number) == 0)
						wxMessageBox(wxT("Duplicate pin number: ") + PinData[0][idx].number);
				}
			}
		}
		wxASSERT(ExistSymbol(filename, symbol));
		RemoveSymbol(filename, symbol);
		result = InsertSymbol(filename, symbol, PartData[0]);
	} else {
		wxASSERT(ExistFootprint(filename, symbol));
		RemoveFootprint(filename, symbol);
		result = InsertFootprint(filename, symbol, PartData[0], Footprint[0].Type >= VER_MM);
	}
	if (!result) {
		wxMessageBox(wxT("Operation failed."));
		return;
	}

	wxString templatename = GetTemplateName(PartData[0]);
	wxString vrmlpath = GetVRMLPath(filename, PartData[0]);
	if (vrmlpath.Length() > 0 && templatename.length() > 0) {
		/* re-generate 3D model
		   first, set all variables to the defaults of the template */
		wxString description = m_txtDescription->GetValue();
		RPNexpression rpn;
		SetVarDefaults(&rpn, templatename, symbol, description);
		SetVarsFromFields(&rpn, false);
		/* get the template to use */
		wxString modelname = GetTemplateHeaderField(templatename, wxT("model"), SymbolMode);
		if (modelname.length() == 0) {
			modelname = templatename;
		} else {
			int idx = modelname.Find(wxT(' '));
			if (idx >= 0)
				modelname = modelname.Left(idx);
		}
		//??? overrule by selection in the property panel (but there currently isn't a combobox for selecting the 3D model template)
		VRMLFromTemplate(vrmlpath, modelname, rpn);
	}

	LoadPart(index, list, 0, 0);
	UpdateDetails(0);
	m_panelView->Refresh();
	if (SymbolMode)
		m_statusBar->SetStatusText(wxT("Saved modified symbol"));
	else
		m_statusBar->SetStatusText(wxT("Saved modified footprint"));
}

void libmngrFrame::OnRevertPart( wxCommandEvent& /*event*/ )
{
	long idxleft = m_listModulesLeft->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	long idxright = m_listModulesRight->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	wxASSERT(idxleft == -1 || idxright == -1);
	if (idxleft >= 0)
		LoadPart(idxleft, m_listModulesLeft, m_choiceModuleLeft, 0);
	else
		LoadPart(idxright, m_listModulesRight, m_choiceModuleRight, 0);
	UpdateDetails(0);
	m_panelView->Refresh();
	m_statusBar->SetStatusText(wxT("Changes reverted"));
}

bool libmngrFrame::CheckTemplateVar(const wxString& varname)
{
	wxASSERT(PartData[0].Count() > 0);
	wxString templatename = GetTemplateName(PartData[0]);
	if (templatename.length() == 0)
		return false;

	if (varname.IsEmpty())
		return true;

	wxString field = GetTemplateHeaderField(templatename, wxT("param"), SymbolMode);
	if (field.length() == 0)
		return false;
	return (field.Find(wxT("@") + varname) >= 0);
}

bool libmngrFrame::SetVarDefaults(RPNexpression *rpn, const wxString& templatename,
																	const wxString& footprintname, const wxString& description,
																	const wxString& prefix, bool silent)
{
	/* first set the defaults on the parameter line */
	wxASSERT(templatename.length() > 0);
	wxString field = GetTemplateHeaderField(templatename, wxT("param"), SymbolMode);
	if (field.length() > 0) {
		rpn->Set(field.utf8_str());
		RPN_ERROR err = rpn->Parse();
		if (err != RPN_EMPTY && !silent)
			wxMessageBox(wxT("The '#param' line in the template has an error."));
	}

	/* then set the defaults from the user settings (possibly overriding those
		 of the #param line) */
	wxConfig *config = new wxConfig(APP_NAME);
	config->SetPath(wxT("/template"));
	wxString varname;
	long token;
	bool ok = config->GetFirstEntry(varname, token);
	while (ok) {
		double val;
		if (config->Read(varname, &val))
			rpn->SetVariable(RPNvariable(varname.utf8_str(), val));
		ok = config->GetNextEntry(varname, token);
	}
	delete config;

	rpn->SetVariable(RPNvariable("NAME", footprintname.utf8_str()));
	rpn->SetVariable(RPNvariable("DESCR", description.utf8_str()));
	rpn->SetVariable(RPNvariable("REF", prefix.utf8_str()));

	return true;
}

bool libmngrFrame::SetVarsFromFields(RPNexpression *rpn, bool SymbolMode)
{
	long val;
	double dim;

	wxString field = m_txtPadCount->GetValue();
	if (field.length() > 0 && field.ToLong(&val) && val > 0)
		rpn->SetVariable(RPNvariable("PT", val));
	field = m_txtBodyLength->GetValue();
	if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
		rpn->SetVariable(RPNvariable("BL", dim));
	field = m_txtBodyWidth->GetValue();
	if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
		rpn->SetVariable(RPNvariable("BW", dim));
	field = m_txtRefLabel->GetValue();
	if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02) {
		if (!m_chkRefLabelVisible->GetValue())
			dim = -dim;
		rpn->SetVariable(RPNvariable("TSR", dim));
	}
	field = m_txtValueLabel->GetValue();
	if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02) {
		if (!m_chkValueLabelVisible->GetValue())
			dim = -dim;
		rpn->SetVariable(RPNvariable("TSV", dim));
	}

	if (!SymbolMode) {
		field = m_txtPadWidth->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("PW", dim));
		field = m_txtPadLength->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("PL", dim));
		field = m_txtAuxPadLength->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("PLA", dim));
		field = m_txtAuxPadWidth->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("PWA", dim));
		field = m_txtPitch->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("PP", dim));
		field = m_txtPadSpanX->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("SH", dim));
		field = m_txtPadSpanY->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("SV", dim));
		field = m_txtDrillSize->GetValue();
		if (field.length() > 0 && field.ToDouble(&dim) && dim > 0.02)
			rpn->SetVariable(RPNvariable("DS", dim));
	}
	return true;
}

bool libmngrFrame::RebuildTemplate()
{
	wxASSERT(PartData[0].Count() > 0);
	wxString templatename = GetTemplateName(PartData[0]);
	wxASSERT(templatename.length() > 0);	/* if there is no template, the fields for
										 the body size are read-only */
	if (templatename.length() == 0)
		return false;

	/* first, set all variables to the defaults of the template */
	wxString leftmod = GetSelection(m_listModulesLeft);
	wxString rightmod = GetSelection(m_listModulesRight);
	wxASSERT(leftmod.IsEmpty() || rightmod.IsEmpty());
	wxString footprintname = (leftmod.length() > 0) ? leftmod : rightmod;
	wxString description = m_txtDescription->GetValue();
	wxString prefix = wxEmptyString;
	if (SymbolMode)
		prefix = GetPrefix(PartData[0]);
	RPNexpression rpn;
	if (SymbolMode)
		rpn.SetVariable(RPNvariable("PT", PinDataCount[0]));	/* other defaults depend on the correct pin count */
	SetVarDefaults(&rpn, templatename, footprintname, description, prefix, true);

	/* then, update the variables from the fields
		 update all fields, because the body size may depend on other settings,
		 such as pitch, too */
	SetVarsFromFields(&rpn, SymbolMode);

	/* refresh the footprint from the template, but only partially */
	wxArrayString templatemodule;
	LoadTemplate(templatename, &templatemodule, SymbolMode);
	bool result;
	if (SymbolMode) {
		result = SymbolFromTemplate(&PartData[0], templatemodule, rpn, PinData[0], PinDataCount[0]);
	} else {
		result = FootprintFromTemplate(&PartData[0], templatemodule, rpn, true);
		Footprint[0].Type = VER_MM;	/* should already be set to this */
	}
	return result;
}

void libmngrFrame::UpdateDetails(int fp)
{
	/* reset all fields and colours */
	m_txtDescription->SetValue(wxEmptyString);
	m_txtDescription->SetToolTip(wxEmptyString);
	m_txtDescription->SetEditable(false);
	m_txtDescription->SetBackgroundColour(PROTECTED);
	if (SymbolMode) {
		m_txtAlias->SetValue(wxEmptyString);
		m_txtFootprintFilter->SetValue(wxEmptyString);
		m_txtPadCount->SetValue(wxEmptyString);
		m_gridPinNames->ClearGrid();
		m_gridPinNames->SetColLabelSize(0);
		if (m_gridPinNames->GetNumberRows() > 0)
			m_gridPinNames->DeleteRows(0, m_gridPinNames->GetNumberRows());
		wxSizer* sizer = m_gridPinNames->GetContainingSizer();
		m_txtBodyLength->SetValue(wxEmptyString);
		m_txtBodyWidth->SetValue(wxEmptyString);
		m_txtRefLabel->SetValue(wxEmptyString);
		m_chkRefLabelVisible->SetValue(false);
		m_txtValueLabel->SetValue(wxEmptyString);
		m_chkValueLabelVisible->SetValue(false);
		wxASSERT(sizer != 0);
		sizer->Layout();

		m_txtAlias->SetEditable(false);
		m_txtFootprintFilter->SetEditable(false);
		m_txtPadCount->SetEditable(false);
		m_gridPinNames->EnableEditing(false);
		m_txtBodyLength->SetEditable(false);
		m_txtBodyWidth->SetEditable(false);
		m_txtRefLabel->SetEditable(false);
		m_chkRefLabelVisible->Enable(false);
		m_txtValueLabel->SetEditable(false);
		m_chkValueLabelVisible->Enable(false);

		m_txtAlias->SetBackgroundColour(PROTECTED);
		m_txtFootprintFilter->SetBackgroundColour(PROTECTED);
		m_txtPadCount->SetBackgroundColour(PROTECTED);
		m_gridPinNames->SetBackgroundColour(PROTECTED);
		m_txtBodyLength->SetBackgroundColour(PROTECTED);
		m_txtBodyWidth->SetBackgroundColour(PROTECTED);
		m_txtRefLabel->SetBackgroundColour(PROTECTED);
		m_chkRefLabelVisible->SetBackgroundColour(wxNullColour);
		m_txtValueLabel->SetBackgroundColour(PROTECTED);
		m_chkValueLabelVisible->SetBackgroundColour(wxNullColour);
	} else {
		m_txtPadCount->SetValue(wxEmptyString);
		m_choicePadShape->SetSelection(0);
		m_txtPadWidth->SetValue(wxEmptyString);
		m_txtPadLength->SetValue(wxEmptyString);
		m_txtPitch->SetValue(wxEmptyString);
		m_txtPadSpanX->SetValue(wxEmptyString);
		m_txtPadSpanY->SetValue(wxEmptyString);
		m_txtDrillSize->SetValue(wxEmptyString);
		m_txtAuxPadLength->SetValue(wxEmptyString);
		m_txtAuxPadWidth->SetValue(wxEmptyString);
		m_txtBodyLength->SetValue(wxEmptyString);
		m_txtBodyWidth->SetValue(wxEmptyString);
		m_txtRefLabel->SetValue(wxEmptyString);
		m_chkRefLabelVisible->SetValue(false);
		m_txtValueLabel->SetValue(wxEmptyString);
		m_chkValueLabelVisible->SetValue(false);

		m_txtPadCount->SetEditable(false);
		m_choicePadShape->Enable(false);
		m_txtPadWidth->SetEditable(false);
		m_txtPadLength->SetEditable(false);
		m_txtPitch->SetEditable(false);
		m_txtPadSpanX->SetEditable(false);
		m_txtPadSpanY->SetEditable(false);
		m_txtDrillSize->SetEditable(false);
		m_txtAuxPadLength->SetEditable(false);
		m_txtAuxPadWidth->SetEditable(false);
		m_txtBodyLength->SetEditable(false);
		m_txtBodyWidth->SetEditable(false);
		m_txtRefLabel->SetEditable(false);
		m_chkRefLabelVisible->Enable(false);
		m_txtValueLabel->SetEditable(false);
		m_chkValueLabelVisible->Enable(false);

		m_txtPadCount->SetBackgroundColour(PROTECTED);
		m_choicePadShape->SetBackgroundColour(wxNullColour);
		m_txtPadWidth->SetBackgroundColour(PROTECTED);
		m_txtPadLength->SetBackgroundColour(PROTECTED);
		m_txtPitch->SetBackgroundColour(PROTECTED);
		m_txtPadSpanX->SetBackgroundColour(PROTECTED);
		m_txtPadSpanY->SetBackgroundColour(PROTECTED);
		m_txtDrillSize->SetBackgroundColour(PROTECTED);
		m_txtAuxPadLength->SetBackgroundColour(PROTECTED);
		m_txtAuxPadWidth->SetBackgroundColour(PROTECTED);
		m_txtBodyLength->SetBackgroundColour(PROTECTED);
		m_txtBodyWidth->SetBackgroundColour(PROTECTED);
		m_txtRefLabel->SetBackgroundColour(PROTECTED);
		m_chkRefLabelVisible->SetBackgroundColour(wxNullColour);
		m_txtValueLabel->SetBackgroundColour(PROTECTED);
		m_chkValueLabelVisible->SetBackgroundColour(wxNullColour);
	}

	m_btnSavePart->Enable(false);
	m_btnRevertPart->Enable(false);

	if (PartData[fp].Count() == 0)
		return;
	bool DefEnable = !CompareMode && !FromRepository[fp];
	wxString templatename = GetTemplateName(PartData[fp]);

	wxString field = GetDescription(PartData[fp], SymbolMode);
	m_txtDescription->SetValue(field);
	m_txtDescription->SetToolTip(field);
	m_txtDescription->SetEditable(DefEnable);
	m_txtDescription->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);

	if (SymbolMode) {
		/* schematic mode */

		field = GetAliases(PartData[fp]);
		m_txtAlias->SetValue(field);
		m_txtAlias->SetEditable(DefEnable);
		m_txtAlias->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);

		field = GetFootprints(PartData[fp]);
		m_txtFootprintFilter->SetValue(field);
		m_txtFootprintFilter->SetEditable(DefEnable);
		m_txtFootprintFilter->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);

		m_txtPadCount->SetValue(wxString::Format(wxT("%d"), PinDataCount[fp]));
		bool enable = templatename.length() > 0 && DefEnable;
		if (enable) {
			/* check whether the template allows multiple pin counts (many 2-pin
				 components only allow 2 pins) */
			field = GetTemplateHeaderField(templatename, wxT("pins"), SymbolMode);
			if (field.length() > 0 && field.Find(wxT(' ')) < 0)
				enable = false;	/* since leading and trailing white-space was already
									 trimmed, when more white-space exists, it must be as
									 a separator */
		}
		m_txtPadCount->SetEditable(enable);
		m_txtPadCount->SetBackgroundColour(enable ? ENABLED : PROTECTED);

		if (m_gridPinNames->GetNumberRows() > 0)
			m_gridPinNames->DeleteRows(0, m_gridPinNames->GetNumberRows());
		m_gridPinNames->InsertRows(0, PinDataCount[fp]);
		m_gridPinNames->EnableEditing(true);
		for (int idx = 0; idx < PinDataCount[fp]; idx++) {
			wxString field = wxString::Format(wxT("%u"), idx + 1);
			m_gridPinNames->SetCellValue((int)idx, 0, PinData[fp][idx].number);
			m_gridPinNames->SetCellValue((int)idx, 1, PinData[fp][idx].name);
			m_gridPinNames->SetCellValue((int)idx, 2, PinTypeNames[PinData[fp][idx].type]);
			m_gridPinNames->SetCellValue((int)idx, 3, PinShapeNames[PinData[fp][idx].shape]);
			m_gridPinNames->SetCellValue((int)idx, 4, PinSectionNames[PinData[fp][idx].section]);
			bool enable = templatename.length() > 0 && DefEnable;
			m_gridPinNames->SetReadOnly(idx, 0, !DefEnable);
			m_gridPinNames->SetReadOnly(idx, 1, !DefEnable);
			m_gridPinNames->SetReadOnly(idx, 2, !DefEnable);
			m_gridPinNames->SetReadOnly(idx, 3, !DefEnable);
			m_gridPinNames->SetReadOnly(idx, 4, !enable);
			m_gridPinNames->SetCellBackgroundColour(idx, 0, DefEnable ? ENABLED : PROTECTED);
			m_gridPinNames->SetCellBackgroundColour(idx, 1, DefEnable ? ENABLED : PROTECTED);
			m_gridPinNames->SetCellBackgroundColour(idx, 2, DefEnable ? ENABLED : PROTECTED);
			m_gridPinNames->SetCellBackgroundColour(idx, 3, DefEnable ? ENABLED : PROTECTED);
			m_gridPinNames->SetCellBackgroundColour(idx, 4, enable ? ENABLED : PROTECTED);
			if (DefEnable) {
				m_gridPinNames->SetCellEditor(idx, 2, new wxGridCellChoiceEditor(sizearray(PinTypeNames), PinTypeNames));
				m_gridPinNames->SetCellEditor(idx, 3, new wxGridCellChoiceEditor(sizearray(PinShapeNames) - 1, PinShapeNames));
			}
			if (enable)
				m_gridPinNames->SetCellEditor(idx, 4, new wxGridCellChoiceEditor(sizearray(PinSectionNames), PinSectionNames));
		}
		if (PinDataCount[fp] > 0) {
			#if defined _WIN32
				m_gridPinNames->SetColLabelSize(16);	//??? should read this from a user-setting
			#else
				m_gridPinNames->SetColLabelSize(20);
			#endif
		}
		m_gridPinNames->AutoSizeColumn(0);
		m_gridPinNames->AutoSizeColumn(1);
		m_gridPinNames->AutoSizeColumn(2);
		m_gridPinNames->AutoSizeColumn(3);
		m_gridPinNames->AutoSizeColumn(4);
		wxSizer* sizer = m_gridPinNames->GetContainingSizer();
		wxASSERT(sizer != 0);
		sizer->Layout();
		m_panelSettings->FitInside();	/* force recalculation of the panel (calling layout is not enough) */

	} else {
		/* footprint mode */

		m_txtPadCount->SetValue(wxString::Format(wxT("%d"), Footprint[fp].PadCount));
		bool enable = templatename.length() > 0 && DefEnable;
		if (enable) {
			/* check whether the template allows multiple pin counts (many 2-pin
				 components only allow 2 pins) */
			field = GetTemplateHeaderField(templatename, wxT("pins"), SymbolMode);
			if (field.length() == 0 || field.Find(wxT(' ')) < 0)
				enable = false;	/* since leading and trailing white-space was already
									 trimmed, when more white-space exists, it must be as
									 a separator */
		}
		m_txtPadCount->SetEditable(enable);
		m_txtPadCount->SetBackgroundColour(enable ? ENABLED : PROTECTED);

		enable = DefEnable;
		int idx;
		switch (Footprint[fp].PadShape) {
		case 'C':
			idx = m_choicePadShape->FindString(wxT("Round"));
			break;
		case 'O':
			idx = m_choicePadShape->FindString(wxT("Stadium"));
			break;
		case 'R':
			idx = m_choicePadShape->FindString(wxT("Rectangular"));
			break;
		case 'S':
			idx = m_choicePadShape->FindString(wxT("Round + square"));
			break;
		case 'T':
			idx = m_choicePadShape->FindString(wxT("Trapezoid"));
			break;
		default:
			idx = m_choicePadShape->FindString(wxT("(varies)"));
			enable = false;
		}
		wxASSERT(idx >= 0);
		m_choicePadShape->SetSelection(idx);
		m_choicePadShape->Enable(enable);
		m_choicePadShape->SetBackgroundColour(enable ? ENABLED : PROTECTED);

		const CoordPair& padsize = Footprint[fp].PadSize[0];
		if (padsize.GetX() > EPSILON) {
			m_txtPadWidth->SetValue(wxString::Format(wxT("%.3f"), padsize.GetX()));
			m_txtPadWidth->SetEditable(DefEnable);
			m_txtPadWidth->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
		}
		if (padsize.GetY() > EPSILON) {
			m_txtPadLength->SetValue(wxString::Format(wxT("%.3f"), padsize.GetY()));
			m_txtPadLength->SetEditable(DefEnable);
			m_txtPadLength->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
		}

		const CoordPair& auxpadsize = Footprint[fp].PadSize[1];
		if (auxpadsize.GetX() > EPSILON) {
			m_txtAuxPadWidth->SetValue(wxString::Format(wxT("%.3f"), auxpadsize.GetX()));
			m_txtAuxPadWidth->SetEditable(DefEnable);
			m_txtAuxPadWidth->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
		}
		if (auxpadsize.GetY() > EPSILON) {
			m_txtAuxPadLength->SetValue(wxString::Format(wxT("%.3f"), auxpadsize.GetY()));
			m_txtAuxPadLength->SetEditable(DefEnable);
			m_txtAuxPadLength->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
		}

		if (Footprint[fp].Pitch > EPSILON) {
			m_txtPitch->SetValue(wxString::Format(wxT("%.3f"), Footprint[fp].Pitch));
			enable = DefEnable && Footprint[fp].RegPadCount > 0 && Footprint[fp].PadLines > 0 && Footprint[fp].OriginCentred;
			m_txtPitch->SetEditable(enable);
			m_txtPitch->SetBackgroundColour(enable ? ENABLED : PROTECTED);
		}

		if (Footprint[fp].SpanHor > EPSILON) {
			m_txtPadSpanX->SetValue(wxString::Format(wxT("%.3f"), Footprint[fp].SpanHor));
			m_txtPadSpanX->SetEditable(DefEnable);
			m_txtPadSpanX->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
		}
		if (Footprint[fp].SpanVer > EPSILON) {
			m_txtPadSpanY->SetValue(wxString::Format(wxT("%.3f"), Footprint[fp].SpanVer));
			m_txtPadSpanY->SetEditable(DefEnable);
			m_txtPadSpanY->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
		}

		if (Footprint[fp].DrillSize > EPSILON) {
			m_txtDrillSize->SetValue(wxString::Format(wxT("%.3f"), Footprint[fp].DrillSize));
			m_txtDrillSize->SetEditable(DefEnable);
			m_txtDrillSize->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
		}
	}

	/* check whether a body size is specified as a default parameter; if not, keep
		 the body size disabled */
	bool sizeset = false;
	if (templatename.length() > 0) {
		field = GetTemplateHeaderField(templatename, wxT("param"), SymbolMode);
		sizeset = (field.Find(wxT("@BL")) >= 0 && field.Find(wxT("@BW")) >= 0);
	}
	if (BodySize[fp].BodyLength > EPSILON) {
		m_txtBodyLength->SetValue(wxString::Format(wxT("%.3f"), BodySize[fp].BodyLength));
		bool enable = sizeset && DefEnable;
		m_txtBodyLength->SetEditable(enable);
		m_txtBodyLength->SetBackgroundColour(enable ? ENABLED : PROTECTED);
	}
	if (BodySize[fp].BodyWidth > EPSILON) {
		m_txtBodyWidth->SetValue(wxString::Format(wxT("%.3f"), BodySize[fp].BodyWidth));
		bool enable = sizeset && DefEnable;
		m_txtBodyWidth->SetEditable(enable);
		m_txtBodyWidth->SetBackgroundColour(enable ? ENABLED : PROTECTED);
	}

	m_txtRefLabel->SetValue(wxString::Format(wxT("%.2f"), LabelData[fp].RefLabelSize));
	m_txtRefLabel->SetEditable(DefEnable);
	m_txtRefLabel->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
	m_chkRefLabelVisible->SetValue(LabelData[fp].RefLabelVisible);
	m_chkRefLabelVisible->Enable(DefEnable);

	m_txtValueLabel->SetValue(wxString::Format(wxT("%.2f"), LabelData[fp].ValueLabelSize));
	m_txtValueLabel->SetEditable(DefEnable);
	m_txtValueLabel->SetBackgroundColour(DefEnable ? ENABLED : PROTECTED);
	m_chkValueLabelVisible->SetValue(LabelData[fp].ValueLabelVisible);
	m_chkValueLabelVisible->Enable(DefEnable);

	FieldEdited = false;	/* clear this flag for all implicit changes */
}