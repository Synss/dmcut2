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
// Author: Mathias Laurin <laurin@chem.s.u-tokyo.ac.jp>

#include "Geometry.hpp"

#include "Site.hpp"
#include "Lattice.hpp"
//#include <algorithm>
double Interactions::TwoBody::Geometry::InteractionEnergy() const
{	//using std::accumulate;
	double energy = 0.0;
	for (NeighborContainer::const_iterator
		 neighbor_container_it = neighbor_container_.begin(), 
		 neighbor_container_end = neighbor_container_.end();
		 neighbor_container_it != neighbor_container_end;
		 ++neighbor_container_it) {
		Surface::Site generic_neighbor(Surface::any, 
									   (*neighbor_container_it)->index(),
									   (*neighbor_container_it)->species());
		energy += energy_container_(site_ptr_, &generic_neighbor);
	}
	return energy;
}

#include <cassert>
#include <algorithm> // transform, sort
#include <functional> // negate
#include <tr1/functional> // bind
#include "Lattice.hpp" // HelicalBoundaryConditions
void Interactions::TwoBody::LinkSites::operator()(Geometry two_body_interactions, 
												  Surface::Site* site_ptr, 
												  RelativeIndexContainer relative_index_container) const
{	using std::tr1::bind; using std::tr1::placeholders::_1; using std::tr1::ref;
	
	// If Site::name is 'hcp' or 'HCP', apply inversion symmetry
	if (site_ptr->name() == "hcp" || site_ptr->name() == "HCP")
		std::transform(relative_index_container.begin(), relative_index_container.end(),
					   relative_index_container.begin(), 
					   std::negate<RelativeIndexContainer::value_type>());
	
	// Set TwoBody::Interactions::site_ptr_
	two_body_interactions.set_site_ptr(site_ptr);
	// Fill TwoBody::Interactions::neighbor_container with pointers to Site at RelativeIndexContainer::value_type
	Geometry::NeighborContainer& neighbor_container = two_body_interactions.neighbor_container();
	std::transform(relative_index_container.begin(), relative_index_container.end(),
				   std::back_inserter(neighbor_container),
				   bind(boundary_conditions_, site_ptr, _1));
	
	// sort the TwoBody::Interactions::neighbor_container
	std::sort(neighbor_container.begin(), neighbor_container.end());
	
	// save into Site::TwoBodyInteractionsContainer
	site_ptr->set_two_body_interactions(two_body_interactions);
}


#include <ostream>
std::ostream&
operator<<(std::ostream& os, const Interactions::TwoBody::Geometry& two_body_interactions)
{ return os << two_body_interactions.name(); }

