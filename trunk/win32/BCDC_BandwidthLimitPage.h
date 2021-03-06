/*
 * Copyright (C) 2003 cologic, cologic@parsoma.net
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

#ifndef BANDWIDTHLIMITPAGE_H
#define BANDWIDTHLIMITPAGE_H

#include "PropPage.h"

class BandwidthLimitPage : public PropPage
{
public:
	BandwidthLimitPage(dwt::Widget* parent);
	virtual ~BandwidthLimitPage();

	virtual void write();
	virtual int getHelpId() { return IDD_BANDWIDTHPAGE; }

protected:
	static FdmItem items[];
	static FdmTextItem texts[];

	void handleThrottleEnableClicked();
	void setControlState();	
};

#endif //BANDWIDTHLIMITPAGE_H
