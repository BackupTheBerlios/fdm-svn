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
 * Henrik Engström, henrikengstrom at home se
 */
 
 // Pothead used ADLSearch.cpp as a template

#include "Fdm-stdinc.h"
#include "../../client/DCPlusPlus.h"

#include "AutoSearch.h"

#include "../../client/File.h"
#include "../../client/SimpleXML.h"
#include "../Fdm-Util.h"

AutoSearchManager::AutoSearchManager() {
	TimerManager::getInstance()->addListener(this);
	Load();

	time = 0;
	pos = collection.begin();
}

AutoSearchManager::~AutoSearchManager() {
	TimerManager::getInstance()->removeListener(this);
	Save();
}

void AutoSearchManager::on(TimerManagerListener::Minute, u_int32_t /*aTick*/) throw() {
	Lock l(cs);
	time++;

	// Not ready to search
	if (time < 5 * 60)
		return;
	// Ready to search, block Autosearch for alternative sources
	else if (time == 5 * 60)
		StaticClientSettings::setBlockAutoSearch(true);

	// If not alright to search, retry in a minute
	if (!SearchManager::getInstance()->okToSearch())
		return;

	// If have some searches
	if (pos != collection.end()) {
		SearchManager::getInstance()->search(pos->searchString, (int64_t)pos->size * AutoSearch::GetSize(pos->typeFileSize), (SearchManager::TypeModes)pos->sourceType, (SearchManager::SizeModes)pos->typeFileSize, "auto");
		pos++;
	} else {
	// Finished autosearch
		time = 0;
		pos = collection.begin();
		StaticClientSettings::setBlockAutoSearch(false);
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//	Load old searches from disk
//
///////////////////////////////////////////////////////////////////////////////
void AutoSearchManager::Load()
{
	// Clear current
	collection.clear();

	// Load file as a string
	try {
		SimpleXML xml;
		xml.fromXML(File(getConfigFile(), File::READ, File::OPEN).read());

		if(xml.findChild("AutoSearch")) {
			xml.stepIn();

			// Predicted several groups of searches to be differentiated
			// in multiple categories. Not implemented yet.
			if(xml.findChild("SearchGroup")) {
				xml.stepIn();

				// Loop until no more searches found
				while(xml.findChild("Search")) {
					xml.stepIn();

					// Found another search, load it
					AutoSearch search;

					if(xml.findChild("SearchString")) {
						search.searchString = xml.getChildData();
					}
					if(xml.findChild("SourceType")) {
						search.sourceType = Util::toInt(xml.getChildData());
					}
					if(xml.findChild("SizeMode")) {
						search.sizeMode = Util::toInt(xml.getChildData());
					}
					if(xml.findChild("Size")) {
						search.size = Util::toInt64(xml.getChildData());
					}
					if(xml.findChild("SizeType")) {
						search.typeFileSize =Util::toInt(xml.getChildData());
					}
					if(xml.findChild("OnlyIfOp")) {
						search.onlyIfOp = (Util::toInt(xml.getChildData()) != 0);
					}
					if(xml.findChild("IsActive")) {
						search.isActive = (Util::toInt(xml.getChildData()) != 0);
					} 

					// Add search to collection
					if(search.searchString.size() > 0) {
						collection.push_back(search);
					}

					// Go to next search
					xml.stepOut();
				}
			}
		}
	} catch(const SimpleXMLException&) {
		return;
	} catch(const FileException&) {
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//	Save current searches to disk
//
///////////////////////////////////////////////////////////////////////////////
void AutoSearchManager::Save()
{
	// Prepare xml string for saving
	try {
		SimpleXML xml;

		xml.addTag("AutoSearch");
		xml.stepIn();

		// Predicted several groups of searches to be differentiated
		// in multiple categories. Not implemented yet.
		xml.addTag("SearchGroup");
		xml.stepIn();

		// Save all	searches
		for(AutoSearchCollection::iterator i = collection.begin(); i != collection.end(); ++i) {
			AutoSearch& search = *i;
			if(search.searchString.size() == 0) {
				continue;
			}
			string type = "type";
			xml.addTag("Search");
			xml.stepIn();

			xml.addTag("SearchString", search.searchString);
			xml.addChildAttrib(type, string("string"));

			xml.addTag("SourceType", search.sourceType);
			xml.addChildAttrib(type, string("int"));

			xml.addTag("SizeMode", search.sizeMode);
			xml.addChildAttrib(type, string("int"));

			xml.addTag("Size", search.size);
			xml.addChildAttrib(type, string("int64"));

			xml.addTag("SizeType", search.typeFileSize);
			xml.addChildAttrib(type, string("int"));

			xml.addTag("OnlyIfOp", search.onlyIfOp);
			xml.addChildAttrib(type, string("bool"));

			xml.addTag("IsActive", search.isActive);
			xml.addChildAttrib(type, string("bool"));

			xml.stepOut();
		}

		xml.stepOut();

		xml.stepOut();

		// Save string to file			
		try {
			File fout(getConfigFile(), File::WRITE, File::CREATE | File::TRUNCATE);
			fout.write(SimpleXML::utf8Header);
			fout.write(xml.toXML());
			fout.close();
		} catch(const FileException&) {
			return;
		}
	} catch(const SimpleXMLException&) {
		return;
	}
}