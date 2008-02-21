/*
 * Copyright (C) 2001-2008 Jacek Sieka, arnetheduck on gmail point com
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

#ifndef AUTO_SEARCH_PROPERTIES_H
#define AUTO_SEARCH_PROPERTIES_H

#include <dcpp/FdmAutoSearch.h>
#include "WidgetFactory.h"

class AutoSearchProperties : public WidgetFactory<SmartWin::WidgetModalDialog>
{
public:
	AutoSearchProperties(SmartWin::Widget* parent, AutoSearch *_search);
	virtual ~AutoSearchProperties();

	int run() { return createDialog(IDD_AUTOSEARCH_PROPERTIES); }
private:
	WidgetTextBoxPtr searchString;
	WidgetTextBoxPtr sizeString;
	WidgetTextBoxPtr matchString;
	WidgetTextBoxPtr excludeString;
	WidgetTextBoxPtr extensionString;
	WidgetTextBoxPtr minSize;
	WidgetTextBoxPtr maxSize;
	WidgetComboBoxPtr sourceType;
	WidgetComboBoxPtr sizeModes;
	WidgetComboBoxPtr sizeType;
	WidgetComboBoxPtr resSizeType;
	WidgetCheckBoxPtr active;
	WidgetCheckBoxPtr onlyIfOp;

	AutoSearch* search;

	bool handleInitDialog();
	void handleFocus();

	void handleOKClicked();
};

#endif // !defined(AUTO_SEARCH_PROPERTIES_H)
