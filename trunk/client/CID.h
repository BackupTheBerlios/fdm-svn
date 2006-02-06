/*
 * Copyright (C) 2001-2005 Jacek Sieka, arnetheduck on gmail point com
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

#if !defined(CID_H)
#define CID_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Encoder.h"
#include "Util.h"

class CID {
public:
	enum { SIZE = 192 / 8 };

	struct Hash {
		size_t operator()(const CID& c) const { return c.toHash(); }
	};
	CID() { memset(cid, 0, sizeof(cid)); }
	explicit CID(const u_int8_t* data) { memcpy(cid, data, sizeof(cid)); }
	explicit CID(const string& base32) { Encoder::fromBase32(base32.c_str(), cid, sizeof(cid)); }

	bool operator==(const CID& rhs) const { return memcmp(cid, rhs.cid, sizeof(cid)) == 0; }
	bool operator<(const CID& rhs) const { return memcmp(cid, rhs.cid, sizeof(cid)) < 0; }

	string toBase32() const { return Encoder::toBase32(cid, sizeof(cid)); }
	string& toBase32(string& tmp) const { return Encoder::toBase32(cid, sizeof(cid), tmp); }
	
	size_t toHash() const { return *reinterpret_cast<const size_t*>(cid); }
	const u_int8_t* getData() const { return cid; }

	bool isZero() const { return find_if(cid, cid+SIZE, bind2nd(not_equal_to<u_int8_t>(), 0)) == (cid+SIZE); }

	static CID generate() { 
		CID cid; 
		u_int32_t* c = (u_int32_t*)&cid.cid; 
		*(c++) = Util::rand();
		*(c++) = Util::rand();
		return cid;
	}

private:
	u_int8_t cid[SIZE];
};

#endif // !defined(CID_H)

/**
 * @file
 * $Id: CID.h,v 1.9 2006/02/05 13:38:44 arnetheduck Exp $
 */
