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
// Author: Mathias Laurin

#include "Frm.hpp"


#include <cassert>
#include "Reaction.hpp" // operator()
void Algorithms::Frm::operator()() {	
	// exception
	if (event_queue_.size() == 0) exit(1);
	
	// get fastest
	Reactions::EventPtr event = PopEvent();
	
	// perform event
	assert(event->Enabled());
	(*event)(); 
	time_ += event->Time();
	
	++reaction_counter_[event->name()];
	
	// add/remove events enabled/disabled
	UpdateQueue(event);
	
	ReorderQueue(event);
	
	assert(event.unique());
}


#include <algorithm>
#include <tr1/functional>
void Algorithms::Frm::Init() {
	// make sure EventsQueue is empty
	if (!event_queue_.empty()) Clear();
	// find all enabled events on the lattice
	using std::tr1::bind; using std::tr1::placeholders::_1;
	for (Surface::Lattice::iterator lattice_iter = lattice_beg_;
		 lattice_iter != lattice_end_; ++ lattice_iter) 
	{ std::for_each(reaction_container_.begin(), reaction_container_.end(),
					bind(&Frm::InsertEvent, this, *lattice_iter, _1)); }
}


void Algorithms::Frm::InsertEvent(Surface::Site* site_ptr, const Reactions::Reaction* const reaction_ptr) {
	// get newly enabled events
	Reactions::Reaction::EventContainer new_event_container = reaction_ptr->GetEvents(site_ptr);
	
	// store copy into EventsQueue
	event_queue_.insert(new_event_container.begin(), new_event_container.end());
	
	// store into EventsPerSiteContainer
	for (Reactions::Reaction::EventContainer::iterator
		 iter = new_event_container.begin(), 
		 last = new_event_container.end();
		 iter != last; ++iter)
	{ events_per_site_container_.insert(std::make_pair(site_ptr, (*iter))); }
}


#include <algorithm>
#include <tr1/functional>
void Algorithms::Frm::UpdateQueue(Reactions::EventPtr event) {
	using std::tr1::bind; using std::tr1::placeholders::_1;
	Reactions::Reaction::SitePtrContainer site_container = event->site_container();
	
	for (Reactions::Reaction::SitePtrContainer::iterator iter = site_container.begin(),
		 last = site_container.end(); iter != last; ++iter) {
		Surface::Site* site_ptr = *iter;
		// Erase events
		// get events to erase from EventsPerSiteContainer
		for (EventsRange events_range = events_per_site_container_.equal_range(site_ptr); 
			 events_range.first != events_range.second; ++events_range.first)
			// remove from EventsQueue
		{ event_queue_.erase(event_queue_.find(events_range.first->second)); }
		// remove from EventsPerSiteContainer
		events_per_site_container_.erase(site_ptr);
		
		// Find enabled events
		using std::tr1::bind; using std::tr1::placeholders::_1;
		std::for_each(reaction_container_.begin(), reaction_container_.end(),
					  bind(&Frm::InsertEvent, this, site_ptr, _1));
	}
}


#include "Site.hpp"
#include "Geometry.hpp"
void Algorithms::Frm::ReorderQueue(Reactions::EventPtr event) {
	const Reactions::Reaction::SitePtrContainer&
	site_container = event->site_container();
	
	// event_sites have already been taken care of
	std::set<Surface::Site*> neighbor_set (site_container.begin(), site_container.end());	
	
	// FOR EACH Site IN site_container
	for (Reactions::Reaction::SitePtrContainer::const_iterator
		 site_container_it = site_container.begin(),
		 site_container_end = site_container.end();
		 site_container_it != site_container_end;
		 ++site_container_it ) {
		// FOR EACH TwoBodyInteractions IN Site
		for (std::pair<Surface::Site::TwoBodyInteractionsContainer::const_iterator, 
			 Surface::Site::TwoBodyInteractionsContainer::const_iterator>
			 two_body_iter = (*site_container_it)->two_body_interactions_range();
			 two_body_iter.first != two_body_iter.second;
			 ++two_body_iter.first) {
			// FOR EACH neighbor Site IN TwoBodyInteractions
			for (Interactions::TwoBody::Geometry::NeighborContainer::const_iterator
				 neighbor_container_it = two_body_iter.first->neighbor_container().begin(),
				 neighbor_container_end = two_body_iter.first->neighbor_container().end();
				 neighbor_container_it != neighbor_container_end;
				 ++neighbor_container_it) {
				// IF NOT already updated THEN update its position in the queue
				if (neighbor_set.insert(*neighbor_container_it).second) {
					// get events to update from EventsPerSiteContainer
					for (EventsRange events_range = events_per_site_container_.equal_range(*neighbor_container_it); 
						 events_range.first != events_range.second; ++events_range.first) {
						// update queue, cf. Meyers, Effective STL, Item 22 pp99-100
						// locate event in queue
						EventsQueue::iterator events_queue_iter = event_queue_.find(events_range.first->second);
						// create local copy of event
						Reactions::EventPtr event = *events_queue_iter;
						// remove original event from queue
						event_queue_.erase(events_queue_iter++);
						// reposition hint
						if (events_queue_iter != event_queue_.begin()) --events_queue_iter;
						// update local copy
						event->UpdateTime();
						// insert updated event into queue
						event_queue_.insert(events_queue_iter, event);
					}
				}
			}
		}
	}
}

