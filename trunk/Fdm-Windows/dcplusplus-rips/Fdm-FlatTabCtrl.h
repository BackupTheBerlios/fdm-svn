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

#if !defined(FDM_FLAT_TAB_CTRL_H)
#define FDM_FLAT_TAB_CTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Fdm-Client/dcplusplus-rips/Fdm-SettingsManager.h"
#include "../../Fdm-Client/dcplusplus-rips/Fdm-ResourceManager.h"

#include "Fdm-WinUtil.h"
#include "../resource.h"

enum {
	FDM_FT_FIRST = WM_APP + 700,
	/** This will be sent when the user presses a tab. WPARAM = HWND */
	FDM_FTM_SELECTED,
	/** The number of rows changed */
	FDM_FTM_ROWS_CHANGED,
	/** Set currently fdmActive tab to the HWND pointed by WPARAM */
	FDM_FTM_SETACTIVE,
	/** Display context menu and return TRUE, or return FALSE for the default one */
	FDM_FTM_CONTEXTMENU,
	/** Close window with postmessage... */
	FDM_WM_REALLY_CLOSE
};

template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE FdmFlatTabCtrlImpl : public CWindowImpl< T, TBase, TWinTraits> {
public:

	enum { FT_EXTRA_SPACE = 18 };

	FdmFlatTabCtrlImpl() : fdmClosing(NULL), fdmRows(1), fdmHeight(0), fdmActive(NULL), fdmMoving(NULL), inFdmTab(false) { 
		black.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	};
	virtual ~FdmFlatTabCtrlImpl() { }

	static LPCTSTR GetWndClassName()
	{
		return _T("FdmFlatTabCtrl");
	}

	void addFdmTab(HWND hWnd, COLORREF color = RGB(0, 0, 0)) {
		FdmTabInfo* i = new FdmTabInfo(hWnd, color);
		dcassert(getFdmTabInfo(hWnd) == NULL);
		fdmTabs.push_back(i);
		viewFdmOrder.push_back(hWnd);
		nextFdmTab = --viewFdmOrder.end();
		fdmActive = i;
		calcFdmRows(false);
		Invalidate();		
	}

	void removeFdmTab(HWND aWnd) {
		FdmTabInfo::ListIter i;
		for(i = fdmTabs.begin(); i != fdmTabs.end(); ++i) {
			if((*i)->hWnd == aWnd)
				break;
		}

		dcassert(i != fdmTabs.end());
		FdmTabInfo* ti = *i;
		if(fdmActive == ti)
			fdmActive = NULL;
		delete ti;
		fdmTabs.erase(i);
		dcassert(find(viewFdmOrder.begin(), viewFdmOrder.end(), aWnd) != viewFdmOrder.end());
		viewFdmOrder.erase(find(viewFdmOrder.begin(), viewFdmOrder.end(), aWnd));
		nextFdmTab = viewFdmOrder.end();
		if(!viewFdmOrder.empty())
			--nextFdmTab;

		calcFdmRows(false);
		Invalidate();
	}

	void startFdmSwitch() {
		nextFdmTab = --viewFdmOrder.end();
		inFdmTab = true;
	}

	void endFdmSwitch() {
		inFdmTab = false;
		if(fdmActive) 
		setFdmTop(fdmActive->hWnd);
	}

	HWND getFdmNext() {
		if(viewFdmOrder.empty())
			return NULL;
		if(nextFdmTab == viewFdmOrder.begin()) {
			nextFdmTab = --viewFdmOrder.end();
		} else {
			--nextFdmTab;
		}
		return *nextFdmTab;
	}
	HWND getFdmPrev() {
		if(viewFdmOrder.empty())
			return NULL;
		nextFdmTab++;
		if(nextFdmTab == viewFdmOrder.end()) {
			nextFdmTab = viewFdmOrder.begin();
		}
		return *nextFdmTab;
	}

	void setFdmActive(HWND aWnd) {
		if(!inFdmTab)
			setFdmTop(aWnd);

		FdmTabInfo* ti = getFdmTabInfo(aWnd);
		dcassert(ti != NULL);
		fdmActive = ti;
		ti->dirty = false;
		calcFdmRows(false);
		Invalidate();
	}

	void setFdmTop(HWND aWnd) {
		dcassert(find(viewFdmOrder.begin(), viewFdmOrder.end(), aWnd) != viewFdmOrder.end());
		viewFdmOrder.erase(find(viewFdmOrder.begin(), viewFdmOrder.end(), aWnd));
		viewFdmOrder.push_back(aWnd);
		nextFdmTab = --viewFdmOrder.end();
	}

	void setFdmDirty(HWND aWnd) {
		FdmTabInfo* ti = getFdmTabInfo(aWnd);
		dcassert(ti != NULL);
		bool inval = ti->updateFdm();
		
		if(fdmActive != ti) {
			if(!ti->dirty) {
				ti->dirty = true;
				inval = true;
			}
		}

		if(inval) {
			calcFdmRows(false);
			Invalidate();
		}
	}

	void setFdmColor(HWND aWnd, COLORREF color) {
		FdmTabInfo* ti = getFdmTabInfo(aWnd);
		if(ti != NULL) {
			ti->pen.DeleteObject();
			ti->pen.CreatePen(PS_SOLID, 1, color);
			Invalidate();
		}
	}

	void updateFdmFdmText(HWND aWnd, LPCTSTR text) {
		FdmTabInfo* ti = getFdmTabInfo(aWnd);
		if(ti != NULL) {
			ti->updateFdmFdmText(text);
			calcFdmRows(false);
			Invalidate();
		}
	}

	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_SIZE, onSize)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, onLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, onLButtonUp)
		MESSAGE_HANDLER(WM_CONTEXTMENU, onContextMenu)
		COMMAND_ID_HANDLER(IDC_FDM_CLOSE_WINDOW, onCloseWindow)
		COMMAND_ID_HANDLER(IDC_FDM_CHEVRON, onChevron)
		COMMAND_RANGE_HANDLER(IDC_FDM_SELECT_WINDOW, IDC_FDM_SELECT_WINDOW+fdmTabs.size(), onSelectWindow)
	END_MSG_MAP()

	LRESULT onLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
		int xPos = GET_X_LPARAM(lParam); 
		int yPos = GET_Y_LPARAM(lParam); 
		int row = getFdmRows() - ((yPos / getFdmTabHeight()) + 1);

		for(FdmTabInfo::ListIter i = fdmTabs.begin(); i != fdmTabs.end(); ++i) {
			FdmTabInfo* t = *i;
			if((row == t->row) && (xPos >= t->xpos) && (xPos < (t->xpos + t->getFdmWidth())) ) {
				// Bingo, this was clicked
				HWND hWnd = GetParent();
				if(hWnd) {
					if(wParam & MK_SHIFT)
						::SendMessage(t->hWnd, WM_CLOSE, 0, 0);
					else 
						fdmMoving = t;
				}
				break;
			}
		}
		return 0;
	}

	LRESULT onLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
		if (fdmMoving) {
			int xPos = GET_X_LPARAM(lParam); 
			int yPos = GET_Y_LPARAM(lParam); 
			int row = getFdmRows() - ((yPos / getFdmTabHeight()) + 1);
			
			bool moveLast = true;

			for(FdmTabInfo::ListIter i = fdmTabs.begin(); i != fdmTabs.end(); ++i) {
				FdmTabInfo* t = *i;
				if((row == t->row) && (xPos >= t->xpos) && (xPos < (t->xpos + t->getFdmWidth())) ) {
					// Bingo, this was clicked
					HWND hWnd = GetParent();
					if(hWnd) {
						if(t == fdmMoving) 
							::SendMessage(hWnd, FDM_FTM_SELECTED, (WPARAM)t->hWnd, 0);
						else{
							//check if the pointer is on the left or right half of the tab
							//to determine where to insert the tab
							moveFdmTabs(t, xPos > (t->xpos + (t->getFdmWidth()/2)));
						}
					}
					moveLast = false;
					break;
				}
			}
			if(moveLast)
				moveFdmTabs(fdmTabs.back(), true);
			fdmMoving = NULL;
		}
		return 0;
	}

	LRESULT onContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };        // location of mouse click 

		ScreenToClient(&pt); 
		int xPos = pt.x;
		int row = getFdmRows() - ((pt.y / getFdmTabHeight()) + 1);

		for(FdmTabInfo::ListIter i = fdmTabs.begin(); i != fdmTabs.end(); ++i) {
			FdmTabInfo* t = *i;
			if((row == t->row) && (xPos >= t->xpos) && (xPos < (t->xpos + t->getFdmWidth())) ) {
				// Bingo, this was clicked, check if the owner wants to handle it...
				if(!::SendMessage(t->hWnd, FDM_FTM_CONTEXTMENU, 0, lParam)) {
					fdmClosing = t->hWnd;
					ClientToScreen(&pt);
					CMenu mnu;
					mnu.CreatePopupMenu();
					mnu.AppendMenu(MF_STRING, IDC_FDM_CLOSE_WINDOW, FDMCTSTRING(CLOSE));
					mnu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_BOTTOMALIGN, pt.x, pt.y, m_hWnd);
				}
				break;
			}
		}
		return 0;
	}

	LRESULT onCloseWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(::IsWindow(fdmClosing))
			::SendMessage(fdmClosing, WM_CLOSE, 0, 0);
		return 0;
	}

	int getFdmTabHeight() { return fdmHeight; };
	int getFdmHeight() { return (getFdmRows() * getFdmTabHeight())+1; };
	int getFdmFill() { return (getFdmTabHeight() + 1) / 2; };

	int getFdmRows() { return fdmRows; };

	void calcFdmRows(bool inval = true) {
		CRect rc;
		GetClientRect(rc);
		int r = 1;
		int w = 0;
		bool notify = false;
		bool needInval = false;

		for(FdmTabInfo::ListIter i = fdmTabs.begin(); i != fdmTabs.end(); ++i) {
			FdmTabInfo* ti = *i;
			if( (r != 0) && ((w + ti->getFdmWidth() + getFdmFill()) > rc.Width()) ) {
				if(r >= SETTING(MAX_TAB_ROWS)) {
					notify |= (fdmRows != r);
					fdmRows = r;
					r = 0;
					chevron.EnableWindow(TRUE);
				} else {
					r++;
					w = 0;
				}
			} 
			ti->xpos = w;
			needInval |= (ti->row != (r-1));
			ti->row = r-1;
			w += ti->getFdmWidth();
		}

		if(r != 0) {
			chevron.EnableWindow(FALSE);
			notify |= (fdmRows != r);
			fdmRows = r;
		}

		if(notify) {
			::SendMessage(GetParent(), FDM_FTM_ROWS_CHANGED, 0, 0);
		}
		if(needInval && inval)
			Invalidate();
	}

	LRESULT onCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) { 
		chevron.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
			BS_PUSHBUTTON , 0, IDC_FDM_CHEVRON);
		chevron.SetWindowText(_T("»"));

		mnu.CreatePopupMenu();

		CDC dc(::GetDC(m_hWnd));
		HFONT oldfont = dc.SelectFont(FdmWinUtil::fdmFont);
		fdmHeight = FdmWinUtil::getTextHeight(dc) + 2;
		dc.SelectFont(oldfont);
		::ReleaseDC(m_hWnd, dc);
		
		return 0;
	}

	LRESULT onSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) { 
		calcFdmRows();
		SIZE sz = { LOWORD(lParam), HIWORD(lParam) };
		chevron.MoveWindow(sz.cx-14, 1, 14, getFdmHeight());
		return 0;
	}
		
	LRESULT onPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		RECT rc;
		bool drawActive = false;
		RECT crc;
		GetClientRect(&crc);

		if(GetUpdateRect(&rc, FALSE)) {
			CPaintDC dc(m_hWnd);
			HFONT oldfont = dc.SelectFont(FdmWinUtil::fdmFont);

			//ATLTRACE("%d, %d\n", rc.left, rc.right);
			for(FdmTabInfo::ListIter i = fdmTabs.begin(); i != fdmTabs.end(); ++i) {
				FdmTabInfo* t = *i;
				
				if(t->row != -1 && t->xpos < rc.right && t->xpos + t->getFdmWidth() + getFdmFill() >= rc.left ) {
					if(t != fdmActive) {
						drawFdmTab(dc, t, t->xpos, t->row);
					} else {
						drawActive = true;
					}
				}
			}
			HPEN oldpen = dc.SelectPen(black);
			for(int r = 0; r < fdmRows; r++) {
				dc.MoveTo(rc.left, r*getFdmTabHeight());
				dc.LineTo(rc.right, r*getFdmTabHeight());
			}

			if(drawActive) {
				dcassert(fdmActive);
				drawFdmTab(dc, fdmActive, fdmActive->xpos, fdmActive->row, true);
				dc.SelectPen(fdmActive->pen);
				int y = (fdmRows - fdmActive->row -1) * getFdmTabHeight();
				dc.MoveTo(fdmActive->xpos, y);
				dc.LineTo(fdmActive->xpos + fdmActive->getFdmWidth() + getFdmFill(), y);
			}
			dc.SelectPen(oldpen);
			dc.SelectFont(oldfont);
		}
		return 0;
	}

	LRESULT onChevron(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		while(mnu.GetMenuItemCount() > 0) {
			mnu.RemoveMenu(0, MF_BYPOSITION);
		}
		int n = 0;
		CRect rc;
		GetClientRect(&rc);
		CMenuItemInfo mi;
		mi.fMask = MIIM_ID | MIIM_TYPE | MIIM_DATA | MIIM_STATE;
		mi.fType = MFT_STRING | MFT_RADIOCHECK;

		for(FdmTabInfo::ListIter i = fdmTabs.begin(); i != fdmTabs.end(); ++i) {
			FdmTabInfo* ti = *i;
			if(ti->row == -1) {
				mi.dwTypeData = (LPTSTR)ti->name;
				mi.dwItemData = (ULONG_PTR)ti->hWnd;
				mi.fState = MFS_ENABLED | (ti->dirty ? MFS_CHECKED : 0);
				mi.wID = IDC_FDM_SELECT_WINDOW + n;
				mnu.InsertMenuItem(n++, TRUE, &mi);
			} 
		}

		POINT pt;
		chevron.GetClientRect(&rc);
		pt.x = rc.right - rc.left;
		pt.y = 0;
		chevron.ClientToScreen(&pt);
		
		mnu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_hWnd);
		return 0;
	}

	LRESULT onSelectWindow(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CMenuItemInfo mi;
		mi.fMask = MIIM_DATA;
		
		mnu.GetMenuItemInfo(wID, FALSE, &mi);
		HWND hWnd = GetParent();
		if(hWnd) {
			SendMessage(hWnd, FDM_FTM_SELECTED, (WPARAM)mi.dwItemData, 0);
		}
		return 0;		
	}

