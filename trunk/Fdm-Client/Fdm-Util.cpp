/* 
 * Copyright (C) 2005 Michael J Jones, mrmikejj at hotmail dot com
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

#include "Fdm-stdinc.h"
#include "../client/DCPlusPlus.h"

#include "Fdm-Util.h"
#include "../Client/Text.h"


tstring FdmUtil::findNickInTString(const tstring aLine) {
	tstring::size_type i;
	tstring::size_type j;

	//Check For <Nick>
	if (((i = aLine.find_first_of('<')) != string::npos) && ((j = aLine.find_first_of('>')) != string::npos && j > i))
		return aLine.substr(i + 1, j - i - 1).c_str();
	return Text::toT("").c_str();
}

void FdmUtil::addIpToMainChat(tstring& aLine, string ip) {
	if (ip != "")
		aLine = (Text::toT("[ " + ip + " ] ") + aLine).c_str();
}