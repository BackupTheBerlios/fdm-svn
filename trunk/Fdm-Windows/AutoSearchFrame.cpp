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

/*
 * Automatic Directory Listing Search
 * Henrik Engstr�m, henrikengstrom at home se
 */
 
// Pothead used ADLSearchFrame.cpp as a template

#include "stdafx.h"
#include "../Resource.h"
#include "../client/DCPlusPlus.h"
#include "AutoSearchFrame.h"
#include "AutoSearchProperties.h"
#include "../Fdm-client/FdmUtil.h"

int AutoSearchFrame::columnIndexes[] = { 
	COLUMN_ACTIVE_SEARCH_STRING,
	COLUMN_SOURCE_TYPE,
	COLUMN_SIZE_MODES,
	COLUMN_SIZE,
	COLUMN_SIZE_TYPE,
	COLUMN_ONLY_WHERE_OP
};
int AutoSearchFrame::columnSizes[] = { 
	120, 
	90, 
	90, 
	90, 
	90,
	90
};
static FdmResourceManager::FdmStrings columnNames[] = { 
	FdmResourceManager::ACTIVE_SEARCH_STRING,
	FdmResourceManager::SOURCE_TYPE,
	FdmResourceManager::SIZE_MODES,
	FdmResourceManager::SIZE,
	FdmResourceManager::SIZE_TYPE,
	FdmResourceManager::ONLY_WHERE_OP
};

// Frame creation
LRESULT AutoSearchFrame::onCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Create status bar
	CreateSimpleStatusBar(ATL_IDS_IDLEMESSAGE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP);
	ctrlStatus.Attach(m_hWndStatusBar);
	int w[1] = { 0 };
	ctrlStatus.SetParts(1, w);

	// Create list control
	ctrlList.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		WS_HSCROLL | WS_VSCROLL | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE, IDC_ADLLIST);
	ctrlList.SetExtendedListViewStyle(LVS_EX_LABELTIP | LVS_EX_HEADERDRAGDROP | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

	// Set background color
	ctrlList.SetBkColor(WinUtil::bgColor);
	ctrlList.SetTextBkColor(WinUtil::bgColor);
	ctrlList.SetTextColor(WinUtil::textColor);

	// Create listview columns
	WinUtil::splitTokens(columnIndexes, FDMSETTING(AUTO_SEARCH_FRAME_ORDER), COLUMN_LAST);
	WinUtil::splitTokens(columnSizes, FDMSETTING(AUTO_SEARCH_FRAME_WIDTHS), COLUMN_LAST);
	for(int j = 0; j < COLUMN_LAST; j++) 
	{
		int fmt = LVCFMT_LEFT;
		ctrlList.InsertColumn(j, FDMCTSTRING_I(columnNames[j]), fmt, columnSizes[j], j);
	}
	ctrlList.SetColumnOrderArray(COLUMN_LAST, columnIndexes);

	// Create buttons
	ctrlAdd.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		BS_PUSHBUTTON , 0, IDC_ADD);
	ctrlAdd.SetWindowText(CTSTRING(NEW));
	ctrlAdd.SetFont(WinUtil::font);

	ctrlEdit.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		BS_PUSHBUTTON , 0, IDC_EDIT);
	ctrlEdit.SetWindowText(CTSTRING(PROPERTIES));
	ctrlEdit.SetFont(WinUtil::font);

	ctrlRemove.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		BS_PUSHBUTTON , 0, IDC_REMOVE);
	ctrlRemove.SetWindowText(CTSTRING(REMOVE));
	ctrlRemove.SetFont(WinUtil::font);

	ctrlMoveUp.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		BS_PUSHBUTTON , 0, IDC_MOVE_UP);
	ctrlMoveUp.SetWindowText(CTSTRING(MOVE_UP));
	ctrlMoveUp.SetFont(WinUtil::font);

	ctrlMoveDown.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		BS_PUSHBUTTON , 0, IDC_MOVE_DOWN);
	ctrlMoveDown.SetWindowText(CTSTRING(MOVE_DOWN));
	ctrlMoveDown.SetFont(WinUtil::font);

	ctrlHelp.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		BS_PUSHBUTTON , 0, IDC_HELP_FAQ);
	ctrlHelp.SetWindowText(CTSTRING(MENU_HELP));
	ctrlHelp.SetFont(WinUtil::font);

	// Create context menu
	contextMenu.CreatePopupMenu();
	contextMenu.AppendMenu(MF_STRING, IDC_ADD,    CTSTRING(NEW));
	contextMenu.AppendMenu(MF_STRING, IDC_REMOVE, CTSTRING(REMOVE));
	contextMenu.AppendMenu(MF_STRING, IDC_EDIT,   CTSTRING(PROPERTIES));

	// Load all searches
	LoadAll();

	bHandled = FALSE;
	return TRUE;
}

