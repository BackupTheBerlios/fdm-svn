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

#ifndef FDM_APPEARANCE_PAGE_H
#define FDM_APPEARANCE_PAGE_H

#include "PropPage.h"

class FdmAppearancePage : public PropPage
{
public:
	FdmAppearancePage(dwt::Widget* parent);
	virtual ~FdmAppearancePage();

	virtual void write();
	virtual int getHelpId() { return IDD_FDM_APPEARANCE_PAGE; }

private:
	static FdmItem items[];
	static FdmTextItem texts[];
	static FdmListItem listItems[];

	COLORREF opSpoke, notOpSpoke, iSpoke, myNickSpoken;
	dwt::BrushPtr bgBrush;
	
	void handleBrowse();
	void handleOpSpoke();
	void handleNotOpSpoke();
	void handleISpoke();
	void handleMyNickSpoken();
};

#endif // !defined(FDM_APPEARANCE_PAGE_H)
