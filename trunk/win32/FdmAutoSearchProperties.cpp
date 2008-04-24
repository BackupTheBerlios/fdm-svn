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

#include "FdmAutoSearchProperties.h"

#include <dcpp/FdmAutoSearch.h>
#include <dcpp/ResourceManager.h>

AutoSearchProperties::AutoSearchProperties(dwt::Widget* parent, AutoSearch *_search) :
	WidgetFactory<dwt::ModalDialog>(parent),
	searchString(0),
	sizeString(0),
	matchString(0),
	excludeString(0),
	extensionString(0),
	minSize(0),
	maxSize(0),
	sourceType(0),
	sizeModes(0),
	sizeType(0),
	resSizeType(0),
	active(0),
	onlyIfOp(0),
	search(_search)
{
	onInitDialog(std::tr1::bind(&AutoSearchProperties::handleInitDialog, this));
	onFocus(std::tr1::bind(&AutoSearchProperties::handleFocus, this));
}

AutoSearchProperties::~AutoSearchProperties() {
}

bool AutoSearchProperties::handleInitDialog() {
	// Translate dialog
	setText(TSTRING(AUTO_SEARCH_PROPERTIES));
	::SetDlgItemText(handle(), IDC_AS_SEARCH, CT_("Search String"));
	::SetDlgItemText(handle(), IDC_AS_SOURCE_TYPE, CT_("File type"));
	::SetDlgItemText(handle(), IDC_AS_FILE_SIZE, CT_("Size"));
	::SetDlgItemText(handle(), IDC_AS_RESULTS, CTSTRING(RESULTS));
	::SetDlgItemText(handle(), IDC_AS_MATCH, CTSTRING(AUTO_MATCH_THESE));
	::SetDlgItemText(handle(), IDC_AS_EXCLUDE, CTSTRING(AUTO_EXCLUDE_THESE));
	::SetDlgItemText(handle(), IDC_AS_EXTENSION, CTSTRING(AUTO_EXTENSION));
	::SetDlgItemText(handle(), IDC_AS_MIN_SIZE, CT_("Min FileSize"));
	::SetDlgItemText(handle(), IDC_AS_MAX_SIZE, CT_("Max FileSize"));

	searchString = attachChild<TextBox>(IDC_AUTOS_SEARCH_STRING);
	searchString->setText(Text::toT(search->searchString));
	searchString->setFocus();

	sourceType = attachChild<ComboBox>(IDC_AUTOS_SOURCE_TYPE);
	sourceType->addValue(T_("Any"));
	sourceType->addValue(T_("Audio"));
	sourceType->addValue(T_("Compressed"));
	sourceType->addValue(T_("Document"));
	sourceType->addValue(T_("Executable"));
	sourceType->addValue(T_("Picture"));
	sourceType->addValue(T_("Video"));
	sourceType->addValue(T_("Directory"));
	sourceType->addValue(T_("TTH"));
	sourceType->setSelected(search->sourceType);

	sizeModes = attachChild<ComboBox>(IDC_AUTOS_SIZE_MODES);
	sizeModes->addValue(T_("Normal"));
	sizeModes->addValue(T_("At least"));
	sizeModes->addValue(T_("At most"));
	sizeModes->setSelected(search->sizeModes);

	sizeString = attachChild<TextBox>(IDC_AUTOS_FILE_SIZE);
	sizeString->setText((search->size > 0) ? Text::toT(Util::toString(search->size)) : Util::emptyStringT);
	
	sizeType = attachChild<ComboBox>(IDC_AUTOS_SIZE_TYPE);
	sizeType->addValue(T_("B"));
	sizeType->addValue(T_("KiB"));
	sizeType->addValue(T_("MiB"));
	sizeType->addValue(T_("GiB"));
	sizeType->setSelected(search->typeFileSize);

	onlyIfOp = attachChild<CheckBox>(IDC_AS_ONLY_IF_OP);
	onlyIfOp->setText(TSTRING(ONLY_WHERE_OP));
	onlyIfOp->setChecked(search->onlyIfOp);

	active = attachChild<CheckBox>(IDC_AS_IS_ACTIVE);
	active->setText(T_("Enabled"));
	active->setChecked(search->isActive);

	matchString = attachChild<TextBox>(IDC_AUTOS_SEARCH_MATCH);
	matchString->setText(Text::toT(search->resultsMatch));

	excludeString = attachChild<TextBox>(IDC_AUTOS_SEARCH_EXCLUDE);
	excludeString->setText(Text::toT(search->resultsExclude));

	extensionString = attachChild<TextBox>(IDC_AUTOS_SEARCH_EXTENSION);
	extensionString->setText(Text::toT(search->resultsExtensions));

	minSize = attachChild<TextBox>(IDC_AUTOS_MIN_SIZE);
	minSize->setText((search->resultsMinSize > 0) ? Text::toT(Util::toString(search->resultsMinSize)) : Util::emptyStringT);

	maxSize = attachChild<TextBox>(IDC_AUTOS_MAX_SIZE);
	maxSize->setText((search->resultsMaxSize > 0) ? Text::toT(Util::toString(search->resultsMaxSize)) : Util::emptyStringT);

	resSizeType = attachChild<ComboBox>(IDC_AUTOS_RES_SIZE_TYPE);
	resSizeType->addValue(T_("B"));
	resSizeType->addValue(T_("KiB"));
	resSizeType->addValue(T_("MiB"));
	resSizeType->addValue(T_("GiB"));
	resSizeType->setSelected(search->resultsTypeFileSize);

	ButtonPtr button = attachChild<Button>(IDOK);
	button->onClicked(std::tr1::bind(&AutoSearchProperties::handleOKClicked, this));

	button = attachChild<Button>(IDCANCEL);
	button->onClicked(std::tr1::bind(&AutoSearchProperties::endDialog, this, IDCANCEL));

	centerWindow();
	
	return false;
}

void AutoSearchProperties::handleFocus() {
	searchString->setFocus();
}

void AutoSearchProperties::handleOKClicked() {
	search->searchString = Text::fromT(searchString->getText());
	search->sourceType = AutoSearch::SourceType(sourceType->getSelected());
	search->sizeModes = AutoSearch::SizeModes(sizeModes->getSelected());

	tstring searchSize = sizeString->getText();
	search->size = searchSize.empty() ? -1 : Util::toInt64(Text::fromT(searchSize));

	search->typeFileSize = AutoSearch::TypeFileSize(sizeType->getSelected());
	search->onlyIfOp = onlyIfOp->getChecked();
	search->isActive = active->getChecked();

	search->resultsMatch = Text::fromT(matchString->getText());
	search->resultsExclude = Text::fromT(excludeString->getText());
	search->resultsExtensions = Text::fromT(extensionString->getText());
	
	tstring minFileSize = minSize->getText();
	search->resultsMinSize = minFileSize.empty() ? -1 : Util::toInt64(Text::fromT(minFileSize));

	tstring maxFileSize = maxSize->getText();
	search->resultsMaxSize = maxFileSize.empty() ? -1 : Util::toInt64(Text::fromT(maxFileSize));
	
	search->resultsTypeFileSize = AutoSearch::TypeFileSize(resSizeType->getSelected());

	endDialog(IDOK);
}
