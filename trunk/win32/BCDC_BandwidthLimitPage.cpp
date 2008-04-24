/*
 * Copyright (C) 2004 cologic, cologic@parsoma.net
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

#include "BCDC_BandwidthLimitPage.h"

#include <dcpp/SettingsManager.h>

#include <dwt/widgets/Spinner.h>

PropPage::FdmTextItem BandwidthLimitPage::texts[] = {
	{ IDC_THROTTLE_ENABLE, ResourceManager::SETTINGS_BANDWIDTH_THROTTLE_ENABLE },
	{ IDC_TEXT_TRANSFER_RATE_LIMITING, ResourceManager::SETTINGS_BANDWIDTH_TEXT_TRANSFER_RATE_LIMITING },
	{ IDC_TEXT_MAXUPLOAD, ResourceManager::SETTINGS_BANDWIDTH_TEXT_MAXUPLOAD },
	{ IDC_TEXT_MAXDOWNLOAD, ResourceManager::SETTINGS_BANDWIDTH_TEXT_MAXDOWNLOAD },
	{ IDC_TIME_BASED_BW_LIMITING, ResourceManager::SETTINGS_BANDWIDTH_THROTTLE_TIME_BASED_BW_LIMITING },
	{ IDC_TEXT_TO, ResourceManager::SETTINGS_BANDWIDTH_TEXT_TO },
	{ IDC_TEXT_TIME_TRANSFER_LIMITING, ResourceManager::SETTINGS_BANDWIDTH_TEXT_TIME_TRANSFER_RATE_LIMITING },
	{ IDC_TEXT_TIME_MAXUPLOAD, ResourceManager::SETTINGS_BANDWIDTH_TEXT_MAXUPLOAD },
	{ IDC_TEXT_TIME_MAXDOWNLOAD, ResourceManager::SETTINGS_BANDWIDTH_TEXT_MAXDOWNLOAD },
	{ 0, ResourceManager::FDM_SETTINGS_AUTO_AWAY }
};

PropPage::FdmItem BandwidthLimitPage::items[] = {
	{ IDC_THROTTLE_ENABLE, SettingsManager::THROTTLE_ENABLE, PropPage::T_BOOL },
	{ IDC_MAXDOWNLOAD, SettingsManager::MAX_DOWNLOAD_SPEED_REAL, PropPage::T_INT },
	{ IDC_MAXUPLOAD, SettingsManager::MAX_UPLOAD_SPEED, PropPage::T_INT },
	{ IDC_TIME_BASED_BW_LIMITING, SettingsManager::TIME_DEPENDENT_THROTTLE, PropPage::T_BOOL },
	{ IDC_MAXDOWNLOAD_TIME, SettingsManager::MAX_DOWNLOAD_SPEED_REAL_TIME, PropPage::T_INT },
	{ IDC_MAXUPLOAD_TIME, SettingsManager::MAX_UPLOAD_SPEED_TIME, PropPage::T_INT },
	{ IDC_BW_START_TIME, SettingsManager::BANDWIDTH_LIMIT_START, PropPage::T_INT },
	{ IDC_BW_END_TIME, SettingsManager::BANDWIDTH_LIMIT_END, PropPage::T_INT },
	{ 0, 0, PropPage::T_END }
};

BandwidthLimitPage::BandwidthLimitPage(dwt::Widget* parent) : PropPage(parent) {
	createDialog(IDD_BANDWIDTHPAGE);

	PropPage::fdmTranslate(handle(), texts);
	PropPage::fdmRead(handle(), items, 0, 0);

	CheckBoxPtr throttleTickBox = attachChild<CheckBox>(IDC_THROTTLE_ENABLE);
	throttleTickBox->onClicked(std::tr1::bind(&BandwidthLimitPage::handleThrottleEnableClicked, this));

	throttleTickBox = attachChild<CheckBox>(IDC_TIME_BASED_BW_LIMITING);
	throttleTickBox->onClicked(std::tr1::bind(&BandwidthLimitPage::handleThrottleEnableClicked, this));

	SpinnerPtr spinner = attachChild<Spinner>(IDC_UPLOADSPEEDSPIN);
	spinner->setRange(0, 999);

	spinner = attachChild<Spinner>(IDC_DOWNLOADSPEEDSPIN);
	spinner->setRange(0, 999);

	spinner = attachChild<Spinner>(IDC_UPLOADSPEEDSPIN_TIME);
	spinner->setRange(0, 999);

	spinner = attachChild<Spinner>(IDC_DOWNLOADSPEEDSPIN_TIME);
	spinner->setRange(0, 999);

	ComboBoxPtr times = attachChild<ComboBox>(IDC_BW_START_TIME);
	times->addValue(TSTRING(SETTINGS_BANDWIDTH_MIDNIGHT));

	for (int i = 1; i < 12; ++i)
		times->addValue(Text::toT(Util::toString(i) + " " + STRING(SETTINGS_BANDWIDTH_AM)));

	times->addValue(TSTRING(SETTINGS_BANDWIDTH_NOON));

	for (int i = 1; i < 12; ++i)
		times->addValue(Text::toT(Util::toString(i) + " " + STRING(SETTINGS_BANDWIDTH_PM)));

	times->setSelected(SETTING(BANDWIDTH_LIMIT_START));

	times = attachChild<ComboBox>(IDC_BW_END_TIME);
	times->addValue(TSTRING(SETTINGS_BANDWIDTH_MIDNIGHT));

	for (int i = 1; i < 12; ++i)
		times->addValue(Text::toT(Util::toString(i) + " " + STRING(SETTINGS_BANDWIDTH_AM)));

	times->addValue(TSTRING(SETTINGS_BANDWIDTH_NOON));

	for (int i = 1; i < 12; ++i)
		times->addValue(Text::toT(Util::toString(i) + " " + STRING(SETTINGS_BANDWIDTH_PM)));

	times->setSelected(SETTING(BANDWIDTH_LIMIT_END));

	setControlState();
}

BandwidthLimitPage::~BandwidthLimitPage() {
}

void BandwidthLimitPage::handleThrottleEnableClicked() {
	setControlState();
}

void BandwidthLimitPage::setControlState() {
	bool stateEnabled = (IsDlgButtonChecked(handle(), IDC_THROTTLE_ENABLE) != 0);
	bool stateTime = (IsDlgButtonChecked(handle(), IDC_TIME_BASED_BW_LIMITING) != 0);
	::EnableWindow(GetDlgItem(handle(), IDC_MAXUPLOAD), stateEnabled);
	::EnableWindow(GetDlgItem(handle(), IDC_UPLOADSPEEDSPIN), stateEnabled);
	::EnableWindow(GetDlgItem(handle(), IDC_MAXDOWNLOAD), stateEnabled);
	::EnableWindow(GetDlgItem(handle(), IDC_DOWNLOADSPEEDSPIN), stateEnabled);
	::EnableWindow(GetDlgItem(handle(), IDC_TIME_BASED_BW_LIMITING), stateEnabled);
	::EnableWindow(GetDlgItem(handle(), IDC_BW_START_TIME), stateEnabled && stateTime);
	::EnableWindow(GetDlgItem(handle(), IDC_BW_END_TIME), stateEnabled && stateTime);
	::EnableWindow(GetDlgItem(handle(), IDC_MAXUPLOAD_TIME), stateEnabled && stateTime);
	::EnableWindow(GetDlgItem(handle(), IDC_UPLOADSPEEDSPIN_TIME), stateEnabled && stateTime);
	::EnableWindow(GetDlgItem(handle(), IDC_MAXDOWNLOAD_TIME), stateEnabled && stateTime);
	::EnableWindow(GetDlgItem(handle(), IDC_DOWNLOADSPEEDSPIN_TIME), stateEnabled && stateTime);
}

void BandwidthLimitPage::write()
{
	PropPage::fdmWrite(handle(), items, 0,0);

	// Do specialized writing here
	// settings->set(XX, YY);

	SettingsManager* settings = SettingsManager::getInstance();

	if (SETTING(MAX_DOWNLOAD_SPEED_REAL) < 0) settings->set(SettingsManager::MAX_DOWNLOAD_SPEED_REAL, 0);
}