// Close window
LRESULT AutoSearchFrame::onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) 
{
	AutoSearchManager::getInstance()->Save();

	WinUtil::saveHeaderOrder(ctrlList, FdmSettingsManager::AUTO_SEARCH_FRAME_ORDER, 
		FdmSettingsManager::AUTO_SEARCH_FRAME_WIDTHS, COLUMN_LAST, columnIndexes, columnSizes);
	
	bHandled = FALSE;
	return 0;
}

// Recalculate frame control layout
void AutoSearchFrame::UpdateLayout(BOOL bResizeBars /* = TRUE */) 
{
	RECT rect;
	GetClientRect(&rect);

	// Position bars and offset their dimensions
	UpdateBarsPosition(rect, bResizeBars);
	if(ctrlStatus.IsWindow()) 
	{
		CRect sr;
		int w[1];
		ctrlStatus.GetClientRect(sr);
		w[0] = sr.Width() - 16;
		ctrlStatus.SetParts(1, w);
	}

	// Position list control
	CRect rc = rect;
	rc.top += 2;
	rc.bottom -= 28;
	ctrlList.MoveWindow(rc);

	// Position buttons
	const long bwidth = 90;
	const long bspace = 10;
	rc = rect;
	rc.bottom -= 2;
	rc.top = rc.bottom - 22;

	rc.left = 2;
	rc.right = rc.left + bwidth;
	ctrlAdd.MoveWindow(rc);

	rc.left += bwidth + 2;
	rc.right = rc.left + bwidth;
	ctrlEdit.MoveWindow(rc);

	rc.left += bwidth + 2;
	rc.right = rc.left + bwidth;
	ctrlRemove.MoveWindow(rc);

	rc.left += bspace;

	rc.left += bwidth + 2;
	rc.right = rc.left + bwidth;
	ctrlMoveUp.MoveWindow(rc);

	rc.left += bwidth + 2;
	rc.right = rc.left + bwidth;
	ctrlMoveDown.MoveWindow(rc);

	rc.left += bspace;

	rc.left += bwidth + 2;
	rc.right = rc.left + bwidth;
	ctrlHelp.MoveWindow(rc);

}

// Keyboard shortcuts
LRESULT AutoSearchFrame::onKeyDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	NMLVKEYDOWN* kd = (NMLVKEYDOWN*) pnmh;
	switch(kd->wVKey) 
	{
	case VK_INSERT:
		PostMessage(WM_COMMAND, IDC_ADD, 0);
		break;
	case VK_DELETE:
		PostMessage(WM_COMMAND, IDC_REMOVE, 0);
		break;
	case VK_RETURN:
		PostMessage(WM_COMMAND, IDC_EDIT, 0);
		break;
	default:
		bHandled = FALSE;
	}
	return 0;
}
	
