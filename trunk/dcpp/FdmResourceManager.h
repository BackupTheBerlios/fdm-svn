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

#ifndef FDM_RESOURCE_MANAGER_H
#define FDM_RESOURCE_MANAGER_H

#include "Singleton.h"

namespace dcpp {

#define FDMSTRING(x) FdmResourceManager::getInstance()->getString(FdmResourceManager::x)
#define FDMCSTRING(x) FdmResourceManager::getInstance()->getString(FdmResourceManager::x).c_str()
#define FDMWSTRING(x) FdmResourceManager::getInstance()->getStringW(FdmResourceManager::x)
#define FDMCWSTRING(x) FdmResourceManager::getInstance()->getStringW(FdmResourceManager::x).c_str()

#define FDMSTRING_I(x) FdmResourceManager::getInstance()->getString(x)
#define FDMCSTRING_I(x) FdmResourceManager::getInstance()->getString(x).c_str()
#define FDMWSTRING_I(x) FdmResourceManager::getInstance()->getStringW(x)
#define FDMCWSTRING_I(x) FdmResourceManager::getInstance()->getStringW(x).c_str()

#ifdef UNICODE
#define FDMTSTRING FDMWSTRING
#define FDMTSTRING_I FDMWSTRING_I
#define FDMCTSTRING FDMCWSTRING
#define FDMCTSTRING_I FDMCWSTRING_I
#else
#define FDMTSTRING FDMSTRING
#define FDMTSTRING_I FDMSTRING_I
#define FDMCTSTRING FDMCSTRING
#define FDMCTSTRING_I FDMCSTRING_I
#endif

class FdmResourceManager : public Singleton<FdmResourceManager> {
public:
	
#include "FdmStringDefs.h"

	void loadLanguage(const string& aFile);
	const string& getString(FdmStrings x) const { dcassert(x >= 0 && x < LAST); return strings[x]; }
	const wstring& getStringW(FdmStrings x) const { dcassert(x >= 0 && x < LAST); return wstrings[x]; }
	bool isRTL() { return rtl; }
	
	template<typename T>
	TStringList getStrings(const T& t) const {
		const size_t n = sizeof(t) / sizeof(t[0]);
		TStringList ret(n);
		for(size_t i = 0; i < n; ++i) {
#ifdef UNICODE
			ret[i] = wstrings[t[i]];
#else
			ret[i] = strings[t[i]];
#endif
		}
		return ret;
	}
private:
	friend class Singleton<FdmResourceManager>;
	
	typedef unordered_map<string, FdmStrings> NameMap;
	typedef NameMap::iterator NameIter;

	FdmResourceManager(): rtl(false) {
		createWide();
	}

	virtual ~FdmResourceManager() { }
	
	static string strings[LAST];
	static wstring wstrings[LAST];
	static string names[LAST];

	bool rtl;

	void createWide();
};

} // namespace dcpp

#endif // !defined(FDM_RESOURCE_MANAGER_H)
