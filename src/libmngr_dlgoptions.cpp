/*
 *  Librarian for KiCad, a free EDA CAD application.
 *  The dialog for the user-interface settings.
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
 *  $Id: libmngr_dlgoptions.cpp 5025 2014-01-30 09:26:58Z thiadmer $
 */
#include "librarymanager.h"
#include "libmngr_dlgoptions.h"
#include <wx/config.h>

libmngrDlgOptions::libmngrDlgOptions( wxWindow* parent )
:
DlgOptions( parent )
{
	wxConfig *config = new wxConfig(APP_NAME);
	long idx;

	idx = config->Read(wxT("display/fontsize"), 8L);
	m_spinFontSize->SetValue(idx);

	idx = config->Read(wxT("display/dimoffset"), 50L);
	m_spinDimOffset->SetValue(idx);

	bool showlabels;
	config->Read(wxT("display/showlabels"), &showlabels, true);
	m_chkDrawLabels->SetValue(showlabels);

	bool fullpaths;
	config->Read(wxT("display/fullpath"), &fullpaths, false);
	m_chkFullPaths->SetValue(fullpaths);

	bool copyvrml;
	config->Read(wxT("settings/copyvrml"), &copyvrml, true);
	m_chkCopyVRML->SetValue(copyvrml);

  bool disabletemplate;
  config->Read(wxT("settings/disabletemplate"), &disabletemplate, false);
  m_chkDisableTemplates->SetValue(disabletemplate);

	delete config;
}

void libmngrDlgOptions::OnOK( wxCommandEvent& event )
{
	wxConfig *config = new wxConfig(APP_NAME);
	int idx;

	idx = m_spinFontSize->GetValue();
	config->Write(wxT("display/fontsize"), idx);

	idx = m_spinDimOffset->GetValue();
	config->Write(wxT("display/dimoffset"), idx);

	bool showlabels = m_chkDrawLabels->GetValue();
	config->Write(wxT("display/showlabels"), showlabels);

	bool fullpaths = m_chkFullPaths->GetValue();
	config->Write(wxT("display/fullpath"), fullpaths);

	bool copyvrml = m_chkCopyVRML->GetValue();
	config->Write(wxT("settings/copyvrml"), copyvrml);

  bool disabletemplate = m_chkDisableTemplates->GetValue();
  config->Write(wxT("settings/disabletemplate"), disabletemplate);

	delete config;
	event.Skip();
}
