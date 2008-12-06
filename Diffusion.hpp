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
#ifndef DIFFUSION_HPP
#define DIFFUSION_HPP

#include <vector>
#include <string>
#include <ostream>
#include <cassert>

#include "Geometry.hpp"
#include "Reaction.hpp"
#include "Bridge.hpp" // used in implementation
#include "Site.hpp"

namespace Surface { class Species; }

namespace Reactions {

/** 
 * \brief Implementation of a simple adsorption event, cf. Reactions::Reaction
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-10-30
 */
class Diffusion: public Reaction
{
public:
	/// \brief Constructor
	/// \param[in] name of the reaction, e.g., "CO diffusion"
	/// \param[in] species to diffuse
	/// \param[in] two_body_interactions provide the neighbor on which to diffuse
	/// \param[in] preexponential_factor to the diffusion rate
	Diffusion(const Name& name,
			  const Surface::Species& species,
			  const Interactions::TwoBody::Geometry& two_body_interactions,
			  const double preexponential_factor):
	name_(name), species_(species), 
	preexponential_factor_(preexponential_factor),
	time_(0.0), origin_ptr_(0), destination_ptr_(0),
	two_body_interactions_ptr_(&two_body_interactions)
	{ };
	
private:
	/// Copy constructor
	Diffusion(const Diffusion& other,
			  Surface::Site* origin_ptr, 
			  Surface::Site* destination_ptr):
	name_(other.name_),	species_(other.species_),
	preexponential_factor_(other.preexponential_factor_),
	time_(other.time_),
	origin_ptr_(origin_ptr), destination_ptr_(destination_ptr),
	two_body_interactions_ptr_(&(origin_ptr->two_body_interactions(*(other.two_body_interactions_ptr_))))
	{ if (Enabled()) UpdateTime(); }
	
	Diffusion(const Diffusion&);
	Diffusion& operator=(const Diffusion&);

public:
	/// Destructor
	~Diffusion()
	{ origin_ptr_ = destination_ptr_ = 0; }

	Name name() const
	{ return name_; }

	void UpdateTime();

	double Time() const
	{ return time_; }

	bool Enabled() const
	{ return origin_ptr_->species() == species_ && destination_ptr_->empty(); }

	void operator()() {
		if (!Enabled()) return;

		origin_ptr_->Clear();
		destination_ptr_->set_species(species_);
	}		
		
	EventContainer GetEvents(const Surface::Site* origin) const;

	SitePtrContainer site_container() const {
		SitePtrContainer site_ptr_container(1, origin_ptr_);
		site_ptr_container.insert(site_ptr_container.end(),
								  two_body_interactions_ptr_->neighbor_container().begin(), 
								  two_body_interactions_ptr_->neighbor_container().end());
		return site_ptr_container;
	}

private:	
	/// Compute activation energy
	double ActivationEnergy() const;

	/// Return interaction energy at saddle point
	double InteractionEnergyBridge() const;

	Name name_;
	Surface::Species species_;
	double preexponential_factor_;
	double time_;
	Surface::Site* origin_ptr_;
	Surface::Site* destination_ptr_;
	const Interactions::TwoBody::Geometry* const two_body_interactions_ptr_;
};

} // namespace Reactions

//inline std::ostream& operator<<(std::ostream& os, const Reactions::Diffusion& diff)
//{ return os << diff.name(); }

#endif
