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

/*
 * Automatic Directory Listing Search
 * Henrik Engstr�m, henrikengstrom at home se
 */
 
 // Pothead used ADLSearch.h as a template

#if !defined(AUTO_SEARCH_H)
#define AUTO_SEARCH_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../client/Util.h"

#include "Fdm-SettingsManager.h"
#include "../../client/SearchManager.h"
#include "../../client/ADLSearch.h"
#include "../../client/ResourceManager.h"
#include "Fdm-ResourceManager.h"

#include "../../client/CriticalSection.h"
#include "../../client/Singleton.h"
#include "../../client/TimerManager.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Class that represent an ADL search
//
///////////////////////////////////////////////////////////////////////////////
class AutoSearch
{
public:

	// Constructor
	AutoSearch() : searchString("<Enter string>"), sourceType(0), sizeMode(1), size(-1), 
		typeFileSize(2), onlyIfOp(false), isActive(true) {}

	string searchString;
	int sourceType;
	int sizeMode;
	int64_t size;
	int typeFileSize;
	bool onlyIfOp;
	bool isActive;

	static tstring TypeModesToTString(int i) {
		switch(i) {
			default:
			case SearchManager::TYPE_ANY: return TSTRING(ANY);
			case SearchManager::TYPE_AUDIO: return TSTRING(AUDIO);
			case SearchManager::TYPE_COMPRESSED: return TSTRING(COMPRESSED);
			case SearchManager::TYPE_DOCUMENT: return TSTRING(DOCUMENT);
			case SearchManager::TYPE_EXECUTABLE: return TSTRING(EXECUTABLE);
			case SearchManager::TYPE_PICTURE: return TSTRING(PICTURE);
			case SearchManager::TYPE_VIDEO: return TSTRING(VIDEO);
			case SearchManager::TYPE_DIRECTORY: return TSTRING(DIRECTORY);
			case SearchManager::TYPE_TTH: return _T("TTH");
		}
	}

	static tstring SizeModesToTString(int i) {
		switch(i) {
			default:
			case SearchManager::SIZE_DONTCARE: return TSTRING(NORMAL);
			case SearchManager::SIZE_ATLEAST: return TSTRING(AT_LEAST);
			case SearchManager::SIZE_ATMOST: return TSTRING(AT_MOST);
		}
	}

	static tstring SizeTypeToTString(int i) {
		switch(i) {
			default:
			case ADLSearch::SizeBytes:		return TSTRING(B);
			case ADLSearch::SizeKibiBytes:	return TSTRING(KiB);
			case ADLSearch::SizeMebiBytes:	return TSTRING(MiB);
			case ADLSearch::SizeGibiBytes:	return TSTRING(GiB);
		}
	}

	static int64_t GetSize(int i) {
		switch(i) {
			default:
			case 0:	return (int64_t)1;
			case 1:	return (int64_t)1024;
			case 2:	return (int64_t)1024 * (int64_t)1024;
			case 3:	return (int64_t)1024 * (int64_t)1024 * (int64_t)1024;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////
//
//	Class that holds all active searches
//
///////////////////////////////////////////////////////////////////////////////
class AutoSearchManager : public Singleton<AutoSearchManager>, private TimerManagerListener
{
public:
	// Constructor/destructor
	AutoSearchManager();
	virtual ~AutoSearchManager();

	// Search collection
	typedef vector<AutoSearch> AutoSearchCollection;
	AutoSearchCollection collection;

	// Load/save search collection to XML file
	void Load();
	void Save();

	string getConfigFile() { return Util::getConfigPath() + "AutoSearch.xml"; }

	virtual void on(TimerManagerListener::Minute, u_int32_t aTick) throw();

private:
	CriticalSection cs;

	long time;
	AutoSearchCollection::iterator pos;
};

#endif // !defined(AUTO_SEARCH_H)
