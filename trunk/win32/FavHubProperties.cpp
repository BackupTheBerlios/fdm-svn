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

#include "FavHubProperties.h"

#include <dcpp/FavoriteManager.h>
#include <dcpp/version.h>

FavHubProperties::FavHubProperties(SmartWin::Widget* parent, FavoriteHubEntry *_entry) :
	SmartWin::WidgetFactory<SmartWin::WidgetModalDialog>(parent),
	name(0),
	address(0),
	description(0),
	nick(0),
	password(0),
	userDescription(0),
	entry(_entry)
{
	onInitDialog(std::tr1::bind(&FavHubProperties::handleInitDialog, this));
	onFocus(std::tr1::bind(&FavHubProperties::handleFocus, this));
}

FavHubProperties::~FavHubProperties() {
}

bool FavHubProperties::handleInitDialog() {
	// Translate dialog
	setText(T_("Favorite Hub Properties"));
	::SetDlgItemText(handle(), IDC_FH_HUB, CT_("Hub"));
	::SetDlgItemText(handle(), IDC_FH_IDENT, CT_("Identification (leave blank for defaults)"));
	::SetDlgItemText(handle(), IDC_FH_NAME, CT_("Name"));
	::SetDlgItemText(handle(), IDC_FH_ADDRESS, CT_("Address"));
	::SetDlgItemText(handle(), IDC_FH_HUB_DESC, CT_("Description"));
	::SetDlgItemText(handle(), IDC_FH_NICK, CT_("Nick"));
	::SetDlgItemText(handle(), IDC_FH_PASSWORD, CT_("Password"));
	::SetDlgItemText(handle(), IDC_FH_USER_DESC, CT_("Description"));

	name = attachTextBox(IDC_HUBNAME);
	name->setText(Text::toT(entry->getName()));
	name->setFocus();
	name->setSelection();
	
	address = attachTextBox(IDC_HUBADDR);
	address->setText(Text::toT(entry->getServer()));

	description = attachTextBox(IDC_HUBDESCR);
	description->setText(Text::toT(entry->getDescription()));

	nick = attachTextBox(IDC_HUBNICK);
	nick->setTextLimit(35);
	nick->setText(Text::toT(entry->getNick(false)));
	nick->onTextChanged(std::tr1::bind(&FavHubProperties::handleTextChanged, this, nick));

	password = attachTextBox(IDC_HUBPASS);
	password->setPassword();
	password->setText(Text::toT(entry->getPassword()));
	password->onTextChanged(std::tr1::bind(&FavHubProperties::handleTextChanged, this, password));

	userDescription = attachTextBox(IDC_HUBUSERDESCR);
	userDescription->setTextLimit(35);
	userDescription->setText(Text::toT(entry->getUserDescription()));

	WidgetButtonPtr button = attachButton(IDOK);
	button->onClicked(std::tr1::bind(&FavHubProperties::handleOKClicked, this));

	button = attachButton(IDCANCEL);
	button->onClicked(std::tr1::bind(&FavHubProperties::endDialog, this, IDCANCEL));

	centerWindow();
	
	return false;
}

void FavHubProperties::handleFocus() {
	name->setFocus();
}

void FavHubProperties::handleTextChanged(WidgetTextBoxPtr textBox) {
	tstring text = textBox->getText();
	bool update = false;

	// Strip ' '
	tstring::size_type i;
	while((i = text.find(' ')) != string::npos) {
		text.erase(i, 1);
		update = true;
	}

	if(update) {
		// Something changed; update window text without changing cursor pos
		long caretPos = textBox->getCaretPos() - 1;
		textBox->setText(text);
		textBox->setSelection(caretPos, caretPos);
	}
}

void FavHubProperties::handleOKClicked() {
	tstring addressText = address->getText();
	if(addressText.empty()) {
		createMessageBox().show(T_("Hub address cannot be empty"), _T(APPNAME) _T(" ") _T(VERSIONSTRING), WidgetMessageBox::BOX_OK, WidgetMessageBox::BOX_ICONEXCLAMATION);
		return;
	}
	entry->setServer(Text::fromT(addressText));
	entry->setName(Text::fromT(name->getText()));
	entry->setDescription(Text::fromT(description->getText()));
	entry->setNick(Text::fromT(nick->getText()));
	entry->setPassword(Text::fromT(password->getText()));
	entry->setUserDescription(Text::fromT(userDescription->getText()));
	FavoriteManager::getInstance()->save();
	endDialog(IDOK);
}