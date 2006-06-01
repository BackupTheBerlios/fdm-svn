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

string SortChat::findNickInTString(const tstring aLine) {
	tstring::size_type i;
	tstring::size_type j;

	if (((i = aLine.find_first_of('<')) != string::npos) && ((j = aLine.find_first_of('>')) != string::npos && j > i))
		return Text::fromT(aLine.substr(i + 1, j - i - 1).c_str());
	return Util::emptyString;
}

void SortChat::FdmCRichEditCtrl::AppendText(LPCTSTR aLine) {
	if (!prepared) {
		CRichEditCtrl::AppendText(aLine);
		return;
	}

	newText = aLine;
	tstring::size_type offSet = 2;
	myBrush.dwMask = CFM_COLOR;
	myBrush.dwEffects = 0;
	myBrush.crTextColor = WinUtil::textColor;
	int originalLinePos = GetFirstVisibleLine();

	if (timeStamps) {
		offSet += newText.find(']', 0);
		colourAndAppend(newText.substr(0, offSet).c_str());
	} else {
		colourAndAppend(newText.substr(0, 1).c_str());
	}

	if (speakersNick != Util::emptyString) {
		if (speakersIP != Util::emptyString)
			colourAndAppend((Text::toT("[ " + speakersIP + " ] ")).c_str());

		myBrush.crTextColor = speakersOpStatus ? StaticSettings::opSpoken : StaticSettings::notOpSpoken;
		colourAndAppend((Text::toT("<" + speakersNick + ">")).c_str());
		myBrush.crTextColor = WinUtil::textColor;
		offSet = newText.find_first_of('>') + 1;
	}

	if (myNick != Util::emptyString)
		if (myNick == speakersNick)
			myBrush.crTextColor = StaticSettings::iSpoke;
		else if (Text::toLower(newText).find(Text::toLower(Text::toT(myNick)), offSet) != string::npos)
			myBrush.crTextColor = StaticSettings::myNickSpoken;

	colourAndAppend(newText.substr(offSet).c_str());

	// Check rest of text for colourable links
	CRichEditCtrl::SetSel(sizeForAppend, -1);
	TCHAR *buf = new TCHAR[1 + GetTextLengthEx() - sizeForAppend];
	GetSelText(buf);
	newText = buf;
	delete buf;

	myBrush.crTextColor = StaticSettings::doubleClickableLink;
	findAndColourAllOf(_T("http://"));
	findAndColourAllOf(_T("www."));
	findAndColourAllOf(_T("ftp://"));
	findAndColourAllOf(_T("irc://"));
	findAndColourAllOf(_T("https://"));
	findAndColourAllOf(_T("mailto:"));
	findAndColourAllOf(_T("dchub://"));
	findAndColourAllOf(_T("adc://"));
	findAndColourAllOf(_T("magnet:?"));

	if (noScroll) {
		// fixme
		// scroll bar doesn't resize properly
		CRichEditCtrl::SetSel(0, 0);
		LineScroll(originalLinePos);
	} else {
		CRichEditCtrl::SetSel(-1, -1);
	}

	prepared = false;
}

void SortChat::FdmCRichEditCtrl::SetSel(long nStartChar, long nEndChar, BOOL notScroll) {
	if (notScroll && nStartChar == 0 && nEndChar == LineIndex(LineFromChar(2000))) {
		// okay dc++ is going to remove some text.
		int currentLinePos = GetFirstVisibleLine();
		int linesToBeRemoved = LineFromChar(2000);
		CRichEditCtrl::SetSel(nStartChar, nEndChar);
		CRichEditCtrl::ReplaceSel(_T(""));
		if (currentLinePos - linesToBeRemoved >= 0) {
			CRichEditCtrl::SetSel(0, 0);
			LineScroll(currentLinePos - linesToBeRemoved);
			ignoreNextAppend = true;
		}
	} else {
		CRichEditCtrl::SetSel(nStartChar, nEndChar);
	}
}

void SortChat::FdmCRichEditCtrl::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo) {
	if (!ignoreNextAppend) {
		CRichEditCtrl::ReplaceSel(lpszNewText, bCanUndo);
		ignoreNextAppend = false;
	}
}

void SortChat::FdmCRichEditCtrl::colourAndAppend(LPCTSTR textToAdd) {
	sizeForAppend = GetTextLengthEx();
    CRichEditCtrl::AppendText(textToAdd);
	colourText(sizeForAppend, -1);
}

void SortChat::FdmCRichEditCtrl::colourText(long startPos, long endPos) {
	CRichEditCtrl::SetSel(startPos, endPos);
	SetWordCharFormat(myBrush);
}

void SortChat::FdmCRichEditCtrl::findAndColourAllOf(tstring textToFind) {
	tstring::size_type startPos = 0;
	tstring::size_type finishPos = 0;
	while ((startPos = newText.find(textToFind, startPos)) != string::npos) {
		finishPos = newText.find_first_of(_T(" <\t\r\n"), startPos);
		colourText(startPos + sizeForAppend, (finishPos != string::npos ? finishPos + sizeForAppend : -1));
		startPos++;
	}
}

void SortChat::FdmCRichEditCtrl::prepareForAppend(OnlineUser* ou) {
	if (ou)
		prepareForAppend(ou->getIdentity().getNick(), ou->getIdentity().isOp(), ou->getIdentity().getIp(), false);
	else
		prepareForAppend(Util::emptyString, false, Util::emptyString, false);
}

void SortChat::FdmCRichEditCtrl::prepareForAppend(string nickOfSpeaker, bool opStatusOfSpeaker, string ipOfSpeaker, BOOL noscroll) {
	speakersNick = nickOfSpeaker;
	speakersOpStatus = opStatusOfSpeaker;
	speakersIP = ipOfSpeaker;
	noScroll = noscroll;
	prepared = true;

	// on the off chance this is set wrong
	ignoreNextAppend = false;
}