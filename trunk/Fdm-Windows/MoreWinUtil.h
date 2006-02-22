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

#if !defined(MORE_WIN_UTIL_H)
#define MORE_WIN_UTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dcplusplus-rips\Fdm-MainFrm.h"

namespace MoreWinUtil
{
	// For allowing for instances
	bool allowMoreInstances(size_t amountOfProcesses);

	// Taking over MainFrame
	void createFdmMainFrameAndAttachToSplitter(FdmMainFrame& fdmMainFrame, CHorSplitterWindow& splitFdmMainFrame, HWND& m_hWnd, const _U_RECT& rcDefault);
	void calculateAndSetToolBarHeight(CToolBarCtrl& ctrlToolbar);
	void sortMainFrameUpdateLayout(CHorSplitterWindow& splitFdmMainFrame, CMDICommandBarCtrl& commandBar, RECT& rect);

	static int mainFrameToolBarSize;
};

#endif // !defined(MORE_WIN_UTIL_H)