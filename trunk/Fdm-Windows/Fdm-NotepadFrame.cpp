#include "../windows/stdafx.h"
#include "../client/DCPlusPlus.h"
#include "Resource.h"
#include "../windows/Resource.h"

#include "Fdm-NotepadFrame.h"
#include "../windows/WinUtil.h"
#include "../client/File.h"

LRESULT FdmNotepadFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	ctrlPad.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);
	
	ctrlPad.LimitText(0);
	ctrlPad.SetFont(WinUtil::font);
	string tmp;
	try {
		tmp = File(Util::getNotepadFile(), File::READ, File::OPEN).read();
	} catch(const FileException&) {
		// ...
	}
	
	if(tmp.empty()) {
		tmp = SETTING(NOTEPAD_TEXT);
		if(!tmp.empty()) {
			dirty = true;
			SettingsManager::getInstance()->set(SettingsManager::NOTEPAD_TEXT, Util::emptyString);
		}
	}

	ctrlPad.SetWindowText(Text::toT(tmp).c_str());
	ctrlPad.EmptyUndoBuffer();
	ctrlClientContainer.SubclassWindow(ctrlPad.m_hWnd);

	bHandled = FALSE;
	return 1;
}

LRESULT FdmNotepadFrame::onClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	
	if(dirty || ctrlPad.GetModify()) {
		AutoArray<TCHAR> buf(ctrlPad.GetWindowTextLength() + 1);
		ctrlPad.GetWindowText(buf, ctrlPad.GetWindowTextLength() + 1);
		try {
			string tmp(Text::fromT(tstring(buf, ctrlPad.GetWindowTextLength())));
			File(Util::getNotepadFile(), File::WRITE, File::CREATE | File::TRUNCATE).write(tmp);
		} catch(const FileException&) {
			// Oops...
		}
	}

	bHandled = FALSE;
	return 0;
	
}

void FdmNotepadFrame::UpdateLayout(BOOL /*bResizeBars*/ /* = TRUE */)
{
	CRect rc;

	GetClientRect(rc);
	
	rc.bottom -= 1;
	rc.top += 1;
	rc.left +=1;
	rc.right -=1;
	ctrlPad.MoveWindow(rc);
	
}

LRESULT FdmNotepadFrame::onLButton(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled) {
	HWND focus = GetFocus();
	bHandled = false;
	if(focus == ctrlPad.m_hWnd) {
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		tstring x;
		tstring::size_type start = (tstring::size_type)WinUtil::textUnderCursor(pt, ctrlPad, x);
		tstring::size_type end = x.find(_T(" "), start);

		if(end == string::npos)
			end = x.length();
		
		bHandled = WinUtil::parseDBLClick(x, start, end);
	}
	return 0;
}