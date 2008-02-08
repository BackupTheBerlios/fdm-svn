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

#include "FdmNotepadFrame.h"

#include <dcpp/File.h>
#include <dcpp/Text.h>
#include <dcpp/ResourceManager.h>

FdmNotepadFrame::FdmNotepadFrame(SmartWin::WidgetTabView* mdiParent) : 
	BaseType(mdiParent, TSTRING(FDM_NOTEPAD), IDR_NOTEPAD),
	pad(0) 
{
	{
		WidgetTextBox::Seed cs = WinUtil::Seeds::textBox;
		cs.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_NOHIDESEL | ES_WANTRETURN;
		pad = createTextBox(cs);
		addWidget(pad);
	}
	
	initStatus();
	
	pad->setTextLimit(0);
	try {
		pad->setText(Text::toT(File(getFdmNotepadFile(), File::READ, File::OPEN).read()));
	} catch(const FileException& e) {
		// Ignore		
	}
	
	pad->setModify(false);

	layout();
}

FdmNotepadFrame::~FdmNotepadFrame() {

}

bool FdmNotepadFrame::preClosing() {
	if(pad->getModify()) {
		try {
			dcdebug("Writing fdm notepad contents\n");
			File(getFdmNotepadFile(), File::WRITE, File::CREATE | File::TRUNCATE).write(Text::fromT(pad->getText()));
		} catch(const FileException& e) {
			dcdebug("Writing failed: %s\n", e.getError().c_str());
			///@todo Notify user			
		}
	}
	return true;
}

void FdmNotepadFrame::layout() {	
	SmartWin::Rectangle r(SmartWin::Point(0, 0), getClientAreaSize());

	layoutStatus(r);

	pad->setBounds(r);
}

string FdmNotepadFrame::getFdmNotepadFile() { return Util::getConfigPath() + "Fdm-Notepad.txt"; }
