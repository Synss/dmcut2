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
#ifndef DMCUT2_LATTICE_SITE_HPP
#define DMCUT2_LATTICE_SITE_HPP

#include <set>
#include <string>
#include <string>
#include <cassert>
#include "Geometry.hpp"
#include "SelfEnergy.hpp"
#include "Species.hpp"
#include "EventPtr.hpp"
#include "Bridge.hpp"

namespace Surface {

namespace {
#include <tr1/functional>
	struct sort_int_on_name: std::tr1::function<bool(Interactions::TwoBody::Geometry, 
													 Interactions::TwoBody::Geometry)> {
		bool operator()(const Interactions::TwoBody::Geometry& lhs, 
						const Interactions::TwoBody::Geometry& rhs) const
		{ return lhs.name() < rhs.name(); }
	};
	
	struct sort_br_on_name: std::tr1::function<bool(Bridge, Bridge)> {
		bool operator()(const Bridge& lhs, const Bridge& rhs) const
		{ return lhs.name() < rhs.name(); }
	};	
}	

/**
 * \brief Site on the lattice
 *
 * Sites contain a species, pointers to neighboring sites (this allows
 * the lattice to be implemented as a linked list) 
 *
 * \par Usage:
 * Copy is forbidden, always pass pointers to sites
 *
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-11-14
 */
class Site
{
public:
	typedef std::string Name;

	typedef std::size_t index_type;
	
	typedef std::set<Interactions::TwoBody::Geometry, sort_int_on_name > TwoBodyInteractionsContainer;
	
	typedef std::set<Bridge, sort_br_on_name > BridgeInteractionsContainer;
	
	typedef std::vector<Reactions::EventPtr> EventContainer;
	
	/// Constructor
	explicit
	Site(const Name& name, index_type index, const Species& species):
	two_body_interactions_container(),
	name_(name), index_(index), adsorbate_(species)
	{ }
	
	/**
	 * \name Accessors
	 */
	/// \{
	
	Site::Name name() const
	{ return name_; }
	
	Species species() const
	{ return adsorbate_; }
	
	/// Return true if no adsorbate
	bool empty() const
	{ return adsorbate_.no_species(); }
	
	index_type index() const
	{ return index_; }
	
	double self_energy() const
	{ return species().no_species() ? 0.0 : self_energy_(*this); }
	
	/// \brief Find two body interactions in the container
	/// \param[in] two_body_interactions
	/// \return Reference to the two_body_interactions in the container
	TwoBodyInteractionsContainer::const_reference
	two_body_interactions(const TwoBodyInteractionsContainer::value_type& two_body_interactions) const
	{
		assert (two_body_interactions_container.find(two_body_interactions)
				!= two_body_interactions_container.end());
		return *two_body_interactions_container.find(two_body_interactions); 
	}	
	
	std::pair<TwoBodyInteractionsContainer::const_iterator, TwoBodyInteractionsContainer::const_iterator>
	two_body_interactions_range()
	{ return std::make_pair(two_body_interactions_container.begin(), two_body_interactions_container.end()); }	
	
	/// \}
	
	/**
	 * \name Modifiers
	 */
	/// \{
	
	void set_species(const Species& species)
	{ adsorbate_ = species; }
	
	void set_self_energy(const Interactions::SelfEnergy& self_energy)
	{ self_energy_ = self_energy; }
	
	/// \brief Add bridge interactions to the container
	/// \param[in] bridge
	void set_bridge_interactions(const Bridge& bridge)
	{ bridge_interactions_container.insert(bridge); }
	
	/// \brief Add two body interactions
	/// \param[in] two_body_interactions
	void set_two_body_interactions(const TwoBodyInteractionsContainer::value_type& two_body_interactions)
	{ two_body_interactions_container.insert(two_body_interactions); }
	
	/// \}
	
	/// \brief Remove adsorbate
	void Clear()
	{ adsorbate_ = no_species; }
	
	/// \brief Sum interaction energy over all neighbors
	/// \return Energy
	double InteractionEnergy() const;
	
	/// \brief Sum interaction energy over all bridge sites
	/// \param[in] other_site_ptr bridge sites are found between this and other_site_ptr
	/// \return Energy
	double InteractionEnergyBridge(const Site* const other_site_ptr) const;
	
private:
	Site (const Site&);
	void operator= (const Site&);
	
	Interactions::SelfEnergy self_energy_;
	
	Site::Name name_;
	
	index_type index_;
	
	Species adsorbate_;
	
	TwoBodyInteractionsContainer two_body_interactions_container;
	
	BridgeInteractionsContainer bridge_interactions_container;
		
	friend std::istream& operator>>(std::istream& is, Site& s);	
};

} // namespace Surface
	
#include "Site-inl.hpp"

#endif