LRESULT AutoSearchFrame::onContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	if(reinterpret_cast<HWND>(wParam) == ctrlList) { 
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		
		if(pt.x == -1 && pt.y == -1) {
			WinUtil::getContextMenuPos(ctrlList, pt);
		}

		int status = ctrlList.GetSelectedCount() > 0 ? MFS_ENABLED : MFS_GRAYED;
		contextMenu.EnableMenuItem(IDC_EDIT, status);
		contextMenu.EnableMenuItem(IDC_REMOVE, status);
		contextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd);
		return TRUE; 
	}
	bHandled = FALSE;
	return FALSE; 
}

// Add new search
LRESULT AutoSearchFrame::onAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
{
	// Invoke edit dialog with fresh search
	AutoSearch search;
	AutoSearchProperties dlg(&search);

	if(dlg.DoModal((HWND)*this) == IDOK)
	{
		// Add new search to the end or if selected, just before
		AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;

		int i = ctrlList.GetNextItem(-1, LVNI_SELECTED);
		if(i < 0)
		{
			// Add to end
			collection.push_back(search);
			i = collection.size() - 1;
		}
		else
		{
			// Add before selection
			collection.insert(collection.begin() + i, search);
		}

		// Update list control
		int j = i;
		while(j < (int)collection.size())
		{
			UpdateSearch(j++);
		}
		ctrlList.EnsureVisible(i, FALSE);
	}

	return 0;
}

// Edit AutoSearchFrame search
LRESULT AutoSearchFrame::onEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
{
	// Get selection info
	int i = ctrlList.GetNextItem(-1, LVNI_SELECTED);
	if(i < 0)
	{
		// Nothing selected
		return 0;
	}

	// Edit existing
	AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;
	AutoSearch search = collection[i];

	// Invoke dialog with selected search
	AutoSearchProperties dlg(&search);
	if(dlg.DoModal((HWND)*this) == IDOK)
	{
		// Update search collection
		collection[i] = search;

		// Update list control
		UpdateSearch(i);	  
	}

	return 0;
}

// Remove searches
LRESULT AutoSearchFrame::onRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
{
	AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;

	// Loop over all selected items
	int i;
	while((i = ctrlList.GetNextItem(-1, LVNI_SELECTED)) >= 0)
	{
		collection.erase(collection.begin() + i);
		ctrlList.DeleteItem(i);
	}
	return 0;
}

// Help
LRESULT AutoSearchFrame::onHelpButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
//	HtmlHelp(m_hWnd, WinUtil::getHelpFile().c_str(), HH_HELP_CONTEXT, IDR_AUTOSEARCH);
	return 0;
}

LRESULT AutoSearchFrame::onHelpKey(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
//	HtmlHelp(m_hWnd, WinUtil::getHelpFile().c_str(), HH_HELP_CONTEXT, IDR_AUTOSEARCH);
	return 0;
}

// Move selected entries up one step
LRESULT AutoSearchFrame::onMoveUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
{
	AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;

	// Get selection
	vector<int> sel;
	int i = -1;
	while((i = ctrlList.GetNextItem(i, LVNI_SELECTED)) >= 0)
	{
		sel.push_back(i);
	}
	if(sel.size() < 1)
	{
		return 0;
	}

	// Find out where to insert
	int i0 = sel[0];
	if(i0 > 0)
	{
		i0 = i0 - 1;
	}

	// Backup selected searches
	AutoSearchManager::AutoSearchCollection backup;
	for(i = 0; i < (int)sel.size(); ++i)
	{
		backup.push_back(collection[sel[i]]);
	}

	// Erase selected searches
	for(i = sel.size() - 1; i >= 0; --i)
	{
		collection.erase(collection.begin() + sel[i]);
	}

	// Insert (grouped together)
	for(i = 0; i < (int)sel.size(); ++i)
	{
		collection.insert(collection.begin() + i0 + i, backup[i]);
	}

	// Update UI
	LoadAll();

	// Restore selection
	for(i = 0; i < (int)sel.size(); ++i)
	{
		ctrlList.SetItemState(i0 + i, LVNI_SELECTED, LVNI_SELECTED);
	}

	return 0;
}

