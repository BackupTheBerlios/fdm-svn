/*
 * Copyright (C) 2001-2006 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#if !defined(ABOUT_DLG_H)
#define ABOUT_DLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../client/HttpConnection.h"
#include "../../client/SimpleXML.h"
#include "../../client/ResourceManager.h"
#include "../../windows/WinUtil.h"
#include "../../Fdm-Client/dcplusplus-rips/Fdm-Version.h"

static const TCHAR fdmThanks[] = _T("Big thanks to everyone mentioned by Arnetheduck in the other about page. And to Arne himself. :)\r\n")
_T("Thanks go out to berlios for hosting the project.\r\n")
_T("Fdm uses a few more projects than dc++, so I would like to also thank\r\n")
_T("Philip Hazel for PCRE, http://www.pcre.org/ \r\n")
_T("D. Richard Hipp for Sqlite, http://www.sqlite.org/ \r\n")
_T("Rob Groves for Cppsqlite, http://www.codeproject.com/database/CppSQLite.asp \r\n")
_T("\r\nI would also like to thank these people who may or may not have already been mentioned by arne, but I think I have extra reason to thank them\r\n")
_T("I hope I haven't missed someone, they're in roughly alphabetical order...=):\r\n")
_T("arnetheduck, Carraya, colognic, DanShUK, Farcry, Flow84, fusbar, GargoyleMT, iceman50, Paka, PPK, ullner, robertone, sulan, TheParanoidOne, TheNOP, Trem.");

class FdmAboutDlg : public CDialogImpl<FdmAboutDlg>, private HttpConnectionListener
{
public:
	enum { IDD = IDD_FDM_ABOUT };
	enum { WM_VERSIONDATA = WM_APP + 53 };

	FdmAboutDlg() { }
	virtual ~FdmAboutDlg() { }

	BEGIN_MSG_MAP(FdmAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_VERSIONDATA, onVersionData)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		SetDlgItemText(IDC_FDM_ABOUT_VERSION, _T("Fdm v") _T(FDMVERSIONSTRING) _T(" ") _T(FDMSVNVERSIONSTRING) _T("\n(c) Copyright 2006 Michael Jones\nFdm  is licenced under GPL\nhttp://fdm.berlios.de/"));
		CEdit ctrl(GetDlgItem(IDC_FDM_ABOUT_THANKS));
		ctrl.FmtLines(TRUE);
		ctrl.AppendText(fdmThanks, TRUE);
		ctrl.Detach();
		SetDlgItemText(IDC_FDM_ABOUT_TTH, WinUtil::tth.c_str());
		SetDlgItemText(IDC_FDM_ABOUT_LATEST, CTSTRING(DOWNLOADING));
		CenterWindow(GetParent());
		c.addListener(this);
		c.downloadFile("http://dcplusplus.sourceforge.net/version.xml");
		return TRUE;
	}

	LRESULT onVersionData(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		tstring* x = (tstring*) wParam;
		SetDlgItemText(IDC_LATEST, x->c_str());
		delete x;
		return 0;
	}
		
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		EndDialog(wID);
		return 0;
	}

private:
	HttpConnection c;

	FdmAboutDlg(const FdmAboutDlg&) { dcassert(0); }
	
	virtual void on(HttpConnectionListener::Data, HttpConnection* /*conn*/, const u_int8_t* buf, size_t len) throw() {
		downBuf.append((char*)buf, len);
	}

	virtual void on(HttpConnectionListener::Complete, HttpConnection* conn, const string&) throw() {
		if(!downBuf.empty()) {
			SimpleXML xml;
			xml.fromXML(downBuf);
			if(xml.findChild("DCUpdate")) {
				xml.stepIn();
				if(xml.findChild("Version")) {
					tstring* x = new tstring(Text::toT(xml.getChildData()));
					PostMessage(WM_VERSIONDATA, (WPARAM) x);
				}
			}
		}
		conn->removeListener(this);
	}

	virtual void on(HttpConnectionListener::Failed, HttpConnection* conn, const string& aLine) throw() {
		tstring* x = new tstring(Text::toT(aLine));
		PostMessage(WM_VERSIONDATA, (WPARAM) x);
		conn->removeListener(this);
	}

	string downBuf;
};

#endif // !defined(ABOUT_DLG_H)
