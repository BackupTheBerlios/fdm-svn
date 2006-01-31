#if !defined(FDM_MAINFRAME_H)
#define FDM_MAINFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"
#include "../windows/FlatTabCtrl.h"
#include "../windows/WinUtil.h"
#include "Fdm-WinUtil.h"

#include "ColourUtil.h"

#define FDM_MAINFRAME_MESSAGE_MAP 25

class FdmMainFrame : public MDITabChildWindowImpl<FdmMainFrame>, public StaticFrame<FdmMainFrame, FdmResourceManager::FDM_NOTEPAD>
{
public:
	DECLARE_FRAME_WND_CLASS_EX(_T("FdmMainFrame"), IDR_NOTEPAD, 0, COLOR_3DFACE);

	FdmMainFrame() : dirty(false),
		ctrlClientContainer(_T("edit"), this, FDM_MAINFRAME_MESSAGE_MAP) { }
	virtual ~FdmMainFrame() { }
	
	typedef MDITabChildWindowImpl<FdmMainFrame> baseClass;
	BEGIN_MSG_MAP(FdmMainFrame)
		MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, onClose)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, onCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, onCtlColor)
		// Carraya test extra toolbar
		COMMAND_ID_HANDLER(ID_FDM_FILE_SETTINGS, FdmWinUtil::OnFdmFileSettings)
		COMMAND_ID_HANDLER(ID_FDM_TEST_FRAME, FdmWinUtil::OnFdmTestFrame)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

	void UpdateLayout(BOOL bResizeBars = TRUE);
	
	LRESULT onCtlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		HWND hWnd = (HWND)lParam;
		HDC hDC = (HDC)wParam;
		if(hWnd == ctrlPad.m_hWnd) {
			::SetBkColor(hDC, ColourUtil::m_ChatTextGeneral.crBackColor);
			::SetTextColor(hDC, ColourUtil::m_ChatTextGeneral.crTextColor);
			return (LRESULT)ColourUtil::bgBrush;
		}
		bHandled = FALSE;
		return FALSE;
	};
	
	
	LRESULT OnFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		ctrlPad.SetFocus();
		return 0;
	}
	CMDICommandBarCtrl m_CmdBar;
private:
	
	bool dirty;
	CEdit ctrlPad;
	CContainedWindow ctrlClientContainer;
	CImageList images;
	CImageList largeImages, largeImagesHot;
};

#endif // !defined(FDM_MAINFRAME_H)
