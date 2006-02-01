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

#include "Fdm-stdafx.h"
#include "../../client/DCPlusPlus.h"
#include "../resource.h"

#include "Fdm-WinUtil.h"

#include "../../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"
#include "../../Fdm-Client/dcplusplus-rips/Fdm-Version.h"

HWND FdmWinUtil::fdmMainWnd = NULL;
HWND FdmWinUtil::fdmMdiClient = NULL;
FdmFlatTabCtrl* FdmWinUtil::fdmTabCtrl = NULL;
HHOOK FdmWinUtil::fdmHook = NULL;
HFONT FdmWinUtil::fdmFont = NULL;
int FdmWinUtil::fdmFontHeight = 0;
HFONT FdmWinUtil::fdmBoldFont = NULL;
CMenu FdmWinUtil::fdmMainMenu;

static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
	if(code == HC_ACTION) {
		if(wParam == VK_CONTROL && LOWORD(lParam) == 1) {
			if(lParam & 0x80000000) {
				FdmWinUtil::fdmTabCtrl->endFdmSwitch();
			} else {
				FdmWinUtil::fdmTabCtrl->startFdmSwitch();
			}
		}
	}
	return CallNextHookEx(FdmWinUtil::fdmHook, code, wParam, lParam);
}


void FdmWinUtil::init(HWND hWnd) {
	fdmMainWnd = hWnd;

	fdmMainMenu.CreateMenu();

	CMenuHandle file;
	file.CreatePopupMenu();
/*
	file.AppendMenu(MF_STRING, IDC_OPEN_FILE_LIST, CTSTRING(MENU_OPEN_FILE_LIST));
	file.AppendMenu(MF_STRING, IDC_OPEN_OWN_LIST, CTSTRING(MENU_OPEN_OWN_LIST));
	file.AppendMenu(MF_STRING, IDC_REFRESH_FILE_LIST, CTSTRING(MENU_REFRESH_FILE_LIST));
	file.AppendMenu(MF_STRING, IDC_OPEN_DOWNLOADS, CTSTRING(MENU_OPEN_DOWNLOADS_DIR));
	file.AppendMenu(MF_SEPARATOR);
	file.AppendMenu(MF_STRING, ID_FILE_QUICK_CONNECT, CTSTRING(MENU_QUICK_CONNECT));
	file.AppendMenu(MF_STRING, IDC_FOLLOW, CTSTRING(MENU_FOLLOW_REDIRECT));
	file.AppendMenu(MF_STRING, ID_FILE_RECONNECT, CTSTRING(MENU_RECONNECT));
	file.AppendMenu(MF_SEPARATOR);
	file.AppendMenu(MF_STRING, ID_FILE_SETTINGS, CTSTRING(MENU_SETTINGS));
	file.AppendMenu(MF_SEPARATOR);
	file.AppendMenu(MF_STRING, ID_APP_EXIT, CTSTRING(MENU_EXIT));
*/
	fdmMainMenu.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)file, FDMCTSTRING(FDM_MENU_FILE));

	CMenuHandle view;
	view.CreatePopupMenu();
/*
	view.AppendMenu(MF_STRING, ID_FILE_CONNECT, CTSTRING(MENU_PUBLIC_HUBS));
	view.AppendMenu(MF_STRING, IDC_QUEUE, CTSTRING(MENU_DOWNLOAD_QUEUE));
	view.AppendMenu(MF_STRING, IDC_VIEW_WAITING_USERS, CTSTRING(WAITING_USERS));
	view.AppendMenu(MF_STRING, IDC_FINISHED, CTSTRING(FINISHED_DOWNLOADS));
	view.AppendMenu(MF_STRING, IDC_FINISHED_UL, CTSTRING(FINISHED_UPLOADS));
	view.AppendMenu(MF_STRING, IDC_FAVORITES, CTSTRING(MENU_FAVORITE_HUBS));
	view.AppendMenu(MF_STRING, IDC_FAVUSERS, CTSTRING(MENU_FAVORITE_USERS));
	view.AppendMenu(MF_STRING, ID_FILE_SEARCH, CTSTRING(MENU_SEARCH));
	view.AppendMenu(MF_STRING, IDC_FILE_ADL_SEARCH, CTSTRING(MENU_ADL_SEARCH));
	view.AppendMenu(MF_STRING, IDC_SEARCH_SPY, CTSTRING(MENU_SEARCH_SPY));
	view.AppendMenu(MF_STRING, IDC_NET_STATS, CTSTRING(MENU_NETWORK_STATISTICS));
	view.AppendMenu(MF_STRING, IDC_NOTEPAD, CTSTRING(MENU_NOTEPAD));
	view.AppendMenu(MF_STRING, IDC_HASH_PROGRESS, CTSTRING(MENU_HASH_PROGRESS));
	view.AppendMenu(MF_STRING, IDC_SYSTEM_LOG, CTSTRING(MENU_SYSTEM_LOG));
	view.AppendMenu(MF_SEPARATOR);
	view.AppendMenu(MF_STRING, ID_VIEW_TOOLBAR, CTSTRING(MENU_TOOLBAR));
	view.AppendMenu(MF_STRING, ID_VIEW_STATUS_BAR, CTSTRING(MENU_STATUS_BAR));
	view.AppendMenu(MF_STRING, ID_VIEW_TRANSFER_VIEW, CTSTRING(MENU_TRANSFER_VIEW));
*/
	fdmMainMenu.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)view, FDMCTSTRING(FDM_MENU_VIEW));

	CMenuHandle window;
	window.CreatePopupMenu();
