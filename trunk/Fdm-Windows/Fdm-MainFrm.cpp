/*
 * Copyright (C) 2001-2005 Jacek Sieka, arnetheduck on gmail point com
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
#include "../../Fdm-Client/dcplusplus-rips/Fdm-SettingsManager.h"

#include "Fdm-MainFrm.h"
#include "AutoSearchFrame.h"

#include "../../windows/WinUtil.h"


LRESULT FdmMainFrame::OnAutoSearchFrame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	AutoSearchFrame::openWindow();
	return 0;
}
/**
 * @file
 * $Id: MainFrm.cpp,v 1.109 2006/01/23 08:00:50 arnetheduck Exp $
 */
