/*
 * Copyright (C) 2004 cologic, cologic@parsoma.net
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
 
// From bcdc Trunk - svn791

#include "stdafx.h"
#include "../client/DCPlusPlus.h"
#include "../Fdm-Windows/Resource.h"

#include "BandwidthLimitPage.h"
#include "../client/SettingsManager.h"
#include "../windows/WinUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FdmPropPage::Item BandwidthLimitPage::items[] = {
	{ IDC_THROTTLE_ENABLE, FdmSettingsManager::THROTTLE_ENABLE, FdmPropPage::T_BOOL },
	{ IDC_MAXDOWNLOAD, FdmSettingsManager::MAX_DOWNLOAD_SPEED_REAL, FdmPropPage::T_INT },
	{ IDC_MAXUPLOAD, FdmSettingsManager::MAX_UPLOAD_SPEED, FdmPropPage::T_INT },
	{ 0, 0, FdmPropPage::T_END }
};

LRESULT BandwidthLimitPage::onInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	FdmPropPage::read((HWND)*this, items);

	CUpDownCtrl spin;
	spin.Attach(GetDlgItem(IDC_UPLOADSPEEDSPIN));
	spin.SetRange32(0, 999);
	spin.Detach();
	spin.Attach(GetDlgItem(IDC_DOWNLOADSPEEDSPIN));
	spin.SetRange32(0, 999);
	spin.Detach();

	setControlState();

	return TRUE;
}

LRESULT BandwidthLimitPage::onControlChecked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	switch (wID) {
		case IDC_THROTTLE_ENABLE:
			setControlState();
			break;
		default:
			break;
	}
	return 0;
}

void BandwidthLimitPage::setControlState() {
	bool stateEnabled = (IsDlgButtonChecked(IDC_THROTTLE_ENABLE) != 0);
	::EnableWindow(GetDlgItem(IDC_MAXUPLOAD), stateEnabled);
	::EnableWindow(GetDlgItem(IDC_UPLOADSPEEDSPIN), stateEnabled);
	::EnableWindow(GetDlgItem(IDC_MAXDOWNLOAD), stateEnabled);
	::EnableWindow(GetDlgItem(IDC_DOWNLOADSPEEDSPIN), stateEnabled);
}

void BandwidthLimitPage::write()
{
	FdmPropPage::write((HWND)*this, items);

	// Do specialized writing here
	// settings->set(XX, YY);

	if (FDMSETTING(MAX_DOWNLOAD_SPEED_REAL) < 0) settings->set(FdmSettingsManager::MAX_DOWNLOAD_SPEED_REAL, 0);
}

LRESULT BandwidthLimitPage::onHelpInfo(LPNMHDR /*pnmh*/) {
	HtmlHelp(m_hWnd, WinUtil::getHelpFile().c_str(), HH_HELP_CONTEXT, IDD_BANDWIDTHPAGE);
	return 0;
}

/**
 * @file BandwidthLimitPage.cpp
 * $Id: BandwidthLimitPage.cpp,v 1.0 2002/05/12 21:54:08 BlackClaw Exp $
 */