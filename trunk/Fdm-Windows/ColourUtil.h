/* 
 * Copyright (C) 2006 Michael J Jones, mrmikejj at hotmail dot com
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

#if !defined(COLOUR_UTIL_H)
#define COLOUR_UTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../client/User.h"

namespace SortChat
{
	string findNickInTString(const tstring aLine);

	class FdmCRichEditCtrl :  public CRichEditCtrl
	{
	public:
		FdmCRichEditCtrl();
		virtual ~FdmCRichEditCtrl() {};

		void extraInitilize(string aMyNick, bool usingTimeStamps);
		void prepareForAppend(OnlineUser* ou);
		void prepareForAppend(string nickOfSpeaker, bool opStatusOfSpeaker, string ipOfSpeaker, BOOL noscroll);

		// over ridden functions
		void AppendText(LPCTSTR newText);
		void SetSel(long nStartChar, long nEndChar, BOOL notScroll);
		void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = 0);

		void setMyNick(string aMyNick) { myNick = aMyNick; }

	private:
		void colourText(long startPos, long endPos);
		void colourAndAppend(LPCTSTR textToAdd, long colourFrom);
		void findAndColourDoubleClickable(const tstring& lookIn, long sizeBeforeAppend);

		// global vars
		CHARFORMAT2 myBrush;

		// settings
		BOOL noScroll;
		bool timeStamps;

		string myNick;
		string speakersNick;
		string speakersIP;
		bool speakersOpStatus;

		// kind of locky thingys
		bool prepared;
		bool ignoreNextAppend;
	};
};

#endif // !defined(COLOUR_UTIL_H)