#if !defined(FDM_MAINFRAME_H)
#define FDM_MAINFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"

#include "../../windows/FlatTabCtrl.h"
//#include "Fdm-FlatTabCtrl.h"
#include "../resource.h"
#include "../../windows/resource.h"

#include "../ColourUtil.h"

#define FDM_MAINFRAME_MESSAGE_MAP 25

class FdmMainFrame : public MDITabChildWindowImpl<FdmMainFrame>, public StaticFrame<FdmMainFrame, FdmResourceManager::FDM_NOTEPAD>
{
public:
	DECLARE_FRAME_WND_CLASS_EX(_T("FdmMainFrame"), IDR_FDM_MAINFRAME, 0, COLOR_3DFACE);

	FdmMainFrame() : dirty(false),
		ctrlClientContainer(_T("edit"), this, FDM_MAINFRAME_MESSAGE_MAP) { }
	virtual ~FdmMainFrame();

	BEGIN_MSG_MAP(FdmMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, onClose)
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFO, onGetToolTip)
		COMMAND_ID_HANDLER(ID_FDM_FILE_SETTINGS, OnFdmFileSettings)
		COMMAND_ID_HANDLER(ID_FDM_TEST_FRAME, OnFdmTestFrame)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT onGetToolTip(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnFdmFileSettings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFdmTestFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void UpdateLayout(BOOL bResizeBars = TRUE);
	CMDICommandBarCtrl m_CmdBar;
private:
	
	bool dirty;
	CEdit ctrlPad;
	CContainedWindow ctrlClientContainer;
	CImageList fdmImages;
	CImageList fdmLargeImages, fdmLargeImagesHot;
	HWND createFdmToolbar();
};

#endif // !defined(FDM_MAINFRAME_H)

/**
 * @file
 * $Id: MainFrm.h,v 1.66 2006/01/23 08:00:50 arnetheduck Exp $
 */