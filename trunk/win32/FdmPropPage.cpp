/*
 * Copyright (C) 2001-2007 Jacek Sieka, arnetheduck on gmail point com
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

#include "PropPage.h"

#include <dcpp/FdmSettingsManager.h>

void PropPage::fdmRead(HWND page, FdmItem const* items, FdmListItem* listItems /* = NULL */, HWND list /* = 0 */)
{
	dcassert(page != NULL);

	FdmSettingsManager* settings = FdmSettingsManager::getInstance();
	
	bool const useDef = true;
	for(FdmItem const* i = items; i->type != T_END; i++)
	{
		switch(i->type)
		{
		case T_STR:
			if(!settings->isDefault(i->setting)) {
				::SetDlgItemText(page, i->itemID,
					Text::toT(settings->get((FdmSettingsManager::StrSetting)i->setting, useDef)).c_str());
			}
			break;
		case T_INT:
			if(!settings->isDefault(i->setting)) {
				::SetDlgItemInt(page, i->itemID,
					settings->get((FdmSettingsManager::IntSetting)i->setting, useDef), FALSE);
			}
			break;
		case T_BOOL:
			if(settings->getBool((FdmSettingsManager::IntSetting)i->setting, useDef))
				::CheckDlgButton(page, i->itemID, BST_CHECKED);
			else
				::CheckDlgButton(page, i->itemID, BST_UNCHECKED);
		case T_CUSTOM: ;
		case T_END: ;
		}
	}

	if(listItems != NULL) {
		ListView_SetExtendedListViewStyle(list, LVS_EX_LABELTIP | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

		RECT rc;
		::GetClientRect(list, &rc);
		LVCOLUMN lv = { LVCF_FMT | LVCF_WIDTH };
		lv.fmt = LVCFMT_LEFT;
		lv.cx = rc.right - rc.left;
		ListView_InsertColumn(list, 0, &lv);

		LVITEM lvi = { LVIF_TEXT };
		for(int i = 0; listItems[i].setting != 0; i++) {
			lvi.iItem = i;
			lvi.pszText = const_cast<TCHAR*>(CTSTRING_I(listItems[i].desc));
			ListView_InsertItem(list, &lvi);
			ListView_SetCheckState(list, i, settings->getBool(FdmSettingsManager::IntSetting(listItems[i].setting), true));
		}

		ListView_SetColumnWidth(list, 0, LVSCW_AUTOSIZE);
	}
}

void PropPage::fdmWrite(HWND page, FdmItem const* items, FdmListItem* listItems /* = NULL */, HWND list /* = NULL */)
{
	dcassert(page != NULL);
	
	FdmSettingsManager* settings = FdmSettingsManager::getInstance();

	AutoArray<TCHAR> buf(SETTINGS_BUF_LEN);
	for(FdmItem const* i = items; i->type != T_END; i++)
	{
		switch(i->type)
		{
		case T_STR:
			{
				::GetDlgItemText(page, i->itemID, buf, SETTINGS_BUF_LEN);
				settings->set((FdmSettingsManager::StrSetting)i->setting, Text::fromT(tstring(buf)));

				break;
			}
		case T_INT:
			{
				::GetDlgItemText(page, i->itemID, buf, SETTINGS_BUF_LEN);
				settings->set((FdmSettingsManager::IntSetting)i->setting, Text::fromT(tstring(buf)));
				break;
			}
		case T_BOOL:
			{
				if(::IsDlgButtonChecked(page, i->itemID) == BST_CHECKED)
					settings->set((FdmSettingsManager::IntSetting)i->setting, true);
				else
					settings->set((FdmSettingsManager::IntSetting)i->setting, false);
			}
		case T_CUSTOM: ;
		case T_END: ;
		}
	}

	if(listItems != NULL) {
		int i;
		for(i = 0; listItems[i].setting != 0; i++) {
			settings->set(FdmSettingsManager::IntSetting(listItems[i].setting), ListView_GetCheckState(list, i) > 0);
		}
	}
}

void PropPage::fdmTranslate(HWND page, FdmTextItem* textItems)
{
	if (textItems != NULL) {
		for(int i = 0; textItems[i].itemID != 0; i++) {
			::SetDlgItemText(page, textItems[i].itemID,
				CTSTRING_I(textItems[i].translatedString));
		}
	}
}
