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
#ifndef ADSORPTION_HPP
#define ADSORPTION_HPP

#include <cassert>
#include <RandomLib/Random.hpp>
#include "Reaction.hpp"
#include "Site.hpp"
#include "Arrhenius.hpp" // update_time
#include "Lattice.hpp" // temperature

namespace Surface { class Species; }

namespace Reactions {

/** 
 * \brief Implementation of a simple adsorption event
 * \todo Take interactions into account
 * \sa Reaction
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-10-30
 */
class Adsorption: public Reaction {
public:
	/// \brief Constructor
	/// \param[in] name of the reaction, e.g., "CO adsorption"
	/// \param[in] species to adsorb
	/// \param[in] preexponential_factor to the adsorption rate
	/// \param[in] activation_energy to the adsorption rate
	Adsorption(const Name& name, const Surface::Species& species,
			   const double preexponential_factor,
			   const double activation_energy):
	name_(name), species_(species),
	preexponential_factor_(preexponential_factor), 
	activation_energy_(activation_energy),
	time_(0.0),
	adsorption_site_(0)
	{ }
	
private:
	/// Copy constructor
	Adsorption(const Adsorption& other, Surface::Site* adsorption_site):
	name_(other.name_), species_(other.species_),
	preexponential_factor_(other.preexponential_factor_),
	activation_energy_(other.activation_energy_),
	time_(other.time_),
	adsorption_site_(adsorption_site)
	{ if (Enabled()) UpdateTime(); }
	
	Adsorption(const Adsorption&);
	Adsorption& operator=(const Adsorption&);
	
public:
	/// Destructor
	~Adsorption() 
	{ adsorption_site_ = 0; };
	
	Name name() const
	{ return name_; }
	
	void UpdateTime()
	{	using std::log; using namespace RandomLib;
		
		const double real_activation_energy = activation_energy_ + adsorption_site_->self_energy();
		const double k = Arrhenius(preexponential_factor_, real_activation_energy, Surface::temperature());
		double r = Random::Global.Fixed();
		while (r == 0.0) r = Random::Global.Fixed();
		assert(r != 0 && r != 1);
		assert(k != 0.0);
		time_ = (-1.0/k)*log(r);
	}
	
	double Time() const
	{ return time_; }
	
	bool Enabled() const
	{ return adsorption_site_->empty(); }
	
	void operator()() {	
		if (!Enabled()) return;
		adsorption_site_->set_species(species_); 
	}
	
	EventContainer GetEvents(const Surface::Site* adsorption_site_ptr) const {
		EventPtr adsorption_ptr (new Adsorption(*this, const_cast<Surface::Site*>(adsorption_site_ptr)));
		return (adsorption_ptr->Enabled() ?
				EventContainer(1, adsorption_ptr) :
				EventContainer());
	}
	
	
	SitePtrContainer site_container() const
	{ return SitePtrContainer(1, adsorption_site_); }
	
private:
	Name name_;
	Surface::Species species_;
	double preexponential_factor_, activation_energy_;
	
	double time_;
	Surface::Site* adsorption_site_;
};
	
} // namespace Reactions

#endif
