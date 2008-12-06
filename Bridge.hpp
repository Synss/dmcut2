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
#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include <string>
#include "EnergyContainer.hpp"
#include "Geometry.hpp"
#include "Species.hpp"

namespace Surface {

/**
 * \brief Bridge sites
 *
 * Bridge sites are defined as the common neighbors of two sites,
 * which neighbors to consider is defined by the Interactions::TwoBody::Geometry
 * arguments passed
 *
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-10-30
 */
class Bridge {
public:	
	/// Define name
	typedef std::string Name;

	/// \brief Constructor
	/// \param[in] name the brige site, e.g., "1st nearest bridge"
	/// \param[in] two_body1
	/// \param[in] two_body2
	Bridge(const Name& name, 
		   const Interactions::TwoBody::Geometry& two_body1, 
		   const Interactions::TwoBody::Geometry& two_body2):
	name_(name), two_body1_(two_body1), two_body2_(two_body2),
	interaction_energy_container() {};
	
	/// \brief Accessor to name
	Name name() const
	{ return name_; }
	
	/// Container of interaction energy
	Interactions::TwoBody::EnergyContainer interaction_energy_container;
	
	/// \brief Compute interaction energy at this bridge site, defined by the common 
	/// neighbors of the two sites provided as arguments
	/// \param[in] site_ptr1
	/// \param[in] site_ptr2
	double InteractionEnergy(const Surface::Site* const site_ptr1, 
							 const Surface::Site* const site_ptr2) const;
	
private:
	//Bridge(const Bridge&);
	//void operator=(const Bridge&);
	
	/// Find the neighbors of the bridge site: common neighbors of the Sites
	void make_neighbors(const Surface::Site* const site_ptr1, 
						const Surface::Site* const site_ptr2, 
						Interactions::TwoBody::Geometry::NeighborContainer& result) const;
	
	/// Name of the Bridge object
	Name name_;
	
	/// The bridge sites are the common neighbors to two_body1_ and two_body2_
	Interactions::TwoBody::Geometry two_body1_, two_body2_;
};
	
} // namespace Surface

#endif
