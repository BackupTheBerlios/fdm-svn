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

#include "stdinc.h"
#include "../client/DCPlusPlus.h"

#include "FdmUtil.h"
#include "../client/Util.h"
#include "../Other-Projects/DC++/client/ResourceManager.h"
#include "../Other-Projects/DC++/client/SettingsManager.h"

#include "dcplusplus-rips/Fdm-Version.h"

bool FdmUtil::toBool(int aInt) {
	if (aInt) return true;
	return false;
}

string FdmUtil::toString(bool aBool) {
	if (aBool) return FDMSTRING(STRING_TRUE);
	return FDMSTRING(STRING_FALSE);
}

string FdmUtil::addFdmDes(string aCurDes) {
	return ("<" FDMAPPNAME " " FDMVERSIONSTRING ">" + aCurDes);
}

bool FdmUtil::getSettingThrottleEnable() {
	return FDMBOOLSETTING(THROTTLE_ENABLE);
}

int FdmUtil::getSettingDownloadSpeed() {
	return FDMSETTING(DOWNLOAD_SPEED);
}

int FdmUtil::getSettingUploadSpeed() {
	return FDMSETTING(UPLOAD_SPEED);
}