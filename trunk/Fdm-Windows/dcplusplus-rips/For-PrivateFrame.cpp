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

#include "../../windows/PrivateFrame.h"

LRESULT PrivateFrame::onContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }; 
	
	if(reinterpret_cast<HWND>(wParam) == ctrlClient) {
		if(pt.x == -1 && pt.y == -1) WinUtil::getContextMenuPos(ctrlClient, pt);
		if (ctrlClient.GetSelSize() > 0) return FdmRightClick::useCopyMenu(chatMenu, pt, m_hWnd, ctrlClient.GetSelectedText()); 
	}
	return FALSE;
}