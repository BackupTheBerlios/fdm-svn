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

#include "stdinc.h"
#include "DCPlusPlus.h"

#include "ClientManager.h"
#include "FdmUtil.h"

namespace dcpp {

string ClientManager::findNickOrIP(const string& findThis) {
	string lowerNick = Text::toLower(findThis);
	bool possibleIp = FdmUtil::isIp(findThis);
	string foundNicks;
	string foundIps;
	int countNicks = 0;
	int countIps = 0;
	
	Lock l(cs);

	for(OnlineIterC i = onlineUsers.begin(); i != onlineUsers.end(); ++i) {
		if (possibleIp) {
			if (i->second->getIdentity().getIp() == findThis) {
				countIps++;
				foundIps += (("In hub:\t") + i->second->getClient().getHubUrl() + ("\twith nick: ") + i->second->getIdentity().getNick() + ("\r\n"));
			}
			// Doubtful but you never know
			if ((Text::toLower(i->second->getIdentity().getNick())).find(lowerNick) != string::npos) {
				countNicks++;
				foundNicks += (("In hub:\t") + i->second->getClient().getHubUrl() + ("\twith nick: ") + i->second->getIdentity().getNick() + ("\r\n"));
			}
		} else {
			if ((Text::toLower(i->second->getIdentity().getNick())).find(lowerNick) != string::npos) {
				countNicks++;
				foundNicks += (("In hub:\t") + i->second->getClient().getHubUrl() + ("\twith nick: ") + i->second->getIdentity().getNick() + ("\r\n"));
			}
		}
	}

	foundNicks = (string("\r\nFound ") + Util::toString(countNicks) + string(" nicks matching: ") + findThis + ("\r\n") + foundNicks);
	foundIps = (string("\r\nFound ") + Util::toString(countIps) + string(" IPs matching: ") + findThis + ("\r\n") + foundIps);

	if (countNicks && countIps)
		return foundNicks + foundIps;
	else if (countNicks && !countIps)
		return foundNicks;
	else if (!countNicks && countIps)
		return foundIps;
	else if (possibleIp)
		return (("No IPs or nicks found matching: ") + findThis);
	else
		return (("No nicks found matching: ") + findThis);
}

} // namespace dcpp

/*OnlineUser* ClientManager::getOnlineUser(const string& aNick, const string& aHubUrl) {
	Lock l(cs);
	for(OnlineIter i = onlineUsers.begin(); i != onlineUsers.end(); ++i) {
		if(i->second->getIdentity().getNick() == aNick) {
			if(i->second->getClient().getHubUrl() == aHubUrl) {
				return i->second;
			}
		}
	}
	return NULL;
}*/

/*string ClientManager::getMyNick(const string& aHubUrl) {
	Lock l(cs);
	pair<OnlineIter, OnlineIter> p = onlineUsers.equal_range(getMe()->getCID());
	for(OnlineIter i = p.first; i != p.second; ++i) {
		if(i->second->getClient().getHubUrl() == aHubUrl) {
			return i->second->getIdentity().getNick();
		}
	}
	return Util::emptyString;
}*/
