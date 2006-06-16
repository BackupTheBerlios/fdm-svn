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

// Pothead used ADLSProperties.h as a template
 
#if !defined(AUTO_SEARCH_PROPERTIES_H)
#define AUTO_SEARCH_PROPERTIES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AutoSearch;

///////////////////////////////////////////////////////////////////////////////
//
//	Dialog for new/edit Auto searches
//
///////////////////////////////////////////////////////////////////////////////
class AutoSearchProperties : public CDialogImpl<AutoSearchProperties>
{
public:

	// Constructor/destructor
	AutoSearchProperties::AutoSearchProperties(AutoSearch *_search) : search(_search) { }
	virtual ~AutoSearchProperties() { }

	// Dilaog unique id
	enum { IDD = IDD_AUTO_SEARCH_PROPERTIES };
	
	// Inline message map
	BEGIN_MSG_MAP(AutoSearchProperties)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()
	
	// Message handlers
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:

	// Current search
	AutoSearch* search;

	// Text boxes
	CEdit ctrlSearch;
	CEdit ctrlSize;

	// Check boxes
	CButton ctrlIsActive;
	CButton ctrlOnlyIfOp;

	// Combo boxes
	CComboBox ctrlSourceType;
	CComboBox ctrlSizeModes;
	CComboBox ctrlSizeType;
};

#endif // !defined(AUTO_SEARCH_PROPERTIES_H)
