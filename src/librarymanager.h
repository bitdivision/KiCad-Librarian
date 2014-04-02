/*
 *  Librarian for KiCad, a free EDA CAD application.
 *  This file is just the definition of the application.
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
 *  $Id: librarymanager.h 5029 2014-02-07 13:53:34Z thiadmer $
 */
#ifndef __librarymanager__
#define __librarymanager__

#include <wx/wx.h>
#include <wx/frame.h>

#if defined __MSDOS__ || defined __WIN32__ || defined _Windows || defined _WIN32 || defined WIN32
	#define DIRSEP_CHAR	'\\'	/* directory separator character */
	#define DIRSEP_STR	"\\"
#elif defined macintosh || defined __APPLE__
	#define DIRSEP_CHAR ':'
	#define DIRSEP_STR	":"
#else
	#define DIRSEP_CHAR '/'
	#define DIRSEP_STR	"/"
#endif

#define APP_NAME	wxT("KiCadLibrarian")

class LibraryManagerApp : public wxApp
{
public:
    virtual bool OnInit();

    wxString GetRootPath() const            /* the path below the ./bin (and others) */
        { return strRootPath; }
    wxString GetBinPath() const             /* the path at which the executables are found */
        { return strBinPath; }
    wxString GetTemplatePath() const        /* the path where the templates are */
        { return strTemplates; }
    wxString GetDocumentationPath() const   /* the path where the documentation is */
        { return strDocumentationPath; }
    wxString GetFontFile() const            /* the full path/filename to the font */
        { return strFontFile; }

private:
    wxString strRootPath;
    wxString strBinPath;
    wxString strTemplates;
    wxString strDocumentationPath;
    wxString strFontFile;
};

extern LibraryManagerApp* theApp;

#endif //__librarymanager__
