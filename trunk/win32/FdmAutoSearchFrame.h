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

#ifndef AUTO_SEARCH_FRAME_H
#define AUTO_SEARCH_FRAME_H

#include "StaticFrame.h"
#include <dcpp/FdmAutoSearch.h>

class AutoSearchFrame : public StaticFrame<AutoSearchFrame> {
public:
	enum Status {
		STATUS_STATUS,
		STATUS_LAST
	};

protected:
	typedef StaticFrame<AutoSearchFrame> BaseType;
	friend class StaticFrame<AutoSearchFrame>;
	friend class MDIChildFrame<AutoSearchFrame>;

	AutoSearchFrame(dwt::TabView* mdiParent);
	virtual ~AutoSearchFrame();

	void layout();

	bool preClosing();
	void postClosing();

private:
	enum {
		COLUMN_FIRST,
		COLUMN_ACTIVE_SEARCH_STRING = COLUMN_FIRST,
		COLUMN_SOURCE_TYPE,
		COLUMN_SIZE_MODES,
		COLUMN_SIZE,
		COLUMN_SIZE_TYPE,
		COLUMN_ONLY_WHERE_OP,
		COLUMN_LAST
	};

	static int columnSizes[COLUMN_LAST];
	static int columnIndexes[COLUMN_LAST];

	TablePtr items;
	ButtonPtr add;
	ButtonPtr properties;
	ButtonPtr up;
	ButtonPtr down;
	ButtonPtr remove;
	ButtonPtr help;

	void handleAdd();
	void handleProperties();
	void handleUp();
	void handleDown();
	void handleRemove();
	void handleDoubleClick();
	bool handleKeyDown(int c);
	LRESULT handleItemChanged(WPARAM /*wParam*/, LPARAM lParam);
	bool handleContextMenu(dwt::ScreenCoordinate sc);

	void addEntry(AutoSearch& search, int index = -1);
	
	string SizeTypeToString(int i) {
		switch(i) {
			default:
			case AutoSearch::SizeBytes:		return "B";
			case AutoSearch::SizeKibiBytes:	return "KiB";
			case AutoSearch::SizeMebiBytes:	return "MiB";
			case AutoSearch::SizeGibiBytes:	return "GiB";
		}
	}

	string SizeModesToString(int i) {
		switch(i) {
			default:
			case AutoSearch::SIZE_DONTCARE: return _("Normal");
			case AutoSearch::SIZE_ATLEAST: return _("At least");
			case AutoSearch::SIZE_ATMOST: return _("At most");
		}
	}

	string TypeModesToString(int i) {
		switch(i) {
			default:
			case AutoSearch::TYPE_ANY: return _("Any");
			case AutoSearch::TYPE_AUDIO: return _("Audio");
			case AutoSearch::TYPE_COMPRESSED: return _("Compressed");
			case AutoSearch::TYPE_DOCUMENT: return _("Document");
			case AutoSearch::TYPE_EXECUTABLE: return _("Executable");
			case AutoSearch::TYPE_PICTURE: return _("Picture");
			case AutoSearch::TYPE_VIDEO: return _("Video");
			case AutoSearch::TYPE_DIRECTORY: return _("Directory");
			case AutoSearch::TYPE_TTH: return _("TTH");
		}
	}
};

#endif // !defined(AUTO_SEARCH_FRAME_H)
