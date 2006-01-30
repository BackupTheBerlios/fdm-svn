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
#include "../Windows/resource.h"  // bad ?? probably

#include "../Fdm-Client/dcplusplus-rips/Fdm-Version.h"
#include "Fdm-WinUtil.h"

#include "../Fdm-Windows/Fdm-Dlg.h"
#include "../Fdm-Windows/Fdm-NotepadFrame.h"

bool FdmWinUtil::allowMoreInstances(size_t amountOfProcesses) {
	if(amountOfProcesses == 0)
		if (::MessageBox(NULL, _T("There is already an instance of ") _T(FDMAPPNAME) _T(" running.\nDo you want to launch another instance anyway?"), _T(FDMAPPNAME) _T(" ") _T(FDMVERSIONSTRING), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST) == IDYES)
			return true;
	return false;
}

void FdmWinUtil::extraToolBarCommmands(CMDICommandBarCtrl& m_CmdBar) {
	m_CmdBar.m_arrCommand.Add(ID_FDM_FILE_SETTINGS);
	m_CmdBar.m_arrCommand.Add(ID_FDM_TEST_FRAME);
}

HWND FdmWinUtil::createFdmToolbar(HWND& m_hWnd, CImageList& largeImages, CImageList largeImagesHot) {
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

LRESULT FdmWinUtil::OnFdmFileSettings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FdmDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT FdmWinUtil::OnFdmTestFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	FdmNotepadFrame::openWindow();
	return 0;
}