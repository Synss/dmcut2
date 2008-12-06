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
#include "Bridge.hpp"

#include <algorithm> // set_intersection
#include <iterator> // back_inserter
#include <cassert>
#include "Site.hpp"
void Surface::Bridge::make_neighbors(const Surface::Site* const site_ptr1, 
									 const Surface::Site* const site_ptr2, 
									 Interactions::TwoBody::Geometry::NeighborContainer& result) const
{	using std::set_intersection; using std::back_inserter; //using util::less_ptr;
	
	const Interactions::TwoBody::Geometry::NeighborContainer& 
	site1_two_body1 = site_ptr1->two_body_interactions(two_body1_).neighbor_container();
	const Interactions::TwoBody::Geometry::NeighborContainer& 
	site2_two_body2 = site_ptr2->two_body_interactions(two_body2_).neighbor_container();
	set_intersection(site1_two_body1.begin(), site1_two_body1.end(),
					 site2_two_body2.begin(), site2_two_body2.end(),
					 back_inserter(result));
	
	const Interactions::TwoBody::Geometry::NeighborContainer& 
	site1_two_body2 = site_ptr1->two_body_interactions(two_body2_).neighbor_container();
	const Interactions::TwoBody::Geometry::NeighborContainer& 
	site2_two_body1 = site_ptr2->two_body_interactions(two_body1_).neighbor_container();
	set_intersection(site1_two_body2.begin(), site1_two_body2.end(),
					 site2_two_body1.begin(), site2_two_body1.end(),
					 back_inserter(result));
	
	// post conditions
	assert(!site1_two_body1.empty() && !site2_two_body2.empty());	
	assert(!site1_two_body2.empty() && !site2_two_body1.empty());
	assert(!result.empty());
}


#include <numeric>
#include <tr1/functional>
#include "Lattice.hpp"
double Surface::Bridge::InteractionEnergy(const Surface::Site* const origin_site_ptr, 
										  const Surface::Site* const destination_site_ptr) const
{	using std::tr1::make_tuple; using std::tr1::bind; typedef std::plus<double> plus;
	using namespace std::tr1::placeholders;
	
	// find neighbors
	Interactions::TwoBody::Geometry::NeighborContainer bridge_neighbors;
	make_neighbors(origin_site_ptr, destination_site_ptr, bridge_neighbors);
	const Surface::Site origin_bridge(Surface::bridge, origin_site_ptr->index(), origin_site_ptr->species());
	const Surface::Site* const origin_bridge_ptr = &origin_bridge;
	/*
	return std::accumulate(bridge_neighbors.begin(),
						   bridge_neighbors.end(),
						   0.0,
						   bind(plus(), _1, bind(&InteractionEnergyContainer::operator(),
												 origin_ptr_bridge,
												 _2)));
	 */
	
	// sum energy on neighbors
	double energy = 0.0;
	for (Interactions::TwoBody::Geometry::NeighborContainer::const_iterator 
		 neighbor_container_it = bridge_neighbors.begin(), 
		 neighbor_container_end = bridge_neighbors.end();
		 neighbor_container_it != neighbor_container_end;
		 ++neighbor_container_it) {
		energy += interaction_energy_container(origin_bridge_ptr, *neighbor_container_it);
	}
	return energy;
}

