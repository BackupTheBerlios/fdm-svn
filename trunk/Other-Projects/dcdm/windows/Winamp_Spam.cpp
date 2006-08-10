/* 
 * Copyright (C) 2004 psf8500
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

#include "../../../Fdm-Windows/dcplusplus-rips/Fdm-stdafx.h"
#include "../../../client/DCPlusPlus.h"


#include "../../../Fdm-Windows/MoreWinUtil.h"
#include "../../../Fdm-client/dcplusplus-rips/Fdm-SettingsManager.h"
#include "../../Nullsoft/Winamp.h"
#include "../../../client/Util.h"

void MoreWinUtil::winampSpam(tstring& message, tstring& status) {
	HWND hwndWinamp = FindWindow(_T("Winamp v1.x"), NULL);
	if (hwndWinamp) {
		StringMap params;
		int waVersion = SendMessage(hwndWinamp,WM_USER, 0, IPC_GETVERSION),
			majorVersion, minorVersion;
		majorVersion = waVersion >> 12;
		if (((waVersion & 0x00F0) >> 4) == 0) {
			minorVersion = ((waVersion & 0x0f00) >> 8) * 10 + (waVersion & 0x000f);
		} else {
			minorVersion = ((waVersion & 0x00f0) >> 4) * 10 + (waVersion & 0x000f);
		}
		params["version"] = Util::toString(majorVersion + minorVersion / 100.0);
		int state = SendMessage(hwndWinamp,WM_USER, 0, IPC_ISPLAYING);
		switch (state) {
			case 0: params["state"] = "stopped";
				break;
			case 1: params["state"] = "playing";
				break;
			case 3: params["state"] = "paused";
		};
		TCHAR titleBuffer[2048];
		int buffLength = sizeof(titleBuffer);
		GetWindowText(hwndWinamp, titleBuffer, buffLength);
		tstring title = titleBuffer;
		params["rawtitle"] = Text::fromT(title);
		// fix the title if scrolling is on, so need to put the stairs to the end of it
		tstring titletmp = title.substr(title.find(_T("***"))+2, title.size());
		title = titletmp + title.substr(0, title.size()-titletmp.size());
		title = title.substr(title.find(_T("."))+2, title.size());
		if (title.rfind(_T("-")) != tstring::npos) {
			params["title"] = Text::fromT(title.substr(0, title.rfind(_T('-')) - 1));
		}
		int curPos = SendMessage(hwndWinamp,WM_USER, 0, IPC_GETOUTPUTTIME);
		int length = SendMessage(hwndWinamp,WM_USER, 1, IPC_GETOUTPUTTIME);
		if (curPos == -1) {
			curPos = 0;
		} else {
			curPos /= 1000;
		}
		int intPercent;
		if (length > 0 ) {
			intPercent = curPos * 100 / length;
		} else {
			length = 0;
			intPercent = 0;
		}
		params["percent"] = Util::toString(intPercent) + "%";
		params["elapsed"] = Util::formatSeconds(curPos);
		params["length"] = Util::formatSeconds(length);
		message = Text::toT(Util::formatParams(FDMSETTING(WINAMP_FORMAT), params, false));
	} else {
		status = _T("Winamp 1.x/2.x is not running");
	}
}