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
#ifndef FRM_HPP
#define FRM_HPP

#include <set>
#include <vector>
#include <map>
#include <utility> // pair
#include <ostream>
#include <tr1/functional> // std::tr1::function
#include "Lattice.hpp"
#include "Reaction.hpp"

namespace Algorithms {
	
	/**
	 * \brief First reaction method algorithm
	 *
	 * \par Usage:
	 * Implemented as a functor
	 * \code
	 * Surface::Lattice lattice(128);
	 * // ... define lattice ...
	 * Frm frm(lattice.begin(), lattice.end());
	 * // ... create reactions Diffusion, Adsorption, etc.
	 * frm.AddReactionPtr(co_diffusion);
	 * frm.Init();
	 * frm(); // perform one Monte-Carlo step
	 * frm(); // etc.
	 * \endcode
	 *
	 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
	 * \date 2008-10-30
	 * \todo Use better container:
	 * change
	 * typedef std::multimap<Surface::Site*, Reactions::EventPtr> EventsPerSiteContainer;
	 * to 
	 * typedef ml_container::associative_vector<Surface::Site*, std::vector<Reactions::EventPtr> > EventsPerSiteContainer;	 
	 */
	class Frm {
		
		/// Functor to sort the events in the queue
		struct SortEventsOnTime:
		std::tr1::function<bool(Reactions::EventPtr, Reactions::EventPtr)>
		{
			bool operator()(Reactions::EventPtr lhs, Reactions::EventPtr rhs) const
			{ return lhs->Time() < rhs->Time(); }
		};
		
		/// Define queue of enabled events
		typedef std::set<Reactions::EventPtr, SortEventsOnTime> EventsQueue;
		
		/// \brief Events per Site
		/// \todo Reimplement using an associative vector
		typedef std::multimap<Surface::Site*, Reactions::EventPtr> EventsPerSiteContainer;
		
		/// Reactions::Reaction container
		typedef std::vector<Reactions::Reaction*> ReactionContainer;	
		
	public:	
		/// Define counter of reactions
		typedef std::map<Reactions::Reaction::Name, int> ReactionCounter;
		
		/// \brief Constructor
		/// \param[in] first iterator to the first element of the lattice
		/// \param[in] last iterator to the last element of the lattice
		Frm(Surface::Lattice::iterator first, Surface::Lattice::iterator last):
		reaction_container_(), event_queue_(), time_(0.0),
		lattice_beg_(first), lattice_end_(last)
		{ }
		
		/// \brief Create and sort the event list
		void Init();
		
		/// \brief Perform fastest event
		void operator()();
		
		/// \brief Clear all events
		void Clear()
		{ events_per_site_container_.clear(); event_queue_.clear(); }		
		
		/**
		 * \name Accessors
		 */
		/// \{
		
		/// \brief Return the simulation time
		/// \return time
		double time() const
		{ return time_; }
		
		/// \brief Return the reaction counter
		/// \return ReactionCounter
		ReactionCounter reaction_counter()
		{ return reaction_counter_; }	
		
		/// \}
		
		/**
		 * \name Modifiers
		 */
		/// \{
		
		/// \brief Set time in the simulation, useful to pass the real time
		/// between several algorithms
		/// \param[in] time
		void set_time(double time)
		{ time_ = time; }
		
		/// \brief Add a reaction the reaction container
		/// \param[in] &reaction
		void set_reaction_ptr(Reactions::Reaction* reaction)
		{ reaction_container_.push_back(reaction); }
		
		/// \}	
		
	private:
		Frm(const Frm&);
		void operator=(const Frm&);
		
		/**
		 * Manage the queue
		 */
		
		Reactions::EventPtr PopEvent() const
		{ return *event_queue_.begin(); }
		
		void InsertEvent(Surface::Site*, const Reactions::Reaction* const);
		
		/// Add/remove events after the state of the lattice has changed
		void UpdateQueue(Reactions::EventPtr);
		
		/// Reorder the events in the queue after the state of the lattice has changed
		void ReorderQueue(Reactions::EventPtr);
		
		/**
		 * Private members
		 */
		
		/// Sorted container of enabled events
		EventsQueue event_queue_;
		ReactionCounter reaction_counter_;	
		ReactionContainer reaction_container_;	
		EventsPerSiteContainer events_per_site_container_;
		
		typedef std::pair<EventsPerSiteContainer::iterator, 
		EventsPerSiteContainer::iterator> EventsRange;
		
		/// Real time, duration of the simulation
		double time_;
		
		/// Perform FRM between lattice_beg_ and lattice_end_
		Surface::Lattice::iterator lattice_beg_, lattice_end_;
	};
	
} // namespace Algorithms

#endif
