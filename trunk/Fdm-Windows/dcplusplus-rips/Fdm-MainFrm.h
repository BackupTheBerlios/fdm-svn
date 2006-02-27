#if !defined(FDM_MAINFRAME_H)
#define FDM_MAINFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"
#include "../resource.h"

namespace FdmMainFrame {
	LRESULT OnFdmFileSettings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFdmTestFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void destroyFdmMainFrame(CImageList& fdmLargeImages, CImageList& fdmLargeImagesHot);
	HWND createFdmToolbar(HWND& m_hWnd, CImageList& fdmLargeImages, CImageList& fdmLargeImagesHot);
	void fdmToolTips(int& idCtrl, LPNMTTDISPINFO& pDispInfo, int& stringId);
};

#endif // !defined(FDM_MAINFRAME_H)

/**
 * @file
 * $Id: MainFrm.h,v 1.66 2006/01/23 08:00:50 arnetheduck Exp $
 */