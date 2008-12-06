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
#ifndef DMCUT2_LATTICE_SPECIES_HPP
#define DMCUT2_LATTICE_SPECIES_HPP

#include <string> // Name

namespace Surface {

/**
 * \brief Species/adsorbates
 *
 *
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-11-14
 */
class Species {
public:
	typedef std::string Name;
	
	/// Constructor
	Species(const Name& name, double molar_mass):
	name_(name), mass_(molar_mass)
	{ }

	/**
	 * \name Accessors
	 */
	/// \{
	
	Name name() const
	{ return name_; }
	
	double mass() const
	{ return mass_; }
	
	/// \}
	
	/// Return true if there is no species.
	bool no_species() const
	{ return name() == "empty"; }
	
	
private:
	/// Name
	Name name_;
	
	/// Molar mass
	double mass_;
};

/// The content of an empty site
const static Species no_species = Species("empty", 0);

/// Equality operator
inline bool 
operator== (const Surface::Species& lhs, const Surface::Species& rhs)
{ return lhs.mass() == rhs.mass(); }

/// Non-equality operator
inline bool 
operator!= (const Surface::Species& lhs, const Surface::Species& rhs)
{ return !(lhs == rhs); }

/// Less-than operator
inline bool
operator< (const Surface::Species& lhs, const Surface::Species& rhs)
{ return lhs.mass() < rhs.mass(); }

} /// namespace Surface

	
#include <ostream>
/// Output operator
inline std::ostream& 
operator<< (std::ostream& os, const Surface::Species& sp)
{ return os << sp.name(); }

#endif
