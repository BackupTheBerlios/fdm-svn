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

// From DC++ Trunk - svn964

#include "stdinc.h"
#include "DCPlusPlus.h"

#include "FdmSettingsManager.h"

#include "SimpleXML.h"
#include "Util.h"
#include "File.h"

namespace dcpp {

const string FdmSettingsManager::settingTags[] =
{
	// Strings
	"FdmLanguageFileLocation",
	"AutoSearchFrameOrder", "AutoSearchFrameWidths",
	"WinampFormat", 
	"SENTRY", 
	// Ints
	"OpSpokeColour", "NotOpSpokeColour", "ISpokeColour", "MyNickSpokenColour",
	"IpInChats", "CCInChats",

	// BCDC Ints
	"MaxUploadSpeed", "MaxDownloadSpeedReal", "ThrottleEnabled",
	"BandwidthLimitStart", "BandwidthLimitEnd", "TimeDependentThrottle", "MaxDownloadSpeedRealTime",
	"MaxUploadSpeedTime", "EvilHiddenSetting", "YetAnotherEvilHiddenSetting",
	"SENTRY",
	// Int64
	"aInt64",
	"SENTRY"
};

FdmSettingsManager::FdmSettingsManager()
{
	for(int i=0; i<SETTINGS_LAST; i++)
		isSet[i] = false;

	for(int j=0; j<INT_LAST-INT_FIRST; j++) {
		intDefaults[j] = 0;
		intSettings[j] = 0;
	}
	for(int k=0; k<INT64_LAST-INT64_FIRST; k++) {
		int64Defaults[k] = 0;
		int64Settings[k] = 0;
	}

	setDefault(FDM_LANGUAGE_FILE, "testing");
	setDefault(OP_SPOKE_COLOUR, 16711680);
	setDefault(NOT_OP_SPOKE_COLOUR, 128);
	setDefault(I_SPOKE_COLOUR, 255);
	setDefault(MY_NICK_SPOKEN_COLOUR, 32768);
	setDefault(WINAMP_FORMAT, "winamp(%[version]) %[state] (%[title]) stats(%[percent] of %[length])");
	setDefault(SHOW_IPS_IN_CHAT, true);
	setDefault(SHOW_CC_IN_CHAT, true);

#ifdef _WIN32
#endif
	setDefault(THROTTLE_ENABLE, false);
	setDefault(TIME_DEPENDENT_THROTTLE, false);
	setDefault(MAX_DOWNLOAD_SPEED_REAL_TIME, 0);
	setDefault(MAX_UPLOAD_SPEED_TIME, 0);
	setDefault(BANDWIDTH_LIMIT_START, 0);
	setDefault(BANDWIDTH_LIMIT_END, 0);
}

void FdmSettingsManager::load(string const& aFileName)
{
	try {
		SimpleXML xml;

		xml.fromXML(File(aFileName, File::READ, File::OPEN).read());

		xml.resetCurrentChild();

		xml.stepIn();

		if(xml.findChild("FdmSettings"))
		{
			xml.stepIn();

			int i;

			for(i=STR_FIRST; i<STR_LAST; i++)
			{
				const string& attr = settingTags[i];
				dcassert(attr.find("SENTRY") == string::npos);

				if(xml.findChild(attr))
					set(StrSetting(i), xml.getChildData());
				xml.resetCurrentChild();
			}
			for(i=INT_FIRST; i<INT_LAST; i++)
			{
				const string& attr = settingTags[i];
				dcassert(attr.find("SENTRY") == string::npos);

				if(xml.findChild(attr))
					set(IntSetting(i), Util::toInt(xml.getChildData()));
				xml.resetCurrentChild();
			}
			for(i=INT64_FIRST; i<INT64_LAST; i++)
			{
				const string& attr = settingTags[i];
				dcassert(attr.find("SENTRY") == string::npos);

				if(xml.findChild(attr))
					set(Int64Setting(i), Util::toInt64(xml.getChildData()));
				xml.resetCurrentChild();
			}

			xml.stepOut();
		}


		fire(FdmSettingsManagerListener::Load(), xml);
		xml.stepOut();

	} catch(const Exception&) {
	}
}

void FdmSettingsManager::save(string const& aFileName) {

	SimpleXML xml;
	xml.addTag("Fdm");
	xml.stepIn();
	xml.addTag("FdmSettings");
	xml.stepIn();

	int i;
	string type("type"), curType("string");

	for(i=STR_FIRST; i<STR_LAST; i++)
	{
		if(isSet[i]) {
			xml.addTag(settingTags[i], get(StrSetting(i), false));
			xml.addChildAttrib(type, curType);
		}
	}

	curType = "int";
	for(i=INT_FIRST; i<INT_LAST; i++)
	{
		if(isSet[i]) {
			xml.addTag(settingTags[i], get(IntSetting(i), false));
			xml.addChildAttrib(type, curType);
		}
	}
	curType = "int64";
	for(i=INT64_FIRST; i<INT64_LAST; i++)
	{
		if(isSet[i])
		{
			xml.addTag(settingTags[i], get(Int64Setting(i), false));
			xml.addChildAttrib(type, curType);
		}
	}
	xml.stepOut();

	fire(FdmSettingsManagerListener::Save(), xml);

	try {
		File out(aFileName + ".tmp", File::WRITE, File::CREATE | File::TRUNCATE);
		BufferedOutputStream<false> f(&out);
		f.write(SimpleXML::utf8Header);
		xml.toXML(&f);
		f.flush();
		out.close();
		File::deleteFile(aFileName);
		File::renameFile(aFileName + ".tmp", aFileName);
	} catch(const FileException&) {
		// ...
	}
}

} // namespace dcpp
