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
LRESULT AutoSearchProperties::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{
	// Translate the texts
	SetWindowText(FDMCTSTRING(AUTO_SEARCH_PROPERTIES));
	SetDlgItemText(IDC_AS_SEARCH, FDMCTSTRING(SEARCH_STRING));
	SetDlgItemText(IDC_AS_SOURCE_TYPE, FDMCTSTRING(SOURCE_TYPE));
	SetDlgItemText(IDC_AS_FILE_SIZE, FDMCTSTRING(SIZE));
	SetDlgItemText(IDC_AS_IS_ACTIVE, FDMCTSTRING(ENABLED));
	SetDlgItemText(IDC_AS_ONLY_IF_OP, FDMCTSTRING(ONLY_WHERE_OP));

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
	SetDlgItemText(IDC_AUTOS_SEARCH_STRING, Text::toT(search->searchString).c_str());
	SetDlgItemText(IDC_AUTOS_FILE_SIZE, Text::toT(search->size > 0 ? Util::toString(search->size) : "").c_str());
	::SendMessage(GetDlgItem(IDC_AUTOS_SOURCE_TYPE), CB_SETCURSEL, search->sourceType, 0L);
	::SendMessage(GetDlgItem(IDC_AUTOS_SIZE_MODES), CB_SETCURSEL, search->sizeMode, 0L);
	::SendMessage(GetDlgItem(IDC_AUTOS_SIZE_TYPE), CB_SETCURSEL, search->typeFileSize, 0L);
	::SendMessage(GetDlgItem(IDC_AS_IS_ACTIVE), BM_SETCHECK, search->isActive ? 1 : 0, 0L);
	::SendMessage(GetDlgItem(IDC_AS_ONLY_IF_OP), BM_SETCHECK, search->onlyIfOp ? 1 : 0, 0L);

	// Center dialog
	CenterWindow(GetParent());

	return FALSE;
}

// Exit dialog
LRESULT AutoSearchProperties::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(wID == IDOK)
	{
		// Update search
		TCHAR buf[256];

		GetDlgItemText(IDC_AUTOS_SEARCH_STRING, buf, 256);
		search->searchString = Text::fromT(buf);

		GetDlgItemText(IDC_AUTOS_FILE_SIZE, buf, 256);
		search->size = (_tcslen(buf) == 0 ? -1 : Util::toInt64(Text::fromT(buf)));

		search->sourceType = ctrlSourceType.GetCurSel();
		search->sizeMode = ctrlSizeModes.GetCurSel();
		search->typeFileSize = ctrlSizeType.GetCurSel();
		search->isActive = (::SendMessage(GetDlgItem(IDC_AS_IS_ACTIVE), BM_GETCHECK, 0, 0L) != 0);
		search->onlyIfOp = (::SendMessage(GetDlgItem(IDC_AS_ONLY_IF_OP), BM_GETCHECK, 0, 0L) != 0);
	}

	EndDialog(wID);
	return 0;
}