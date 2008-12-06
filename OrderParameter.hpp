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
#ifndef ORDER_PARAMETER_HPP
#define ORDER_PARAMETER_HPP

#include <map>
#include <utility>
#include <tr1/functional>
#include "Lattice.hpp"
#include "Species.hpp"

namespace Surface {
	
	/**
	 * \brief Functor: Compute order parameters on the Lattice
	 *
	 * This functor is called on each Site of an hexagonal Lattice to compute order parameters
	 * \par References:
	 * - Glosi et al., Can J Phys/Rev can phys 61(11): 1515-1527 (1983)
	 * - Piercy et al., Phys Rev B 45(4): 1869 (1992)
	 *
	 * \par Usage: 
	 * \code
	 * OrderParameter op(Lattice::lattice.width(), Lattice::multiplicity());
	 * std::for_each(Lattice::lattice.begin(), Lattice::lattice.end(), op);
	 * \endcode
	 * 
	 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
	 * \date 2008-10-30
	 */
	class OrderParameter: public std::tr1::function<void(Site*)> {
		
	public:
		/// \brief Constructor
		/// \param[in] lattice_width of the lattice (number of columns)
		/// \param[in] multiplicity of the lattice
		OrderParameter(Lattice::size_type lattice_width, int multiplicity):
		impl_map_(),
		lattice_width_(lattice_width), 
		multiplicity_(multiplicity), 
		energy_counter_(0),
		species_counter_()
		{ };
		
		/// \brief Copy constructor
		/// 
		/// Shallow copy necessary to use with std::for_each
		OrderParameter(const OrderParameter& other):
		impl_map_(other.impl_map_),
		lattice_width_(other.lattice_width_),
		multiplicity_(other.multiplicity_),
		energy_counter_(other.energy_counter_),
		species_counter_(other.species_counter_)
		{ };
		
		/// \brief Assignment operator
		///
		/// Shallow copy
		/// \todo verify correctness
		OrderParameter operator=(const OrderParameter& other) {
			if (&other == this) { return *this; }
			impl_map_ = other.impl_map_;
			lattice_width_ = other.lattice_width_;
			multiplicity_ = other.multiplicity_;
			energy_counter_ = other.energy_counter_;
			species_counter_ = other.species_counter_;
			return *this;
		}
		
		/// \brief Callback on each site of the lattice
		/// \param[in] site
		void operator()(const Site* site);
		
		/**
		 * \name Return order parameter per species per site
		 * \param[in] species 
		 * \param[in] site_name
		 * \return order parameter
		 */
		
		/// \{
		
		/// Order parameter for a \f$2\times2\f$ superstructure
		double p2x2(const Species& species, const Site::Name& site_name) const;
		
		/// Order parameter for a \f$2\times1\f$ superstructure
		double p2x1(const Species& species, const Site::Name& site_name) const;
		
		/// Order parameter for a \f$\sqrt3\times\sqrt3\f$ superstructure
		double sqrt3xsqrt3(const Species& species, const Site::Name& site_name) const;
		
		/// \}
		
		/**
		 * \name Information collected over the whole lattice
		 */
		
		/// \{
		
		/// \brief Sum of the interaction energies computed on the lattice
		/// \return energy
		double TotalEnergy() const;
		
		/// \brief Number of species on the Lattice
		/// \param[in] species
		/// \return population
		Lattice::size_type Population(const Species& species) const;
		
		/// \}
		
	private:
		/// Implementation: Treat parameters for one species at one site
		class Impl {
		public:
			/// Constructor
			Impl(): p(3), r(3)  {};
			
			/// \brief Copy constructor
			///
			/// Shallow copy
			Impl(const Impl& other):
			p(other.p), r(other.r) {};
			
			/// \brief Assignment operator
			///
			/// Shallow copy
			/// \todo verify correctness
			Impl operator=(const Impl& other) {
				if (&other == this) { return *this; }
				p = other.p;
				r = other.r;
				return *this;
			};
			
			/// Callback
			void operator()(const int row, const int col);
			
			/// Order parameter \f$2\times2\f$, not normalized
			double p2x2() const;
			
			/// Order parameter \f$2\times1\f$, not normalized
			double p2x1() const;
			
			/// Order parameter \f$\sqrt3\times\sqrt3\f$, not normalized
			double sqrt3xsqrt3() const;
			
		private:
			std::vector<double> p;
			std::vector<double> r;		
		};
		
		typedef std::pair<Species, Site::Name> key;
		typedef std::map<key, Impl> Impl_map;
		
		Impl_map impl_map_;
		
		// number of columns
		Lattice::size_type lattice_width_;
		
		int multiplicity_;
		
		double energy_counter_;
		
		std::map<Species, Lattice::size_type> species_counter_;		
	};
	
} // namespace Surface

#endif
