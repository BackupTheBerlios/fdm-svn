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

#ifndef DCPLUSPLUS_WIN32_APPEARANCE_2_PAGE_H
#define DCPLUSPLUS_WIN32_APPEARANCE_2_PAGE_H

#include "PropPage.h"

class Appearance2Page : public PropPage
{
public:
	Appearance2Page(dwt::Widget* parent);
	virtual ~Appearance2Page();

	virtual void write();

private:
	static TextItem texts[];

	struct SoundOption {
		const char* text;
		int setting;
		tstring file;
		unsigned helpId;
	};
	static SoundOption soundOptions[];

	LabelPtr example;
	TablePtr sounds;
	LabelPtr beepFileLabel;
	TextBoxPtr beepFile;
	ButtonPtr browse;

	COLORREF fg, bg, upBar, downBar;
	dwt::FontPtr font;
	LOGFONT logFont;
	int oldSelection;

	void handleBackgroundClicked();
	void handleTextClicked();
	void handleULClicked();
	void handleDLClicked();
	void handleSoundsHelp(HWND hWnd, unsigned id);
	void handleSelectionChanged();
	void handleBrowseClicked();

	void setBeepEnabled(bool enabled);
	void saveSoundOptions();
};

#endif // !defined(DCPLUSPLUS_WIN32_APPEARANCE_2_PAGE_H)
