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

#include "RightClick.h"
#include "../windows/WinUtil.h"

LRESULT FdmRightClick::onCopyChat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	WinUtil::setClipboard(selectedText);
	return 0;
}

void FdmRightClick::createCopyMenu(CMenu& chatMenu) {
	chatMenu.CreatePopupMenu();
	chatMenu.AppendMenu(MF_STRING, IDC_COPY_CHAT, FDMCTSTRING(COPY));
}

BOOL FdmRightClick::useCopyMenu(CMenu& chatMenu, POINT& pt, HWND m_hWnd, tstring aSelectedText) {
	chatMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd);
	selectedText = aSelectedText;
	return TRUE;
}