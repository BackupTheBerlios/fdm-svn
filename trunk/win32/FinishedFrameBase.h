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

#ifndef DCPLUSPLUS_WIN32_FINISHED_FRAME_BASE_H
#define DCPLUSPLUS_WIN32_FINISHED_FRAME_BASE_H

#include "StaticFrame.h"
#include "TypedTable.h"
#include "TextFrame.h"
#include "ShellContextMenu.h"
#include "HoldRedraw.h"

#include <dcpp/File.h>
#include <dcpp/FinishedManager.h>
#include <dcpp/TimerManager.h>

template<class T, bool in_UL>
class FinishedFrameBase : 
	public StaticFrame<T>, 
	private FinishedManagerListener 
{
	typedef StaticFrame<T> BaseType;
public:
	enum Status {
		STATUS_STATUS,
		STATUS_COUNT,
		STATUS_BYTES,
		STATUS_SPEED,
		STATUS_LAST
	};

protected:
	typedef MDIChildFrame<T> MDIChildType;
	friend class StaticFrame<T>;
	friend class MDIChildFrame<T>;
	typedef FinishedFrameBase<T, in_UL> ThisType;
	
	FinishedFrameBase(dwt::TabView* mdiParent, const tstring& title, unsigned helpId, int icon) :
		BaseType(mdiParent, title, helpId, icon),
		items(0),
		totalBytes(0),
		totalTime(0)
	{
		{
			items = static_cast<T*>(this)->addChild(typename WidgetItems::Seed());
			items->setTableStyle(LVS_EX_LABELTIP | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT);
			addWidget(items);

			items->createColumns(WinUtil::getStrings(columnNames));
			items->setColumnOrder(WinUtil::splitTokens(SettingsManager::getInstance()->get(in_UL ? SettingsManager::FINISHED_UL_ORDER : SettingsManager::FINISHED_ORDER), columnIndexes));
			items->setColumnWidths(WinUtil::splitTokens(SettingsManager::getInstance()->get(in_UL ? SettingsManager::FINISHED_UL_WIDTHS : SettingsManager::FINISHED_WIDTHS), columnSizes));
			items->setSort(COLUMN_DONE);
			
			items->setSmallImageList(WinUtil::fileImages);

			items->onDblClicked(std::tr1::bind(&ThisType::handleDoubleClick, this));
			items->onKeyDown(std::tr1::bind(&ThisType::handleKeyDown, this, _1));
			items->onContextMenu(std::tr1::bind(&ThisType::handleContextMenu, this, _1));
		}

		this->initStatus();

		setStatusHelpId(STATUS_COUNT, in_UL ? IDH_FINISHED_UL_COUNT : IDH_FINISHED_DL_COUNT);
		setStatusHelpId(STATUS_BYTES, in_UL ? IDH_FINISHED_UL_BYTES : IDH_FINISHED_DL_BYTES);
		setStatusHelpId(STATUS_SPEED, in_UL ? IDH_FINISHED_UL_SPEED : IDH_FINISHED_DL_SPEED);

		layout();

		onSpeaker(std::tr1::bind(&ThisType::handleSpeaker, this, _1, _2));

		FinishedManager::getInstance()->addListener(this);
		updateList(FinishedManager::getInstance()->lockList(in_UL));
		FinishedManager::getInstance()->unlockList();
	}

	virtual ~FinishedFrameBase() { }

	void layout() {
		dwt::Rectangle r(this->getClientAreaSize());

		this->layoutStatus(r);
		items->setBounds(r);
	}

	bool preClosing() {
		FinishedManager::getInstance()->removeListener(this);
		return true;
	}

	void postClosing() {
		clearList();

		SettingsManager::getInstance()->set(in_UL ? SettingsManager::FINISHED_UL_ORDER : SettingsManager::FINISHED_ORDER, WinUtil::toString(items->getColumnOrder()));
		SettingsManager::getInstance()->set(in_UL ? SettingsManager::FINISHED_UL_WIDTHS : SettingsManager::FINISHED_WIDTHS, WinUtil::toString(items->getColumnWidths()));
	}

private:
	enum {
		SPEAK_ADD_LINE,
		SPEAK_REMOVE,
		SPEAK_REMOVE_ALL
	};

	enum {
		COLUMN_FIRST,
		COLUMN_FILE = COLUMN_FIRST,
		COLUMN_DONE,
		COLUMN_PATH,
		COLUMN_NICK,
		COLUMN_HUB,
		COLUMN_SIZE,
		COLUMN_SPEED,
		COLUMN_CRC32,
		COLUMN_LAST
	};

	static int columnSizes[COLUMN_LAST];
	static int columnIndexes[COLUMN_LAST];
	static const char* columnNames[COLUMN_LAST];

	class ItemInfo : public FastAlloc<ItemInfo> {
	public:
		ItemInfo(FinishedItemPtr entry_) : entry(entry_) {
			columns[COLUMN_FILE] = Text::toT(Util::getFileName(entry->getTarget()));
			columns[COLUMN_DONE] = Text::toT(Util::formatTime("%Y-%m-%d %H:%M:%S", entry->getTime()));
			columns[COLUMN_PATH] = Text::toT(Util::getFilePath(entry->getTarget()));
			columns[COLUMN_NICK] = Text::toT(entry->getUser());
			columns[COLUMN_HUB] = Text::toT(entry->getHub());
			columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(entry->getSize()));
			columns[COLUMN_SPEED] = Text::toT(Util::formatBytes(entry->getAvgSpeed()) + "/s");
			columns[COLUMN_CRC32] = entry->getCrc32Checked() ? T_("Yes") : T_("No");
		}

		FinishedItemPtr entry;

		const tstring& getText(int col) const {
			return columns[col];
		}
		int getImage() const {
			return WinUtil::getIconIndex(Text::toT(entry->getTarget()));
		}
		
		static int compareItems(ItemInfo* a, ItemInfo* b, int col) {
			switch(col) {
				case COLUMN_SIZE: return compare(a->entry->getSize(), b->entry->getSize());
				case COLUMN_SPEED: return compare(a->entry->getAvgSpeed(), b->entry->getAvgSpeed());
				default: return lstrcmpi(a->columns[col].c_str(), b->columns[col].c_str());
			}

		}

		void openFile() {
			WinUtil::openFile(Text::toT(entry->getTarget()));
		}

		void openFolder() {
			WinUtil::openFolder(Text::toT(entry->getTarget()));
		}

	private:
		tstring columns[COLUMN_LAST];
	};

	typedef TypedTable<ItemInfo> WidgetItems;
	typedef WidgetItems* WidgetItemsPtr;
	WidgetItemsPtr items;

	int64_t totalBytes, totalTime;

	LRESULT handleSpeaker(WPARAM wParam, LPARAM lParam) {
		if(wParam == SPEAK_ADD_LINE) {
			FinishedItemPtr entry = (FinishedItemPtr)lParam;
			addEntry(entry);
			this->setDirty(in_UL ? SettingsManager::BOLD_FINISHED_UPLOADS : SettingsManager::BOLD_FINISHED_DOWNLOADS);
			updateStatus();
		} else if(wParam == SPEAK_REMOVE) {
			updateStatus();
		} else if(wParam == SPEAK_REMOVE_ALL) {
			clearList();
			updateStatus();
		}
		return 0;
	}

	void handleDoubleClick() {
		if(items->hasSelected())
			items->getSelectedData()->openFile();
	}

	bool handleKeyDown(int c) {
		if(c == VK_DELETE) {
			this->postMessage(WM_COMMAND, IDC_REMOVE);
			return true;
		}
		return false;
	}

	bool handleContextMenu(dwt::ScreenCoordinate pt) {
		if(items->hasSelected()) {
			if(pt.x() == -1 && pt.y() == -1) {
				pt = items->getContextMenuPos();
			}

			if(BOOLSETTING(SHOW_SHELL_MENU) && items->countSelected() == 1) {
				string path = items->getSelectedData()->entry->getTarget();
				if(File::getSize(path) != -1) {
					CShellContextMenu shellMenu;
					shellMenu.SetPath(Text::utf8ToWide(path));

					typename T::Menu::Seed cs = WinUtil::Seeds::menu;
					cs.ownerDrawn = false;
					typename T::MenuPtr pShellMenu = this->addChild(cs);
					pShellMenu->appendItem(IDC_VIEW_AS_TEXT, T_("&View as text"), std::tr1::bind(&ThisType::handleViewAsText, this));
					pShellMenu->appendItem(IDC_OPEN_FILE, T_("&Open"), std::tr1::bind(&ThisType::handleOpenFile, this));
					pShellMenu->appendItem(IDC_OPEN_FOLDER, T_("Open &folder"), std::tr1::bind(&ThisType::handleOpenFolder, this));
					pShellMenu->appendSeparatorItem();
					pShellMenu->appendItem(IDC_REMOVE, T_("&Remove"), std::tr1::bind(&ThisType::handleRemove, this));
					pShellMenu->appendItem(IDC_REMOVE_ALL, T_("Remove &all"), std::tr1::bind(&ThisType::handleRemoveAll, this));
					pShellMenu->appendSeparatorItem();

					UINT idCommand = shellMenu.ShowContextMenu(pShellMenu, pt);
					if(idCommand != 0)
						this->postMessage(WM_COMMAND, idCommand);
					return true;
				}
			}

			typename T::MenuPtr contextMenu = this->addChild(WinUtil::Seeds::menu);
			contextMenu->appendItem(IDC_VIEW_AS_TEXT, T_("&View as text"), std::tr1::bind(&ThisType::handleViewAsText, this));
			contextMenu->appendItem(IDC_OPEN_FILE, T_("&Open"), std::tr1::bind(&ThisType::handleOpenFile, this));
			contextMenu->appendItem(IDC_OPEN_FOLDER, T_("Open &folder"), std::tr1::bind(&ThisType::handleOpenFolder, this));
			contextMenu->appendSeparatorItem();
			contextMenu->appendItem(IDC_REMOVE, T_("&Remove"), std::tr1::bind(&ThisType::handleRemove, this));
			contextMenu->appendItem(IDC_REMOVE_ALL, T_("Remove &all"), std::tr1::bind(&ThisType::handleRemoveAll, this));
			contextMenu->setDefaultItem(IDC_OPEN_FILE);
			contextMenu->trackPopupMenu(pt, TPM_LEFTALIGN | TPM_RIGHTBUTTON);
			return true;
		}
		return false;
	}

	void handleViewAsText() {
		int i = -1;
		while((i = items->getNext(i, LVNI_SELECTED)) != -1)
			new TextFrame(this->getParent(), items->getData(i)->entry->getTarget());
	}

	void handleOpenFile() {
		items->forEachSelected(&ItemInfo::openFile);
	}

	void handleOpenFolder() {
		items->forEachSelected(&ItemInfo::openFolder);
	}

	void handleRemove() {
		int i;
		while((i = items->getNext(-1, LVNI_SELECTED)) != -1) {
			FinishedManager::getInstance()->remove(items->getData(i)->entry, in_UL);
			items->erase(i);
		}
	}

	void handleRemoveAll() {
		FinishedManager::getInstance()->removeAll(in_UL);
	}

	void updateStatus() {
		setStatus(STATUS_COUNT, str(TFN_("%1% item", "%1% items", items->size()) % items->size()));
		setStatus(STATUS_BYTES, Text::toT(Util::formatBytes(totalBytes)));
		setStatus(STATUS_SPEED, str(TF_("%1%/s") % Text::toT(Util::formatBytes((totalTime > 0) ? totalBytes * ((int64_t)1000) / totalTime : 0))));
	}

	void updateList(const FinishedItemList& fl) {
		HoldRedraw hold(items);
		for(FinishedItemList::const_iterator i = fl.begin(); i != fl.end(); ++i) {
			addEntry(*i);
		}
		updateStatus();
	}

	void addEntry(FinishedItemPtr entry) {
		totalBytes += entry->getChunkSize();
		totalTime += entry->getMilliSeconds();

		int loc = items->insert(new ItemInfo(entry));
		items->ensureVisible(loc);
	}

	void clearList() {
		items->clear();
	}

	virtual void on(Added, bool upload, FinishedItemPtr entry) throw() {
		if(upload == in_UL)
			this->speak(SPEAK_ADD_LINE, (LPARAM)entry);
	}

	virtual void on(Removed, bool upload, FinishedItemPtr entry) throw() {
		if(upload == in_UL) {
			totalBytes -= entry->getChunkSize();
			totalTime -= entry->getMilliSeconds();
			this->speak(SPEAK_REMOVE);
		}
	}

	virtual void on(RemovedAll, bool upload) throw() {
		if(upload == in_UL) {
			totalBytes = 0;
			totalTime = 0;
			this->speak(SPEAK_REMOVE_ALL);
		}
	}
};

template <class T, bool in_UL>
int FinishedFrameBase<T, in_UL>::columnIndexes[] = { COLUMN_DONE, COLUMN_FILE, COLUMN_PATH, COLUMN_NICK, COLUMN_HUB, COLUMN_SIZE, COLUMN_SPEED, COLUMN_CRC32 };

template <class T, bool in_UL>
int FinishedFrameBase<T, in_UL>::columnSizes[] = { 100, 110, 290, 125, 80, 80, 80, 90 };

template <class T, bool in_UL>
const char* FinishedFrameBase<T, in_UL>::columnNames[] = { 
	N_("Filename"),
	N_("Time"),
	N_("Path"), 
	N_("Nick"),
	N_("Hub"),
	N_("Size"),
	N_("Speed"),
	N_("CRC Checked")
};

#endif // !defined(DCPLUSPLUS_WIN32_FINISHED_FRAME_BASE_H)
