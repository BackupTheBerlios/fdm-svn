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

#include "stdinc.h"
#include "DCPlusPlus.h"

#include "Fdm.h"

#include "ResourceManager.h"
#include "FdmSettingsManager.h"
//#include "dcplusplus-rips/AutoSearch.h"
//#include "UserDatabase.h"

namespace dcpp {

void startUpFdm() {
//	ResourceManager::newInstance();
	FdmSettingsManager::newInstance();

	FdmSettingsManager::getInstance()->load();
	bcdcStartUpStuff();

	if(!FDMSETTING(FDM_LANGUAGE_FILE).empty())
		ResourceManager::getInstance()->loadLanguage(FDMSETTING(FDM_LANGUAGE_FILE));

//	AutoSearchManager::newInstance();
	
//	UserDatabase::newInstance();
}

void shutDownFdm() {
//	UserDatabase::deleteInstance();
//	AutoSearchManager::deleteInstance();

	FdmSettingsManager::getInstance()->save();

	FdmSettingsManager::deleteInstance();
//	ResourceManager::deleteInstance();
}

} // namespace dcpp
