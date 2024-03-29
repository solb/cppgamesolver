/*
 * Copyright (C) 2014 Sol Boucher
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with it.  If not, see <http://www.gnu.org/licenses/>.
 */

// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 03/05/14 Project 1

#ifndef WHEEL_CONFIG_H_
#define WHEEL_CONFIG_H_

#include "Configuration.h"

// A configuration for wheeling and dealing using backtracking
class WheelConfig : public Configuration {
	private:
		// Number of triads
		const unsigned num_triads_;

		// Number of spaces (i.e. thrice the number of triads)
		const unsigned num_spaces_;

		// Sum of each triad
		const unsigned each_sum_;

		// Each index is the sum of the *index+1*th intertriad connection
		const std::vector<unsigned> gap_sums_;

		// Each index reflects whether the number *index+1* may be placed
		std::vector<bool> available_spaces_; // coords [0, num_spaces_)

		// The numbers that have been placed in triads/positions
		std::vector<unsigned> config_; // vals (0, num_spaces_]

		// Index next config_ space that needs to be filled: possibly its length
		unsigned to_place_;

		// String representation of this instance
		mutable std::string repr_;

	public:
		WheelConfig(unsigned num_triads, std::vector<unsigned> &&gap_sums, std::vector<unsigned> &&config = std::vector<unsigned>());

		std::vector<std::shared_ptr<Configuration>> successors() const;

		bool is_nonempty() const;

		bool is_goal() const;

		operator const std::string &() const;

	private:
		// Copy constructor placing next_placed in the next available spot
		WheelConfig(const WheelConfig &predecessor, unsigned next_placed);
};

#endif
