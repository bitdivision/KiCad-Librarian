/*
 *  Librarian for KiCad, a free EDA CAD application.
 *  The dialog for the link to a remote repository.
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
 *  $Id: libmngr_dlgremotelink.cpp 5019 2014-01-13 11:18:57Z thiadmer $
 */
#include "librarymanager.h"
#include "libmngr_dlgremotelink.h"
#include <wx/config.h>
#include "remotelink.h"

libmngrDlgRemoteLink::libmngrDlgRemoteLink( wxWindow* parent )
:
DlgRemoteLink( parent )
{
  ReadFields();
}

void libmngrDlgRemoteLink::OnOK( wxCommandEvent& /*event*/ )
{
  WriteFields();
  curlReset();
  this->EndDialog(wxID_OK);
}

void libmngrDlgRemoteLink::OnCancel( wxCommandEvent& /*event*/ )
{
  this->EndDialog(wxID_CANCEL);
}

void libmngrDlgRemoteLink::OnSignUp( wxCommandEvent& /*event*/ )
{
  WriteFields();
  libmngrDlgRemoteSignUp dlg(this);
  dlg.ShowModal();
  ReadFields();
}

void libmngrDlgRemoteLink::ReadFields()
{
  wxConfig *config = new wxConfig(APP_NAME);
  wxString field;

  field = config->Read(wxT("repository/url"));
  m_txtURL->SetValue(field);

  field = config->Read(wxT("repository/user"));
  m_txtUserName->SetValue(field);

  field = config->Read(wxT("repository/pwd"));
  field = Scramble(field);	/* this un-scrambles the string */
  m_txtPassword->SetValue(field);

  field = config->Read(wxT("repository/hostuser"));
  m_txtAuthUser->SetValue(field);

  field = config->Read(wxT("repository/hostpwd"));
  field = Scramble(field);	/* this un-scrambles the string */
  m_txtAuthPWD->SetValue(field);

  long flags;
  config->Read(wxT("repository/hostverify"), &flags, 0x03);
  m_checkVerifyPeer->SetValue((flags & 0x01) != 0);
  m_checkVerifyHost->SetValue((flags & 0x02) != 0);

  delete config;
}

void libmngrDlgRemoteLink::WriteFields()
{
  wxConfig *config = new wxConfig(APP_NAME);
  wxString field;

  field = m_txtURL->GetValue();
  config->Write(wxT("repository/url"), field);

  field = m_txtUserName->GetValue();
  config->Write(wxT("repository/user"), field);

  field = m_txtPassword->GetValue();
  field = Scramble(field);
  config->Write(wxT("repository/pwd"), field);

  field = m_txtAuthUser->GetValue();
  config->Write(wxT("repository/hostuser"), field);

  field = m_txtAuthPWD->GetValue();
  field = Scramble(field);
  config->Write(wxT("repository/hostpwd"), field);

  long flags = 0;
  if (m_checkVerifyPeer->GetValue())
	  flags |= 0x01;
  if (m_checkVerifyHost->GetValue())
	  flags |= 0x02;
  config->Write(wxT("repository/hostverify"), flags);

  delete config;
}



libmngrDlgRemoteSignUp::libmngrDlgRemoteSignUp( wxWindow* parent )
:
DlgRemoteSignUp( parent )
{
  wxConfig *config = new wxConfig(APP_NAME);
  wxString field;

  field = config->Read(wxT("repository/url"));
  m_txtURL->SetValue(field);

  field = config->Read(wxT("repository/user"));
  m_txtUserName->SetValue(field);

  field = config->Read(wxT("repository/email"));
  m_txtEmail->SetValue(field);

  field = config->Read(wxT("repository/hostuser"));
  m_txtAuthUser->SetValue(field);

  field = config->Read(wxT("repository/hostpwd"));
  field = Scramble(field);	/* this un-scrambles the string */
  m_txtAuthPWD->SetValue(field);

  long flags;
  config->Read(wxT("repository/hostverify"), &flags, 0x03);
  m_checkVerifyPeer->SetValue((flags & 0x01) != 0);
  m_checkVerifyHost->SetValue((flags & 0x02) != 0);

  delete config;
}

void libmngrDlgRemoteSignUp::OnOK( wxCommandEvent& event )
{
  wxString url = m_txtURL->GetValue();
  wxString name = m_txtUserName->GetValue();
  wxString email = m_txtEmail->GetValue();
  wxString hostuser = m_txtAuthUser->GetValue();
  wxString hostpwd = m_txtAuthPWD->GetValue();
  long hostverify = 0;
  if (m_checkVerifyPeer->GetValue())
	  hostverify |= 0x01;
  if (m_checkVerifyHost->GetValue())
	  hostverify |= 0x02;

  wxString msg = curlAddUser(url, name, email, hostuser, hostpwd, hostverify);
  if (msg.length() == 0) {
    wxConfig *config = new wxConfig(APP_NAME);
    config->Write(wxT("repository/url"), url);
    config->Write(wxT("repository/user"), name);
    config->Write(wxT("repository/email"), email);
    config->Write(wxT("repository/hostuser"), hostuser);
    config->Write(wxT("repository/hostpwd"), Scramble(hostpwd));
    config->Write(wxT("repository/hostverify"), hostverify);
    delete config;

    wxMessageBox(wxT("Sign-up succeeded\nYou will receive a password on the supplied e-mail address."));
    event.Skip();
  } else {
    wxMessageBox(wxT("Sign-up failure\nServer message: ") + msg);
  }
}

