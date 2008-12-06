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
#ifndef DESORPTION_HPP
#define DESORPTION_HPP

#include <cassert>
#include <RandomLib/Random.hpp>
#include "Reaction.hpp"
#include "Site.hpp"
#include "Arrhenius.hpp" // update_time
#include "Lattice.hpp" // temperature

namespace Surface { class Species; }

namespace Reactions {

/** 
 * \brief Implementation of a desorption event
 * \todo Take interactions into account
 * \sa Reaction
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-10-30
 */
class Desorption: public Reaction 
	{
	public:
		/// \brief Constructor
		/// \param[in] name of the reaction, e.g., "CO desorption"
		/// \param[in] species to desorb
		/// \param[in] preexponential_factor to the desorption rate
		/// \param[in] activation_energy to the desorption rate
		Desorption(const Name& name, const Surface::Species& species,
				   const double preexponential_factor,
				   const double activation_energy):
		name_(name), species_(species),
		preexponential_factor_(preexponential_factor), 
		activation_energy_(activation_energy),
		time_(0.0),
		desorption_site_(0)
		{ }
		
	private:
		/// Copy constructor
		Desorption(const Desorption& other, Surface::Site* desorption_site):
		name_(other.name_), species_(other.species_),
		preexponential_factor_(other.preexponential_factor_),
		activation_energy_(other.activation_energy_),
		time_(other.time_),
		desorption_site_(desorption_site)
		{ if (Enabled()) UpdateTime(); }
		
		Desorption(const Desorption&);
		Desorption& operator=(const Desorption&);
		
	public:
		/// Destructor
		~Desorption() 
		{ desorption_site_ = 0; };
		
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
		{ return desorption_site_->empty(); }
		
		void operator()()
		{
			if (!Enabled()) return;
			desorption_site_->set_species(species_);
		}
		
		EventContainer GetEvents(const Surface::Site* desorption_site_ptr) const {
			EventPtr desorption_ptr (new Desorption(*this, const_cast<Surface::Site*>(desorption_site_ptr)));
			return (desorption_ptr->Enabled() ?
					EventContainer(1, desorption_ptr) :
					EventContainer());
		}
		
		
		SitePtrContainer site_container() const
		{ return SitePtrContainer(1, desorption_site_); }
		
	private:
		Name name_;
		Surface::Species species_;
		double preexponential_factor_, activation_energy_;
		
		double time_;
		Surface::Site* desorption_site_;
	};
	
} // namespace Reactions

#endif