private:
	class FdmTabInfo {
	public:

		typedef vector<FdmTabInfo*> List;
		typedef typename List::iterator ListIter;

		enum { MAX_LENGTH = 20 };

		FdmTabInfo(HWND aWnd, COLORREF c) : hWnd(aWnd), len(0), xpos(0), row(0), dirty(false) { 
			pen.CreatePen(PS_SOLID, 1, c);
			memset(&size, 0, sizeof(size));
			memset(&boldSize, 0, sizeof(boldSize));
			name[0] = 0;
			updateFdm();
		};

		HWND hWnd;
		CPen pen;
		TCHAR name[MAX_LENGTH];
		size_t len;
		SIZE size;
		SIZE boldSize;
		int xpos;
		int row;
		bool dirty;

		bool updateFdm() {
			TCHAR name2[MAX_LENGTH];
			len = (size_t)::GetWindowTextLength(hWnd);
			if(len >= MAX_LENGTH) {
				::GetWindowText(hWnd, name2, MAX_LENGTH - 3);
				name2[MAX_LENGTH - 4] = _T('.');
				name2[MAX_LENGTH - 3] = _T('.');
				name2[MAX_LENGTH - 2] = _T('.');
				name2[MAX_LENGTH - 1] = 0;
				len = MAX_LENGTH - 1;
			} else {
				::GetWindowText(hWnd, name2, MAX_LENGTH);
			}
			if(_tcscmp(name, name2) == 0) {
				return false;
			}
			_tcscpy(name, name2);
			CDC dc(::GetDC(hWnd));
			HFONT f = dc.SelectFont(FdmWinUtil::fdmFont);
			dc.GetTextExtent(name, len, &size);
			dc.SelectFont(FdmWinUtil::fdmBoldFont);
			dc.GetTextExtent(name, len, &boldSize);
			dc.SelectFont(f);		
			::ReleaseDC(hWnd, dc);
			return true;
		};

		bool updateFdmFdmText(LPCTSTR text) {
			len = _tcslen(text);
			if(len >= MAX_LENGTH) {
				::_tcsncpy(name, text, MAX_LENGTH - 3);
				name[MAX_LENGTH - 4] = '.';
				name[MAX_LENGTH - 3] = '.';
				name[MAX_LENGTH - 2] = '.';
				name[MAX_LENGTH - 1] = 0;
				len = MAX_LENGTH - 1;
			} else {
				_tcscpy(name, text);
			}
			CDC dc(::GetDC(hWnd));
			HFONT f = dc.SelectFont(FdmWinUtil::fdmFont);
			dc.GetTextExtent(name, len, &size);
			dc.SelectFont(FdmWinUtil::fdmBoldFont);
			dc.GetTextExtent(name, len, &boldSize);
			dc.SelectFont(f);		
			::ReleaseDC(hWnd, dc);
			return true;
		};

		int getFdmWidth() {
			return (dirty ? boldSize.cx : size.cx) + FT_EXTRA_SPACE;
		}
	};

	void moveFdmTabs(FdmTabInfo* aTab, bool after){
		if(fdmMoving == NULL)
			return;

		FdmTabInfo::ListIter i, j;
		//remove the tab we're fdmMoving
		for(j = fdmTabs.begin(); j != fdmTabs.end(); ++j){
			if((*j) == fdmMoving){
				fdmTabs.erase(j);
				break;
			}
		}

		//find the tab we're going to insert before or after
		for(i = fdmTabs.begin(); i != fdmTabs.end(); ++i){
			if((*i) == aTab){
				if(after)
					++i;
				break;
			}
		}

		fdmTabs.insert(i, fdmMoving);
		fdmMoving = NULL;

		calcFdmRows(false);
		Invalidate();	
	}

	HWND fdmClosing;
	CButton chevron;
	CMenu mnu;
	
	int fdmRows;
	int fdmHeight;

	FdmTabInfo* fdmActive;
	FdmTabInfo* fdmMoving;
	typename FdmTabInfo::List fdmTabs;
	CPen black;

	typedef list<HWND> WindowList;
	typedef WindowList::iterator WindowIter;

	WindowList viewFdmOrder;
	WindowIter nextFdmTab;

	bool inFdmTab;

	FdmTabInfo* getFdmTabInfo(HWND aWnd) {
		for(FdmTabInfo::ListIter i	= fdmTabs.begin(); i != fdmTabs.end(); ++i) {
			if((*i)->hWnd == aWnd)
				return *i;
		}
		return NULL;
	}

	/**
	 * Draws a tab
	 * @return The width of the tab
	 */
	void drawFdmTab(CDC& dc, FdmTabInfo* tab, int pos, int row, bool aActive = false) {
		
		int ypos = (getFdmRows() - row - 1) * getFdmTabHeight();

		HPEN oldpen = dc.SelectPen(black);
		
		POINT p[4];
		dc.BeginPath();
		dc.MoveTo(pos, ypos);
		p[0].x = pos + tab->getFdmWidth() + getFdmFill();
		p[0].y = ypos;
		p[1].x = pos + tab->getFdmWidth();
		p[1].y = ypos + getFdmTabHeight();
		p[2].x = pos + getFdmFill();
		p[2].y = ypos + getFdmTabHeight();
		p[3].x = pos;
		p[3].y = ypos;
		
		dc.PolylineTo(p, 4);
		dc.CloseFigure();
		dc.EndPath();
		
		HBRUSH oldbrush = dc.SelectBrush(GetSysColorBrush(aActive ? COLOR_WINDOW : COLOR_BTNFACE));
		dc.FillPath();
		
		dc.MoveTo(p[1].x + 1, p[1].y);
		dc.LineTo(p[0].x + 1, p[0].y);
		dc.MoveTo(p[2]);
		dc.LineTo(p[3]);
		if(!fdmActive || (tab->row != (fdmRows - 1)) )
			dc.LineTo(p[0]);
		
		dc.SelectPen(tab->pen);
		dc.MoveTo(p[1]);
		dc.LineTo(p[0]);
		dc.MoveTo(p[1]);
		dc.LineTo(p[2]);
		
		dc.SelectPen(oldpen);
		dc.SelectBrush(oldbrush);
		
		dc.SetBkMode(TRANSPARENT);

		if(tab->dirty) {
			HFONT f = dc.SelectFont(FdmWinUtil::fdmBoldFont);
			dc.TextOut(pos + getFdmFill() / 2 + FT_EXTRA_SPACE / 2, ypos + 1, tab->name, tab->len);
			dc.SelectFont(f);		
		} else {
			dc.TextOut(pos + getFdmFill() / 2 + FT_EXTRA_SPACE / 2, ypos + 1, tab->name, tab->len);
		}
	};
};

