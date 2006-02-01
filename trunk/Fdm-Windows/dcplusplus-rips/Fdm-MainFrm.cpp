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
#include "../Resource.h"
#include "../../windows/Resource.h"

#include "Fdm-MainFrm.h"
#include "../../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"

#include "../Fdm-Dlg.h"
#include "Fdm-NotepadFrame.h"

FdmMainFrame::~FdmMainFrame() {
	m_CmdBar.m_hImageList = NULL;

	images.Destroy();
	largeImages.Destroy();
	largeImagesHot.Destroy();
}

LRESULT FdmMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	ctrlPad.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
	WS_HSCROLL | WS_VSCROLL | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS, WS_EX_CLIENTEDGE);
	
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

	// attach menu
	m_CmdBar.AttachMenu(m_hMenu);
	m_CmdBar.m_arrCommand.Add(ID_FDM_FILE_SETTINGS);
	m_CmdBar.m_arrCommand.Add(ID_FDM_TEST_FRAME);

	// FIXME
	// load command bar images
	images.CreateFromImage(IDB_TOOLBAR, 16, 16, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);
	m_CmdBar.m_hImageList = images;

	HWND hWndToolBar = createFdmToolbar();
	
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	bHandled = FALSE;
	return 1;
}

LRESULT FdmMainFrame::onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	bHandled = FALSE;
	return 0;
	
}

void FdmMainFrame::UpdateLayout(BOOL /*bResizeBars*/ /* = TRUE */)
{
	CRect rc;

	GetClientRect(rc);
	
	rc.bottom -= 1;
	rc.top += 1;
	rc.left +=1;
	rc.right -=1;
	ctrlPad.MoveWindow(rc);	
}

LRESULT FdmMainFrame::onGetToolTip(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/) {
	LPNMTTDISPINFO pDispInfo = (LPNMTTDISPINFO)pnmh;
	pDispInfo->szText[0] = 0;

	if((idCtrl != 0) && !(pDispInfo->uFlags & TTF_IDISHWND))
	{
		int stringId = -1;
		switch(idCtrl) {
			case ID_FDM_FILE_SETTINGS: stringId = FdmResourceManager::MENU_FDM_SETTINGS; break;
			case ID_FDM_TEST_FRAME: stringId = FdmResourceManager::MENU_TEST_FRAME; break;
		}
		if(stringId != -1) {
			_tcsncpy(pDispInfo->lpszText, FDMCTSTRING_I((FdmResourceManager::FdmStrings)stringId), 79);
			pDispInfo->uFlags |= TTF_DI_SETITEM;
		}
	}
	return 0;
}

HWND FdmMainFrame::createFdmToolbar() {
	CToolBarCtrl ctrlToolbar;
	largeImages.CreateFromImage(IDB_FDM_TOOLBAR20, 20, 15, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);
	largeImagesHot.CreateFromImage(IDB_FDM_TOOLBAR20_HOT, 20, 15, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);

	ctrlToolbar.Create(m_hWnd, NULL, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 0, ATL_IDW_TOOLBAR);
	ctrlToolbar.SetImageList(largeImages);
	ctrlToolbar.SetHotImageList(largeImagesHot);

	const int numButtons = 3;

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
	tb[n].idCommand = ID_FDM_TEST_FRAME;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

	ctrlToolbar.SetButtonStructSize();
	ctrlToolbar.AddButtons(numButtons, tb);
	ctrlToolbar.AutoSize();

	return ctrlToolbar.m_hWnd;
}

LRESULT FdmMainFrame::OnFdmFileSettings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FdmDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT FdmMainFrame::OnFdmTestFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FdmNotepadFrame::openWindow();
	return 0;
}
/**
 * @file
 * $Id: MainFrm.cpp,v 1.109 2006/01/23 08:00:50 arnetheduck Exp $
 */
