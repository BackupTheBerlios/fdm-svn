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

#include "Fdm-stdafx.h"
#include "../client/DCPlusPlus.h"
#include "resource.h"

#include "MoreWinUtil.h"
#include "../Fdm-Client/dcplusplus-rips/Fdm-Version.h"
#include "../Fdm-Client/dcplusplus-rips/Fdm-SettingsManager.h"

bool MoreWinUtil::allowMoreInstances(size_t amountOfProcesses) {
	if(amountOfProcesses == 0)
		if (::MessageBox(NULL, _T("There is already an instance of ") _T(FDMAPPNAME) _T(" running.\nDo you want to launch another instance anyway?"), _T(FDMAPPNAME) _T(" ") _T(FDMVERSIONSTRING), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST) == IDYES)
			return true;
	return false;
}

void MoreWinUtil::initilizeColours() {
	StaticSettings::opSpoken		= FDMSETTING(OP_SPOKE_COLOUR);
	StaticSettings::notOpSpoken		= FDMSETTING(NOT_OP_SPOKE_COLOUR);
	StaticSettings::iSpoke			= FDMSETTING(I_SPOKE_COLOUR);
	StaticSettings::myNickSpoken	= FDMSETTING(MY_NICK_SPOKEN_COLOUR);
}

COLORREF StaticSettings::opSpoken		= 0;
COLORREF StaticSettings::notOpSpoken	= 0;
COLORREF StaticSettings::iSpoke			= 0;
COLORREF StaticSettings::myNickSpoken	= 0;