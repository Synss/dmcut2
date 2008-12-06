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

#include "Diffusion.hpp"

/*
 *	Diffusion
 *
 */

#include "Arrhenius.hpp"
#include "Lattice.hpp" // temperature
#include <RandomLib/Random.hpp> // http://charles.karney.info/random/
#include <cassert>
void Reactions::Diffusion::UpdateTime()
{ 	using std::log; using namespace RandomLib;
	
	const double k = Arrhenius(preexponential_factor_, ActivationEnergy(), Surface::temperature());
	double r = Random::Global.Fixed();
	while (r == 0.0) r = Random::Global.Fixed();
	assert(r != 0 && r != 1);
	assert(k != 0.0);
	time_ = (-1.0/k)*log(r);
}


#include <algorithm> // max
double Reactions::Diffusion::ActivationEnergy() const
{	using std::max;

	if (!Enabled()) return 0.0;
	
	// initial energy
	const double energy_init = origin_ptr_->self_energy() + origin_ptr_->InteractionEnergy();
	
	// activation energy at the saddle point
	const double energy_saddle_minus_init = InteractionEnergyBridge() - energy_init;
	
	// final energy
	// do move
	origin_ptr_->Clear();
	destination_ptr_->set_species(species_);
	const double energy_final = destination_ptr_->self_energy() + destination_ptr_->InteractionEnergy();
	// undo move
	origin_ptr_->set_species(species_);
	destination_ptr_->Clear();
	
	const double energy_final_minus_init = energy_final - energy_init;
	
	return energy_saddle_minus_init > energy_final_minus_init ? 
	max(energy_saddle_minus_init, 0.0) : max(energy_final_minus_init, 0.0);
}


#include <numeric> // accumulate
#include <tr1/functional> // bind, plus
#include <cassert>
#include <cstddef> // NULL
#include "Lattice.hpp"
#include "Bridge.hpp"
double Reactions::Diffusion::InteractionEnergyBridge() const
{	assert(origin_ptr_ != NULL && destination_ptr_ != NULL);
	
	// Accumulate results from Bridge::interaction_energy(...)
	return origin_ptr_->InteractionEnergyBridge(destination_ptr_);
}


#include <cassert>
#include <algorithm>
Reactions::Reaction::EventContainer
Reactions::Diffusion::GetEvents(const Surface::Site* origin_ptr) const
{	
	// get neighbors on which to diffuse
	const Interactions::TwoBody::Geometry::NeighborContainer&
	destination_container = origin_ptr->two_body_interactions(*two_body_interactions_ptr_).neighbor_container();
	Reaction::EventContainer ev;
	for (Interactions::TwoBody::Geometry::NeighborContainer::const_iterator
		 destination_container_it = destination_container.begin(), 
		 destination_container_end = destination_container.end();
		 destination_container_it != destination_container_end;
		 ++destination_container_it)
	{
		{
			// look for enabled diffusion from origin
			EventPtr diffusion_ptr (new Diffusion(*this, const_cast<Surface::Site*>(origin_ptr), *destination_container_it));
			if (diffusion_ptr->Enabled()) ev.push_back(diffusion_ptr);
		}
		{	// FIXME remove duplicates
			// look for enabled diffusion from neighbor
			EventPtr diffusion_ptr (new Diffusion(*this, *destination_container_it, const_cast<Surface::Site*>(origin_ptr)));
			if (diffusion_ptr->Enabled()) ev.push_back(diffusion_ptr);
		}
	}
	return ev;
}
