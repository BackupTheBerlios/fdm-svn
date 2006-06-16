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

// Pothead used ADLSProperties.cpp as a template
 
#include "Fdm-stdafx.h"
#include "../../client/DCPlusPlus.h"
#include "../Resource.h"

#include "AutoSearchProperties.h"

#include "../../client/SearchManager.h"
#include "../../client/ADLSearch.h"
#include "../../client/Util.h"
#include "../../client/ResourceManager.h"
#include "../../Fdm-client/dcplusplus-rips/Fdm-ResourceManager.h"
#include "../../Fdm-client/dcplusplus-rips/AutoSearch.h"

// Initialize dialog
LRESULT AutoSearchProperties::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&) {
	// Translate the texts
	SetWindowText(FDMCTSTRING(AUTO_SEARCH_PROPERTIES));
	SetDlgItemText(IDC_AS_SEARCH, FDMCTSTRING(SEARCH_STRING));
	SetDlgItemText(IDC_AS_SOURCE_TYPE, FDMCTSTRING(SOURCE_TYPE));
	SetDlgItemText(IDC_AS_FILE_SIZE, FDMCTSTRING(SIZE));
	SetDlgItemText(IDC_AS_IS_ACTIVE, FDMCTSTRING(ENABLED));
	SetDlgItemText(IDC_AS_ONLY_IF_OP, FDMCTSTRING(ONLY_WHERE_OP));

	// Initialize text boxes
	ctrlSearch.Attach(GetDlgItem(IDC_AUTOS_SEARCH_STRING));
	ctrlSize.Attach(GetDlgItem(IDC_AUTOS_FILE_SIZE));

	// Initialize check boxes
	ctrlIsActive.Attach(GetDlgItem(IDC_AS_IS_ACTIVE));
	ctrlOnlyIfOp.Attach(GetDlgItem(IDC_AS_ONLY_IF_OP));

	// Initialize combo boxes
	ctrlSourceType.Attach(GetDlgItem(IDC_AUTOS_SOURCE_TYPE));
	ctrlSourceType.AddString(CTSTRING(ANY));
	ctrlSourceType.AddString(CTSTRING(AUDIO));
	ctrlSourceType.AddString(CTSTRING(COMPRESSED));
	ctrlSourceType.AddString(CTSTRING(DOCUMENT));
	ctrlSourceType.AddString(CTSTRING(EXECUTABLE));
	ctrlSourceType.AddString(CTSTRING(PICTURE));
	ctrlSourceType.AddString(CTSTRING(VIDEO));
	ctrlSourceType.AddString(CTSTRING(DIRECTORY));
	ctrlSourceType.AddString(_T("TTH"));
	ctrlSourceType.SetCurSel(0);

	ctrlSizeModes.Attach(GetDlgItem(IDC_AUTOS_SIZE_MODES));
	ctrlSizeModes.AddString(CTSTRING(NORMAL));
	ctrlSizeModes.AddString(CTSTRING(AT_LEAST));
	ctrlSizeModes.AddString(CTSTRING(AT_MOST));
	ctrlSizeModes.SetCurSel(1);

	ctrlSizeType.Attach(GetDlgItem(IDC_AUTOS_SIZE_TYPE));
	ctrlSizeType.AddString(CTSTRING(B));
	ctrlSizeType.AddString(CTSTRING(KiB));
	ctrlSizeType.AddString(CTSTRING(MiB));
	ctrlSizeType.AddString(CTSTRING(GiB));
	ctrlSizeType.SetCurSel(0);

	// Load search data
	ctrlSearch.SetWindowText(Text::toT(search->searchString).c_str());
	ctrlSize.SetWindowText(Text::toT(search->size > 0 ? Util::toString(search->size) : "").c_str());

	ctrlIsActive.SetCheck(search->isActive ? 1 : 0);
	ctrlOnlyIfOp.SetCheck(search->onlyIfOp ? 1 : 0);

	ctrlSourceType.SetCurSel(search->sourceType);
	ctrlSizeModes.SetCurSel(search->sizeMode);
	ctrlSizeType.SetCurSel(search->typeFileSize);

	// Center dialog
	CenterWindow(GetParent());

	return FALSE;
}

// Exit dialog
LRESULT AutoSearchProperties::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if(wID == IDOK)	{
		// Update search
		TCHAR buf[256];

		ctrlSearch.GetWindowText(buf, 256);
		search->searchString = Text::fromT(buf);

		ctrlSize.GetWindowText(buf, 256);
		search->size = Util::toInt64(Text::fromT(buf));

		search->isActive = (ctrlIsActive.GetCheck() == 1);
		search->onlyIfOp = (ctrlOnlyIfOp.GetCheck() == 1);

		search->sourceType = ctrlSourceType.GetCurSel();
		search->sizeMode = ctrlSizeModes.GetCurSel();
		search->typeFileSize = ctrlSizeType.GetCurSel();
	}

	EndDialog(wID);
	return 0;
}