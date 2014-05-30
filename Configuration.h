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

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <memory>
#include <string>
#include <vector>

// A configuration for a backtracking problem
class Configuration {
	public:
		// Provide a default constructor even though we're disabling copy/assign
		Configuration() = default;

		// Make subclass instances destruct safely
		virtual ~Configuration() = default;

		// Return all this instance's *valid* successors
		virtual std::vector<std::shared_ptr<Configuration>> successors() const = 0;

		// Reveal whether this is a non-empty (non-initial) state
		virtual bool is_nonempty() const = 0;

		// Decide whether this is the long-sought fruit of our endeavers
		virtual bool is_goal() const = 0;

		// Represent this instance as a string, valid as long as this object is
		virtual operator const std::string &() const = 0;

		// Represent this instance as a string literal, valid until destruction
		operator const char *() const;

	// Disable copy and assign
	Configuration(const Configuration &tocopy) = delete;
	Configuration &operator=(const Configuration &toassign) = delete;
};

#endif
