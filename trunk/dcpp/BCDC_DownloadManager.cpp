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

#include "DownloadManager.h"

#include "SettingsManager.h"

namespace dcpp {

void DownloadManager::throttleReturnBytes(uint32_t b) {
	if (b > 0 && b < 2*mByteSlice) {
		mBytesSpokenFor -= b;
		if (mBytesSpokenFor < 0)
			mBytesSpokenFor = 0;
	}
}

size_t DownloadManager::throttleGetSlice()  {
	if (mThrottleEnable) {
		size_t left = mDownloadLimit - mBytesSpokenFor;
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
			return 0;
	} else {
		return (size_t)-1;
	}
}

size_t DownloadManager::throttleCycleTime() {
	if (mThrottleEnable)
		return mCycleTime;
	return 0;
}

void DownloadManager::throttleZeroCounters() {
	mBytesSpokenFor = 0;
	mBytesSent = 0;
}

void DownloadManager::throttleBytesTransferred(uint32_t i) {
	mBytesSent += i;
}

void DownloadManager::throttleSetup() {
// called once a second, plus when a download starts
// from the constructor to BufferedSocket
// with 64k, a few people get winsock error 0x2747
#define INBUFSIZE 64*1024
	unsigned int num_transfers = getDownloadCount();
	mDownloadLimit = SETTING(MAX_DOWNLOAD_SPEED_REAL_A_PARALLEL_LEVEL_OF_HOPEFULLY_USEFUL_INDIRECTION_IN_AN_ABSURDLY_LONG_IDENTIFIER)*1024;
	mThrottleEnable = BOOLSETTING(THROTTLE_ENABLE) && (mDownloadLimit > 0) && (num_transfers > 0);
	if (mThrottleEnable) {
		if (mDownloadLimit <= (INBUFSIZE * 10 * num_transfers)) {
			mByteSlice = mDownloadLimit / (7 * num_transfers);
			if (mByteSlice > INBUFSIZE)
				mByteSlice = INBUFSIZE;
			mCycleTime = 1000 / 10;
		}
		else {
			mByteSlice = INBUFSIZE;
			mCycleTime = 1000 * INBUFSIZE / mDownloadLimit;
		}
	}
}

} // namespace dcpp
