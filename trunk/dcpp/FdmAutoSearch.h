/*
 * Copyright (C) 2001-2008 Jacek Sieka, arnetheduck on gmail point com
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

#include "Util.h"

#include "ResourceManager.h"
#include "SearchManager.h"
#include "ADLSearch.h"

#include "CriticalSection.h"
#include "Singleton.h"
#include "TimerManager.h"
#include "SearchManagerListener.h"

namespace dcpp {
	
class AutoSearch
{
public:

	AutoSearch() : searchString("<Enter string>"), sourceType(TYPE_ANY), sizeModes(SIZE_DONTCARE), size(0), 
					typeFileSize(SizeMebiBytes), resultsTypeFileSize(SizeMebiBytes), onlyIfOp(false), isActive(true),  
					resultsMatch(Util::emptyString), resultsExclude(Util::emptyString), resultsExtensions(Util::emptyString),
					resultsMinSize(0), resultsMaxSize(0) {}
	virtual ~AutoSearch() {}
		
	// Search
	string searchString;

	enum SourceType {
		TYPE_ANY = 0,
		TYPE_AUDIO,
		TYPE_COMPRESSED,
		TYPE_DOCUMENT,
		TYPE_EXECUTABLE,
		TYPE_PICTURE,
		TYPE_VIDEO,
		TYPE_DIRECTORY,
		TYPE_TTH
	} sourceType;

	enum SizeModes {
		SIZE_DONTCARE = 0,
		SIZE_ATLEAST,
		SIZE_ATMOST
	} sizeModes;
	
	int64_t size;

	enum TypeFileSize {
		SizeBytes = 0,
		SizeKibiBytes,
		SizeMebiBytes,
		SizeGibiBytes
	} typeFileSize, resultsTypeFileSize;

	bool onlyIfOp;
	bool isActive;

	string resultsMatch;
	string resultsExclude;
	string resultsExtensions;
	int64_t resultsMinSize;
	int64_t resultsMaxSize;

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
	typedef vector<AutoSearch> SearchCollection;
	SearchCollection collection;

	// Load/save search collection to XML file
	void Load();
	void Save();

	virtual void on(TimerManagerListener::Minute, uint32_t aTick) throw();
	virtual void on(SearchManagerListener::SR, SearchResult*) throw();

private:
	CriticalSection cs;

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

	string getConfigFile() { return Util::getConfigPath() + "AutoSearch.xml"; }
};

} // namespace dcpp

#endif // !defined(AUTO_SEARCH_H)
