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

#include "resource.h"

#include "FdmMoreWinUtil.h"
#include <dcpp/FdmVersion.h>
#include <dcpp/Util.h>
#include <dcpp/Text.h>
//#include "../Other-Projects/DC++/client/SettingsManager.h"

bool MoreWinUtil::allowMoreInstances() {
	if (::MessageBox(NULL, _T("There is already an instance of ") _T(FDMAPPNAME) _T(" running.\nDo you want to launch another instance anyway?"), _T(FDMAPPNAME) _T(" ") _T(FDMVERSIONSTRING), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST) == IDYES)
		return true;
	return false;
}

bool MoreWinUtil::possibleCommand(tstring cmd, tstring /*param*/, tstring& message, tstring& status) {
	if(Util::stricmp(cmd.c_str(), _T("fdm")) == 0 || Util::stricmp(cmd.c_str(), _T("fdm++")) == 0) {
		string tmp = "\r\nSmile and be happy. :)\r\nhttp://fdm.berlios.de/ <";
		tmp += FDMAPPNAME;
		tmp += " ";
		tmp += FDMVERSIONSTRING;
		tmp += ">";
		message = Text::toT(tmp);
	} else if((Util::stricmp(cmd.c_str(), _T("winamp")) == 0) || (Util::stricmp(cmd.c_str(), _T("w")) == 0)) {
		MoreWinUtil::winampSpam(message, status);
	} else {
		return false;
	}
	return true;
}

string MoreWinUtil::findNickInString(const string aLine) {
	string::size_type i;
	string::size_type j;

	//Check For <Nick>
	if (((i = aLine.find_first_of('<')) != string::npos) && ((j = aLine.find_first_of('>')) != string::npos && j > i))
		return aLine.substr(i + 1, j - i - 1);
	return "";
}

void MoreWinUtil::addIPToString(string& aLine, string ip) {
	if (ip != "")
		aLine = "[ " + ip + " ] " + aLine;
}

//void MoreWinUtil::addFdmStuffToExceptionInfo(char* buf, File& aFile) {
/*	sprintf(buf, "FDM Version: %s\t %s\r\n", FDMVERSIONSTRING, FDMSVNVERSIONSTRING);
	aFile.write(buf, strlen(buf));*/
//}

void MoreWinUtil::initilize() {
/*	StaticWindowsSettings::opSpoken			= FDMSETTING(OP_SPOKE_COLOUR);
	StaticWindowsSettings::notOpSpoken		= FDMSETTING(NOT_OP_SPOKE_COLOUR);
	StaticWindowsSettings::iSpoke			= FDMSETTING(I_SPOKE_COLOUR);
	StaticWindowsSettings::myNickSpoken		= FDMSETTING(MY_NICK_SPOKEN_COLOUR);*/
}
/*
COLORREF StaticWindowsSettings::opSpoken			= 0;
COLORREF StaticWindowsSettings::notOpSpoken			= 0;
COLORREF StaticWindowsSettings::iSpoke				= 0;
COLORREF StaticWindowsSettings::myNickSpoken		= 0;
COLORREF StaticWindowsSettings::doubleClickableLink = RGB(0,0,200);
*/
