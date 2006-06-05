/*
 * Copyright (C) 2001-2006 Jacek Sieka, arnetheduck on gmail point com
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
#include "../../client/DCPlusPlus.h"

#include "../../windows/WinUtil.h"

int WinUtil::textUnderCursor(POINT p, CRichEditCtrl& ctrl, tstring& x) {
	
	int i = ctrl.CharFromPos(p);
	int line = ctrl.LineFromChar(i);
	int c = LOWORD(i) - ctrl.LineIndex(line);
	int len = ctrl.LineLength(i) + 1;
	if(len < 3) {
		return 0;
	}

	AutoArray<TCHAR> buf(len);
	ctrl.GetLine(line, buf, len);
	x = tstring(buf, len-1);

	string::size_type start = x.find_last_of(_T(" <\t\r\n"), c);
	if(start == string::npos)
		start = 0;
	else
		start++;

	return start;
}

void WinUtil::getContextMenuPos(CRichEditCtrl& aEdit, POINT& aPt) {
	CRect erc;
	aEdit.GetRect(&erc);
	aPt.x = erc.Width() / 2;
	aPt.y = erc.Height() / 2;
	aEdit.ClientToScreen(&aPt);
}