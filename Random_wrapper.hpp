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
/**
 * @file
 * wrapper for random number generators
 * it offers an interface similar to RandomLib, by Charles Karney
 * and uses it by default. However, being in this wrapper allows
 * changing generator easily.
 */

#ifndef RANDOM_HPP
#define RANDOM_HPP

// http://charles.karney.info/random/
#include <RandomLib/Random.hpp>

namespace Random {
	void Reseed()
	{ ::RandomLib::Random::Global.Reseed(); }

	void Reseed(int seed)
	{ RandomLib::Random::Global.Reseed(seed); }

	double Fixed()
	{ return ::RandomLib::Random::Global.Fixed(); }
	
	int Integer()
	{ return ::RandomLib::Random::Global.Integer(); }
	
	int Integer(int n)
	{ return ::RandomLib::Random::Global.Integer(n); }
}


#endif
