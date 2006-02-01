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

#if !defined(FDM_WIN_UTIL_H)
#define FDM_WIN_UTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../client/Util.h"
#include "../../client/SettingsManager.h"
#include "../../client/User.h"
#include "../../client/MerkleTree.h"

#include "Fdm-FlatTabCtrl.h"

class FdmFlatTabCtrl;

template<class T, int title>
class FdmStaticFrame {
public:
	virtual ~FdmStaticFrame() { frame = NULL; };

	static T* frame;
	static void openWindow() {
		if(frame == NULL) {
			frame = new T();
			frame->CreateEx(FdmWinUtil::fdmMdiClient, frame->rcDefault, FDMCTSTRING_I(FdmResourceManager::FdmStrings(title)));
		} else {
			// match the behavior of MainFrame::onSelected()
			HWND hWnd = frame->m_hWnd;
			if(frame->MDIGetActive() != hWnd) {
				frame->MDIActivate(hWnd);
			} else if(BOOLSETTING(TOGGLE_ACTIVE_WINDOW)) {
				::SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
				frame->MDINext(hWnd);
				hWnd = frame->MDIGetActive();
			}
			if(::IsIconic(hWnd))
				::ShowWindow(hWnd, SW_RESTORE);
		}
	}
};

template<class T, int title>
T* FdmStaticFrame<T, title>::frame = NULL;

class FdmWinUtil {
public:
	static CMenu fdmMainMenu;
	static HWND fdmMainWnd;
	static HWND fdmMdiClient;
	static HFONT fdmFont;
	static int fdmFontHeight;
	static HFONT fdmBoldFont;
	static FdmFlatTabCtrl* fdmTabCtrl;
	static HHOOK fdmHook;
	static void init(HWND hWnd);
	static void uninit();

	static int getTextHeight(HWND wnd, HFONT fnt) {
		HDC dc = ::GetDC(wnd);
		int h = getTextHeight(dc, fnt);
		::ReleaseDC(wnd, dc);
		return h;
	}

	static int getTextHeight(HDC dc, HFONT fnt) {
		HGDIOBJ old = ::SelectObject(dc, fnt);
		int h = getTextHeight(dc);
		::SelectObject(dc, old);
		return h;
	}

	static int getTextHeight(HDC dc) {
		TEXTMETRIC tm;
		::GetTextMetrics(dc, &tm);
		return tm.tmHeight;
	}

	template<class T> static HWND hiddenCreateEx(T& p) throw() {
		HWND active = (HWND)::SendMessage(fdmMdiClient, WM_MDIGETACTIVE, 0, 0);
		::LockWindowUpdate(fdmMdiClient);
		HWND ret = p.CreateEx(fdmMdiClient);
		if(active && ::IsWindow(active))
			::SendMessage(fdmMdiClient, WM_MDIACTIVATE, (WPARAM)active, 0);
		::LockWindowUpdate(0);
		return ret;
	}
	template<class T> static HWND hiddenCreateEx(T* p) throw() {
		return hiddenCreateEx(*p);
	}

private:
	static int CALLBACK browseCallbackProc(HWND hwnd, UINT uMsg, LPARAM /*lp*/, LPARAM pData);		
	
};

#endif // !defined(FDM_WIN_UTIL_H)

/**
 * @file
 * $Id: WinUtil.h,v 1.52 2006/01/23 08:00:50 arnetheduck Exp $
 */
