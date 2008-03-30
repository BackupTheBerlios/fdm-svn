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

#include "MainWindow.h"

#include <SettingsManager.h>
#include <ConnectionManager.h>

void ConnectionManager::bcdcThrottleSetup() {
	//Govern upload rate && adjust it based on the time-based upload limit
	if (SETTING(MAX_UPLOAD_SPEED) < 6 && SETTING(MAX_UPLOAD_SPEED) && (2*SETTING(MAX_UPLOAD_SPEED) < SETTING(MAX_DOWNLOAD_SPEED_REAL) || SETTING(MAX_UPLOAD_SPEED) && !SETTING(MAX_DOWNLOAD_SPEED_REAL)))
		SettingsManager::getInstance()->set(SettingsManager::MAX_DOWNLOAD_SPEED_REAL, 2*SETTING(MAX_UPLOAD_SPEED));
	if (SETTING(MAX_UPLOAD_SPEED_TIME) < 6 && SETTING(MAX_UPLOAD_SPEED_TIME) && (2*SETTING(MAX_UPLOAD_SPEED_TIME) < SETTING(MAX_DOWNLOAD_SPEED_REAL_TIME) || SETTING(MAX_UPLOAD_SPEED_TIME) && !SETTING(MAX_DOWNLOAD_SPEED_REAL_TIME)))
		SettingsManager::getInstance()->set(SettingsManager::MAX_DOWNLOAD_SPEED_REAL_TIME, 2*SETTING(MAX_UPLOAD_SPEED_TIME));

	time_t currentTime;
	time(&currentTime);
	int currentHour = localtime(&currentTime)->tm_hour;

	if (SETTING(TIME_DEPENDENT_THROTTLE) &&
			((SETTING(BANDWIDTH_LIMIT_START) < SETTING(BANDWIDTH_LIMIT_END) &&
			currentHour >= SETTING(BANDWIDTH_LIMIT_START) && currentHour < SETTING(BANDWIDTH_LIMIT_END)) ||
			(SETTING(BANDWIDTH_LIMIT_START) > SETTING(BANDWIDTH_LIMIT_END) &&
			(currentHour >= SETTING(BANDWIDTH_LIMIT_START) || currentHour < SETTING(BANDWIDTH_LIMIT_END))))
	) {
		//want to keep this out of the upload limiting code proper, where it might otherwise work more naturally
		SettingsManager::getInstance()->set(SettingsManager::MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO, SETTING(MAX_UPLOAD_SPEED_TIME));
		SettingsManager::getInstance()->set(SettingsManager::MAX_DOWNLOAD_SPEED_REAL_A_PARALLEL_LEVEL_OF_HOPEFULLY_USEFUL_INDIRECTION_IN_AN_ABSURDLY_LONG_IDENTIFIER, SETTING(MAX_DOWNLOAD_SPEED_REAL_TIME));
	} else {
		SettingsManager::getInstance()->set(SettingsManager::MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO, SETTING(MAX_UPLOAD_SPEED));
		SettingsManager::getInstance()->set(SettingsManager::MAX_DOWNLOAD_SPEED_REAL_A_PARALLEL_LEVEL_OF_HOPEFULLY_USEFUL_INDIRECTION_IN_AN_ABSURDLY_LONG_IDENTIFIER, SETTING(MAX_DOWNLOAD_SPEED_REAL));
	}
}
