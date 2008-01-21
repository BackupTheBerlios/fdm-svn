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

#include "../client/Util.h"

#include "../Other-Projects/DC++/client/SettingsManager.h"
#include "../Other-Projects/DC++/client/ResourceManager.h"
#include "../client/SearchManager.h"
#include "../client/ADLSearch.h"
#include "../client/ResourceManager.h"

#include "../client/CriticalSection.h"
#include "../client/Singleton.h"
#include "../client/TimerManager.h"
#include "../client/SearchManagerListener.h"

class AutoSearch
{
public:

	AutoSearch() : searchString("<Enter string>"), sourceType(0), sizeMode(1), size(-1), 
					typeFileSize(2), onlyIfOp(false), isActive(true),  
					resultsMatch(Util::emptyString), resultsExclude(Util::emptyString), resultsExtensions(Util::emptyString),
					resultsMinSize(0), resultsMaxSize(0), resultsTypeFileSize(2) {}
	virtual ~AutoSearch() {}
		
	// Search
	string searchString;
	int sourceType;
	int sizeMode;
	int64_t size;
	int typeFileSize;
	bool onlyIfOp;
	bool isActive;
	
	// Search Results
	string resultsMatch;
	string resultsExclude;
	string resultsExtensions;
	int64_t resultsMinSize;
	int64_t resultsMaxSize;
	int resultsTypeFileSize;

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

class AutoSearchManager : public Singleton<AutoSearchManager>, private TimerManagerListener, private SearchManagerListener
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

	bool getBlockAutoSearch() { return blockAutoSearch; }

	virtual void on(TimerManagerListener::Minute, uint32_t aTick) throw();
	virtual void on(SearchManagerListener::SR, SearchResult*) throw();

private:
	CriticalSection cs;

	bool blockAutoSearch;
	int curPos;
	long time;
	long timeToSearch;
	StringList clientsWhereOp;

	// Setup results on search, instead of every search result should save a few resources
	void clearAndAddToStringList(StringList& aStringList, string aString);

	StringList resMatch;
	StringList resExclude;
	StringList resExtensions;
	int64_t resMinSize;
	int64_t resMaxSize;

	// Same goes for not having to construct and destruct certain objects
	string extension;
	string fullPathInLower;
	string::size_type lastDot;
	StringList::const_iterator iter;

};

#endif // !defined(AUTO_SEARCH_H)