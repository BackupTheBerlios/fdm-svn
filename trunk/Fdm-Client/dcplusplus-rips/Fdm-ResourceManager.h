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

#if !defined(FDM_RESOURCE_MANAGER_H)
#define FDM_RESOURCE_MANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../client/Singleton.h"

class FdmResourceManager : public Singleton<FdmResourceManager> {
public:
	
#include "Fdm-StringDefs.h"

	void loadLanguage(const string& aFile);
	const string& getFdmString(FdmStrings x) const { dcassert(x >= 0 && x < LAST); return fdmstrings[x]; };
	const wstring& getFdmStringW(FdmStrings x) const { dcassert(x >= 0 && x < LAST); return fdmwstrings[x]; };

private:
	friend class Singleton<FdmResourceManager>;
	
	typedef HASH_MAP<string, FdmStrings> FdmNameMap;
	typedef FdmNameMap::iterator FdmNameIter;

	FdmResourceManager() {
		createFdmWide();
	};

	virtual ~FdmResourceManager() { };
	
	static string fdmstrings[LAST];
	static wstring fdmwstrings[LAST];
	static string fdmnames[LAST];

	void createFdmWide();
};


#define FDMSTRING(x) FdmResourceManager::getInstance()->getFdmString(FdmResourceManager::x)
#define FDMCSTRING(x) FdmResourceManager::getInstance()->getFdmString(FdmResourceManager::x).c_str()
#define FDMWSTRING(x) FdmResourceManager::getInstance()->getFdmStringW(FdmResourceManager::x)
#define FDMCWSTRING(x) FdmResourceManager::getInstance()->getFdmStringW(FdmResourceManager::x).c_str()

#define FDMSTRING_I(x) FdmResourceManager::getInstance()->getFdmString(x)
#define FDMCSTRING_I(x) FdmResourceManager::getInstance()->getFdmString(x).c_str()
#define FDMWSTRING_I(x) FdmResourceManager::getInstance()->getFdmStringW(x)
#define FDMCWSTRING_I(x) FdmResourceManager::getInstance()->getFdmStringW(x).c_str()

#ifdef UNICODE
#define FDMTSTRING WSTRING
#define FDMCTSTRING CWSTRING
#define FDMCTSTRING_I CWSTRING_I
#else
#define FDMTSTRING STRING
#define FDMCTSTRING CSTRING
#endif


#endif // !defined(FDM_RESOURCE_MANAGER_H)

/**
 * @file
 * $Id: ResourceManager.h,v 1.15 2005/04/24 08:13:11 arnetheduck Exp $
 */
