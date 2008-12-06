/*
    dmcut2, a library for Monte-Carlo simulations at surfaces
    Copyright (C) 2008 Mathias Laurin <mathias_laurin@users.sourceforge.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "EnergyContainer.hpp"

#include "Site.hpp"
double
Interactions::TwoBody::EnergyContainer::operator()(const Surface::Site* const one_site_ptr, 
												const Surface::Site* const other_site_ptr) const
{	using std::tr1::make_tuple;
	
	EnergyContainer::container_type::const_iterator
	cit = container_.find(make_tuple(one_site_ptr->species(), one_site_ptr->name(), 
									 other_site_ptr->species(), other_site_ptr->name()));
	return (cit != container_.end()) ? cit->second : 0.0;
}
