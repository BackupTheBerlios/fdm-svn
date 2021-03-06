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

#include "CertificatesPage.h"

#include <dcpp/SettingsManager.h>
#include <dcpp/CryptoManager.h>
#include "WinUtil.h"

static const WinUtil::HelpItem helpItems[] = {
	{ IDC_SETTINGS_PRIVATE_KEY_FILE, IDH_SETTINGS_CERTIFICATES_PRIVATE_KEY_FILE },
	{ IDC_TLS_PRIVATE_KEY_FILE, IDH_SETTINGS_CERTIFICATES_PRIVATE_KEY_FILE },
	{ IDC_BROWSE_PRIVATE_KEY, IDH_SETTINGS_CERTIFICATES_PRIVATE_KEY_FILE },
	{ IDC_SETTINGS_CERTIFICATE_FILE, IDH_SETTINGS_CERTIFICATES_CERTIFICATE_FILE },
	{ IDC_TLS_CERTIFICATE_FILE, IDH_SETTINGS_CERTIFICATES_CERTIFICATE_FILE },
	{ IDC_BROWSE_CERTIFICATE, IDH_SETTINGS_CERTIFICATES_CERTIFICATE_FILE },
	{ IDC_SETTINGS_TRUSTED_CERTIFICATES_PATH, IDH_SETTINGS_CERTIFICATES_TRUSTED_CERTIFICATES_PATH },
	{ IDC_TLS_TRUSTED_CERTIFICATES_PATH, IDH_SETTINGS_CERTIFICATES_TRUSTED_CERTIFICATES_PATH },
	{ IDC_BROWSE_TRUSTED_PATH, IDH_SETTINGS_CERTIFICATES_TRUSTED_CERTIFICATES_PATH },
	{ 0, 0 }
};

PropPage::TextItem CertificatesPage::texts[] = {
	{ IDC_SETTINGS_PRIVATE_KEY_FILE, N_("Private key file") },
	{ IDC_SETTINGS_CERTIFICATE_FILE, N_("Own certificate file") },
	{ IDC_SETTINGS_TRUSTED_CERTIFICATES_PATH, N_("Trusted certificates path") },
	{ IDC_GENERATE_CERTS, N_("Generate certificates") },
	{ 0, 0 }
};

PropPage::Item CertificatesPage::items[] = {
	{ IDC_TLS_CERTIFICATE_FILE, SettingsManager::TLS_CERTIFICATE_FILE, PropPage::T_STR },
	{ IDC_TLS_PRIVATE_KEY_FILE, SettingsManager::TLS_PRIVATE_KEY_FILE, PropPage::T_STR },
	{ IDC_TLS_TRUSTED_CERTIFICATES_PATH, SettingsManager::TLS_TRUSTED_CERTIFICATES_PATH, PropPage::T_STR },
	{ 0, 0, PropPage::T_END }
};

PropPage::ListItem CertificatesPage::listItems[] = {
	{ SettingsManager::USE_TLS, N_("Use TLS when remote client supports it"), IDH_SETTINGS_CERTIFICATES_USE_TLS },
	{ SettingsManager::ALLOW_UNTRUSTED_HUBS, N_("Allow TLS connections to hubs without trusted certificate"), IDH_SETTINGS_CERTIFICATES_ALLOW_UNTRUSTED_HUBS },
	{ SettingsManager::ALLOW_UNTRUSTED_CLIENTS, N_("Allow TLS connections to clients without trusted certificate"), IDH_SETTINGS_CERTIFICATES_ALLOW_UNTRUSTED_CLIENTS },
	{ 0, 0 }
};

CertificatesPage::CertificatesPage(dwt::Widget* parent) : PropPage(parent) {
	createDialog(IDD_CERTIFICATESPAGE);
	setHelpId(IDH_CERTIFICATESPAGE);

	WinUtil::setHelpIds(this, helpItems);
	PropPage::translate(handle(), texts);
	PropPage::read(handle(), items);

	privateKeyFile = attachChild<TextBox>(IDC_TLS_PRIVATE_KEY_FILE);
	attachChild<Button>(IDC_BROWSE_PRIVATE_KEY)->onClicked(std::tr1::bind(&CertificatesPage::handleBrowsePrivateKeyClicked, this));

	certificateFile = attachChild<TextBox>(IDC_TLS_CERTIFICATE_FILE);
	attachChild<Button>(IDC_BROWSE_CERTIFICATE)->onClicked(std::tr1::bind(&CertificatesPage::handleBrowseCertificateClicked, this));

	trustedCertificatesPath = attachChild<TextBox>(IDC_TLS_TRUSTED_CERTIFICATES_PATH);
	attachChild<Button>(IDC_BROWSE_TRUSTED_PATH)->onClicked(std::tr1::bind(&CertificatesPage::handleBrowseTrustedPathClicked, this));

	attachChild<Button>(IDC_GENERATE_CERTS)->onClicked(std::tr1::bind(&CertificatesPage::handleGenerateCertsClicked, this));

	attachChild(options, IDC_TLS_OPTIONS);
	PropPage::read(listItems, options);
}

CertificatesPage::~CertificatesPage() {
}

void CertificatesPage::write() {
	PropPage::write(handle(), items);
	PropPage::write(listItems, options);
}

void CertificatesPage::handleBrowsePrivateKeyClicked() {
	tstring target = privateKeyFile->getText();
	if(createLoadDialog().setInitialDirectory(target).open(target))
		privateKeyFile->setText(target);
}

void CertificatesPage::handleBrowseCertificateClicked() {
	tstring target = certificateFile->getText();
	if(createLoadDialog().setInitialDirectory(target).open(target))
		certificateFile->setText(target);
}

void CertificatesPage::handleBrowseTrustedPathClicked() {
	tstring target = trustedCertificatesPath->getText();
	if(createFolderDialog().open(target))
		trustedCertificatesPath->setText(target);
}

void CertificatesPage::handleGenerateCertsClicked() {
	try {
		CryptoManager::getInstance()->generateCertificate();
	} catch(const CryptoException& e) {
		createMessageBox().show(Text::toT(e.getError()), _T("Error generating certificate"), MessageBox::BOX_OK, MessageBox::BOX_ICONSTOP);
	}
}
