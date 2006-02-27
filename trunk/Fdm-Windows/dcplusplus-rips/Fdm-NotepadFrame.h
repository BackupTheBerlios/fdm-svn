/*
 * Copyright (C) 2001-2005 Jacek Sieka, arnetheduck on gmail point com
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

#if !defined(FDM_NOTEPAD_FRAME_H)
#define FDM_NOTEPAD_FRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"
#include "../windows/FlatTabCtrl.h"
#include "../../windows/resource.h"
#include "../../windows/WinUtil.h"
#include "../ColourUtil.h"

#define FDM_NOTEPAD_MESSAGE_MAP 13

class FdmNotepadFrame : public MDITabChildWindowImpl<FdmNotepadFrame>, public StaticFrame<FdmNotepadFrame, FdmResourceManager::FDM_NOTEPAD, true>
{
public:
	DECLARE_FRAME_WND_CLASS_EX(_T("FdmNotepadFrame"), IDR_FDM_NOTEPAD, 0, COLOR_3DFACE);

	FdmNotepadFrame() : dirty(false),
		ctrlClientContainer(_T("edit"), this, FDM_NOTEPAD_MESSAGE_MAP) { }
	virtual ~FdmNotepadFrame() { }
	
	typedef MDITabChildWindowImpl<FdmNotepadFrame> baseClass;
	BEGIN_MSG_MAP(FdmNotepadFrame)
		MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, onClose)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, onCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, onCtlColor)
		CHAIN_MSG_MAP(baseClass)
	ALT_MSG_MAP(FDM_NOTEPAD_MESSAGE_MAP)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, onLButton)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT onLButton(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void UpdateLayout(BOOL bResizeBars = TRUE);
	
	LRESULT onCtlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		HWND hWnd = (HWND)lParam;
		HDC hDC = (HDC)wParam;
		if(hWnd == ctrlPad.m_hWnd) {
			::SetBkColor(hDC, ColourUtil::m_ChatTextGeneral.crBackColor);
			::SetTextColor(hDC, ColourUtil::m_ChatTextGeneral.crTextColor);
			return (LRESULT)ColourUtil::bgBrush;
		}
		bHandled = FALSE;
		return FALSE;
	};
	
	
	LRESULT OnFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		ctrlPad.SetFocus();
		return 0;
	}
	
private:
	
	bool dirty;
	CEdit ctrlPad;
	CContainedWindow ctrlClientContainer;
};

#endif // !defined(FDM_NOTEPAD_FRAME_H)

/**
 * @file
 * $Id: NotepadFrame.h,v 1.15 2005/11/12 10:23:02 arnetheduck Exp $
 */