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

#ifndef FDM_DCPP_SETTINGS_MANAGER_H
#define FDM_DCPP_SETTINGS_MANAGER_H

#include "Util.h"
#include "Speaker.h"
#include "Singleton.h"

namespace dcpp {

class SimpleXML;

class FdmSettingsManagerListener {
public:
	virtual ~FdmSettingsManagerListener() { }
	template<int I>	struct X { enum { TYPE = I }; };

	typedef X<0> Load;
	typedef X<1> Save;

	virtual void on(Load, SimpleXML&) throw() { }
	virtual void on(Save, SimpleXML&) throw() { }
};

class FdmSettingsManager : public Singleton<FdmSettingsManager>, public Speaker<FdmSettingsManagerListener>
{
public:

	enum StrSetting { STR_FIRST,
		FDM_LANGUAGE_FILE = STR_FIRST,
		AUTO_SEARCH_FRAME_ORDER, AUTO_SEARCH_FRAME_WIDTHS,
		WINAMP_FORMAT, 
		STR_LAST };

	enum IntSetting { INT_FIRST = STR_LAST + 1,
		OP_SPOKE_COLOUR = INT_FIRST, NOT_OP_SPOKE_COLOUR, I_SPOKE_COLOUR, MY_NICK_SPOKEN_COLOUR,
		SHOW_IPS_IN_CHAT, THROTTLE_ENABLE, DOWNLOAD_SPEED, UPLOAD_SPEED,
		INT_LAST };

	enum Int64Setting { INT64_FIRST = INT_LAST + 1,
		AINT64 = INT64_FIRST,
		INT64_LAST,
		SETTINGS_LAST = INT64_LAST };

	const string& get(StrSetting key, bool useDefault = true) const {
		return (isSet[key] || !useDefault) ? strSettings[key - STR_FIRST] : strDefaults[key - STR_FIRST];
	}

	int get(IntSetting key, bool useDefault = true) const {
		return (isSet[key] || !useDefault) ? intSettings[key - INT_FIRST] : intDefaults[key - INT_FIRST];
	}
	int64_t get(Int64Setting key, bool useDefault = true) const {
		return (isSet[key] || !useDefault) ? int64Settings[key - INT64_FIRST] : int64Defaults[key - INT64_FIRST];
	}

	bool getBool(IntSetting key, bool useDefault = true) const {
		return (get(key, useDefault) != 0);
	}

	void set(StrSetting key, string const& value) {
			strSettings[key - STR_FIRST] = value;
		isSet[key] = !value.empty();
	}

	void set(IntSetting key, int value) {
		intSettings[key - INT_FIRST] = value;
		isSet[key] = true;
	}

	void set(IntSetting key, const string& value) {
		if(value.empty()) {
			intSettings[key - INT_FIRST] = 0;
			isSet[key] = false;
		} else {
			intSettings[key - INT_FIRST] = Util::toInt(value);
			isSet[key] = true;
		}
	}

	void set(Int64Setting key, int64_t value) {
		int64Settings[key - INT64_FIRST] = value;
		isSet[key] = true;
	}

	void set(Int64Setting key, const string& value) {
		if(value.empty()) {
			int64Settings[key - INT64_FIRST] = 0;
			isSet[key] = false;
		} else {
			int64Settings[key - INT64_FIRST] = Util::toInt64(value);
			isSet[key] = true;
		}
	}

	void set(IntSetting key, bool value) { set(key, (int)value); }

	void setDefault(StrSetting key, string const& value) {
		strDefaults[key - STR_FIRST] = value;
	}

	void setDefault(IntSetting key, int value) {
		intDefaults[key - INT_FIRST] = value;
	}
	void setDefault(Int64Setting key, int64_t value) {
		int64Defaults[key - INT64_FIRST] = value;
	}

	bool isDefault(int aSet) { return !isSet[aSet]; }

	void load() {
		load(Util::getConfigPath() + "Fdm.xml");
	}
	void save() {
		save(Util::getConfigPath() + "Fdm.xml");
	}

	void load(const string& aFileName);
	void save(const string& aFileName);

private:
	friend class Singleton<FdmSettingsManager>;
	FdmSettingsManager();
	virtual ~FdmSettingsManager() throw() { }

	static const string settingTags[SETTINGS_LAST+1];

	string strSettings[STR_LAST - STR_FIRST];
	int    intSettings[INT_LAST - INT_FIRST];
	int64_t int64Settings[INT64_LAST - INT64_FIRST];
	string strDefaults[STR_LAST - STR_FIRST];
	int    intDefaults[INT_LAST - INT_FIRST];
	int64_t int64Defaults[INT64_LAST - INT64_FIRST];
	bool isSet[SETTINGS_LAST];
};

// Shorthand accessor macros
#define FDMSETTING(k) (FdmSettingsManager::getInstance()->get(FdmSettingsManager::k, true))
#define FDMBOOLSETTING(k) (FdmSettingsManager::getInstance()->getBool(FdmSettingsManager::k, true))

} // namespace dcpp

#endif // !defined(FDM_SETTINGS_MANAGER_H)
