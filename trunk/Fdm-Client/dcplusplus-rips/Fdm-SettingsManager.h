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

#if !defined(FDM_SETTINGS_MANAGER_H)
#define FDM_SETTINGS_MANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../client/Util.h"
#include "../../client/Speaker.h"
#include "../../client/Singleton.h"

class SimpleXML;

class FdmSettingsManagerListener {
public:
	template<int I>	struct X { enum { TYPE = I };  };

	typedef X<0> Load;
	typedef X<1> Save;

	virtual void on(Load, SimpleXML*) throw() { }
	virtual void on(Save, SimpleXML*) throw() { }
};

class FdmSettingsManager : public Singleton<FdmSettingsManager>, public Speaker<FdmSettingsManagerListener>
{
public:

	enum StrFdmSetting { STR_FDM_FIRST,
		FDM_LANGUAGE_FILE = STR_FDM_FIRST, 
		ASTRING,
		STR_FDM_LAST };

	enum IntFdmSetting { INT_FDM_FIRST = STR_FDM_LAST + 1,
		AINT = INT_FDM_FIRST, 
		INT_FDM_LAST };

	enum Int64FdmSetting { INT64_FDM_FIRST = INT_FDM_LAST + 1,
		AINT64 = INT64_FDM_FIRST, 
		INT64_FDM_LAST, FDM_SETTINGS_LAST = INT64_FDM_LAST };

	const string& getFdm(StrFdmSetting key, bool useDefault = true) const {
		return (isFdmSet[key] || !useDefault) ? strFdmSettings[key - STR_FDM_FIRST] : strFdmDefaults[key - STR_FDM_FIRST];
	}

	int getFdm(IntFdmSetting key, bool useDefault = true) const {
		return (isFdmSet[key] || !useDefault) ? intFdmSettings[key - INT_FDM_FIRST] : intFdmDefaults[key - INT_FDM_FIRST];
	}
	int64_t getFdm(Int64FdmSetting key, bool useDefault = true) const {
		return (isFdmSet[key] || !useDefault) ? int64FdmSettings[key - INT64_FDM_FIRST] : int64FdmDefaults[key - INT64_FDM_FIRST];
	}

	bool getFdmBool(IntFdmSetting key, bool useDefault = true) const {
		return (getFdm(key, useDefault) != 0);
	}

	void setFdm(StrFdmSetting key, string const& value) {
		strFdmSettings[key - STR_FDM_FIRST] = value;
		isFdmSet[key] = !value.empty();
	}

	void setFdm(IntFdmSetting key, int value) {
		intFdmSettings[key - INT_FDM_FIRST] = value;
		isFdmSet[key] = true;
	}

	void setFdm(IntFdmSetting key, const string& value) {
		if(value.empty()) {
			intFdmSettings[key - INT_FDM_FIRST] = 0;
			isFdmSet[key] = false;
		} else {
			intFdmSettings[key - INT_FDM_FIRST] = Util::toInt(value);
			isFdmSet[key] = true;
		}
	}

	void setFdm(Int64FdmSetting key, int64_t value) {
		int64FdmSettings[key - INT64_FDM_FIRST] = value;
		isFdmSet[key] = true;
	}

	void setFdm(Int64FdmSetting key, const string& value) {
		if(value.empty()) {
			int64FdmSettings[key - INT64_FDM_FIRST] = 0;
			isFdmSet[key] = false;
		} else {
			int64FdmSettings[key - INT64_FDM_FIRST] = Util::toInt64(value);
			isFdmSet[key] = true;
		}
	}

	void setFdm(IntFdmSetting key, bool value) { setFdm(key, (int)value); }

	void setFdmDefault(StrFdmSetting key, string const& value) {
		strFdmDefaults[key - STR_FDM_FIRST] = value;
	}

	void setFdmDefault(IntFdmSetting key, int value) {
		intFdmDefaults[key - INT_FDM_FIRST] = value;
	}
	void setFdmDefault(Int64FdmSetting key, int64_t value) {
		int64FdmDefaults[key - INT64_FDM_FIRST] = value;
	}

	bool isFdmDefault(int aSet) { return !isFdmSet[aSet]; };

	void load() {
		load(Util::getAppPath() + "Fdm.xml");
	}
	void save() {
		save(Util::getAppPath() + "Fdm.xml");
	}

	void load(const string& aFileName);
	void save(const string& aFileName);

private:
	friend class Singleton<FdmSettingsManager>;
	FdmSettingsManager();
	virtual ~FdmSettingsManager() throw() { }

	static const string settingFdmTags[FDM_SETTINGS_LAST+1];

	string	strFdmSettings[STR_FDM_LAST - STR_FDM_FIRST];
	int		intFdmSettings[INT_FDM_LAST - INT_FDM_FIRST];
	int64_t int64FdmSettings[INT64_FDM_LAST - INT64_FDM_FIRST];
	string	strFdmDefaults[STR_FDM_LAST - STR_FDM_FIRST];
	int		intFdmDefaults[INT_FDM_LAST - INT_FDM_FIRST];
	int64_t int64FdmDefaults[INT64_FDM_LAST - INT64_FDM_FIRST];
	bool	isFdmSet[FDM_SETTINGS_LAST];
};

// Shorthand accessor macros
#define FDMSETTING(k) (FdmSettingsManager::getInstance()->getFdm(FdmSettingsManager::k, true))
#define FDMBOOLSETTING(k) (FdmSettingsManager::getInstance()->getFdmBool(FdmSettingsManager::k, true))

#endif // !defined(FDM_SETTINGS_MANAGER_H)

/**
 * @file
 * $Id: SettingsManager.h,v 1.101 2005/08/07 13:05:44 arnetheduck Exp $
 */