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
	SetDlgItemText(IDC_AS_RESULTS, FDMCTSTRING(RESULTS));
	SetDlgItemText(IDC_AS_MATCH, FDMCTSTRING(AUTO_MATCH_THESE));
	SetDlgItemText(IDC_AS_EXCLUDE, FDMCTSTRING(AUTO_EXCLUDE_THESE));
	SetDlgItemText(IDC_AS_EXTENSION, FDMCTSTRING(AUTO_EXTENSION));
	SetDlgItemText(IDC_AS_MIN_SIZE, FDMCTSTRING(MIN_SIZE));
	SetDlgItemText(IDC_AS_MAX_SIZE, FDMCTSTRING(MAX_SIZE));

	// Initialize text boxes
	ctrlSearch.Attach(GetDlgItem(IDC_AUTOS_SEARCH_STRING));
	ctrlSize.Attach(GetDlgItem(IDC_AUTOS_FILE_SIZE));
	ctrlMatch.Attach(GetDlgItem(IDC_AUTOS_SEARCH_MATCH));
	ctrlExclude.Attach(GetDlgItem(IDC_AUTOS_SEARCH_EXCLUDE));
	ctrlExtension.Attach(GetDlgItem(IDC_AUTOS_SEARCH_EXTENSION));
	ctrlMinSize.Attach(GetDlgItem(IDC_AUTOS_MIN_SIZE));
	ctrlMaxSize.Attach(GetDlgItem(IDC_AUTOS_MAX_SIZE));

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

	ctrlResSizeType.Attach(GetDlgItem(IDC_AUTOS_RES_SIZE_TYPE));
	ctrlResSizeType.AddString(CTSTRING(B));
	ctrlResSizeType.AddString(CTSTRING(KiB));
	ctrlResSizeType.AddString(CTSTRING(MiB));
	ctrlResSizeType.AddString(CTSTRING(GiB));
	ctrlResSizeType.SetCurSel(0);

	// Load search data
	ctrlSearch.SetWindowText(Text::toT(search->searchString).c_str());
	ctrlSize.SetWindowText(Text::toT(search->size > 0 ? Util::toString(search->size) : "").c_str());

	ctrlIsActive.SetCheck(search->isActive ? 1 : 0);
	ctrlOnlyIfOp.SetCheck(search->onlyIfOp ? 1 : 0);

	ctrlSourceType.SetCurSel(search->sourceType);
	ctrlSizeModes.SetCurSel(search->sizeMode);
	ctrlSizeType.SetCurSel(search->typeFileSize);

	// Load Result Matching
	ctrlMatch.SetWindowText(Text::toT(search->resultsMatch).c_str());
	ctrlExclude.SetWindowText(Text::toT(search->resultsExclude).c_str());
	ctrlExtension.SetWindowText(Text::toT(search->resultsExtensions).c_str());
	ctrlMinSize.SetWindowText(Text::toT(search->resultsMinSize > 0 ? Util::toString(search->resultsMinSize) : "").c_str());
	ctrlMaxSize.SetWindowText(Text::toT(search->resultsMaxSize > 0 ? Util::toString(search->resultsMaxSize) : "").c_str());
	ctrlResSizeType.SetCurSel(search->resultsTypeFileSize);

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

		// Update Expected Results

		ctrlMatch.GetWindowText(buf, 256);
		search->resultsMatch = Text::fromT(buf);

		ctrlExclude.GetWindowText(buf, 256);
		search->resultsExclude = Text::fromT(buf);

		ctrlExtension.GetWindowText(buf, 256);
		search->resultsExtensions = Text::fromT(buf);

		ctrlMinSize.GetWindowText(buf, 256);
		search->resultsMinSize = Util::toInt64(Text::fromT(buf));

		ctrlMaxSize.GetWindowText(buf, 256);
		search->resultsMaxSize = Util::toInt64(Text::fromT(buf));

		search->resultsTypeFileSize = ctrlResSizeType.GetCurSel();
	}

	EndDialog(wID);
	return 0;
}