class FdmFlatTabCtrl : public FdmFlatTabCtrlImpl<FdmFlatTabCtrl> {
public:
	DECLARE_FRAME_WND_CLASS_EX(GetWndClassName(), IDR_FDM_FLAT_TAB, 0, COLOR_3DFACE);
};

template <class T, int C = RGB(128, 128, 128), class TBase = CMDIWindow, class TWinTraits = CMDIChildWinTraits>
class ATL_NO_VTABLE MDIFdmTabChildWindowImpl : public CMDIChildWindowImpl<T, TBase, TWinTraits> {
public:

	MDIFdmTabChildWindowImpl() : createdFdm(false) { };
	FdmFlatTabCtrl* getFdmTab() { return FdmWinUtil::fdmTabCtrl; };

	virtual void OnFinalMessage(HWND /*hWnd*/) { delete this; }

 	typedef MDIFdmTabChildWindowImpl<T, C, TBase, TWinTraits> thisClass;
	typedef CMDIChildWindowImpl<T, TBase, TWinTraits> baseClass;
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_CLOSE, onClose)
		MESSAGE_HANDLER(WM_SYSCOMMAND, onSysCommand)
		MESSAGE_HANDLER(WM_FORWARDMSG, onForwardMsg)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		MESSAGE_HANDLER(WM_MDIACTIVATE, onMDIActivate)
		MESSAGE_HANDLER(WM_DESTROY, onDestroy)
		MESSAGE_HANDLER(WM_SETTEXT, onSetText)
		MESSAGE_HANDLER(FDM_WM_REALLY_CLOSE, onReallyClose)
		MESSAGE_HANDLER(WM_NOTIFYFORMAT, onNotifyFormat)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
	
	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
	DWORD dwStyle = 0, DWORD dwExStyle = 0,
	UINT nMenuID = 0, LPVOID lpCreateParam = NULL)
	{
		ATOM atom = T::GetWndClassInfo().Register(&m_pfnSuperWindowProc);

		if(nMenuID != 0)
#if (_ATL_VER >= 0x0700)
			m_hMenu = ::LoadMenu(ATL::_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nMenuID));
