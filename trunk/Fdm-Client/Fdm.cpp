/* 
 * Copyright (C) 2005 Michael J Jones, mrmikejj at hotmail dot com
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

#include "../client/stdinc.h"
#include "../client/DCPlusPlus.h"

#include "Fdm.h"

#include "../client/File.h"

#include "dcplusplus-rips/Fdm-ResourceManager.h"
#include "dcplusplus-rips/Fdm-SettingsManager.h"

#include "UserDatabase.h"

void Fdm::startUpFdm() {
	FdmResourceManager::newInstance();
	FdmSettingsManager::newInstance();

	FdmSettingsManager::getInstance()->load();

	if(!FDMSETTING(FDM_LANGUAGE_FILE).empty()) {
		FdmResourceManager::getInstance()->loadLanguage(FDMSETTING(FDM_LANGUAGE_FILE));
	}

	UserDatabase::newInstance();
}

void Fdm::shutDownFdm() {
	UserDatabase::deleteInstance();

	FdmSettingsManager::getInstance()->save();

	FdmSettingsManager::deleteInstance();
	FdmResourceManager::deleteInstance();
}