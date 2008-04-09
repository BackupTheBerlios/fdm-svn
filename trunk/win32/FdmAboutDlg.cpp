/*
 * Copyright (C) 2001-2008 Jacek Sieka, arnetheduck on gmail point com
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

#include "stdafx.h"

#include "FdmAboutDlg.h"

#include <dcpp/SimpleXML.h>
#include <dcpp/FdmVersion.h>
#include "WinUtil.h"

static const char fdmThanks[] = "Big thanks to everyone mentioned by Arnetheduck in the other about page. And to Arne himself. :)"
"Thanks go out to berlios for hosting the project. "
"\r\nI would also like to thank these people who may or may not have already been mentioned by arne, but I think I have extra reason to thank them "
"I hope I haven't missed someone, they're in roughly alphabetical order...=) : "
"arnetheduck, Carraya, colognic, DanShUK, Farcry, Flow84, fusbar, GargoyleMT, iceman50, Lightgirl_xp, Paka, poy, PPK, ullner, robertone, sulan, TheParanoidOne, TheNOP, Trem. ";

FdmAboutDlg::FdmAboutDlg(SmartWin::Widget* parent) : WidgetFactory<SmartWin::WidgetModalDialog>(parent) {
	onInitDialog(std::tr1::bind(&FdmAboutDlg::handleInitDialog, this));
	onSpeaker(std::tr1::bind(&FdmAboutDlg::handleSpeaker, this, _1, _2));
}

FdmAboutDlg::~FdmAboutDlg() {
}

bool FdmAboutDlg::handleInitDialog() {
	setItemText(IDC_FDM_ABOUT_VERSION, Text::toT("Fdm v" VERSIONSTRING " " SVNVERSIONSTRING "\n(c) Copyright 2005-2008 Michael Jones\nFdm  is licenced under GPL\nhttp://fdm.berlios.de/"));
	setItemText(IDC_FDM_ABOUT_THANKS, Text::toT(fdmThanks));
	setItemText(IDC_FDM_ABOUT_LATEST, T_("Downloading..."));

	attachButton(IDOK)->onClicked(std::tr1::bind(&FdmAboutDlg::endDialog, this, IDOK));

	centerWindow();

	c.addListener(this);
	c.downloadFile("http://fdm.berlios.de/version.xml");

	return false;
}

LRESULT FdmAboutDlg::handleSpeaker(WPARAM wParam, LPARAM lParam) {
 	switch(wParam) {
 	case SPEAK_VERSIONDATA: {
  		boost::scoped_ptr<tstring> x(reinterpret_cast<tstring*>(lParam));
		setItemText(IDC_FDM_ABOUT_LATEST, *x);
	} break;
 	}
	return 0;
}

void FdmAboutDlg::on(HttpConnectionListener::Data, HttpConnection* /*conn*/, const uint8_t* buf, size_t len) throw() {
	downBuf.append((char*)buf, len);
}

void FdmAboutDlg::on(HttpConnectionListener::Complete, HttpConnection* conn, const string&) throw() {
	if(!downBuf.empty()) {
		SimpleXML xml;
		xml.fromXML(downBuf);
		if(xml.findChild("DCUpdate")) {
			xml.stepIn();
			if(xml.findChild("Version")) {
				tstring* x = new tstring(Text::toT(xml.getChildData()));
				speak(SPEAK_VERSIONDATA, reinterpret_cast<LPARAM>(x));
			}
		}
	}
	conn->removeListener(this);
}

void FdmAboutDlg::on(HttpConnectionListener::Failed, HttpConnection* conn, const string& aLine) throw() {
	tstring* x = new tstring(Text::toT(aLine));
	speak(SPEAK_VERSIONDATA, reinterpret_cast<LPARAM>(x));
	conn->removeListener(this);
}
