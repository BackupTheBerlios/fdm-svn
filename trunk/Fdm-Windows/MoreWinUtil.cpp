/* 
 * Copyright (C) 2006 Michael J Jones, mrmikejj at hotmail dot com
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
#include "../client/DCPlusPlus.h"
#include "resource.h"

#include "MoreWinUtil.h"
#include "../Fdm-Client/dcplusplus-rips/Fdm-Version.h"
#include "../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"
#include "../Fdm-Client/dcplusplus-rips/Fdm-SettingsManager.h"

bool MoreWinUtil::allowMoreInstances(size_t amountOfProcesses) {
	if(amountOfProcesses == 0)
		if (::MessageBox(NULL, _T("There is already an instance of ") _T(FDMAPPNAME) _T(" running.\nDo you want to launch another instance anyway?"), _T(FDMAPPNAME) _T(" ") _T(FDMVERSIONSTRING), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST) == IDYES)
			return true;
	return false;
}

void MoreWinUtil::additionMenuStuff(CMenu& mainMenu) {
	CMenuHandle fdmMenu;
	fdmMenu.CreatePopupMenu();

	fdmMenu.AppendMenu(MF_STRING, IDD_FDM_ABOUT, FDMCTSTRING(MENU_FDM_ABOUT));
	fdmMenu.AppendMenu(MF_SEPARATOR);

	mainMenu.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)fdmMenu, FDMCTSTRING(MENU_FDM));
}

bool MoreWinUtil::possibleCommand(tstring cmd, tstring /*param*/, tstring& message, tstring& status) {
	if(Util::stricmp(cmd.c_str(), _T("fdm")) == 0 || Util::stricmp(cmd.c_str(), _T("fdm++")) == 0) {
		string tmp = "\r\nSmile and be happy. :)\r\nhttp://fdm.berlios.de/ <";
		tmp += FDMAPPNAME;
		tmp += " ";
		tmp += FDMVERSIONSTRING;
		tmp += ">";
		message = Text::toT(tmp);
	} else if(Util::stricmp(cmd.c_str(), _T("huh")) == 0) {
		string tmp = "\r\n\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?              huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?      huh?        huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?                         huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?                           huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?                            huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?                           huh?\r\n";
		tmp += "huh?huh?huh?huh?huh?          huh?                            huh?          huh?huh?huh?huh?huh?                       huh?\r\n";
		tmp += "huh?huh?huh?huh?huh?          huh?                            huh?          huh?huh?huh?huh?huh?                  huh?\r\n";
		tmp += "huh?huh?huh?huh?huh?          huh?                            huh?          huh?huh?huh?huh?huh?              huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?            huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?            huh?\r\n";
		tmp += "huh?                        huh?          huh?                            huh?          huh?                        huh?            huh?\r\n";
		tmp += "huh?                        huh?             huh?                       huh?            huh?                        huh?            huh?\r\n";
		tmp += "huh?                        huh?                 huh?              huh?                 huh?                        huh?\r\n";
		tmp += "huh?                        huh?                        huh?huh?                        huh?                        huh?            huh?\r\n";
	    tmp += "\r\n";
		message = Text::toT(tmp); 
	} else if((Util::stricmp(cmd.c_str(), _T("winamp")) == 0) || (Util::stricmp(cmd.c_str(), _T("w")) == 0)) {
		MoreWinUtil::winampSpam(message, status);
	} else {
		return false;
	}
	return true;
}

void MoreWinUtil::initilize() {
	StaticWindowsSettings::opSpoken			= FDMSETTING(OP_SPOKE_COLOUR);
	StaticWindowsSettings::notOpSpoken		= FDMSETTING(NOT_OP_SPOKE_COLOUR);
	StaticWindowsSettings::iSpoke			= FDMSETTING(I_SPOKE_COLOUR);
	StaticWindowsSettings::myNickSpoken		= FDMSETTING(MY_NICK_SPOKEN_COLOUR);
}

COLORREF StaticWindowsSettings::opSpoken			= 0;
COLORREF StaticWindowsSettings::notOpSpoken			= 0;
COLORREF StaticWindowsSettings::iSpoke				= 0;
COLORREF StaticWindowsSettings::myNickSpoken		= 0;
COLORREF StaticWindowsSettings::doubleClickableLink = RGB(0,0,200);