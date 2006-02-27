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

template<class T>
class FdmMainFrameToolBar {
public:
	BEGIN_MSG_MAP(FdmMainFrameToolBar)
		COMMAND_ID_HANDLER(ID_FDM_FILE_SETTINGS, FdmMainFrame::OnFdmFileSettings)
		COMMAND_ID_HANDLER(ID_FDM_TEST_FRAME, FdmMainFrame::OnFdmTestFrame)
	END_MSG_MAP()
};

namespace MoreWinUtil
{
	// For allowing for instances
	bool allowMoreInstances(size_t amountOfProcesses);
};

#endif // !defined(MORE_WIN_UTIL_H)