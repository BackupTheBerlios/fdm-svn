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

#include "stdafx.h"

#include "resource.h"

#include "FdmAutoSearchFrame.h"

#include <dcpp/Client.h>
#include <dcpp/FdmUtil.h>
#include <dcpp/ResourceManager.h>
#include "HoldRedraw.h"
#include "FdmAutoSearchProperties.h"

int AutoSearchFrame::columnIndexes[] = { COLUMN_ACTIVE_SEARCH_STRING, COLUMN_SOURCE_TYPE, COLUMN_SIZE_MODES, COLUMN_SIZE, COLUMN_SIZE_TYPE, COLUMN_ONLY_WHERE_OP };
int AutoSearchFrame::columnSizes[] = { 120, 90, 90, 90, 90, 90 };
static ResourceManager::Strings columnNames[] = { 
	ResourceManager::ACTIVE_SEARCH_STRING,
	ResourceManager::SOURCE_TYPE,
	ResourceManager::SIZE_MODES,
	ResourceManager::SIZE,
	ResourceManager::SIZE_TYPE,
	ResourceManager::ONLY_WHERE_OP
};

AutoSearchFrame::AutoSearchFrame(SmartWin::WidgetTabView* mdiParent) :
	BaseType(mdiParent, TSTRING(MENU_FDM_AUTOSEARCH_FRAME), IDR_AUTOSEARCH),
	add(0),
	properties(0),
	up(0),
	down(0),
	remove(0),
	help(0)
{
	{
		WidgetListView::Seed cs = WinUtil::Seeds::listView;
		cs.lvStyle |= LVS_EX_CHECKBOXES;
		items = createListView(cs);
		addWidget(items);

		items->createColumns(ResourceManager::getInstance()->getStrings(columnNames));
		items->setColumnOrder(WinUtil::splitTokens(FDMSETTING(AUTO_SEARCH_FRAME_ORDER), columnIndexes));
		items->setColumnWidths(WinUtil::splitTokens(FDMSETTING(AUTO_SEARCH_FRAME_WIDTHS), columnSizes));
		items->setColor(WinUtil::textColor, WinUtil::bgColor);

		items->onDblClicked(std::tr1::bind(&AutoSearchFrame::handleDoubleClick, this));
		items->onKeyDown(std::tr1::bind(&AutoSearchFrame::handleKeyDown, this, _1));
		items->onRaw(std::tr1::bind(&AutoSearchFrame::handleItemChanged, this, _1, _2), SmartWin::Message(WM_NOTIFY, LVN_ITEMCHANGED));
		items->onContextMenu(std::tr1::bind(&AutoSearchFrame::handleContextMenu, this, _1));
	}

	{
		WidgetButton::Seed cs = WinUtil::Seeds::button;

		cs.caption = T_("&New...");
		add = createButton(cs);
		add->onClicked(std::tr1::bind(&AutoSearchFrame::handleAdd, this));
		addWidget(add);

		cs.caption = T_("&Properties");
		properties = createButton(cs);
		properties->onClicked(std::tr1::bind(&AutoSearchFrame::handleProperties, this));
		addWidget(properties);

		cs.caption = T_("Move &Up");
		up = createButton(cs);
		up->onClicked(std::tr1::bind(&AutoSearchFrame::handleUp, this));
		addWidget(up);

		cs.caption = T_("Move &Down");
		down = createButton(cs);
		down->onClicked(std::tr1::bind(&AutoSearchFrame::handleDown, this));
		addWidget(down);

		cs.caption = T_("&Remove");
		remove = createButton(cs);
		remove->onClicked(std::tr1::bind(&AutoSearchFrame::handleRemove, this));
		addWidget(remove);

		cs.caption = T_("&Help");
		help = createButton(cs);
		help->onClicked(std::tr1::bind(&AutoSearchFrame::handleHelp, this));
		addWidget(help);
	}

	initStatus();

	layout();

	// Load all searches
	AutoSearchManager::SearchCollection& collection = AutoSearchManager::getInstance()->collection;
	for(AutoSearchManager::SearchCollection::iterator i = collection.begin(); i != collection.end(); ++i)
		addEntry(*i);
}

