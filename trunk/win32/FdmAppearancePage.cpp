/*
 * Copyright (C) 2001-2007 Jacek Sieka, arnetheduck on gmail point com
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

#include "FdmAppearancePage.h"

#include <dcpp/SettingsManager.h>

#include "WinUtil.h"

PropPage::FdmTextItem FdmAppearancePage::texts[] = {
	{ IDC_FDM_SETTINGS_TEXT_COLOURING, ResourceManager::SETTINGS_APPEARANCE_TEXT_COLOURING },
	{ IDC_FDM_SETTINGS_OP_SPOKE, ResourceManager::SETTINGS_APPEARANCE_OP_SPOKE },
	{ IDC_FDM_SETTINGS_NOT_OP_SPOKE, ResourceManager::SETTINGS_APPEARANCE_NOT_OP_SPOKE },
	{ IDC_FDM_SETTINGS_I_SPOKE, ResourceManager::SETTINGS_APPEARANCE_I_SPOKE },
	{ IDC_FDM_SETTINGS_MY_NICK_SPOKEN, ResourceManager::SETTINGS_APPEARANCE_MY_NICK_SPOKEN },
	{ IDC_FDM_SETTINGS_LANGUAGE_FILE, ResourceManager::SETTINGS_FDM_LANGUAGE_FILE },
	{ IDC_FDM_SETTINGS_BROWSE, ResourceManager::FDM_BROWSE_ACCEL },
	{ IDC_SETTINGS_FDM_APPEARANCE_OPTIONS, ResourceManager::OPTIONS },
	{ IDC_FDM_SETTINGS_REQUIRES_RESTART, ResourceManager::SETTINGS_APPEARANCE_REQUIRES_RESTART },
	{ 0, ResourceManager::FDM_SETTINGS_AUTO_AWAY }
};

PropPage::FdmItem FdmAppearancePage::items[] = {
	{ IDC_FDM_LANGUAGE, SettingsManager::FDM_LANGUAGE_FILE, PropPage::T_STR },
	{ 0, 0, PropPage::T_END }
};

PropPage::FdmListItem FdmAppearancePage::listItems[] = {
	{ SettingsManager::SHOW_IPS_IN_CHAT, ResourceManager::SETTINGS_APPEARANCE_SHOW_IPS_IN_CHATS },
	{ SettingsManager::SHOW_CC_IN_CHAT, ResourceManager::SETTINGS_APPEARANCE_SHOW_CC_IN_CHATS },
	{ 0, ResourceManager::FDM_SETTINGS_AUTO_AWAY }
};

FdmAppearancePage::FdmAppearancePage(dwt::Widget* parent) : PropPage(parent) {
	createDialog(IDD_FDM_APPEARANCE_PAGE);

	PropPage::fdmTranslate(handle(), texts);
	PropPage::fdmRead(handle(), items, listItems, ::GetDlgItem(handle(), IDC_FDM_APPEARANCE_BOOLEANS));

	opSpoke = SETTING(OP_SPOKE_COLOUR);
	notOpSpoke = SETTING(NOT_OP_SPOKE_COLOUR);
	iSpoke = SETTING(I_SPOKE_COLOUR);
	myNickSpoken = SETTING(MY_NICK_SPOKEN_COLOUR);

	ButtonPtr button = attachChild<Button>(IDC_FDM_SETTINGS_BROWSE);
	button->onClicked(std::tr1::bind(&FdmAppearancePage::handleBrowse, this));

	button = attachChild<Button>(IDC_FDM_SETTINGS_OP_SPOKE);
	button->onClicked(std::tr1::bind(&FdmAppearancePage::handleOpSpoke, this));

	button = attachChild<Button>(IDC_FDM_SETTINGS_NOT_OP_SPOKE);
	button->onClicked(std::tr1::bind(&FdmAppearancePage::handleNotOpSpoke, this));

	button = attachChild<Button>(IDC_FDM_SETTINGS_I_SPOKE);
	button->onClicked(std::tr1::bind(&FdmAppearancePage::handleISpoke, this));

	button = attachChild<Button>(IDC_FDM_SETTINGS_MY_NICK_SPOKEN);
	button->onClicked(std::tr1::bind(&FdmAppearancePage::handleMyNickSpoken, this));

	::EnableWindow(GetDlgItem(handle(), IDC_FDM_SETTINGS_OP_SPOKE), false);
	::EnableWindow(GetDlgItem(handle(), IDC_FDM_SETTINGS_NOT_OP_SPOKE), false);
	::EnableWindow(GetDlgItem(handle(), IDC_FDM_SETTINGS_I_SPOKE), false);
	::EnableWindow(GetDlgItem(handle(), IDC_FDM_SETTINGS_MY_NICK_SPOKEN), false);
}

FdmAppearancePage::~FdmAppearancePage() {
}

void FdmAppearancePage::write() {
	PropPage::fdmWrite(handle(), items, listItems, ::GetDlgItem(handle(), IDC_FDM_APPEARANCE_BOOLEANS));

	SettingsManager* settings = SettingsManager::getInstance();

	settings->set(SettingsManager::OP_SPOKE_COLOUR, (int)opSpoke);
	settings->set(SettingsManager::NOT_OP_SPOKE_COLOUR, (int)notOpSpoke);
	settings->set(SettingsManager::I_SPOKE_COLOUR, (int)iSpoke);
	settings->set(SettingsManager::MY_NICK_SPOKEN_COLOUR, (int)myNickSpoken);
}

void FdmAppearancePage::handleBrowse() {
	TCHAR buf[MAX_PATH];
	static const TCHAR types[] = _T("Language Files\0*.xml\0All Files\0*.*\0");

	::GetDlgItemText(handle(), IDC_FDM_LANGUAGE, buf, MAX_PATH);
	tstring x = buf;

//	if(WinUtil::browseFile(x, handle(), false, Text::toT(Util::getDataPath()), types) == IDOK) {
		//::SetDlgItemText(handle(), IDC_FDM_LANGUAGE, x.c_str());
//	}
}

void FdmAppearancePage::handleOpSpoke() {
	ColorDialog::ColorParams colorParams(opSpoke);
	if(createColorDialog().open(colorParams))
		opSpoke = colorParams.getColor();
}

void FdmAppearancePage::handleNotOpSpoke() {
	ColorDialog::ColorParams colorParams(notOpSpoke);
	if(createColorDialog().open(colorParams))
		notOpSpoke = colorParams.getColor();
}

void FdmAppearancePage::handleISpoke() {
	ColorDialog::ColorParams colorParams(iSpoke);
	if(createColorDialog().open(colorParams))
		iSpoke = colorParams.getColor();
}

void FdmAppearancePage::handleMyNickSpoken() {
	ColorDialog::ColorParams colorParams(myNickSpoken);
	if(createColorDialog().open(colorParams))
		myNickSpoken = colorParams.getColor();
}
