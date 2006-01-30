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

#if !defined(FDM_MAIN_FRAME_H)
#define FDM_MAIN_FRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FdmMainFrame : public CMDIFrameWindowImpl<FdmMainFrame>,
		public CMessageFilter, public CIdleHandler, public CSplitterImpl<FdmMainFrame, false>
{
public:
	DECLARE_WND_CLASS(_T("FdmToolBar"))

	FdmMainFrame();
	virtual ~FdmMainFrame();

	CMDICommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CMDIFrameWindowImpl<FdmMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;
		
		HWND hWnd = MDIGetActive();
		if(hWnd != NULL)
			return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);
		
		return FALSE;
	}
	
	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	typedef CSplitterImpl<FdmMainFrame, false> splitterBase;
	BEGIN_MSG_MAP(FdmMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	static void extraToolBarCommmands(CMDICommandBarCtrl& m_CmdBar);

private:
	CImageList images;
	CImageList largeImages, largeImagesHot;

	HWND createFdmToolbar();
};

#endif // !defined(FDM_MAIN_FRAME_H)