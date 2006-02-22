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

#if !defined(DETECTIONDATABASE_H)
#define DETECTIONDATABASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../client/Singleton.h"

class DetectionDatabase : public Singleton<DetectionDatabase>
{
public:
	DetectionDatabase() {
		createDetectionDB();
	};

	virtual ~DetectionDatabase() throw() {
	};

	static void createDetectionDB();
	static void removeDetectionDB();
};

#endif // !defined(DETECTIONDATABASE_H)