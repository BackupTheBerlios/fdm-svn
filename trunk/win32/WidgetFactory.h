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

#ifndef DCPLUSPLUS_WIN32_WIDGETFACTORY2_H_
#define DCPLUSPLUS_WIN32_WIDGETFACTORY2_H_

#include "TextBox.h"
#include "ComboBox.h"
#include "WidgetPaned.h"

/**
 * This is where stuff that eventually should be moved to smartwin goes
 */
template<typename ContainerWidgetType>
class WidgetFactory : public dwt::WidgetFactory<ContainerWidgetType> {
public:
	explicit WidgetFactory(dwt::Widget* parent) : dwt::WidgetFactory<ContainerWidgetType>(parent) { }

	/// TextBox class type.
	typedef ::TextBox TextBox;

	/// TextBox object type.
	typedef typename TextBox::ObjectType TextBoxPtr;

	/// ComboBox class type.
	typedef ::ComboBox ComboBox;

	/// ComboBox object type.
	typedef typename ComboBox::ObjectType ComboBoxPtr;

	/// VPaned class type.
	typedef WidgetPaned< false > WidgetVPaned;

	/// VPaned object type.
	typedef typename WidgetVPaned::ObjectType WidgetVPanedPtr;

	WidgetVPanedPtr createVPaned( const typename WidgetVPaned::Seed & cs = WidgetVPaned::Seed() ) {
		return dwt::WidgetCreator< WidgetVPaned >::create( this, cs );
	}

	/// HPaned class type.
	typedef WidgetPaned< true > WidgetHPaned;

	/// HPaned object type.
	typedef typename WidgetHPaned::ObjectType WidgetHPanedPtr;

	WidgetHPanedPtr createHPaned( const typename WidgetHPaned::Seed & cs = WidgetHPaned::Seed() ) {
		return dwt::WidgetCreator< WidgetHPaned >::create( this, cs );
	}

};

#endif /*WIDGETFACTORY2_H_*/
