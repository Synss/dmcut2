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
#ifndef LATTICE_HPP
#define LATTICE_HPP

#include <algorithm>
#include <tr1/functional>
#include "Site.hpp"

/// \brief Everything concerning the surface
/// \todo Change the representation from a Lattice of Sites on the surface
/// to Atoms from the TopLayer, each Atom contains Sites: atop, bridge, hcp, 
/// fcc, one of each (could use ENUM)
/// geometry then depends on Atoms, not Sites, which makes it much simpler
namespace Surface {
	
	/**
	 * \name Temperature of the lattice
	 */
	/// \{
	extern unsigned temperature_; 
	
	/// Accessor
	inline unsigned temperature()
	{ return temperature_; }	
	
	/// Modifier
	inline void set_temperature(unsigned temperature)
	{ temperature_ = temperature; }
	/// \}
	
	/**
	 * \name Define names for the sites on the lattice
	 */
	/// \{
	extern std::vector<Site::Name> name_set_;
	extern Site::Name fcc;
	extern Site::Name hcp;
	extern Site::Name bridge;
	extern Site::Name any;
	
	/// Accessor
	inline std::vector<Site::Name> name_set()
	{ return name_set_; }
	
	/// Return multiplicity of the lattice
	inline std::vector<Site::Name>::size_type multiplicity()
	{ return name_set_.size(); }
	/// \}
	
	
	/**
	 * \brief The lattice
	 *
	 * The lattice is implementef as a vector of pointers to Site and provides
	 * most of the accessors of std::vector
	 *
	 * \par Usage
	 * The methods Populate, Clean and SetBridge
	 * are provided for convenience but actually only call the 
	 * functional-programming style functions defined with the same name
	 * in this scope.
	 *
	 * \sa
	 * - Surface::Clean
	 * - Surface::Populate
	 * - Surface::SetBridge
	 *
	 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
	 * \date 2008-10-30
	 */
	class Lattice {	
		
		typedef std::vector<Site*> SiteContainer;
		
	public:
		
		//typedef SiteContainer::pointer pointer;
		//typedef SiteContainer::reference reference;
		//typedef SiteContainer::const_reference const_reference;
		
		/// Define iterator to the lattice
		typedef SiteContainer::iterator iterator;
		
		/// Define const iterator to the lattice
		typedef SiteContainer::const_iterator const_iterator;
		
		/// Define the size of the lattice
		typedef SiteContainer::size_type size_type;
		
		/// \brief Constructor
		/// \param[in] length_side one dimension of the lattice
		Lattice(const size_type length_side):
		length_side_(length_side), site_container_() {
			name_set_.push_back(fcc);
			name_set_.push_back(hcp);
			// bridge, etc. are treated separately
			
			const size_type sz = length_side*length_side; 
			site_container_.reserve(sz);
			for (size_type i = 0; i != sz; ++i) {
				const Site::Name 
				name = name_set()[i % multiplicity()];
				site_container_.push_back(new Site(name, i, no_species));
			}
		}
		
		/// \brief Destructor
		~Lattice() {
			for (iterator iter = begin(), last = end();
				 iter != last; ++iter) 
			{ delete *iter; *iter = NULL; }
		}
		
		/// \brief Populate the lattice.
		///
		/// Equivalent to \code Lattice::Populate(begin(), end(), coverage, species); \endcode
		void Populate(double coverage, const Species& species);
		
		/// \brief Clean the lattice.
		///
		/// Equivalent to \code Lattice::Clean(begin(), end()); \endcode
		void Clean();
		
		/// \brief Set interactions on bridge sites
		///
		/// Equivalent to \code Lattice::SetBridge(begin(), end(), bridge); \endcode
		void set_bridge(const Bridge& bridge);
		
		/**
		 * \name Accessors
		 */
		/// \{
		
		/// Width (number of columns)
		size_type width()
		{ return length_side_; }
		
		/// Height (number of rows)
		size_type height()
		{ return length_side_; }
		
		/// \}
		
		/** 
		 * \name Operations on the lattice provided by std::vector
		 * \see http://www.sgi.com/tech/stl/Vector.html
		 */
		/// \{
				
		iterator begin()
		{ return site_container_.begin(); }
		
		iterator end()
		{ return site_container_.end(); }
		
		const_iterator begin() const
		{ return site_container_.begin(); }
		
		const_iterator end() const
		{ return site_container_.end(); }
		
		size_type size() const
		{ return site_container_.size(); }
		
		bool empty() const
		{ return site_container_.empty(); }
		
		Site* operator[](size_type n)
		{ return site_container_[n]; }
		
		const Site* const operator[](size_type n) const
		{ return site_container_[n]; }
		
		Site* at(size_type n)
		{ return site_container_.at(n); }
		
		const Site* const at(size_type n) const
		{ return site_container_.at(n); }
		
		/// \}
		
	private:
		Lattice (const Lattice&);
		void operator= (const Lattice&);
		
		const size_type length_side_;
		SiteContainer site_container_;
	};
	
	/**
	 * \name Actions over a lattice or sublattice, use with std::for_each
	 */
	/// \{
	
	/// \brief Populate the lattice
	/// \param[in] first lattice iterator
	/// \param[in] last  lattice iterator
	/// \param[in] coverage up to which lattice should be populated
	/// \param[in] species with which to populate the lattice
	void Populate(Lattice::iterator first, Lattice::iterator last,
				  double coverage, const Species& species);
	
	/// \brief Remove all species on the lattice
	/// \param[in] first lattice iterator
	/// \param[in] last  lattice iterator
	void Clean(Lattice::iterator first, Lattice::iterator last);
	
	/// \brief Set bridge sites on the lattice
	/// \param[in] first lattice iterator
	/// \param[in] last  lattice iterator
	/// \param[in] bridge type of bridge to consider
	void SetBridge(Lattice::iterator first, Lattice::iterator last,
				   const Bridge& bridge);
	
	/// \}
	
	/**
	 * \brief Functor: helical boundary conditions
	 */
	class HelicalBoundaryConditions: 
	public std::tr1::function<Site*(Site*, Interactions::TwoBody::RelativeIndexContainer::value_type)> {
	public:
		/// \brief Constructor
		/// \param[in] first iterator to the first element of the lattice
		/// \param[in] size of the lattice
		HelicalBoundaryConditions(Surface::Lattice::iterator first,
								  Surface::Lattice::size_type size):
		first_(first), size_lattice_(size) 
		{ };
		
		/// \brief Apply the boundary conditions
		/// \param[in] site_ptr site from which to look for neighbors
		/// \param[in] offset of the neirhbor
		/// \return a pointer to a site on the lattice at position offset from site_ptr
		Site* operator()(const Site* site_ptr, 
						 Interactions::TwoBody::RelativeIndexContainer::value_type offset);
		
	private:
		Surface::Lattice::iterator first_;
		Surface::Lattice::size_type size_lattice_;
	};
	
} // namespace Surface


/// Output operator
#include <ostream>
inline std::ostream& 
operator<< (std::ostream& os, const Surface::Lattice& lattice) {
	for (Surface::Lattice::const_iterator iter = lattice.begin(),
		 last = lattice.end(); iter != last; ++iter) 
	{ os << **iter << "\n"; }
	return os;
}

#endif
