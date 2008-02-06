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

#ifndef FDM_MORE_WIN_UTIL_H
#define FDM_MORE_WIN_UTIL_H

//#include "../client/File.h"

class MoreWinUtil {
public:
	// For allowing for instances
	static bool allowMoreInstances();

	static bool possibleCommand(tstring cmd, tstring param, tstring& message, tstring& status);
	static void winampSpam(tstring param, tstring& message, tstring& status);

	static string findNickInString(const string aLine);
	static void addIPToString(string& aLine, string ip);
	
	static tstring commands;
//	void addFdmStuffToExceptionInfo(char* buf, File& aFile);

//	void initilize();
};

/*class StaticWindowsSettings 
{
public:
	static COLORREF opSpoken;
	static COLORREF notOpSpoken;
	static COLORREF iSpoke;
	static COLORREF myNickSpoken;
	static COLORREF doubleClickableLink;
};*/

#endif // !defined(FDM_MORE_WIN_UTIL_H)
