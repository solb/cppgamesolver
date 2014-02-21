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

		// Decide whether this is the long-sought fruit of our endeavers
		virtual bool is_goal() const = 0;

		// Represent this instance as a string
		virtual operator const std::string() const = 0;

		// Represent this instance as a string literal
		operator const char *() const;

	// Disable copy and assign
	Configuration(const Configuration &tocopy) = delete;
	Configuration &operator=(const Configuration &toassign) = delete;
};

#endif