// Move selected entries down one step
LRESULT AutoSearchFrame::onMoveDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) 
{
	AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;

	// Get selection
	vector<int> sel;
	int i = -1;
	while((i = ctrlList.GetNextItem(i, LVNI_SELECTED)) >= 0)
	{
		sel.push_back(i);
	}
	if(sel.size() < 1)
	{
		return 0;
	}

	// Find out where to insert
	int i0 = sel[sel.size() - 1] + 2;
	if(i0 > (int)collection.size())
	{
		i0 = collection.size();
	}

	// Backup selected searches
	AutoSearchManager::AutoSearchCollection backup;
	for(i = 0; i < (int)sel.size(); ++i)
	{
		backup.push_back(collection[sel[i]]);
	}

	// Erase selected searches
	for(i = sel.size() - 1; i >= 0; --i)
	{
		collection.erase(collection.begin() + sel[i]);
		if(i < i0)
		{
			i0--;
		}
	}

	// Insert (grouped together)
	for(i = 0; i < (int)sel.size(); ++i)
	{
		collection.insert(collection.begin() + i0 + i, backup[i]);
	}

	// Update UI
	LoadAll();

	// Restore selection
	for(i = 0; i < (int)sel.size(); ++i)
	{
		ctrlList.SetItemState(i0 + i, LVNI_SELECTED, LVNI_SELECTED);
	}
	ctrlList.EnsureVisible(i0, FALSE);

	return 0;
}

// Clicked 'Active' check box
LRESULT AutoSearchFrame::onItemChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) 
{
	NMITEMACTIVATE* item = (NMITEMACTIVATE*)pnmh;

	if((item->uChanged & LVIF_STATE) == 0)
		return 0;
	if((item->uOldState & INDEXTOSTATEIMAGEMASK(0xf)) == 0)
		return 0;
	if((item->uNewState & INDEXTOSTATEIMAGEMASK(0xf)) == 0)
		return 0;

	if(item->iItem >= 0)
	{
		// Set new active status check box
		AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;
		AutoSearch& search = collection[item->iItem];
		search.isActive = (ctrlList.GetCheckState(item->iItem) != 0);
	}
	return 0;
}

// Double-click on list control
LRESULT AutoSearchFrame::onDoubleClickList(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) 
{
	NMITEMACTIVATE* item = (NMITEMACTIVATE*)pnmh;

	if(item->iItem >= 0) {
		// Treat as onEdit command
		PostMessage(WM_COMMAND, IDC_EDIT, 0);
	} else if(item->iItem == -1) {
		PostMessage(WM_COMMAND, IDC_ADD, 0);
	}

	return 0;
}

// Load all searches from manager
void AutoSearchFrame::LoadAll()
{
	// Clear current contents
	ctrlList.DeleteAllItems();

	// Load all searches
	AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;
	for(unsigned long l = 0; l < collection.size(); l++)
	{
		UpdateSearch(l, FALSE);
	}
}

// Update a specific search item
void AutoSearchFrame::UpdateSearch(int index, BOOL doDelete)
{
	AutoSearchManager::AutoSearchCollection& collection = AutoSearchManager::getInstance()->collection;

	// Check args
	if(index >= (int)collection.size())
	{
		return;
	}
	AutoSearch& search = collection[index];

	// Delete from list control
	if(doDelete)
	{
		ctrlList.DeleteItem(index);
	}

	// Generate values
	TStringList line;
	tstring fs;
	line.push_back(Text::toT(search.searchString));
	line.push_back(AutoSearch::TypeModesToTString(search.sourceType));
	line.push_back(AutoSearch::SizeModesToTString(search.sizeMode));
	line.push_back(Text::toT(Util::toString(search.size)));
	line.push_back(AutoSearch::SizeTypeToTString(search.typeFileSize));
	line.push_back(Text::toT(FdmUtil::toString(FdmUtil::toBool(search.onlyIfOp))));

	// Insert in list control
	ctrlList.insert(index, line);

	// Update 'Active' check box
	ctrlList.SetCheckState(index, search.isActive);
}