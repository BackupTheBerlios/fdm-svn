#include "../windows/stdafx.h"
#include "../client/DCPlusPlus.h"
#include "Resource.h"
#include "../windows/Resource.h"

#include "Fdm-MainFrame.h"
#include "../windows/WinUtil.h"
#include "../client/File.h"

LRESULT FdmMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	ctrlPad.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
	WS_HSCROLL | WS_VSCROLL | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS, WS_EX_CLIENTEDGE);
	
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

	// attach menu
	m_CmdBar.AttachMenu(m_hMenu);
	FdmWinUtil::extraToolBarCommmands(m_CmdBar);

	// load command bar images
	images.CreateFromImage(IDB_TOOLBAR, 16, 16, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);
	m_CmdBar.m_hImageList = images;

	HWND hWndToolBar = FdmWinUtil::createFdmToolbar(m_hWnd, largeImages, largeImagesHot);
	
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