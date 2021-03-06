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

#include "MagnetDlg.h"

#include "WinUtil.h"

MagnetDlg::MagnetDlg(dwt::Widget* parent, const tstring& aHash, const tstring& aFileName) :
	WidgetFactory<dwt::ModalDialog>(parent),
	//queue(0),
	search(0),
	doNothing(0),
	//remember(0),
	mHash(aHash),
	mFileName(aFileName)
{
	onInitDialog(std::tr1::bind(&MagnetDlg::handleInitDialog, this));
	onFocus(std::tr1::bind(&MagnetDlg::handleFocus, this));
}

MagnetDlg::~MagnetDlg() {
}

bool MagnetDlg::handleInitDialog() {
	setText(T_("MAGNET Link detected"));
	setItemText(IDC_MAGNET_TEXT, T_("DC++ has detected a MAGNET link with a file hash that can be searched for on the Direct Connect network.  What would you like to do?"));
	setItemText(IDC_MAGNET_HASH, T_("File Hash:"));
	attachChild<TextBox>(IDC_MAGNET_DISP_HASH)->setText(mHash.c_str());
	setItemText(IDC_MAGNET_NAME, T_("Filename:"));
	attachChild<TextBox>(IDC_MAGNET_DISP_NAME)->setText(mFileName);

	//queue = attachRadioButton(IDC_MAGNET_1_QUEUE);
	//queue->setText(T_("Add this file to your download queue"));
	//queue->onClicked(std::tr1::bind(&MagnetDlg::handleRadioButtonClicked, this, queue));
	::ShowWindow(getItem(IDC_MAGNET_1_QUEUE), false);

	attachChild(search, IDC_MAGNET_2_SEARCH);
	search->setText(T_("Start a search for this file"));
	search->setFocus();
	//search->onClicked(std::tr1::bind(&MagnetDlg::handleRadioButtonClicked, this, search));

	attachChild(doNothing, IDC_MAGNET_3_NOTHING);
	doNothing->setText(T_("Do nothing"));
	//doNothing->onClicked(std::tr1::bind(&MagnetDlg::handleRadioButtonClicked, this, doNothing));

	//remember = attachChild<CheckBox>(IDC_MAGNET_REMEMBER);
	//remember->setText(T_("Do the same action next time without asking"));
	::ShowWindow(getItem(IDC_MAGNET_REMEMBER), false);

	::CheckRadioButton(handle(), IDC_MAGNET_1_QUEUE, IDC_MAGNET_3_NOTHING, IDC_MAGNET_2_SEARCH);

	ButtonPtr button = attachChild<Button>(IDOK);
	button->onClicked(std::tr1::bind(&MagnetDlg::handleOKClicked, this));

	button = attachChild<Button>(IDCANCEL);
	button->onClicked(std::tr1::bind(&MagnetDlg::endDialog, this, IDCANCEL));

	centerWindow();

	return false;
}

void MagnetDlg::handleFocus() {
	search->setFocus();
}

//void MagnetDlg::handleRadioButtonClicked(RadioButtonPtr radioButton) {
	//if(radioButton == queue || radioButton == search)
		//remember->setEnabled(true);
	//else if(radioButton == doNothing) {
		//if(remember->getChecked())
			//remember->setChecked(false);
		//remember->setEnabled(false);
	//}
//}

void MagnetDlg::handleOKClicked() {
	//if(remember->getChecked()) {
	//	SettingsManager::getInstance()->set(SettingsManager::MAGNET_ASK,  false);
	//	if(queue->getChecked())
	//		SettingsManager::getInstance()->set(SettingsManager::MAGNET_ACTION, SettingsManager::MAGNET_AUTO_DOWNLOAD);
	//	else if(search->getChecked())
	//		SettingsManager::getInstance()->set(SettingsManager::MAGNET_ACTION, SettingsManager::MAGNET_AUTO_SEARCH);
	//}

	if(search->getChecked()) {
		TTHValue tmphash(Text::fromT(mHash));
		WinUtil::searchHash(tmphash);
	} //else if(queue->getChecked()) {
		// FIXME: Write this code when the queue is more tth-centric
	//}

	endDialog(IDOK);
}
