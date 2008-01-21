/*
 * Copyright (C) 2001-2007 Jacek Sieka, arnetheduck on gmail point com
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

#ifndef FDM_ABOUT_DLG_H
#define FDM_ABOUT_DLG_H

#include <dcpp/HttpConnection.h>
#include "resource.h"
#include "AspectSpeaker.h"

class FdmAboutDlg :
	public SmartWin::WidgetFactory<SmartWin::WidgetModalDialog>,
	public AspectSpeaker<FdmAboutDlg>,
	private HttpConnectionListener

{
public:
	FdmAboutDlg(SmartWin::Widget* parent);
	virtual ~FdmAboutDlg();

	int run() { return createDialog(IDD_FDM_ABOUTBOX); }

private:
	enum Speakers {
		SPEAK_VERSIONDATA
	};

	HttpConnection c;

	string downBuf;

	bool handleInitDialog();
	LRESULT handleSpeaker(WPARAM wParam, LPARAM lParam);

	virtual void on(HttpConnectionListener::Data, HttpConnection* /*conn*/, const uint8_t* buf, size_t len) throw();
	virtual void on(HttpConnectionListener::Complete, HttpConnection* conn, const string&) throw();
	virtual void on(HttpConnectionListener::Failed, HttpConnection* conn, const string& aLine) throw();
};

#endif // !defined(FDM_ABOUT_DLG_H)
