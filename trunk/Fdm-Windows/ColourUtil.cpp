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

#include "ColourUtil.h"
#include "../Windows/WinUtil.h"
#include "../Client/Text.h"

void ColourUtil::colourRichEditCtrl(CRichEditCtrl& ctrlClient, long amountOfCharsBeforeAddition, string myNick, string sourceNick, bool isOp, const tstring aLine) {
	long amountOfCharsAfterAddition = ctrlClient.GetTextLengthEx();
	CHARFORMAT2 myBrush;
	myBrush.dwMask = CFM_COLOR;
	myBrush.dwEffects = 0;

	bool myNickSpoken = false;
	tstring temp = Text::toLower(aLine);
	tstring temp2 = Text::toT(Text::toLower(myNick)).c_str();
	if (temp.find(temp2) != string::npos)
		myNickSpoken = true;

	ctrlClient.SetSel(amountOfCharsBeforeAddition, amountOfCharsAfterAddition);

	// mynick spoke
	if (myNick == sourceNick) {
		myBrush.crTextColor = RGB(139,0,0);
	} else {
	// someone else spoken
		if (myNickSpoken) {
			// and said mynick
			myBrush.crTextColor = RGB(0,100,0);
		} else {
			myBrush.crTextColor = WinUtil::textColor;
		}
	}
	ctrlClient.SetWordCharFormat(myBrush);

	if (isOp && (sourceNick != "" )) {
		TCHAR *buf = new TCHAR[1 + amountOfCharsAfterAddition - amountOfCharsBeforeAddition];
		ctrlClient.GetSelText(buf);
		tstring::size_type i = 0;
		while (true) {
			if (buf[i] == '<')
				break;
			else
				i++;
		}

		ctrlClient.SetSel(amountOfCharsBeforeAddition + i, (amountOfCharsBeforeAddition + i + sourceNick.length() + 2));
		myBrush.crTextColor = RGB(0,0,205);
		ctrlClient.SetWordCharFormat(myBrush);
	}

	ctrlClient.SetSel(0,0);
}