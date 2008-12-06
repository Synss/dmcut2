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

#ifndef SELF_ENERGY_HPP
#define SELF_ENERGY_HPP

#include <string>
#include <utility> // pair
#include <ml_container/associative_vector.hpp>

#include "Species.hpp"
namespace Surface { class Site; }

namespace Interactions {
	/**
	 * \brief Container for self energy
	 *
	 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
	 * \date 2008-10-30
	 * \sa Interactions::TwoBody::EnergyContainer
	 */
	class SelfEnergy {
		/// Define energy lookup key <origin species, origin site>
		typedef std::pair<Surface::Species, std::string> lookup_key;
		
		/// Define energy lookup table
		typedef ml_container::associative_vector< lookup_key, double > container_type;
		
	public:
		typedef container_type::key_type key_type;
		typedef container_type::value_type value_type;
		typedef container_type::mapped_type mapped_type;
		typedef container_type::reference reference;
		typedef container_type::const_reference const_reference;	
		
		/// \brief Return self energy on site
		/// \param[in] site
		/// \return energy
		mapped_type operator()(const Surface::Site& site) const;
		
		/// \brief Fill container
		/// \param[in] key pair<Surface::Species, Site::Name>
		/// \return reference to object created
		reference operator[](const key_type& k)
		{ return container_[k]; }
		
	private:
		//SelfEnergy(const SelfEnergy&);
		//void operator=(const SelfEnergy&);
		
		container_type container_;	
	};
} // namespace Interactions

#endif
