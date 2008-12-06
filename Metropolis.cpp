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

#include "Metropolis.hpp"

#include <cassert>
// http://charles.karney.info/random/
#include <RandomLib/Random.hpp>
#include <cstddef> // NULL
void Algorithms::Metropolis::operator()() const {
	using RandomLib::Random;
	
	Surface::Lattice::size_type lattice_size = last_ - first_;
	
	for (Surface::Lattice::size_type iteration = 0; 
		 iteration != lattice_size; ++iteration) {
		Surface::Site* origin_ptr = get_origin();
		if (origin_ptr->empty() ||
			(species_.name() != "Any species" && origin_ptr->species() != species_)) 
			continue;
		
		Surface::Site* destination_ptr = get_destination(origin_ptr);
		if (!destination_ptr->empty()) continue;
		
		// Get energy before move
		const double energy_init = origin_ptr->InteractionEnergy();
		
		// Try move
		const Surface::Species adsorbate = origin_ptr->species();
		origin_ptr->Clear();
		destination_ptr->set_species(adsorbate);
		
		// Get energy after move
		const double energy_fin = destination_ptr->InteractionEnergy();
		
		// Accept move
		const double energy_diff = energy_fin - energy_init;
		if (energy_diff <= 0.0) continue;
		
		const double r = Random::Global.Fixed();
		if ( r <= Arrhenius(1.0, energy_diff, Surface::temperature()) ) continue;
		
		// Reject move
		destination_ptr->Clear();
		origin_ptr->set_species(adsorbate);
	}
}


#include <RandomLib/Random.hpp>
inline Surface::Site*
Algorithms::Metropolis::get_origin() const {
	using RandomLib::Random;
	Surface::Lattice::size_type lattice_size = last_ - first_;
	
	const Surface::Lattice::size_type random_index = Random::Global.Integer(lattice_size);
	return *(first_ + random_index);
}

#include <tr1/functional>
namespace Algorithms {
	namespace {
		/// Functor, return true is \a Site is empty
		struct is_empty: std::tr1::function<bool(const Surface::Site&)> {
			bool operator()(const Surface::Site* sp) const
			{ return sp->species() == Surface::no_species; }
		};
	} // anonymous namespace
} // namespace Algorithm

#include <RandomLib/Random.hpp>
#include <algorithm> // equal_range, sort, pair
Surface::Site* 
Algorithms::Metropolis::get_destination(const Surface::Site* origin_ptr) const {
	using namespace Interactions::TwoBody;
	
	// Chose destination: Find an empty site in the neighborhood
	Geometry::NeighborContainer  // copy
	neighbors = origin_ptr->two_body_interactions(neighbors_).neighbor_container();
	
	Geometry::NeighborContainer::const_iterator
	neighbors_end = std::partition(neighbors.begin(), neighbors.end(), is_empty());
	
	const Geometry::NeighborContainer::size_type
	range_nb = neighbors_end - neighbors.begin(); // count possible sites
	return range_nb > 1 ? 
	*(neighbors.begin() + RandomLib::Random::Global.Integer(range_nb)) :
	*neighbors.begin();
}
