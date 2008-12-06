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
#ifndef REACTION_HPP
#define REACTION_HPP

#include <string>
#include <vector>
#include <ostream>
#include <tr1/memory>

#include "Species.hpp"
#include "EventPtr.hpp"

namespace Surface { class Site; }

/// The chemical reactions
namespace Reactions {

/**
 * \brief Abstract base class for reactions
 *
 * \par Usage:
 * Adding new 'Reactions', i.e., diffusion, adsorbtion, etc. bound to a site on the
 * lattice boils down to inheriting from this abstract base class (ABC) and
 * providing a do_update(), and a do_enabled() methods, a public constructor (ctor)
 * and a private copy constructor.
 * The ctor can also add relevant information (another Site, for example) to the
 * derived class.
 *
 * - do_enabled() checks, whether the event is enabled;
 * - do_update() updates the state of the lattice; performs the 'Reaction'
 *
 * \par Usage:
 * The class is implemented as a functor:
 * calling its operator() performs the reaction
 *
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2008-10-30
 */
class Reaction
	// If the parameters are known at compile-time, there can be a run speed
	// improvement by using templates, see
	// http://www.parashift.com/c++-faq-lite/pointers-to-members.html#faq-33.11
	//
	// Could also use the curiously recurring template pattern
	{
		
	public:
		/// Define name
		typedef std::string Name;
		
		/// Container of Reaction::pointer s
		typedef std::vector<EventPtr> EventContainer;
		
		/// Container of Surface::Site* on which the reaction is performed
		typedef std::vector<Surface::Site*> SitePtrContainer;
		
		/// Destructor
		virtual ~Reaction() { };
		
		/**
		 * \name Accessors
		 */
		/// \{
		
		/// name
		virtual Name name() const = 0;
		
		/// \}
		
		/// Draw time from \f$-\frac{1}{k_{cc'}}\ln u\f$, with \f$k_{cc'}\f$ Arrhenius expression
		virtual void UpdateTime() = 0;
		
		/// Return reaction duration
		virtual double Time() const = 0;
		
		/// Test whether the Reaction is enabled
		virtual bool Enabled() const = 0;
		
		/// Callback, perform Reaction, return time
		virtual void operator()() = 0;
		
		/// Return newed ptr to enabled events
		virtual EventContainer GetEvents(const Surface::Site*) const = 0;
		
		/// Return Surface::Site* to be updated after the Reaction has been performed
		virtual SitePtrContainer site_container() const = 0;		
	};

} // namespace Reactions
	
#endif
