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
// Author: Mathias Laurin <laurin@chem.s.u-tokyo.ac.jp>
/*
* 4 sublattices such as
*     m1  m2  m1  m2 ...
*   m3  m4  m3  m4 ...
* m1  m2  m1  m2 ...
*
* N  = m1 + m2 + m3 + m4 // total number adsorbates
* p0 = m1 + m2 - m3 - m4;
* p1 = m1 - m2 + m3 - m4;
* p2 = m1 - m2 - m3 + m4;
*
* order 2x2 = (1/N) * (p0*p0 * p1*p1 * p2*p2)^(1/6)
* order 2x1 = (1/(N*sqrt(3))) * sqrt(p0*p0 + p1*p1 + p2*p2)
*/

#include "OrderParameter.hpp"
#include <cmath>

#include <iostream>
#include <utility>

#define inline

void Surface::OrderParameter::operator()(const Site* site_ptr)
{
	// global parameters
	energy_counter_ += site_ptr->self_energy() + site_ptr->InteractionEnergy();
	++species_counter_[site_ptr->species()];
	
	// parameters per species, per site
	const unsigned int
	row = site_ptr->index() / lattice_width_,
	col = site_ptr->index() % lattice_width_ / multiplicity_;
	
	impl_map_[std::make_pair(site_ptr->species(), site_ptr->name())](row, col);
}

inline double Surface::OrderParameter::TotalEnergy() const
{ return energy_counter_; }

inline Surface::Lattice::size_type 
Surface::OrderParameter::Population(const Species& species) const
{ 
	return species_counter_.find(species) != species_counter_.end() ?
	species_counter_.find(species)->second : 0; 
}


#include <utility>
inline double 
Surface::OrderParameter::p2x2(const Species& species, const Site::Name& site_name) const
{	using std::make_pair;
	// Return the order parameter if there is any such species, or 0
	return impl_map_.find(make_pair(species, site_name)) != impl_map_.end() ?
	impl_map_.find(make_pair(species, site_name))->second.p2x2() / 
	species_counter_.find(species)->second : 0;
}

inline double
Surface::OrderParameter::p2x1(const Species& species, const Site::Name& site_name) const
{	using std::make_pair;
	return impl_map_.find(make_pair(species, site_name)) != impl_map_.end() ?
	impl_map_.find(make_pair(species, site_name))->second.p2x1() / 
	species_counter_.find(species)->second : 0;
}

inline double
Surface::OrderParameter::sqrt3xsqrt3(const Species& species, const Site::Name& site_name) const
{	using std::make_pair;
	return impl_map_.find(make_pair(species, site_name)) != impl_map_.end() ?
	impl_map_.find(make_pair(species, site_name))->second.sqrt3xsqrt3() / 
	(lattice_width_ * lattice_width_ / multiplicity_) : 0;
}

void Surface::OrderParameter::Impl::operator()(const int row, const int col)
{
	// Divide the lattice 4 sublattices
	if (col % 2 == 0) {
		if (row % 2 == 0) {
			++p[0]; ++p[1]; ++p[2];  // row even, col even
		} else {
			++p[0]; --p[1]; --p[2];  // row odd, col even
		}
	} else {
		if (col % 2 == 0) {
			--p[0]; ++p[1]; --p[2];  // row even, col odd
		} else {
			--p[0]; --p[1]; ++p[2];  // row odd, col odd
		}
	}
	// Divide the lattice in 3 sublattices
	int i = (row - col) % 3;
	if (i < 0) i += 3; // sign of modulo is implementation dependent
	++r[i];
}

inline double
Surface::OrderParameter::Impl::p2x2() const
{ return pow(p[0]*p[0] * p[1]*p[1] * p[2]*p[2], 1.0/6.0); }

inline double
Surface::OrderParameter::Impl::p2x1() const
{ return sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]) / sqrt(3.0) ; }

inline double
Surface::OrderParameter::Impl::sqrt3xsqrt3() const
{
	std::cout << " r[0] = " << r[0] << " r[1] = " << r[1] << " r[2] = " << r[2];
	
	const long double q1 = 2.0*r[0] - r[1] - r[2];
	const long double q2 = sqrt(3.0) * (r[1] - r[2]);
	return 1.5 * sqrt(q1*q1 + q2*q2);
}

#undef inline