/*
	window.AppendMenu(MF_STRING, ID_WINDOW_CASCADE, CTSTRING(MENU_CASCADE));
	window.AppendMenu(MF_STRING, ID_WINDOW_TILE_HORZ, CTSTRING(MENU_HORIZONTAL_TILE));
	window.AppendMenu(MF_STRING, ID_WINDOW_TILE_VERT, CTSTRING(MENU_VERTICAL_TILE));
	window.AppendMenu(MF_STRING, ID_WINDOW_ARRANGE, CTSTRING(MENU_ARRANGE));
	window.AppendMenu(MF_STRING, ID_WINDOW_MINIMIZE_ALL, CTSTRING(MENU_MINIMIZE_ALL));
	window.AppendMenu(MF_STRING, ID_WINDOW_RESTORE_ALL, CTSTRING(MENU_RESTORE_ALL));
	window.AppendMenu(MF_SEPARATOR);
	window.AppendMenu(MF_STRING, IDC_CLOSE_DISCONNECTED, CTSTRING(MENU_CLOSE_DISCONNECTED));
*/
	fdmMainMenu.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)window, FDMCTSTRING(FDM_MENU_WINDOW));

	CMenuHandle help;
	help.CreatePopupMenu();
/*
	help.AppendMenu(MF_STRING, IDC_HELP_CONTENTS, CTSTRING(MENU_CONTENTS));
	help.AppendMenu(MF_SEPARATOR);
	help.AppendMenu(MF_STRING, IDC_HELP_CHANGELOG, CTSTRING(MENU_CHANGELOG));
	help.AppendMenu(MF_STRING, ID_APP_ABOUT, CTSTRING(MENU_ABOUT));
	help.AppendMenu(MF_SEPARATOR);
	help.AppendMenu(MF_STRING, IDC_HELP_HOMEPAGE, CTSTRING(MENU_HOMEPAGE));
	help.AppendMenu(MF_STRING, IDC_HELP_DOWNLOADS, CTSTRING(MENU_HELP_DOWNLOADS));
	help.AppendMenu(MF_STRING, IDC_HELP_GEOIPFILE, CTSTRING(MENU_HELP_GEOIPFILE));
	help.AppendMenu(MF_STRING, IDC_HELP_TRANSLATIONS, CTSTRING(MENU_HELP_TRANSLATIONS));
	help.AppendMenu(MF_STRING, IDC_HELP_FAQ, CTSTRING(MENU_FAQ));
	help.AppendMenu(MF_STRING, IDC_HELP_HELP_FORUM, CTSTRING(MENU_HELP_FORUM));
	help.AppendMenu(MF_STRING, IDC_HELP_DISCUSS, CTSTRING(MENU_DISCUSS));
	help.AppendMenu(MF_STRING, IDC_HELP_REQUEST_FEATURE, CTSTRING(MENU_REQUEST_FEATURE));
	help.AppendMenu(MF_STRING, IDC_HELP_REPORT_BUG, CTSTRING(MENU_REPORT_BUG));
	help.AppendMenu(MF_STRING, IDC_HELP_DONATE, CTSTRING(MENU_DONATE));
*/
	fdmMainMenu.AppendMenu(MF_POPUP, (UINT_PTR)(HMENU)help, FDMCTSTRING(FDM_MENU_HELP));

	LOGFONT lf;
	fdmFont = ::CreateFontIndirect(&lf);
	fdmFontHeight = FdmWinUtil::getTextHeight(fdmMainWnd, fdmFont);
	lf.lfWeight = FW_BOLD;
	fdmBoldFont = ::CreateFontIndirect(&lf);

	fdmHook = SetWindowsHookEx(WH_KEYBOARD, &KeyboardProc, NULL, GetCurrentThreadId());
}

void FdmWinUtil::uninit() {
	fdmMainMenu.DestroyMenu();

	::DeleteObject(fdmFont);
	::DeleteObject(fdmBoldFont);

	UnhookWindowsHookEx(fdmHook);
}

int CALLBACK FdmWinUtil::browseCallbackProc(HWND hwnd, UINT uMsg, LPARAM /*lp*/, LPARAM pData) {
	switch(uMsg) {
	case BFFM_INITIALIZED: 
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
		break;
	}
	return 0;
}

/**
 * @file
 * $Id: FdmWinUtil.cpp,v 1.95 2006/01/06 14:44:32 arnetheduck Exp $
 */