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

void ColourUtil::colourRichEditCtrl(CRichEditCtrl& ctrlClient, string myNick, string sourceNick, bool isOp) {
	initilize(ctrlClient);

	// Colour nick
	if (sourceNick != "") {
		long firstChar = newText.find('<', 0);
		long secondChar = newText.find('>', 0);
		colourText(ctrlClient, isOp ? RGB(0,0,205) : RGB(139,0,0), origNumChars + firstChar, origNumChars + secondChar + 1);
		offSet = secondChar + 1;
	} else if (timeStamps) {
		offSet = newText.find(']', 0);
	}

	// See if text needs special colouring
	if (myNick == sourceNick)
		colourText(ctrlClient, RGB(255,0,0), origNumChars + offSet, newNumChars);
	else if (Text::toLower(newText).find(Text::toLower(Text::toT(myNick))) != string::npos)
		colourText(ctrlClient, RGB(0,100,0), origNumChars + offSet, newNumChars);

	// Check for clickable link
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("http://"));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("www."));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("ftp://"));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("irc://"));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("https://"));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("mailto:"));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("dchub://"));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("adc://"));
	findAndColourAllOf(ctrlClient, RGB(0,0,200), _T("magnet:?"));

	// Reset Brush Colour
	colourText(ctrlClient, WinUtil::textColor, newNumChars, newNumChars);

	ctrlClient.SetRedraw(TRUE);
	ctrlClient.Invalidate();
	ctrlClient.UpdateWindow();
}

void ColourUtil::initilize(CRichEditCtrl& ctrlClient) {
	newNumChars = ctrlClient.GetTextLengthEx();
	newText.reserve(1 + newNumChars - origNumChars);

	offSet = 0;

	myBrush.dwMask = CFM_COLOR;
	myBrush.dwEffects = 0;
	myBrush.crTextColor = WinUtil::textColor;

	ctrlClient.SetSel(origNumChars, newNumChars);
	TCHAR *buf = new TCHAR[1 + newNumChars - origNumChars];
	ctrlClient.GetSelText(buf);
	newText = buf;
	delete buf;
}

void ColourUtil::colourText(CRichEditCtrl& ctrlClient, COLORREF colour, long startPos, long endPos) {
	myBrush.crTextColor = colour;
	ctrlClient.SetSel(startPos, endPos);
	ctrlClient.SetWordCharFormat(myBrush);
}

void ColourUtil::findAndColourAllOf(CRichEditCtrl& ctrlClient, COLORREF colour, tstring textToFind) {
	long startPos = offSet;
	long finishPos = offSet;
	while ((startPos = newText.find(textToFind, startPos)) != string::npos) {
		finishPos = newText.find_first_of(_T(" <\t\r\n"), startPos);
		finishPos = (finishPos != string::npos ? finishPos + origNumChars : newNumChars);
		colourText(ctrlClient, colour, startPos + origNumChars, finishPos);
		startPos++;
	}
}