/*
 *  Librarian for KiCad, a free EDA CAD application.
 *  The dialog for adjusting the template variables.
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
 *  $Id: libmngr_dlgtemplate.h 5019 2014-01-13 11:18:57Z thiadmer $
 */
#ifndef __libmngr_dlgtemplate__
#define __libmngr_dlgtemplate__

/**
@file
Subclass of DlgTemplateOpts, which is generated by wxFormBuilder.
*/

#include "libmngr_gui_base.h"

//// end generated include


/** Implementing DlgTemplateOpts */
class libmngrDlgTemplateOpts : public DlgTemplateOpts
{
	protected:
		// Handlers for DlgTemplateOpts events.
		void OnOK( wxCommandEvent& event );
	public:
		/** Constructor */
		libmngrDlgTemplateOpts( wxWindow* parent );
	//// end generated class members

};

#endif // __libmngr_dlgtemplate__
