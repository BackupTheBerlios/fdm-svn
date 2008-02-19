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
#include "DCPlusPlus.h"

#include "FdmUtil.h"

#include "FdmSettingsManager.h"
#include "FdmVersion.h"
#include "Text.h"
#include "Util.h"
#include "StringTokenizer.h"

namespace dcpp {

bool FdmUtil::isIp(string aString) {
// maybe todo
// if use reg exp's in future replace this with one
	int count = 0;
	StringTokenizer<string> tok(aString, '.');
	for(StringIter i = tok.getTokens().begin(); i != tok.getTokens().end(); ++i) {
		count++;
		if (!isNumber(*i))
			return false;
		if (Util::toInt(*i) < 0 || Util::toInt(*i) > 255)
			return false;
	}
	if (count == 4)
		return true;
	return false;
}

bool FdmUtil::isNumber(string aString) {
	const char* asChar = aString.c_str();
	size_t sizeOfString = strlen(asChar);
	for(size_t i = 0; i < sizeOfString; ++i) {
		if(!isdigit(asChar[i]))
			return false;
	}
	return true;
}

string FdmUtil::addFdmDes(string aCurDes) {
	string answer = (aCurDes + "<" FDMAPPNAME " " FDMVERSIONSTRING);
	answer += FDMBOOLSETTING(THROTTLE_ENABLE) && FDMSETTING(MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO) ? (" B:" + Util::toString(FDMSETTING(MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO)) + ">") : ">";
	return answer;
}

} // namespace dcpp
//#include "../client/Util.h"
//#include "ResourceManager.h"
//#include "../Other-Projects/DC++/client/SettingsManager.h"

//#include "dcplusplus-rips/Fdm-Version.h"

/*bool FdmUtil::toBool(int aInt) {
	if (aInt) return true;
	return false;
}

string FdmUtil::toString(bool aBool) {
	if (aBool) return STRING(STRING_TRUE);
	return STRING(STRING_FALSE);
}

bool FdmUtil::getSettingThrottleEnable() {
	return FDMBOOLSETTING(THROTTLE_ENABLE);
}

int FdmUtil::getSettingDownloadSpeed() {
	return FDMSETTING(DOWNLOAD_SPEED);
}

int FdmUtil::getSettingUploadSpeed() {
	return FDMSETTING(UPLOAD_SPEED);
}*/
