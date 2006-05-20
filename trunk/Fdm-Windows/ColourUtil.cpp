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

long ColourUtil::findPositionOfChar(CRichEditCtrl ctrlClient, long size, char theChar) {
	long thePos = 0;
	TCHAR *buf = new TCHAR[size];
	ctrlClient.GetSelText(buf);

	while (true) {
		if (buf[thePos] == theChar || thePos == size)
			break;
		else
			thePos++;
	}

	delete buf;
	return thePos;
}

void ColourUtil::colourRichEditCtrl(CRichEditCtrl& ctrlClient, long originalNoOfChars, int currentLinePosition, string myNick, string sourceNick, bool isOp, bool myNickSpoken, bool timeStamps) {
	long newNoOfChars = ctrlClient.GetTextLengthEx();
	CHARFORMAT2 myBrush;
	myBrush.dwMask = CFM_COLOR;
	myBrush.dwEffects = 0;
	myBrush.crTextColor = WinUtil::textColor;

	ctrlClient.SetSel(originalNoOfChars, newNoOfChars);
	if (sourceNick != "") {
		long firstChar = findPositionOfChar(ctrlClient, 1 + newNoOfChars - originalNoOfChars, '<');

		if (isOp) {
		// colour speakers nick, if op
			long secondChar = findPositionOfChar(ctrlClient, 1 + newNoOfChars - originalNoOfChars, '>');
			colourText(ctrlClient, myBrush, RGB(0,0,205), originalNoOfChars + firstChar, originalNoOfChars + secondChar + 1);
			originalNoOfChars += secondChar + 1;
		} else {
		// different colour if not op
			long secondChar = findPositionOfChar(ctrlClient, 1 + newNoOfChars - originalNoOfChars, '>');
			colourText(ctrlClient, myBrush, RGB(139,0,0), originalNoOfChars + firstChar, originalNoOfChars + secondChar + 1);
			originalNoOfChars += secondChar + 1;
		}
	} else if (timeStamps) {
		// Offset text to colour
		originalNoOfChars += findPositionOfChar(ctrlClient, 1 + newNoOfChars - originalNoOfChars, ']') + 1;
	}

	// mynick spoke
	if (myNick == sourceNick)
		colourText(ctrlClient, myBrush, RGB(255,0,0), originalNoOfChars, newNoOfChars);
	else if (myNickSpoken)
	// someone else spoken mynick
		colourText(ctrlClient, myBrush, RGB(0,100,0), originalNoOfChars, newNoOfChars);
	else
	// normal text then
		colourText(ctrlClient, myBrush, WinUtil::textColor, originalNoOfChars, newNoOfChars);

	// Check for clickable link

	if (currentLinePosition != -1) {
		ctrlClient.SetSel(0, 0);
		colourText(ctrlClient, myBrush, WinUtil::textColor, 0, 0);
		ctrlClient.LineScroll(currentLinePosition, 0);
	} else {
		ctrlClient.SetSel(newNoOfChars, newNoOfChars);
		colourText(ctrlClient, myBrush, WinUtil::textColor, newNoOfChars, newNoOfChars);
	}
}