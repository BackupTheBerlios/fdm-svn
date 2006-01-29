#include "../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"

#if !defined(FDM_NOTEPAD_FRAME_H)
#define FDM_NOTEPAD_FRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../windows/FlatTabCtrl.h"
#include "../windows/WinUtil.h"

#define FDM_NOTEPAD_MESSAGE_MAP 13

class FdmNotepadFrame : public MDITabChildWindowImpl<FdmNotepadFrame>, public StaticFrame<FdmNotepadFrame, FdmResourceManager::FDM_NOTEPAD>
{
public:
	DECLARE_FRAME_WND_CLASS_EX(_T("FdmNotepadFrame"), IDR_NOTEPAD, 0, COLOR_3DFACE);

	FdmNotepadFrame() : dirty(false),
		ctrlClientContainer(_T("edit"), this, FDM_NOTEPAD_MESSAGE_MAP) { }
	virtual ~FdmNotepadFrame() { }
	
	typedef MDITabChildWindowImpl<FdmNotepadFrame> baseClass;
	BEGIN_MSG_MAP(FdmNotepadFrame)
		MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, onClose)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, onCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, onCtlColor)
		CHAIN_MSG_MAP(baseClass)
	ALT_MSG_MAP(FDM_NOTEPAD_MESSAGE_MAP)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, onLButton)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT onLButton(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void UpdateLayout(BOOL bResizeBars = TRUE);
	
	LRESULT onCtlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		HWND hWnd = (HWND)lParam;
		HDC hDC = (HDC)wParam;
		if(hWnd == ctrlPad.m_hWnd) {
			::SetBkColor(hDC, WinUtil::bgColor);
			::SetTextColor(hDC, WinUtil::textColor);
			return (LRESULT)WinUtil::bgBrush;
		}
		bHandled = FALSE;
		return FALSE;
	};
	
	
	LRESULT OnFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		ctrlPad.SetFocus();
		return 0;
	}
	
private:
	
	bool dirty;
	CEdit ctrlPad;
	CContainedWindow ctrlClientContainer;
};

#endif // !defined(FDM_NOTEPAD_FRAME_H)