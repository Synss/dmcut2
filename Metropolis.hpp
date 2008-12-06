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
#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include "Arrhenius.hpp"
#include "Lattice.hpp"

/// Integration of the master equation
namespace Algorithms {
	
	/** 
	 * \brief Metropolis algorithm
	 *
	 * \par Usage:
	 * Implemented as a functor
	 * \code
	 * Surface::Lattice lattice(128);
	 * // ... define lattice ...
	 * Interactions::TwoBody::Geometry first_nearest_neighbors("1st NN");
	 * // ... define neighbors ...
	 * Metropolis metropolis(Lattice::lattice.begin(), Lattice::lattice.end(),
	 *                       first_nearest_neighbors);
	 * metropolis(); // perform one metropolis step
	 * metropolis(); // etc.
	 * \endcode
	 *
	 * \par Reference:
	 * Metropolis et al., J Chem Phys 21 1087
	 *
	 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
	 * \date 2008-11
	 */
	class Metropolis {
	public:	
		/// \brief Constructor
		/// \param[in] first element of the lattice
		/// \param[in] last element of the lattice
		/// \param[in] neighbors on which to walk
		Metropolis(Surface::Lattice::iterator first, 
				   Surface::Lattice::iterator last,
				   const Interactions::TwoBody::Geometry& neighbors):	 ///< neighbors on which to walk
		first_(first), last_(last),
		neighbors_(neighbors), species_(Surface::Species("Any species", 0))
		{ };
		
		/// \brief Constructor
		/// \param[in] first element of the lattice
		/// \param[in] last element of the lattice
		/// \param[in] neighbors on which to walk
		/// \param[in] species that performs the walk
		Metropolis(Surface::Lattice::iterator first, 
				   Surface::Lattice::iterator last,
				   const Interactions::TwoBody::Geometry& neighbors,	 ///< neighbors on which to walk
				   const Surface::Species species): ///< species to move
		first_(first), last_(last),
		neighbors_(neighbors), species_(species)
		{ };
		
		/// Perform one Metropolis step
		void operator()() const;
		
	private:
		Metropolis(const Metropolis&);
		void operator=(const Metropolis&);
		
		Surface::Site* get_origin() const;
		
		Surface::Site* get_destination(const Surface::Site* origin_ptr) const;
		
		Surface::Lattice::iterator first_, last_;
		
		Interactions::TwoBody::Geometry neighbors_;
		
		Surface::Species species_;
	};
	
} // namespace Algorithms

#endif
