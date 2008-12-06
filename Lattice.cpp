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

#include "Lattice.hpp"

// global parameters in namespace Surface
unsigned Surface::temperature_ = 300;
std::vector<Surface::Site::Name> Surface::name_set_;
Surface::Site::Name Surface::fcc = "fcc";
Surface::Site::Name Surface::hcp = "hcp"; 
Surface::Site::Name Surface::bridge = "bridge";
Surface::Site::Name Surface::any = "any";

/// Randomly populate with Species up to coverage
void Surface::Lattice::Populate(double coverage, const Species& species) 
{ Surface::Populate(begin(), end(), coverage, species); }

/// Remove all adsorbate from the lattice
void Surface::Lattice::Clean()
{ Surface::Clean(begin(), end()); }

void Surface::Lattice::set_bridge(const Bridge& bridge)
{ Surface::SetBridge(begin(), end(), bridge); }

// http://charles.karney.info/random/
#include <RandomLib/Random.hpp>
void Surface::Populate(Lattice::iterator first, Lattice::iterator last,
					   double coverage, const Species& species) {
	const Lattice::size_type lattice_sz = last - first;
	Lattice::size_type number_adsorbates = static_cast<Lattice::size_type>(coverage * lattice_sz / multiplicity());
	while (number_adsorbates) {
		const Lattice::size_type offset = RandomLib::Random::Global.Integer(lattice_sz);
		const Lattice::iterator iter = first + offset;
		if ((*iter)->empty()) { 
			(*iter)->set_species(species);
			--number_adsorbates;
		}
	}
}

#include <algorithm>
#include <tr1/functional>
void Surface::Clean(Lattice::iterator first, Lattice::iterator last) {	
	using std::tr1::bind; using std::tr1::placeholders::_1;
	std::for_each(first, last, bind(&Site::Clear, _1));
}

void Surface::SetBridge(Lattice::iterator first, Lattice::iterator last,
						const Bridge& bridge) {
	using std::tr1::bind; using std::tr1::placeholders::_1;
	std::for_each(first, last, 
				  bind(&Site::set_bridge_interactions, _1, bridge));
}


Surface::Site* Surface::HelicalBoundaryConditions::operator()
(const Site* site_ptr, Interactions::TwoBody::RelativeIndexContainer::value_type offset) {
	Interactions::TwoBody::RelativeIndexContainer::value_type 
	index_neighbor = site_ptr->index() + offset;
	if (index_neighbor < 0)
	{ index_neighbor += size_lattice_; }
	else if (index_neighbor >= size_lattice_)
	{ index_neighbor -= size_lattice_; }
	return *(first_ + index_neighbor);
}
