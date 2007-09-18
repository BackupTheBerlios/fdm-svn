/*
 * Copyright (C) 2001-2006 Jacek Sieka, arnetheduck on gmail point com
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

// From DC++ Trunk - svn700

#if !defined(FDM_RESOURCE_MANAGER_H)
#define FDM_RESOURCE_MANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../client/Singleton.h"

class FdmResourceManager : public Singleton<FdmResourceManager> {
public:
	
#include "../Fdm-Client/dcplusplus-rips/Fdm-StringDefs.h"

	void loadLanguage(const string& aFile);
	const string& getString(FdmStrings x) const { dcassert(x >= 0 && x < LAST); return strings[x]; }
	const wstring& getStringW(FdmStrings x) const { dcassert(x >= 0 && x < LAST); return wstrings[x]; }
	bool isRTL() { return rtl; }
private:
	friend class Singleton<FdmResourceManager>;
	
	typedef HASH_MAP<string, FdmStrings> NameMap;
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
#define FDMCTSTRING FDMCWSTRING
#define FDMCTSTRING_I FDMCWSTRING_I
#else
#define FDMTSTRING FDMSTRING
#define FDMCTSTRING FDMCSTRING
#endif


#endif // !defined(FDM_RESOURCE_MANAGER_H)
