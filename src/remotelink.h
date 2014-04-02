/*
 *  Librarian for KiCad, a free EDA CAD application.
 *  Utility functions transferring modules or footprints to/from a repository.
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
 *  $Id: remotelink.h 5024 2014-01-27 08:54:56Z thiadmer $
 */
#ifndef REMOTELINK_H
#define REMOTELINK_H

#include <wx/string.h>

bool curlInit();
bool curlReset();
void curlCleanup();

wxString curlAddUser(const wxString& url, const wxString& user, const wxString& email,
					 const wxString& hostuser, const wxString& hostpwd, long flags);
wxString curlList(wxArrayString* list, const wxString& category);
wxString curlGet(const wxString& partname, const wxString& author, const wxString& category, wxArrayString* module);
wxString curlPut(const wxString& partname, const wxString& category, const wxArrayString& module);
wxString curlDelete(const wxString& partname, const wxString& category);

wxString URLEncode(const wxString& string);
wxString URLDecode(const wxString& string);
wxString Scramble(const wxString& source);

#endif /* REMOTELINK_H */