AutoSearchFrame::~AutoSearchFrame() {
}

void AutoSearchFrame::layout() {
	SmartWin::Rectangle r(SmartWin::Point(0, 0), getClientAreaSize());

	layoutStatus(r);

	/// @todo dynamic width
	const int ybutton = add->getTextSize(_T("A")).y + 10;
	const int xbutton = 90;
	const int xborder = 10;

	SmartWin::Rectangle rb(r.getBottom(ybutton));
	r.size.y -= ybutton;
	items->setBounds(r);

	rb.size.x = xbutton;
	add->setBounds(rb);

	rb.pos.x += xbutton + xborder;
	properties->setBounds(rb);

	rb.pos.x += xbutton + xborder;
	up->setBounds(rb);

	rb.pos.x += xbutton + xborder;
	down->setBounds(rb);

	rb.pos.x += xbutton + xborder;
	remove->setBounds(rb);

	rb.pos.x += xbutton + xborder;
	help->setBounds(rb);
}

bool AutoSearchFrame::preClosing() {
	AutoSearchManager::getInstance()->Save();
	return true;
}

void AutoSearchFrame::postClosing() {
	FdmSettingsManager::getInstance()->set(FdmSettingsManager::AUTO_SEARCH_FRAME_ORDER, WinUtil::toString(items->getColumnOrder()));
	FdmSettingsManager::getInstance()->set(FdmSettingsManager::AUTO_SEARCH_FRAME_WIDTHS, WinUtil::toString(items->getColumnWidths()));
}

void AutoSearchFrame::handleAdd() {
	AutoSearch search;
	AutoSearchProperties dlg(this, &search);
	if(dlg.run() == IDOK) {
		AutoSearchManager::SearchCollection& collection = AutoSearchManager::getInstance()->collection;

		int index;

		// Add new search to the end or if selected, just before
		if(items->getSelectedCount() == 1) {
			index = items->getSelectedIndex();
			collection.insert(collection.begin() + index, search);
		} else {
			index = -1;
			collection.push_back(search);
		}

		addEntry(search, index);
	}
}

void AutoSearchFrame::handleProperties() {
	// Get selection info
	std::vector<unsigned> selected = items->getSelected();
	for(std::vector<unsigned>::const_iterator i = selected.begin(); i != selected.end(); ++i) {
		// Edit existing
		AutoSearchManager::SearchCollection& collection = AutoSearchManager::getInstance()->collection;
		AutoSearch search = collection[*i];

		// Invoke dialog with selected search
		AutoSearchProperties dlg(this, &search);
		if(dlg.run() == IDOK) {
			// Update search collection
			collection[*i] = search;

			// Update list control
			HoldRedraw hold(items);
			items->erase(*i);
			addEntry(search, *i);
			items->select(*i);
		}
	}
}

void AutoSearchFrame::handleUp() {
	AutoSearchManager::SearchCollection& collection = AutoSearchManager::getInstance()->collection;
	HoldRedraw hold(items);
	std::vector<unsigned> selected = items->getSelected();
	for(std::vector<unsigned>::const_iterator i = selected.begin(); i != selected.end(); ++i) {
		if(*i > 0) {
			AutoSearch search = collection[*i];
			swap(collection[*i], collection[*i - 1]);
			items->erase(*i);
			addEntry(search, *i - 1);
			items->select(*i - 1);
		}
	}
}

