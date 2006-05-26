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

namespace SortChat
{
	class ColourUtil
	{
	public:
		ColourUtil(long orig, bool timestamps) { origNumChars = orig; timeStamps = timestamps;	}
		~ColourUtil() {	}

		void colourRichEditCtrl(CRichEditCtrl& ctrlClient, string myNick, bool isOp);

	private:
		bool timeStamps;
		CHARFORMAT2 myBrush;
		long origNumChars;
		long newNumChars;
		long offSet;
		tstring newText;

		void initilize(CRichEditCtrl& ctrlClient);
		void colourText(CRichEditCtrl& ctrlClient, COLORREF colour, long startPos, long endPos);
		void findAndColourAllOf(CRichEditCtrl& ctrlClient, COLORREF colour, tstring textToFind);
	};

	string findNickInTString(const tstring aLine);
	void addIpToChat(tstring& aLine, string ip);
};

#endif // !defined(COLOUR_UTIL_H)