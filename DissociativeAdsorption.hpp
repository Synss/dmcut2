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
#ifndef DISSOCIATIVE_ADSORPTION_HPP
#define DISSOCIATIVE_ADSORPTION_HPP

#include <cassert>
#include <RandomLib/Random.hpp>
#include "Reaction.hpp"
#include "Site.hpp"
#include "Arrhenius.hpp" // update_time
#include "Lattice.hpp" // temperature

namespace Surface { class Species; }

namespace Reactions {

/** 
 * \brief Implementation of a dissociative adsorption event
 * \todo Take interactions into account
 * \sa Reaction
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-10-30
 */
class DissociativeAdsorption: public Reaction 
	{
	public:
		/// \brief Constructor
		/// \param[in] name of the reaction, e.g., "O2 adsorption"
		/// \param[in] species to adsorb
		/// \param[in] two_body_interactions on which neighbor to adsorb the dissociated species
		/// \param[in] preexponential_factor to the adsorption rate
		/// \param[in] activation_energy to the adsorption rate
		DissociativeAdsorption(const Name& name, const Surface::Species& species,
				   const Interactions::TwoBody::Geometry& two_body_interactions,
				   const double preexponential_factor,
				   const double activation_energy):
		name_(name), species_(species),
		preexponential_factor_(preexponential_factor), 
		activation_energy_(activation_energy),
		time_(0.0),
		two_body_interactions_(two_body_interactions),
		adsorption_site_(0), neighbor_adsorption_site_(0)
		{ }
		
	private:
		/// Copy constructor
		DissociativeAdsorption(const DissociativeAdsorption& other, 
							   Surface::Site* adsorption_site, 
							   Surface::Site* neighbor_adsorption_site):
		name_(other.name_), species_(other.species_),
		preexponential_factor_(other.preexponential_factor_),
		activation_energy_(other.activation_energy_),
		time_(other.time_),
		two_body_interactions_(other.two_body_interactions_),
		adsorption_site_(adsorption_site), neighbor_adsorption_site_(neighbor_adsorption_site)
		{ if (Enabled()) UpdateTime(); }
		
		DissociativeAdsorption(const DissociativeAdsorption&);
		DissociativeAdsorption& operator=(const DissociativeAdsorption&);
		
	public:
		/// Destructor
		~DissociativeAdsorption() 
		{ adsorption_site_ = neighbor_adsorption_site_ = 0; };
		
		Name name() const
		{ return name_; }
		
		void UpdateTime()
		{	using std::log; using namespace RandomLib;
			
			const double k = Arrhenius(preexponential_factor_, activation_energy_, Surface::temperature());
			double r = Random::Global.Fixed();
			while (r == 0.0) r = Random::Global.Fixed();
			assert(r != 0 && r != 1);
			assert(k != 0.0);
			time_ = (-1.0/k)*log(r);
		}
		
		double Time() const
		{ return time_; }
		
		bool Enabled() const
		{ return adsorption_site_->empty() && neighbor_adsorption_site_->empty(); }
		
		void operator()()
		{
			if (!Enabled()) return; // exception
			
			adsorption_site_->set_species(species_);
			neighbor_adsorption_site_->set_species(species_);
		}
		
		EventContainer GetEvents(const Surface::Site* adsorption_site_ptr) const
		{
			const Interactions::TwoBody::Geometry::NeighborContainer& 
			neighbor_container = adsorption_site_ptr->two_body_interactions(two_body_interactions_).neighbor_container();
			Reaction::EventContainer ev;

			for (Interactions::TwoBody::Geometry::NeighborContainer::const_iterator
					neighbor_container_it = neighbor_container.begin(),
					neighbor_container_end = neighbor_container.end();
					neighbor_container_it != neighbor_container_end;
					++neighbor_container_it)
			{
				EventPtr dissociative_adsorption_ptr 
					(new DissociativeAdsorption(*this, const_cast<Surface::Site*>(adsorption_site_ptr), 
												*neighbor_container_it));
				if (dissociative_adsorption_ptr->Enabled()) ev.push_back(dissociative_adsorption_ptr);
			}
			return ev;
		}
		
		SitePtrContainer site_container() const
		{
			SitePtrContainer site_ptr_container(1, adsorption_site_);
			site_ptr_container.insert(site_ptr_container.end(),
					two_body_interactions_.neighbor_container().begin(), two_body_interactions_.neighbor_container().end());
			return site_ptr_container;
		}
		
	private:
		Name name_;
		Surface::Species species_;
		double preexponential_factor_, activation_energy_;
		
		double time_;
		Interactions::TwoBody::Geometry two_body_interactions_;
		Surface::Site * adsorption_site_, * neighbor_adsorption_site_;
	};

} //namespace Reactions
	
#endif

