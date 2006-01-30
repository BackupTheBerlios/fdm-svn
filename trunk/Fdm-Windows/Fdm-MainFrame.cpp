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

#include "../Windows/stdafx.h"
#include "../client/DCPlusPlus.h"
#include "resource.h"

#include "Fdm-MainFrame.h"
#include "../Windows/WinUtil.h"

FdmMainFrame::FdmMainFrame() { 
};


FdmMainFrame::~FdmMainFrame() {
	m_CmdBar.m_hImageList = NULL;

	images.Destroy();
	largeImages.Destroy();
	largeImagesHot.Destroy();
}

void FdmMainFrame::extraToolBarCommmands(CMDICommandBarCtrl& m_CmdBar) {
	m_CmdBar.m_arrCommand.Add(ID_FDM_FILE_SETTINGS);
	m_CmdBar.m_arrCommand.Add(ID_FDM_TEST_FRAME);
}

LRESULT FdmMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

	m_hMenu = WinUtil::mainMenu;

	// attach menu
	m_CmdBar.AttachMenu(m_hMenu);
	// load command bar images
	images.CreateFromImage(IDB_FDM_TOOLBAR20, 16, 16, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);
	m_CmdBar.m_hImageList = images;

	FdmMainFrame::extraToolBarCommmands(m_CmdBar);

	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = createFdmToolbar();

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	
	// Carraya test extra toolbar
//	HWND hWndFdmToolBar = createFdmToolbar();
//	AddSimpleReBarBand(hWndFdmToolBar, NULL, TRUE);
	// We want to pass this one on to the splitter...hope it get's there...

	bHandled = FALSE;
	return 0;
}

// Carraya test extra toolbar
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
