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

#include "stdafx.h"
#include "../../client/DCPlusPlus.h"
#include "../resource.h"

#include "Fdm-NotepadFrame.h"
#include "../../windows/WinUtil.h"
#include "../../client/File.h"
#include "../../Fdm-Client/dcplusplus-rips/Fdm-SettingsManager.h"

LRESULT FdmNotepadFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	ctrlPad.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);
	
	ctrlPad.LimitText(0);
	ctrlPad.SetFont(WinUtil::font);
	string tmp;
	try {
		tmp = File(getFdmNotepadFile(), File::READ, File::OPEN).read();
	} catch(const FileException&) {
		// ...
	}
	
	ctrlPad.SetWindowText(Text::toT(tmp).c_str());
	ctrlPad.EmptyUndoBuffer();
	ctrlClientContainer.SubclassWindow(ctrlPad.m_hWnd);

	bHandled = FALSE;
	return 1;
}

LRESULT FdmNotepadFrame::onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {

	if(ctrlPad.GetModify()) {
		AutoArray<TCHAR> buf(ctrlPad.GetWindowTextLength() + 1);
		ctrlPad.GetWindowText(buf, ctrlPad.GetWindowTextLength() + 1);
		try {
			string tmp(Text::fromT(tstring(buf, ctrlPad.GetWindowTextLength())));
			File(getFdmNotepadFile(), File::WRITE, File::CREATE | File::TRUNCATE).write(tmp);
		} catch(const FileException&) {
			// Oops...
		}
	}

	bHandled = FALSE;
	return 0;
	
}

void FdmNotepadFrame::UpdateLayout(BOOL /*bResizeBars*/ /* = TRUE */)
{
	CRect rc;

	GetClientRect(rc);
	
	rc.bottom -= 1;
	rc.top += 1;
	rc.left +=1;
	rc.right -=1;
	ctrlPad.MoveWindow(rc);
	
}

LRESULT FdmNotepadFrame::onLButton(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) {
	HWND focus = GetFocus();
	bHandled = false;
	if(focus == ctrlPad.m_hWnd) {
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		tstring x;
		tstring::size_type start = (tstring::size_type)WinUtil::textUnderCursor(pt, ctrlPad, x);
		tstring::size_type end = x.find(_T(" "), start);

		if(end == string::npos)
			end = x.length();
		
		bHandled = WinUtil::parseDBLClick(x, start, end);
	}
	return 0;
}
