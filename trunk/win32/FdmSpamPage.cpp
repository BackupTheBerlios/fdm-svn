/*
 * Copyright (C) 2006 Michael J Jones, mrmikejj at hotmail dot com
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

#include "resource.h"

#include "FdmSpamPage.h"

#include <dcpp/SettingsManager.h>

#include "WinUtil.h"

static const WinUtil::HelpItem helpItems[] = {
	{ 0, 0 }
};

PropPage::FdmTextItem FdmSpamPage::texts[] = {
	{ 0, ResourceManager::FDM_SETTINGS_AUTO_AWAY }
};

PropPage::FdmItem FdmSpamPage::items[] = {
	{ 0, 0, PropPage::T_END }
};

FdmSpamPage::FdmSpamPage(SmartWin::Widget* parent) : PropPage(parent) {
	createDialog(IDD_FDM_SPAM_PAGE);

	PropPage::fdmTranslate(handle(), texts);
	PropPage::fdmRead(handle(), items, 0, 0);
}

FdmSpamPage::~FdmSpamPage() {
}

void FdmSpamPage::write() {
	PropPage::fdmWrite(handle(), items, 0, 0);
}
