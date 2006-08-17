/*
 * Copyright (C) 2001-2006 Jacek Sieka, arnetheduck on gmail point com
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

#include "stdafx.h"
#include "../../client/DCPlusPlus.h"
#include "../Resource.h"

#include "Fdm-AppearancePage.h"
#include "../../Fdm-Client/dcplusplus-rips/Fdm-SettingsManager.h"
#include "../../client/StringTokenizer.h"
#include "../../windows/WinUtil.h"

FdmPropPage::TextItem FdmAppearancePage::texts[] = {
	{ IDC_FDM_SETTINGS_TEXT_COLOURING, FdmResourceManager::SETTINGS_APPEARANCE_TEXT_COLOURING },
	{ IDC_FDM_SETTINGS_OP_SPOKE, FdmResourceManager::SETTINGS_APPEARANCE_OP_SPOKE },
	{ IDC_FDM_SETTINGS_NOT_OP_SPOKE, FdmResourceManager::SETTINGS_APPEARANCE_NOT_OP_SPOKE },
	{ IDC_FDM_SETTINGS_I_SPOKE, FdmResourceManager::SETTINGS_APPEARANCE_I_SPOKE },
	{ IDC_FDM_SETTINGS_MY_NICK_SPOKEN, FdmResourceManager::SETTINGS_APPEARANCE_MY_NICK_SPOKEN },
	{ IDC_FDM_SETTINGS_LANGUAGE_FILE, FdmResourceManager::SETTINGS_FDM_LANGUAGE_FILE },
	{ IDC_FDM_SETTINGS_BROWSE, FdmResourceManager::FDM_BROWSE_ACCEL },
	{ IDC_FDM_SETTINGS_REQUIRES_RESTART, FdmResourceManager::SETTINGS_APPEARANCE_REQUIRES_RESTART },
	{ 0, FdmResourceManager::FDM_SETTINGS_AUTO_AWAY }
};

FdmPropPage::Item FdmAppearancePage::items[] = {
	{ IDC_FDM_LANGUAGE, FdmSettingsManager::FDM_LANGUAGE_FILE, FdmPropPage::T_STR },
	{ 0, 0, FdmPropPage::T_END }
};

LRESULT FdmAppearancePage::onInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	FdmPropPage::translate((HWND)(*this), texts);
	FdmPropPage::read((HWND)(*this), items);

	// Do specialized reading here
	opSpoke = FDMSETTING(OP_SPOKE_COLOUR);
	notOpSpoke = FDMSETTING(NOT_OP_SPOKE_COLOUR);
	iSpoke = FDMSETTING(I_SPOKE_COLOUR);
	myNickSpoken = FDMSETTING(MY_NICK_SPOKEN_COLOUR);

	return TRUE;
}

void FdmAppearancePage::write()
{
	settings->set(FdmSettingsManager::OP_SPOKE_COLOUR, (int)opSpoke);
	settings->set(FdmSettingsManager::NOT_OP_SPOKE_COLOUR, (int)notOpSpoke);
	settings->set(FdmSettingsManager::I_SPOKE_COLOUR, (int)iSpoke);
	settings->set(FdmSettingsManager::MY_NICK_SPOKEN_COLOUR, (int)myNickSpoken);

	FdmPropPage::write((HWND)(*this), items);
}

LRESULT FdmAppearancePage::onPickColor(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	switch (wID) {
		case IDC_FDM_SETTINGS_OP_SPOKE: {
				CColorDialog colPicker(opSpoke, 0, *this);
				if(colPicker.DoModal() == IDOK) 
					opSpoke = colPicker.GetColor();
			}
			break;
		case IDC_FDM_SETTINGS_NOT_OP_SPOKE: {
				CColorDialog colPicker(notOpSpoke, 0, *this);
				if(colPicker.DoModal() == IDOK) 
					notOpSpoke = colPicker.GetColor();
			}
			break;
		case IDC_FDM_SETTINGS_I_SPOKE: {
				CColorDialog colPicker(iSpoke, 0, *this);
				if(colPicker.DoModal() == IDOK) 
					iSpoke = colPicker.GetColor();
			}
			break;
		case IDC_FDM_SETTINGS_MY_NICK_SPOKEN: {
				CColorDialog colPicker(myNickSpoken, 0, *this);
				if(colPicker.DoModal() == IDOK) 
					myNickSpoken = colPicker.GetColor();
			}
			break;
		default:
			break;
	}
	return true;
}

LRESULT FdmAppearancePage::onBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	TCHAR buf[MAX_PATH];
	static const TCHAR types[] = _T("Language Files\0*.xml\0All Files\0*.*\0");

	GetDlgItemText(IDC_FDM_LANGUAGE, buf, MAX_PATH);
	tstring x = buf;

	if(WinUtil::browseFile(x, m_hWnd, false, Text::toT(Util::getAppPath()), types) == IDOK) {
		SetDlgItemText(IDC_FDM_LANGUAGE, x.c_str());
	}
	return 0;
}

LRESULT FdmAppearancePage::onHelpInfo(LPNMHDR /*pnmh*/) {
//	HtmlHelp(m_hWnd, WinUtil::getHelpFile().c_str(), HH_HELP_CONTEXT, IDD_FDMAPPEARANCEPAGEH);
	return 0;
}

LRESULT FdmAppearancePage::onHelp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
//	HtmlHelp(m_hWnd, WinUtil::getHelpFile().c_str(), HH_HELP_CONTEXT, IDD_FDMAPPEARANCEPAGEH);
	return 0;
}
