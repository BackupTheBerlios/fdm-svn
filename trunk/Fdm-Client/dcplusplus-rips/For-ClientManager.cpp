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

#include "Fdm-stdinc.h"
#include "../../client/DCPlusPlus.h"

#include "../../client/ClientManager.h"

OnlineUser* ClientManager::getOnLineUser(const string& aNick, const string& aHubUrl) {
	Lock l(cs);
	for(OnlineIter i = onlineUsers.begin(); i != onlineUsers.end(); ++i) {
		if(i->second->getIdentity().getNick() == aNick) {
			if(i->second->getClient().getHubUrl() == aHubUrl) {
				return i->second;
			}
		}
	}
	return NULL;
}

string ClientManager::getMyNick(const string& aHubUrl) {
	Lock l(cs);
	pair<OnlineIter, OnlineIter> p = onlineUsers.equal_range(getMe()->getCID());
	for(OnlineIter i = p.first; i != p.second; ++i) {
		if(i->second->getClient().getHubUrl() == aHubUrl) {
			return i->second->getIdentity().getNick();
		}
	}
	return Util::emptyString;
}