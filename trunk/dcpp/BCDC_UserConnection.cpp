/*
 * Copyright (C) 2004 cologic, cologic@parsoma.net
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

#include "UserConnection.h"

#include "FdmSettingsManager.h"

namespace dcpp {

void UserConnection::setFlag(MaskType aFlag) {
	if(FDMSETTING(MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO) == 1337 && FDMBOOLSETTING(THROTTLE_ENABLE)) {
		if(aFlag == UserConnection::FLAG_UPLOAD) {
//			socket->setThreadPriority(Thread::LOW);
		} else if(aFlag == UserConnection::FLAG_DOWNLOAD) {
//			socket->setThreadPriority(Thread::NORMAL);
		}
	}
	this->dcpp::Flags::setFlag(aFlag);
}

} // namespace dcpp
