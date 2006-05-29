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
#include "../Client/Text.h"
#include "../Client/Util.h"
#include "../Windows/WinUtil.h"
#include "MoreWinUtil.h"

void SortChat::ColourUtil::colourRichEditCtrl(CRichEditCtrl& ctrlClient, string myNick, bool isOp) {
	initilize(ctrlClient);
	string sourceNick = findNickInTString(newText);

	// Colour beginning text
	if (sourceNick != "") {
		offSet = newText.find('<', 0);
		colourText(ctrlClient, WinUtil::textColor, origNumChars, origNumChars + offSet);
	} else if (timeStamps) {
		offSet = newText.find(']', 0) + 1;
		colourText(ctrlClient, WinUtil::textColor, origNumChars, origNumChars + offSet);
	}

	// Colour nick
	if (sourceNick != "") {
        long tempOffSet = newText.find('>', offSet) + 1;
		colourText(ctrlClient, isOp ? StaticSettings::opSpoken : StaticSettings::notOpSpoken, origNumChars + offSet, origNumChars + tempOffSet);
		offSet = tempOffSet;
	}

	// See if text needs special colouring
	if (myNick == sourceNick)
		colourText(ctrlClient, StaticSettings::iSpoke, origNumChars + offSet, newNumChars);
	else if (Text::toLower(newText).find(Text::toLower(Text::toT(myNick)), offSet) != string::npos)
		colourText(ctrlClient, StaticSettings::myNickSpoken, origNumChars + offSet, newNumChars);

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

void SortChat::ColourUtil::initilize(CRichEditCtrl& ctrlClient) {
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

void SortChat::ColourUtil::colourText(CRichEditCtrl& ctrlClient, COLORREF colour, long startPos, long endPos) {
	myBrush.crTextColor = colour;
	ctrlClient.SetSel(startPos, endPos);
	ctrlClient.SetWordCharFormat(myBrush);
}

void SortChat::ColourUtil::findAndColourAllOf(CRichEditCtrl& ctrlClient, COLORREF colour, tstring textToFind) {
	long startPos = offSet;
	long finishPos = offSet;
	while ((startPos = newText.find(textToFind, startPos)) != string::npos) {
		finishPos = newText.find_first_of(_T(" <\t\r\n"), startPos);
		finishPos = (finishPos != string::npos ? finishPos + origNumChars : newNumChars);
		colourText(ctrlClient, colour, startPos + origNumChars, finishPos);
		startPos++;
	}
}

string SortChat::findNickInTString(const tstring aLine) {
	tstring::size_type i;
	tstring::size_type j;

	if (((i = aLine.find_first_of('<')) != string::npos) && ((j = aLine.find_first_of('>')) != string::npos && j > i))
		return Text::fromT(aLine.substr(i + 1, j - i - 1).c_str());
	return Util::emptyString;
}

void SortChat::addIpToChat(tstring& aLine, string ip) {
	if (ip != "")
		aLine = (Text::toT("[ " + ip + " ] ") + aLine).c_str();
}
