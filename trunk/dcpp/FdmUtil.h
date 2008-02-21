/* 
 * Copyright (C) 2005 Michael J Jones, mrmikejj at hotmail dot com
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

#ifndef FDM_UTIL_H
#define FDM_UTIL_H

namespace dcpp
{

class FdmUtil
{
public:
	static bool isIp(string aString);
	static bool isNumber(string aString);

	static bool toBool(int aInt);
	static string toString(bool aBool);

	static string addFdmDes(string aCurDes);
};

} // namespace dcpp

#endif // !defined(FDM_UTIL_H)
