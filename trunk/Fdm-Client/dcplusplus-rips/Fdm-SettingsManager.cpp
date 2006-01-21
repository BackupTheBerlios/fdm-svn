/*
 * Copyright (C) 2001-2005 Jacek Sieka, arnetheduck on gmail point com
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

#include "../../client/stdinc.h"
#include "../../client/DCPlusPlus.h"

#include "Fdm-SettingsManager.h"
#include "Fdm-ResourceManager.h"

#include "../../client/SimpleXML.h"
#include "../../client/Util.h"
#include "../../client/File.h"

const string FdmSettingsManager::settingFdmTags[] =
{
	// Strings
	"FdmLanguageFileLocation", 
	"aString",
	"SENTRY", 
	// Ints
	"aInt",
	"SENTRY",
	// Int64
	"aInt64",
	"SENTRY"
};

FdmSettingsManager::FdmSettingsManager()
{
	for(int i=0; i<FDM_SETTINGS_LAST; i++)
		isFdmSet[i] = false;

	for(int j=0; j<INT_FDM_LAST-INT_FDM_FIRST; j++) {
		intFdmDefaults[j] = 0;
		intFdmSettings[j] = 0;
	}
	for(int k=0; k<INT64_FDM_LAST-INT64_FDM_FIRST; k++) {
		int64FdmDefaults[k] = 0;
		int64FdmSettings[k] = 0;
	}

	setFdmDefault(FDM_LANGUAGE_FILE, "testing");
	setFdmDefault(ASTRING, "test");
	setFdmDefault(AINT, 4);
	setFdmDefault(AINT64, 10);

#ifdef _WIN32
#endif
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
			
			for(i=STR_FDM_FIRST; i<STR_FDM_LAST; i++)
			{
				const string& attr = settingFdmTags[i];
				dcassert(attr.find("SENTRY") == string::npos);
				
				if(xml.findChild(attr))
					setFdm(StrFdmSetting(i), xml.getChildData());
				xml.resetCurrentChild();
			}
			for(i=INT_FDM_FIRST; i<INT_FDM_LAST; i++)
			{
				const string& attr = settingFdmTags[i];
				dcassert(attr.find("SENTRY") == string::npos);
				
				if(xml.findChild(attr))
					setFdm(IntFdmSetting(i), Util::toInt(xml.getChildData()));
				xml.resetCurrentChild();
			}
			for(i=INT64_FDM_FIRST; i<INT64_FDM_LAST; i++)
			{
				const string& attr = settingFdmTags[i];
				dcassert(attr.find("SENTRY") == string::npos);
				
				if(xml.findChild(attr))
					setFdm(Int64FdmSetting(i), Util::toInt64(xml.getChildData()));
				xml.resetCurrentChild();
			}
			
			xml.stepOut();
		}
	
		fire(FdmSettingsManagerListener::Load(), &xml);
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
	
	for(i=STR_FDM_FIRST; i<STR_FDM_LAST; i++) {
		if(isFdmSet[i]) {
			xml.addTag(settingFdmTags[i], getFdm(StrFdmSetting(i), false));
			xml.addChildAttrib(type, curType);
		}
	}

	curType = "int";
	for(i=INT_FDM_FIRST; i<INT_FDM_LAST; i++) {
		if(isFdmSet[i]) {
			xml.addTag(settingFdmTags[i], getFdm(IntFdmSetting(i), false));
			xml.addChildAttrib(type, curType);
		}
	}

	curType = "int64";
	for(i=INT64_FDM_FIRST; i<INT64_FDM_LAST; i++) {
		if(isFdmSet[i]) {
			xml.addTag(settingFdmTags[i], getFdm(Int64FdmSetting(i), false));
			xml.addChildAttrib(type, curType);
		}
	}
	xml.stepOut();
	
	fire(FdmSettingsManagerListener::Save(), &xml);

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
/**
 * @file
 * $Id: SettingsManager.cpp,v 1.129 2005/08/07 13:05:44 arnetheduck Exp $
 */
