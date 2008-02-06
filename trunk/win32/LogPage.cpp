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

#include "LogPage.h"

#include <dcpp/SettingsManager.h>

#include <dcpp/LogManager.h>
#include "WinUtil.h"

PropPage::TextItem LogPage::texts[] = {
	{ IDC_SETTINGS_LOGGING, N_("Logging") },
	{ IDC_SETTINGS_LOG_DIR, N_("Directory") },
	{ IDC_BROWSE_LOG, N_("&Browse...") },
	{ IDC_SETTINGS_FORMAT, N_("Format") },
	{ IDC_SETTINGS_FILE_NAME, N_("Filename") },
	{ 0, 0 }
};

PropPage::Item LogPage::items[] = {
	{ IDC_LOG_DIRECTORY, SettingsManager::LOG_DIRECTORY, PropPage::T_STR },
	{ 0, 0, PropPage::T_END }
};

PropPage::ListItem LogPage::listItems[] = {
	{ SettingsManager::LOG_MAIN_CHAT, N_("Log main chat") },
	{ SettingsManager::LOG_PRIVATE_CHAT, N_("Log private chat") },
	{ SettingsManager::LOG_DOWNLOADS, N_("Log downloads") },
	{ SettingsManager::LOG_UPLOADS, N_("Log uploads") },
	{ SettingsManager::LOG_SYSTEM, N_("Log system messages") },
	{ SettingsManager::LOG_STATUS_MESSAGES, N_("Log status messages") },
	{ SettingsManager::LOG_FILELIST_TRANSFERS, N_("Log filelist transfers") },
	{ 0, 0 }
};

LogPage::LogPage(SmartWin::Widget* parent) : PropPage(parent) {
	createDialog(IDD_LOGPAGE);

	PropPage::translate(handle(), texts);
	PropPage::read(handle(), items, listItems, ::GetDlgItem(handle(), IDC_LOG_OPTIONS));

	for(int i = 0; i < LogManager::LAST; ++i) {
		TStringPair pair;
		pair.first = Text::toT(LogManager::getInstance()->getSetting(i, LogManager::FILE));
		pair.second = Text::toT(LogManager::getInstance()->getSetting(i, LogManager::FORMAT));
		options.push_back(pair);
	}

	::EnableWindow(::GetDlgItem(handle(), IDC_LOG_FORMAT), false);
	::EnableWindow(::GetDlgItem(handle(), IDC_LOG_FILE), false);

	oldSelection = -1;

	attachButton(IDC_BROWSE_LOG)->onClicked(std::tr1::bind(&LogPage::handleBrowseClicked, this));

	WidgetListViewPtr dataGrid = attachList(IDC_LOG_OPTIONS);
	dataGrid->onRaw(std::tr1::bind(&LogPage::handleItemChanged, this, dataGrid, _1, _2), SmartWin::Message(WM_NOTIFY, LVN_ITEMCHANGED));
}

LogPage::~LogPage() {
}

void LogPage::write()
{
	PropPage::write(handle(), items, listItems, ::GetDlgItem(handle(), IDC_LOG_OPTIONS));

	const string& s = SETTING(LOG_DIRECTORY);
	if(s.length() > 0 && s[s.length() - 1] != '\\') {
		SettingsManager::getInstance()->set(SettingsManager::LOG_DIRECTORY, s + '\\');
	}
	File::ensureDirectory(SETTING(LOG_DIRECTORY));

	//make sure we save the last edit too, the user
	//might not have changed the selection
	getValues();

	for(int i = 0; i < LogManager::LAST; ++i) {
		string tmp = Text::fromT(options[i].first);
		if(Util::stricmp(Util::getFileExt(tmp), ".log") != 0)
			tmp += ".log";

		LogManager::getInstance()->saveSetting(i, LogManager::FILE, tmp);
		LogManager::getInstance()->saveSetting(i, LogManager::FORMAT, Text::fromT(options[i].second));
	}
}

void LogPage::handleBrowseClicked() {
	tstring dir = Text::toT(SETTING(LOG_DIRECTORY));
	if(WinUtil::browseDirectory(dir, handle()))
	{
		// Adjust path string
		if(dir.size() > 0 && dir[dir.size() - 1] != '\\')
			dir += '\\';

		::SetDlgItemText(handle(), IDC_LOG_DIRECTORY, dir.c_str());
	}
}

HRESULT LogPage::handleItemChanged(WidgetListViewPtr dataGrid, WPARAM wParam, LPARAM lParam) {
	getValues();

	int sel = dataGrid->getSelectedIndex();

	if(sel >= 0 && sel < LogManager::LAST) {
		BOOL checkState = dataGrid->isChecked(sel) ? TRUE : FALSE;
		::EnableWindow(::GetDlgItem(handle(), IDC_LOG_FORMAT), checkState);
		::EnableWindow(::GetDlgItem(handle(), IDC_LOG_FILE), checkState);

		::SetDlgItemText(handle(), IDC_LOG_FILE, options[sel].first.c_str());
		::SetDlgItemText(handle(), IDC_LOG_FORMAT, options[sel].second.c_str());

		//save the old selection so we know where to save the values
		oldSelection = sel;
	} else {
		::EnableWindow(::GetDlgItem(handle(), IDC_LOG_FORMAT), FALSE);
		::EnableWindow(::GetDlgItem(handle(), IDC_LOG_FILE), FALSE);

		::SetDlgItemText(handle(), IDC_LOG_FILE, _T(""));
		::SetDlgItemText(handle(), IDC_LOG_FORMAT, _T(""));
	}

	return 0;
}

void LogPage::getValues() {
	if(oldSelection >= 0) {
		TCHAR buf[512];

		if(::GetDlgItemText(handle(), IDC_LOG_FILE, buf, 512) > 0)
			options[oldSelection].first = buf;
		if(::GetDlgItemText(handle(), IDC_LOG_FORMAT, buf, 512) > 0)
			options[oldSelection].second = buf;
	}
}