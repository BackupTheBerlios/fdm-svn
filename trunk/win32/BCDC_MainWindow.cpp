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

#include <dcpp/FdmSettingsManager.h>
#include <dcpp/SettingsManager.h>
#include <dcpp/ClientManager.h>

void MainWindow::bcdcThrottleSetup() {

	//Govern upload rate && adjust it based on the time-based upload limit
	if (FDMSETTING(MAX_UPLOAD_SPEED) < 6 && FDMSETTING(MAX_UPLOAD_SPEED) && (2*FDMSETTING(MAX_UPLOAD_SPEED) < FDMSETTING(MAX_DOWNLOAD_SPEED_REAL) || FDMSETTING(MAX_UPLOAD_SPEED) && !FDMSETTING(MAX_DOWNLOAD_SPEED_REAL)))
		FdmSettingsManager::getInstance()->set(FdmSettingsManager::MAX_DOWNLOAD_SPEED_REAL, 2*FDMSETTING(MAX_UPLOAD_SPEED));
	if (FDMSETTING(MAX_UPLOAD_SPEED_TIME) < 6 && FDMSETTING(MAX_UPLOAD_SPEED_TIME) && (2*FDMSETTING(MAX_UPLOAD_SPEED_TIME) < FDMSETTING(MAX_DOWNLOAD_SPEED_REAL_TIME) || FDMSETTING(MAX_UPLOAD_SPEED_TIME) && !FDMSETTING(MAX_DOWNLOAD_SPEED_REAL_TIME)))
		FdmSettingsManager::getInstance()->set(FdmSettingsManager::MAX_DOWNLOAD_SPEED_REAL_TIME, 2*FDMSETTING(MAX_UPLOAD_SPEED_TIME));

	time_t currentTime;
	time(&currentTime);
	int currentHour = localtime(&currentTime)->tm_hour;

	if (FDMSETTING(TIME_DEPENDENT_THROTTLE) &&
			((FDMSETTING(BANDWIDTH_LIMIT_START) < FDMSETTING(BANDWIDTH_LIMIT_END) &&
			currentHour >= FDMSETTING(BANDWIDTH_LIMIT_START) && currentHour < FDMSETTING(BANDWIDTH_LIMIT_END)) ||
			(FDMSETTING(BANDWIDTH_LIMIT_START) > FDMSETTING(BANDWIDTH_LIMIT_END) &&
			(currentHour >= FDMSETTING(BANDWIDTH_LIMIT_START) || currentHour < FDMSETTING(BANDWIDTH_LIMIT_END))))
	) {
		//want to keep this out of the upload limiting code proper, where it might otherwise work more naturally
		FdmSettingsManager::getInstance()->set(FdmSettingsManager::MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO, FDMSETTING(MAX_UPLOAD_SPEED_TIME));
		FdmSettingsManager::getInstance()->set(FdmSettingsManager::MAX_DOWNLOAD_SPEED_REAL_A_PARALLEL_LEVEL_OF_HOPEFULLY_USEFUL_INDIRECTION_IN_AN_ABSURDLY_LONG_IDENTIFIER, FDMSETTING(MAX_DOWNLOAD_SPEED_REAL_TIME));
	} else {
		FdmSettingsManager::getInstance()->set(FdmSettingsManager::MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO, FDMSETTING(MAX_UPLOAD_SPEED));
		FdmSettingsManager::getInstance()->set(FdmSettingsManager::MAX_DOWNLOAD_SPEED_REAL_A_PARALLEL_LEVEL_OF_HOPEFULLY_USEFUL_INDIRECTION_IN_AN_ABSURDLY_LONG_IDENTIFIER, FDMSETTING(MAX_DOWNLOAD_SPEED_REAL));
	}
}
