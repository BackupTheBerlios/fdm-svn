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

#include "Fdm-stdafx.h"
#include "../client/DCPlusPlus.h"
#include "resource.h"

#include "../Fdm-Client/dcplusplus-rips/Fdm-Version.h"
#include "MoreWinUtil.h"

bool MoreWinUtil::allowMoreInstances(size_t amountOfProcesses) {
	if(amountOfProcesses == 0)
		if (::MessageBox(NULL, _T("There is already an instance of ") _T(FDMAPPNAME) _T(" running.\nDo you want to launch another instance anyway?"), _T(FDMAPPNAME) _T(" ") _T(FDMVERSIONSTRING), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST) == IDYES)
			return true;
	return false;
}

int MoreWinUtil::calculateMainFrameSize(CMDICommandBarCtrl& commandBar) {
	CRect commandBarRect;
	commandBar.GetClientRect(commandBarRect);
	int commandBarSize = commandBarRect.Height();
	return 10 + commandBarSize + commandBarSize + mainFrameToolBarSize;
}

int MoreWinUtil::calculateToolBarHeight(CToolBarCtrl& ctrlToolbar) {
	CRect toolBarRect;
	ctrlToolbar.GetItemRect(0, toolBarRect);
	return toolBarRect.Height();
}

CRect MoreWinUtil::makeRectangle(int top, int bottom, int left, int right) {
	CRect rectForFdmMainFrame;
	rectForFdmMainFrame.top = top;
	rectForFdmMainFrame.bottom = bottom;
	rectForFdmMainFrame.left = left;
	rectForFdmMainFrame.right = right;
	return rectForFdmMainFrame;
}

void MoreWinUtil::setMainFrameToolBarSize(int aSize) { 
	MoreWinUtil::mainFrameToolBarSize = aSize; 
}