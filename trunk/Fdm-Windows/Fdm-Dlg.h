#include "Resource.h"
#include "../Fdm-Client/dcplusplus-rips/Fdm-Version.h"

#if !defined(FDM_DLG_H)
#define FDM_DLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class FdmDlg : public CDialogImpl<FdmDlg>
{
public:

	enum { IDD = IDD_FDM_DIALOG };

	FdmDlg() { };
	virtual ~FdmDlg() { };

	BEGIN_MSG_MAP(FdmDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtrlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtrlColor)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtrlColor)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnCtrlColor(UINT, WPARAM wParam, LPARAM, BOOL&) {
		SetBkMode((HDC)wParam, TRANSPARENT); // transparent background needed for the rest to draw
		SetTextColor((HDC)wParam, RGB(0, 21, 180)); // blueish text
		return (LRESULT)AtlGetStockBrush(WHITE_BRUSH); // white brush (background drawn after bk mode)
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		SetDlgItemText(IDC_FDM_SETTINGS_TEXT, _T("Fdm settings text test. Fdm version: ") _T(FDMVERSIONSTRING));
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		EndDialog(wID);
		return 0;
	}
private:

	FdmDlg(const FdmDlg&) {};

};

#endif // !defined(FDM_DLG_H)