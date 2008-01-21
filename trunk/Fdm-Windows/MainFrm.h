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

#if !defined(FDM_MAINFRAME_H)
#define FDM_MAINFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Other-Projects/DC++/client/ResourceManager.h"
#include "../Fdm-Windows/resource.h"

class FdmMainFrame {
public:
	BEGIN_MSG_MAP(FdmMainFrame)
		COMMAND_ID_HANDLER(IDD_FDM_ABOUT, OnFdmAppAbout)
		COMMAND_ID_HANDLER(ID_FDM_FILE_SETTINGS, OnFdmFileSettings)
		COMMAND_ID_HANDLER(ID_FDM_NOTEPAD_FRAME, OnFdmNotepadFrame)
		COMMAND_ID_HANDLER(ID_AUTO_SEARCH_FRAME, OnAutoSearchFrame)
	END_MSG_MAP()

	LRESULT OnFdmAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFdmFileSettings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFdmNotepadFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAutoSearchFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	static void destroyFdmToolbar(CImageList& fdmLargeImages, CImageList& fdmLargeImagesHot);
	static HWND createFdmToolbar(CImageList& fdmLargeImages, CImageList& fdmLargeImagesHot);
	static void fdmToolTips(int& idCtrl, LPNMTTDISPINFO& pDispInfo, int& stringId);
};

#endif // !defined(FDM_MAINFRAME_H)

/**
 * @file
 * $Id: MainFrm.h,v 1.66 2006/01/23 08:00:50 arnetheduck Exp $
 */