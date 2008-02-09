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

#include "FdmMoreWinUtil.h"

#include "resource.h"

#include <dcpp/FdmSettingsManager.h>
#include <dcpp/FdmUtil.h>
#include <dcpp/FdmVersion.h>
#include <dcpp/ClientManager.h>
#include <dcpp/Socket.h>
#include <dcpp/Text.h>
#include <dcpp/Util.h>

#include <Ws2tcpip.h>

bool MoreWinUtil::allowMoreInstances() {
// todo
// probably broke
// some other time
	if (::MessageBox(NULL, _T("There is already an instance of ") _T(FDMAPPNAME) _T(" running.\nDo you want to launch another instance anyway?"), _T(FDMAPPNAME) _T(" ") _T(FDMVERSIONSTRING), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST) == IDYES)
		return true;
	return false;
}

tstring MoreWinUtil::commands = _T("\r\n /fdm \t or \t /fdm++ \t Fdm's spam,")
								_T("\r\n /find something \t \t \t Searches for nick or ip in all connected hubs")
								_T("\r\n /dns something \t or \t /resolve something \t Resolves a DNS to an IP address.  Or an IP address to it's 'real' DNS")
								_T("\r\n /winamp \t or \t /w \t Winamp spam.  Text after the command will be appended to the end of the winamp spam text");

bool MoreWinUtil::possibleCommand(tstring cmd, tstring param, tstring& message, tstring& status) {
	if(Util::stricmp(cmd.c_str(), _T("fdm")) == 0 || Util::stricmp(cmd.c_str(), _T("fdm++")) == 0) {
		string tmp = "\r\nSmile and be happy. :)\r\nhttp://fdm.berlios.de/ <";
		tmp += FDMAPPNAME;
		tmp += " ";
		tmp += FDMVERSIONSTRING;
		tmp += ">";
		message = Text::toT(tmp);
	} else if((Util::stricmp(cmd.c_str(), _T("winamp")) == 0) || (Util::stricmp(cmd.c_str(), _T("w")) == 0)) {
		MoreWinUtil::winampSpam(param, message, status);
	} else if((Util::stricmp(cmd.c_str(), _T("dns")) == 0) || (Util::stricmp(cmd.c_str(), _T("resolve")) == 0)) {
		if (param.empty()) {
			status = _T("An IP or DNS must be supplied.  Syntax /resolve somedns.com or /resolve 1.2.3.4");
			return true;
		}
		string toResolve = Text::fromT(param);
		if (FdmUtil::isIp(toResolve)) {
			struct sockaddr_in ip;
			ip.sin_family = AF_INET;
			ip.sin_addr.s_addr = inet_addr(toResolve.c_str());
			char result[NI_MAXHOST];
			
			int isError = getnameinfo((struct sockaddr*)&ip, sizeof(struct sockaddr), result, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
			if (isError)
				status = _T("The IP ") + param + _T(" failed to resolve");
			else
				status = _T("The IP ") + param + _T(" resolves to ") + Text::toT(result);
		} else {
			string result = Socket::resolve(toResolve);
			if (result.empty())
				status = _T("The DNS ") + param + _T(" failed to resolve");
			else
				status = _T("The DNS ") + param + _T(" resolves to ") + Text::toT(result);
		}
	} else if(Util::stricmp(cmd.c_str(), _T("find")) == 0) {
		if (param.empty()) {
			status = _T("An nick or IP must be supplied.  Syntax /find abc or /resolve 1.2.3.4");
			return true;
		}
		status = Text::toT(ClientManager::getInstance()->findNickOrIP(Text::fromT(param)));
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

void MoreWinUtil::addCountryIPToString(string& aLine, string ip) {
	if (ip != "") {
		if (FDMSETTING(SHOW_IPS_IN_CHAT))
			aLine = "[ " + ip + " ] " + aLine;
		if (FDMSETTING(SHOW_CC_IN_CHAT))
			aLine = "[ " + Util::getIpCountry(ip) + " ] " + aLine;
	}
}

//void MoreWinUtil::addFdmStuffToExceptionInfo(char* buf, File& aFile) {
/*	sprintf(buf, "FDM Version: %s\t %s\r\n", FDMVERSIONSTRING, FDMSVNVERSIONSTRING);
	aFile.write(buf, strlen(buf));*/
//}

/*void MoreWinUtil::initilize() {
	StaticWindowsSettings::opSpoken			= FDMSETTING(OP_SPOKE_COLOUR);
	StaticWindowsSettings::notOpSpoken		= FDMSETTING(NOT_OP_SPOKE_COLOUR);
	StaticWindowsSettings::iSpoke			= FDMSETTING(I_SPOKE_COLOUR);
	StaticWindowsSettings::myNickSpoken		= FDMSETTING(MY_NICK_SPOKEN_COLOUR);
}*/
/*
COLORREF StaticWindowsSettings::opSpoken			= 0;
COLORREF StaticWindowsSettings::notOpSpoken			= 0;
COLORREF StaticWindowsSettings::iSpoke				= 0;
COLORREF StaticWindowsSettings::myNickSpoken		= 0;
COLORREF StaticWindowsSettings::doubleClickableLink = RGB(0,0,200);
*/
