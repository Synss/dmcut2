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
#ifdef DISABLE_INLINE
#define inline
#endif

#include <numeric> // accumulate
#include <tr1/functional> // bind, plus
#include "Geometry.hpp"
inline double
Surface::Site::InteractionEnergy() const
{	using std::accumulate; typedef std::plus<double> plus;
	using std::tr1::bind; using namespace std::tr1::placeholders;
	
	return species().no_species() ?
	0.0 : accumulate(two_body_interactions_container.begin(), two_body_interactions_container.end(),
					 0.0, // initial value
					 bind(plus(), _1, bind(&Interactions::TwoBody::Geometry::InteractionEnergy, _2)));
}


inline double 
Surface::Site::InteractionEnergyBridge(const Site* const other_site_ptr) const
{
	using std::accumulate; typedef std::plus<double> plus;
	using std::tr1::bind; using namespace std::tr1::placeholders;
	
	return species().no_species() ?
	0.0 : accumulate(bridge_interactions_container.begin(),
				     bridge_interactions_container.end(),
				     0.0,
				     bind(plus(), _1, bind(&Bridge::InteractionEnergy, _2, this, other_site_ptr)));
}

#include <ostream>
inline std::ostream&
operator<< (std::ostream& os, const Surface::Site& s)
{ return os << s.index() << "\t" << s.name() << "\t" << s.species(); }

/*
#include <istream>
std::istream&
operator>>(std::istream& is, Surface::Site& s)
{
	// expect same format as operator<<
	s.index_ << is.titi();
	s.name_ << is.tata();
	s.species_ << is.toto();
	return is;
}
*/

#ifdef DISABLE_INLINE
#undef inline
#endif
