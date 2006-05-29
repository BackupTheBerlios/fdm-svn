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
#include "../../Fdm-Client/dcplusplus-rips/Fdm-SettingsManager.h"

#include "Fdm-MainFrm.h"
#include "Fdm-PropertiesDlg.h"
#include "Fdm-NotepadFrame.h"

#include "../../windows/WinUtil.h"

void FdmMainFrame::destroyFdmMainFrame(CImageList& fdmLargeImages, CImageList& fdmLargeImagesHot) {
	fdmLargeImages.Destroy();
	fdmLargeImagesHot.Destroy();
}

HWND FdmMainFrame::createFdmToolbar(CImageList& fdmLargeImages, CImageList& fdmLargeImagesHot) {
	CToolBarCtrl ctrlToolbar;
	fdmLargeImages.CreateFromImage(IDB_FDM_TOOLBAR20, 20, 15, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);
	fdmLargeImagesHot.CreateFromImage(IDB_FDM_TOOLBAR20_HOT, 20, 15, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);

	ctrlToolbar.Create(WinUtil::mainWnd, NULL, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 0, ATL_IDW_TOOLBAR);
	ctrlToolbar.SetImageList(fdmLargeImages);
	ctrlToolbar.SetHotImageList(fdmLargeImagesHot);

	const int numButtons = 5;

	TBBUTTON tb[numButtons];
	memset(tb, 0, sizeof(tb));
	int n = 0, bitmap = 0;

	tb[n].iBitmap = bitmap++;
	tb[n].idCommand = ID_FDM_FILE_SETTINGS;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;


	n++;
	tb[n].fsStyle = TBSTYLE_SEP;

	n++;
	tb[n].iBitmap = bitmap++;
	tb[n].idCommand = ID_FDM_FAVHUB_FRAME;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;


	n++;
	tb[n].fsStyle = TBSTYLE_SEP;

	n++;
	tb[n].iBitmap = bitmap++;
	tb[n].idCommand = ID_FDM_NOTEPAD_FRAME;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

	ctrlToolbar.SetButtonStructSize();
	ctrlToolbar.AddButtons(numButtons, tb);
	ctrlToolbar.AutoSize();

	return ctrlToolbar.m_hWnd;
}

void FdmMainFrame::fdmToolTips(int& idCtrl, LPNMTTDISPINFO& pDispInfo, int& stringId) {
	switch(idCtrl) {
		case ID_FDM_FILE_SETTINGS: stringId = FdmResourceManager::MENU_FDM_SETTINGS; break;
		case ID_FDM_NOTEPAD_FRAME: stringId = FdmResourceManager::MENU_NOTEPAD_FRAME; break;
	}

	if (stringId != -1) {
		_tcsncpy(pDispInfo->lpszText, FDMCTSTRING_I((FdmResourceManager::FdmStrings)stringId), 79); 
		pDispInfo->uFlags |= TTF_DI_SETITEM;
	}

	stringId = -1;
}

LRESULT FdmMainFrame::OnFdmFileSettings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FdmPropertiesDlg dlg(WinUtil::mainWnd, FdmSettingsManager::getInstance());

	if(dlg.DoModal(WinUtil::mainWnd) == IDOK)
		SettingsManager::getInstance()->save();
	return 0;
}

LRESULT FdmMainFrame::OnFdmNotepadFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FdmNotepadFrame::openWindow();
	return 0;
}

LRESULT FdmMainFrame::OnFdmFavHubFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FdmNotepadFrame::openWindow();
	return 0;
}
/**
 * @file
 * $Id: MainFrm.cpp,v 1.109 2006/01/23 08:00:50 arnetheduck Exp $
 */
