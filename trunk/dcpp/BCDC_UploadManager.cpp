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

#include "UploadManager.h"

#include "SettingsManager.h"

namespace dcpp {

size_t UploadManager::throttleGetSlice()  {
	if (mThrottleEnable) {
		size_t left = mUploadLimit - mBytesSpokenFor;
		if (left > 0) {
			if (left > 2*mByteSlice) {
				mBytesSpokenFor += mByteSlice;
				return mByteSlice;
			}
			else {
				mBytesSpokenFor += left;
				return left;
			}
		}
		else
			return 16; // must send > 0 bytes or threadSendFile thinks the transfer is complete
	} else {
		return (size_t)-1;
	}
}

size_t UploadManager::throttleCycleTime() {
	if (mThrottleEnable)
		return mCycleTime;
	return 0;
}

void UploadManager::throttleZeroCounters()  { // throttling
	if (mThrottleEnable) {
		mBytesSpokenFor = 0;
		mBytesSent = 0;
	}
}

void UploadManager::throttleBytesTransferred(uint32_t i)  { // throttling
	mBytesSent += i;
}

void UploadManager::throttleSetup() {
// called once a second, plus when uploads start
// from the constructor to BufferedSocket
	size_t INBUFSIZE = SETTING(SOCKET_OUT_BUFFER);
	unsigned int num_transfers = getUploadCount();
	mUploadLimit = SETTING(MAX_UPLOAD_SPEED_YAY_ANOTHER_LEVEL_OF_INDIRECTION_GO_GO_GO)*1024;
	mThrottleEnable = BOOLSETTING(THROTTLE_ENABLE) && (mUploadLimit > 0 && mUploadLimit != 1337) && (num_transfers > 0);
	if (mThrottleEnable) {
		if (mUploadLimit <= (INBUFSIZE * 10 * num_transfers)) {
			mByteSlice = mUploadLimit / (5 * num_transfers);
			if (mByteSlice > INBUFSIZE)
				mByteSlice = INBUFSIZE;
			mCycleTime = 1000 / 10;
		}
		else {
			mByteSlice = INBUFSIZE;
			mCycleTime = 1000 * INBUFSIZE / mUploadLimit;
		}
	}
}

} // namespace dcpp
