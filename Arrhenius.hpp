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
/** \file Arrhenius.hpp
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net> 
 * \date 2007-09-12
 */

#ifndef ARRHENIUS_HPP
#define ARRHENIUS_HPP

#include <cmath> // provide exp

/// Boltzman constant
extern double BOLTZMAN;

/// \brief Equation of Arrhenius
/// \param[in] preexponential_factor
/// \param[in] activation_energy
/// \param[in] temperature
/// return \f$ \nu \exp \frac{E_a}{k_B T} \f$
inline double Arrhenius(double preexponential_factor, double activation_energy, double temperature)
{ return preexponential_factor*exp(-activation_energy/(BOLTZMAN*temperature)); }

#endif
