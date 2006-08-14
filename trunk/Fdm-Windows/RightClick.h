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

#if !defined(FDM_RIGHT_CLICK_H)
#define FDM_RIGHT_CLICK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class FdmRightClick {
public:
	BEGIN_MSG_MAP(RightClick)
		COMMAND_ID_HANDLER(IDC_COPY_CHAT, onCopyChat)
	END_MSG_MAP()

	LRESULT onCopyChat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void createCopyMenu(CMenu& chatMenu);
	BOOL useCopyMenu(CMenu& chatMenu, POINT& pt, HWND m_hWnd, tstring aSelectedText);

private:
	tstring selectedText;
};

#endif // !defined(FDM_RIGHT_CLICK_H)