void AutoSearchFrame::handleDown() {
	AutoSearchManager::SearchCollection& collection = AutoSearchManager::getInstance()->collection;
	HoldRedraw hold(items);
	std::vector<unsigned> selected = items->getSelected();
	for(std::vector<unsigned>::reverse_iterator i = selected.rbegin(); i != selected.rend(); ++i) {
		if(*i < items->size() - 1) {
			AutoSearch search = collection[*i];
			swap(collection[*i], collection[*i + 1]);
			items->erase(*i);
			addEntry(search, *i + 1);
			items->select(*i + 1);
		}
	}
}

void AutoSearchFrame::handleRemove() {
	AutoSearchManager::SearchCollection& collection = AutoSearchManager::getInstance()->collection;
	int i;
	while((i = items->getNext(-1, LVNI_SELECTED)) != -1) {
		collection.erase(collection.begin() + i);
		items->erase(i);
	}
}

void AutoSearchFrame::handleHelp() {
	HtmlHelp(handle(), WinUtil::getHelpFile().c_str(), HH_HELP_CONTEXT, IDR_AUTOSEARCH);
}

void AutoSearchFrame::handleDoubleClick() {
	if(items->hasSelection()) {
		handleProperties();
	} else {
		handleAdd();
	}
}

bool AutoSearchFrame::handleKeyDown(int c) {
	switch(c) {
	case VK_INSERT:
		handleAdd();
		return true;
	case VK_DELETE:
		handleRemove();
		return true;
	case VK_RETURN:
		handleProperties();
		return true;
	}
	return false;
}

LRESULT AutoSearchFrame::handleItemChanged(WPARAM /*wParam*/, LPARAM lParam) {
	LPNMITEMACTIVATE item = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

	if((item->uChanged & LVIF_STATE) == 0)
		return 0;
	if((item->uOldState & INDEXTOSTATEIMAGEMASK(0xf)) == 0)
		return 0;
	if((item->uNewState & INDEXTOSTATEIMAGEMASK(0xf)) == 0)
		return 0;

	if(item->iItem >= 0)
	{
		// Set new active status check box
		AutoSearchManager::SearchCollection& collection = AutoSearchManager::getInstance()->collection;
		AutoSearch& search = collection[item->iItem];
		search.isActive = items->isChecked(item->iItem);
	}
	return 0;
}

bool AutoSearchFrame::handleContextMenu(SmartWin::ScreenCoordinate pt) {
	if(pt.x() == -1 && pt.y() == -1) {
		pt = items->getContextMenuPos();
	}

	WidgetMenuPtr contextMenu = createMenu(WinUtil::Seeds::menu);
	contextMenu->appendItem(IDC_ADD, T_("&New..."), std::tr1::bind(&AutoSearchFrame::handleAdd, this));
	contextMenu->appendItem(IDC_EDIT, T_("&Properties"), std::tr1::bind(&AutoSearchFrame::handleProperties, this));
	contextMenu->appendItem(IDC_REMOVE, T_("&Remove"), std::tr1::bind(&AutoSearchFrame::handleRemove, this));

	bool status = items->hasSelection();
	contextMenu->setItemEnabled(IDC_EDIT, false, status);
	contextMenu->setItemEnabled(IDC_REMOVE, false, status);

	contextMenu->trackPopupMenu(pt, TPM_LEFTALIGN | TPM_RIGHTBUTTON);
	return true;
}

void AutoSearchFrame::addEntry(AutoSearch& search, int index) {
	TStringList l;
	l.push_back(Text::toT(search.searchString));
	l.push_back(Text::toT(TypeModesToString(search.sourceType)));
	l.push_back(Text::toT(SizeModesToString(search.sizeModes)));
	l.push_back(Text::toT(Util::toString(search.size)));
	l.push_back(Text::toT(SizeTypeToString(search.typeFileSize)));
	l.push_back(Text::toT(FdmUtil::toString(FdmUtil::toBool(search.onlyIfOp))));
	int itemCount = items->insert(l, 0, index);
	if(index == -1)
		index = itemCount;
	items->setChecked(index, search.isActive);
	items->ensureVisible(index);
}
