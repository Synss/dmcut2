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
#ifndef DMCUT2_INTERACTIONS_TWO_BODY_GEOMETRY_HPP
#define DMCUT2_INTERACTIONS_TWO_BODY_GEOMETRY_HPP

#include <string>
#include <set>
#include <vector>
#include <ostream>

#include "EnergyContainer.hpp"
#include "Species.hpp"

namespace Surface { class Site; }

/// Multi-body interactions between the sites
namespace Interactions {
/// Two-body interactions
namespace TwoBody {

/// Define container of relative indexes to neighboring sites
typedef std::vector<std::ptrdiff_t> RelativeIndexContainer;

/**
 * \brief Geometry of the interactions
 *
 * - First, define the interaction energy on any site by providing an InteractionEnergyContainer
 * - Second, associate these interaction energies with the sites on the lattice using the functor LinkSites 
 *
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-10-30
 */	
class Geometry {	
public:	
	/// Define container for pointers to the neighboring sites
	typedef std::vector< Surface::Site* > NeighborContainer;
	
	/// Define type for the name
	typedef std::string Name;
	
	/// \brief Constructor
	/// \param[in] name of the interactions, e.g., "1st nearest neighbors"
	Geometry(const Name& name):
	name_(name), site_ptr_(NULL),
	neighbor_container_(), energy_container_() {};
	
	/**
	 * \name Accessors
	 */
	/// \{
	
	/// \brief Name accessor
	Name name() const
	{ return name_; }
	
	/// \brief Accessor to the container of neighboring sites
	const NeighborContainer& neighbor_container() const
	{ return neighbor_container_; }
		
	/// \}
	
	/**
	 * \name Modifiers
	 */
	/// \{
	
	/// \brief Set interaction energy
	void set_interaction_energy_container(const EnergyContainer& energy_container)
	{ energy_container_ = energy_container; }
	
	/// \}
	
	/// \brief Compute interaction energy on the associate site
	/// \return interaction energy
	double InteractionEnergy() const;
	
private:
	//Geometry(const Geometry&);
	//void operator=(const Geometry&);
	
	NeighborContainer& neighbor_container()
	{ return neighbor_container_; }

	void set_site_ptr(Surface::Site* site_ptr)
	{ site_ptr_ = site_ptr; }

	Name name_;
	Surface::Site* site_ptr_;
	NeighborContainer neighbor_container_;
	EnergyContainer energy_container_;
	
	friend class LinkSites;
};

} // namespace TwoBody
} // namespace Interactions

namespace Surface { 
	class HelicalBoundaryConditions; 
	class Site;
}

#include <tr1/functional>

namespace Interactions {
namespace TwoBody {
/// \brief Functor: Generate the linked list of sites
/// This functor accomplishes two things:
/// - create the linked list gives the geometry of the lattice
/// - associates two-body interaction energies with each site
struct LinkSites: std::tr1::function<void(Surface::Site*, RelativeIndexContainer)>
{
	/// \brief Constructor
	/// \param[in] boundary_conditions
	LinkSites(const Surface::HelicalBoundaryConditions& boundary_conditions):
	boundary_conditions_(boundary_conditions) {};
	
	/// \brief Callback
	/// \param[in] interactions two-body interactions to which associate a site
	/// \param[in] site_ptr a pointer to a site on the lattice
	/// \param[in] relative_index_container give the coordinates of the neighbors to \param site_ptr
	void operator()(Geometry interactions, 
					Surface::Site* site_ptr,
					RelativeIndexContainer relative_index_container) const;
	
private:
	const Surface::HelicalBoundaryConditions& boundary_conditions_;
};

} // namespace TwoBody
} // namespace Interactions
	
/// Output operator
std::ostream& operator<<(std::ostream&, const Interactions::TwoBody::Geometry&);

#endif
