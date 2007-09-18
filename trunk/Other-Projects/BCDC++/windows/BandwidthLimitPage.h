/*
 * Copyright (C) 2003 cologic, cologic@parsoma.net
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

#ifndef BANDWIDTHLIMITPAGE_H
#define BANDWIDTHLIMITPAGE_H

#include <atlcrack.h>
#include "../Fdm-Windows/dcplusplus-rips/Fdm-PropPage.h"

class BandwidthLimitPage : public CPropertyPage<IDD_BANDWIDTHPAGE>, public FdmPropPage
{
public:
	BandwidthLimitPage(FdmSettingsManager *s) : FdmPropPage(s) { 
		SetTitle(FDMCTSTRING(SETTINGS_BANDWIDTH));
		m_psp.dwFlags |= PSP_HASHELP;
	};
	~BandwidthLimitPage() { };

	BEGIN_MSG_MAP_EX(BandwidthLimitPage)
		MESSAGE_HANDLER(WM_INITDIALOG, onInitDialog)
		COMMAND_ID_HANDLER(IDC_THROTTLE_ENABLE, onControlChecked)
		NOTIFY_CODE_HANDLER_EX(PSN_HELP, onHelpInfo)
	END_MSG_MAP()

	LRESULT onInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT onControlChecked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT onHelpInfo(LPNMHDR /*pnmh*/);

	void setControlState();
	// Common PropPage interface
	PROPSHEETPAGE *getPSP() { return (PROPSHEETPAGE *)*this; }
	virtual void write();

protected:
	static Item items[];
};

#endif //BANDWIDTHLIMITPAGE_H

/**
 * @file BandwidthLimitPage.h
 * $Id: BandwidthLimitPage.h,v 1.0 2002/05/12 21:54:08 arnetheduck Exp $
 */