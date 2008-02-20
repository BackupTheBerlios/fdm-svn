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

#include "BufferedSocket.h"

#include "FdmSettingsManager.h"
#include "TimerManager.h"

namespace dcpp {

void BufferedSocket::bcdcThreadRead(DownloadManager *dm, size_t& readsize, bool& throttling) {
	if(mode == MODE_DATA)
	{
		uint32_t getMaximum;
		throttling = dm->throttle();
		if (throttling)
		{
			getMaximum = dm->throttleGetSlice();
			readsize = (uint32_t)min((int64_t)inbuf.size(), (int64_t)getMaximum);
			if (readsize <= 0  || readsize > inbuf.size()) { // FIX
				sleep(dm->throttleCycleTime());
				return;
			}
		}
	}
}

void BufferedSocket::bcdcThreadRead2(int left, DownloadManager *dm, size_t readsize, bool throttling) {
	if (throttling) {
		if (left > 0 && left < (int)readsize) {
			dm->throttleReturnBytes(left - readsize);
		}
		uint32_t sleep_interval =  dm->throttleCycleTime();
		Thread::sleep(sleep_interval);
	}
}

void BufferedSocket::bcdcThreadSendFile(size_t& writeSize, size_t sockSize, size_t writePos, UploadManager *um, bool& throttling, size_t& start, size_t& sendMaximum) {
	throttling = FDMBOOLSETTING(THROTTLE_ENABLE);
	if(throttling) {
		start = TimerManager::getTick();
		sendMaximum = um->throttleGetSlice();
		if(sendMaximum < 0) {
			throttling = false;
			writeSize = min(sockSize / 2, writeBuf.size() - writePos);
		} else {
			writeSize = min(min(sockSize / 2, writeBuf.size() - writePos), sendMaximum);
		}
	} else {
		writeSize = min(sockSize / 2, writeBuf.size() - writePos);
	}
}

void BufferedSocket::bcdcThreadSendFile2(UploadManager *um, bool throttling, size_t start, size_t& current) {
	if(throttling) {
		int32_t cycle_time = um->throttleCycleTime();
		current = TimerManager::getTick();
		int32_t sleep_time = cycle_time - (current - start);
		if (sleep_time > 0 && sleep_time <= cycle_time) {
			Thread::sleep(sleep_time);
		}
	}
}

} // namespace dcpp
