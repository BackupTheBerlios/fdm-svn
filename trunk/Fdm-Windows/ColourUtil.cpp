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

tstring SortChat::findNickInTString(const tstring aLine) {
	tstring::size_type i, j;
	if (((i = aLine.find_first_of('<')) != string::npos) && ((j = aLine.find_first_of('>')) != string::npos && j > i))
		return aLine.substr(i + 1, j - i - 1).c_str();
	return Util::emptyStringT;
}

SortChat::FdmCRichEditCtrl::FdmCRichEditCtrl() {
	myBrush.dwMask = CFM_COLOR;
	myBrush.dwEffects = 0;
	myBrush.crTextColor = WinUtil::textColor;

	timeStamps = BOOLSETTING(TIME_STAMPS);
	prepared = false;
	ignoreNextAppend = false;
};

void SortChat::FdmCRichEditCtrl::AppendText(LPCTSTR aLine) {
	// Just incase AppendText starts being used elsewhere DC++ code
	if (!prepared) {
		CRichEditCtrl::AppendText(aLine);
		return;
	}

	// LPCTSTR's . . . CString disabled . . . meh :P
	tstring newText = aLine;
	// Set this to 2, for the \r\n which is always added to the start
	tstring::size_type offSet = 2;

	// Need to know this later
	int originalLinePos = GetFirstVisibleLine();

	if (timeStamps)
		offSet += newText.find(']', 0);

	// Add begining text
	colourAndAppend(newText.substr(0, offSet).c_str(), GetTextLengthEx());

	if (speakersNick != Util::emptyString) {
		// Add Ip if known
		if (speakersIP != Util::emptyString)
			colourAndAppend((Text::toT("[ " + speakersIP + " ] ")).c_str(), GetTextLengthEx());

		// Change Brush Colour and Append <Nick>
		myBrush.crTextColor = speakersOpStatus ? StaticSettings::opSpoken : StaticSettings::notOpSpoken;
		colourAndAppend((Text::toT("<" + speakersNick + ">")).c_str(), GetTextLengthEx());

		// Change back to default brush colour, and offset to end of nick text
		myBrush.crTextColor = WinUtil::textColor;
		offSet = newText.find_first_of('>') + 1;
	}

	// Maybe colour rest of text differently
	if (myNick != Util::emptyString)
		if (myNick == speakersNick)
			myBrush.crTextColor = StaticSettings::iSpoke;
		else if (Text::toLower(newText).find(Text::toLower(Text::toT(myNick)), offSet) != string::npos)
			myBrush.crTextColor = StaticSettings::myNickSpoken;

	// Add rest of text
	long sizeBeforeAppend = GetTextLengthEx();
	colourAndAppend(newText.substr(offSet).c_str(), sizeBeforeAppend);

	// Check rest of text for colourable links
	CRichEditCtrl::SetSel(sizeBeforeAppend, -1);
	TCHAR *buf = new TCHAR[1 + GetTextLengthEx() - sizeBeforeAppend];
	GetSelText(buf);
	newText = buf;
	delete buf;

	myBrush.crTextColor = StaticSettings::doubleClickableLink;
	findAndColourDoubleClickable(newText, sizeBeforeAppend);

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
		// do it for it, and try and preserve scroll position
		int currentLinePos = GetFirstVisibleLine();
		int linesToBeRemoved = LineFromChar(2000);
		CRichEditCtrl::SetSel(nStartChar, nEndChar);
		CRichEditCtrl::ReplaceSel(_T(""));

		// scroll if possible
		if (currentLinePos - linesToBeRemoved >= 0) {
			CRichEditCtrl::SetSel(0, 0);
			LineScroll(currentLinePos - linesToBeRemoved);
		}

		// since already been removed
		ignoreNextAppend = true;
	} else {
		// normal selection
		CRichEditCtrl::SetSel(nStartChar, nEndChar);
	}
}

void SortChat::FdmCRichEditCtrl::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo) {
	if (!ignoreNextAppend) {
		CRichEditCtrl::ReplaceSel(lpszNewText, bCanUndo);
		ignoreNextAppend = false;
	}
}

void SortChat::FdmCRichEditCtrl::colourText(long startPos, long endPos) {
	CRichEditCtrl::SetSel(startPos, endPos);
	SetWordCharFormat(myBrush);
}

void SortChat::FdmCRichEditCtrl::colourAndAppend(LPCTSTR textToAdd, long colourFrom) {
    CRichEditCtrl::AppendText(textToAdd);
	colourText(colourFrom, -1);
}

static const tstring doubleClickableLinks[] = {
	_T("http://"), _T("https://"), _T("www."), _T("ftp://"), _T("mailto:"), 
	_T("irc://"), _T("dchub://"), _T("adc://"), _T("magnet:?"), Util::emptyStringT
};

void SortChat::FdmCRichEditCtrl::findAndColourDoubleClickable(const tstring& lookIn, long sizeBeforeAppend) {
	tstring::size_type startPos, finishPos;
	for (int i = 0; doubleClickableLinks[i] != Util::emptyStringT; i++) {
		startPos = 0;
		finishPos = 0;
		while ((startPos = lookIn.find(doubleClickableLinks[i], startPos)) != string::npos) {
			finishPos = lookIn.find_first_of(_T(" <\t\r\n"), startPos);
			colourText(startPos + sizeBeforeAppend, (finishPos != string::npos ? finishPos + sizeBeforeAppend : -1));
			startPos++;
		}
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
	myBrush.crTextColor = WinUtil::textColor;
	noScroll = noscroll;
	prepared = true;
}