#else //!(_ATL_VER >= 0x0700)
			m_hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(nMenuID));
#endif //!(_ATL_VER >= 0x0700)

		if(m_hMenu == NULL)
			m_hMenu = FdmWinUtil::fdmMainMenu;

		dwStyle = T::GetWndStyle(dwStyle);
		dwExStyle = T::GetWndExStyle(dwExStyle);

		dwExStyle |= WS_EX_MDICHILD;   // force this one
		m_pfnSuperWindowProc = ::DefMDIChildProc;
		m_hWndMDIClient = hWndParent;
		ATLASSERT(::IsWindow(m_hWndMDIClient));

		if(rect.m_lpRect == NULL)
			rect.m_lpRect = &TBase::rcDefault;

		// If the currently fdmActive MDI child is maximized, we want to create this one maximized too
		ATL::CWindow wndParent = hWndParent;
		BOOL bMaximized = FALSE;

		if(MDIGetActive(&bMaximized) == NULL)
			bMaximized = BOOLSETTING(MDI_MAXIMIZED);

		if(bMaximized)
			wndParent.SetRedraw(FALSE);

		HWND hWnd = CFrameWindowImplBase<TBase, TWinTraits >::Create(hWndParent, rect.m_lpRect, szWindowName, dwStyle, dwExStyle, (UINT)0U, atom, lpCreateParam);

		if(bMaximized)
		{
			// Maximize and redraw everything
			if(hWnd != NULL)
				MDIMaximize(hWnd);
			wndParent.SetRedraw(TRUE);
			wndParent.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
			::SetFocus(GetMDIFrame());   // focus will be set back to this window
		}
		else if(hWnd != NULL && ::IsWindowVisible(m_hWnd) && !::IsChild(hWnd, ::GetFocus()))
		{
			::SetFocus(hWnd);
		}

		return hWnd;
	}

	LRESULT onNotifyFormat(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
#ifdef _UNICODE
		return NFR_UNICODE;
#else
		return NFR_ANSI;
#endif		
	}

	// All MDI windows must have this in wtl it seems to handle ctrl-tab and so on...
	LRESULT onForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
		return baseClass::PreTranslateMessage((LPMSG)lParam);
	}

	LRESULT onSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
		if(wParam == SC_NEXTWINDOW) {
			HWND next = getFdmTab()->getFdmNext();
			if(next != NULL) {
				MDIActivate(next);
				return 0;
			}
		} else if(wParam == SC_PREVWINDOW) {
			HWND next = getFdmTab()->getFdmPrev();
			if(next != NULL) {
				MDIActivate(next);
				return 0;
			}
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT onCreate(UINT /* uMsg */, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		bHandled = FALSE;
		dcassert(getFdmTab());
		getFdmTab()->addFdmTab(m_hWnd, C);
		createdFdm = true;
		return 0;
	}
	
	LRESULT onMDIActivate(UINT /*uMsg*/, WPARAM /*wParam */, LPARAM lParam, BOOL& bHandled) {
		dcassert(getFdmTab());
		if((m_hWnd == (HWND)lParam))
			getFdmTab()->setFdmActive(m_hWnd);

		bHandled = FALSE;
		return 1; 
	}

	LRESULT onDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		bHandled = FALSE;
		dcassert(getFdmTab());
		getFdmTab()->removeFdmTab(m_hWnd);
		if(m_hMenu == FdmWinUtil::fdmMainMenu)
			m_hMenu = NULL;

		BOOL bMaximized = FALSE;
		if(::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized) != NULL)
			SettingsManager::getInstance()->set(SettingsManager::MDI_MAXIMIZED, (bMaximized>0));

		return 0;
	}

	LRESULT onReallyClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		MDIDestroy(m_hWnd);
		return 0;
	}

	LRESULT onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled */) {
		PostMessage(FDM_WM_REALLY_CLOSE);
		return 0;
	}

	LRESULT onSetText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) {
		bHandled = FALSE;
		dcassert(getFdmTab());
		if(createdFdm) {
			getFdmTab()->updateFdmFdmText(m_hWnd, (LPCTSTR)lParam);
		}
		return 0;
	}

	LRESULT onKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		if(wParam == VK_CONTROL && LOWORD(lParam) == 1) {
			getFdmTab()->startFdmSwitch();
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT onKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
		if(wParam == VK_CONTROL) {
			getFdmTab()->endFdmSwitch();
		}
		bHandled = FALSE;
		return 0;
		
	}

	void setFdmDirty() {
		dcassert(getFdmTab());
		getFdmTab()->setFdmDirty(m_hWnd);
	}
	void setFdmTabColor(COLORREF color) {
		dcassert(getFdmTab());
		getFdmTab()->setFdmColor(m_hWnd, color);
	}

private:
	bool createdFdm;
};

#endif // !defined(FDM_FLAT_TAB_CTRL_H)

/**
 * @file
 * $Id: FlatTabCtrl.h,v 1.40 2005/11/28 01:21:06 arnetheduck Exp $
 */
