#include "Resource.h"

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
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
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