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

void ColourUtil::colourText(CRichEditCtrl& ctrlClient, CHARFORMAT2 myBrush, COLORREF colour, long startPos, long endPos) {
		myBrush.crTextColor = colour;
		ctrlClient.SetSel(startPos, endPos);
		ctrlClient.SetWordCharFormat(myBrush);
}

void ColourUtil::findPositionsOfChars(CRichEditCtrl ctrlClient, long size, long& firstChar, long& secondChar) {
		TCHAR *buf = new TCHAR[size];
		ctrlClient.GetSelText(buf);
		
		while (true) {
			if (buf[firstChar] == '<')
				break;
			else
				firstChar++;
		}
		
		// we can skip forward a bit for this one
		secondChar = firstChar + 1;

		while (true) {
			if (buf[secondChar] == '>')
				break;
			else
				secondChar++;
		}

		delete buf;
}

void ColourUtil::colourRichEditCtrl(CRichEditCtrl& ctrlClient, long originalNoOfChars, string myNick, string sourceNick, bool isOp, bool myNickSpoken, int currentLinePosition) {
	long newNoOfChars = ctrlClient.GetTextLengthEx();
	CHARFORMAT2 myBrush;
	myBrush.dwMask = CFM_COLOR;
	myBrush.dwEffects = 0;

	// mynick spoke
	if (myNick == sourceNick)
		colourText(ctrlClient, myBrush, RGB(139,0,0), originalNoOfChars, newNoOfChars);
	else if (myNickSpoken)
	// someone else spoken mynick
		colourText(ctrlClient, myBrush, RGB(0,100,0), originalNoOfChars, newNoOfChars);
	else
		colourText(ctrlClient, myBrush, WinUtil::textColor, originalNoOfChars, newNoOfChars);

	// colour speakers nick
	if (isOp && (sourceNick != "" )) {
		long firstChar = 0;
		long secondChar = 0;
		findPositionsOfChars(ctrlClient, 1 + newNoOfChars - originalNoOfChars, firstChar, secondChar);
		colourText(ctrlClient, myBrush, RGB(0,0,205), originalNoOfChars + firstChar, originalNoOfChars + secondChar + 1);
	}

	if (currentLinePosition != -1) {
		ctrlClient.SetSel(0, 0);
		ctrlClient.LineScroll(currentLinePosition, 0);
	} else {
		ctrlClient.SetSel(newNoOfChars, newNoOfChars);
	}
}