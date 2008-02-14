/*
 * Copyright (C) 2001-2007 Jacek Sieka, arnetheduck on gmail point com
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

#include "stdafx.h"

#include "HubFrame.h"
#include <dcpp/ResourceManager.h>

tstring HubFrame::getStatusAverageShared() const {
	int64_t available;
	size_t userCount = 0;
	if (users->getSelectedCount() > 1) {
		available = users->forEachSelectedT(CountAvailable()).available;
		userCount = users->getSelectedCount();
	} else {
		available = std::for_each(userMap.begin(), userMap.end(), CountAvailable()).available;
		for(UserMap::const_iterator i = userMap.begin(); i != userMap.end(); ++i){
			UserInfo* ui = i->second;
			if(!ui->isHidden())
				userCount++;
		}
	}

	return (TSTRING(AVERAGE) + Text::toT(": " + Util::formatBytes(userCount > 0 ? available / userCount : 0)));
}
