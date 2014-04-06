// Sol Boucher
// CSC-541-01 Strout's F(ant)abulous C++11 Concoction
// 04/09/14 Project 2

#ifndef BOX_CONFIG_H_
#define BOX_CONFIG_H_

#include "Configuration.h"

// A configuration for boxing with lasers using backtracking
class BoxConfig : public Configuration {
	private:
		// The edge label sentinel character indicating a hit
		static const char HIT_CHAR = 'h';

		// The edge label sentinel character indicating a reflection
		static const char REFL_CHAR = 'r';

		// Representation of an empty spot in the grid
		static const char REPR_EMPT = '\'';

		// Representation of a device in the grid
		static const char REPR_DEVC = '*';

		// The indices of the edge labels for a given side of the board
		static const std::vector<std::vector<char>>::size_type TOP_EDGE = 0;
		static const std::vector<std::vector<char>>::size_type RIGHT_EDGE =
				1;
		static const std::vector<std::vector<char>>::size_type BOTTOM_EDGE =
				2;
		static const std::vector<std::vector<char>>::size_type LEFT_EDGE =
				3;

		// Number of "black box" devices that need to be placed
		const unsigned num_devices_;

		// Edge labels (for the board border described by the *_EDGE constants)
		const std::vector<std::vector<char>> edge_labels_;

		// Play board, where each true represents a mysterious device
		std::vector<std::vector<bool>> board_;

		// Which device to move in order to generate further configurations
		unsigned nth_device_;

		// String representation of this instance
		mutable std::string repr_;

	public:
		BoxConfig(unsigned num_devices,
				std::vector<std::vector<char>> &&edge_labels);

		std::vector<std::shared_ptr<Configuration>> successors() const;

		bool is_nonempty() const;

		bool is_goal() const;

		operator const std::string &() const;

	private:
		// Copy constructor for manufacturing successors. Moves the nth_device
		// to the first unused position after the current last device's
		// coordinates. The new config will move the next device. By default,
		// no device is acutally changed: the next one to move merely increments
		// Precondition: There *must* be space at the end of all things!
		BoxConfig(const BoxConfig &basis, unsigned nth_device = -1);

		// Start tracing from the spots on the border, returning whether valid
		bool is_valid() const;

		// Trace from a specific spot on the border, returning whether the
		// labels on the two ends of the laser match
		bool trace_from_label(char edge_label,
				std::vector<std::vector<bool>>::size_type r,
				std::vector<std::vector<bool>>::size_type c,
				std::vector<std::vector<bool>>::size_type dr,
				std::vector<std::vector<bool>>::size_type dc) const;

		// Manipulate row/column deltas to reflect a CW or CCW rotation
		static void rotate_deltas(std::vector<std::vector<bool>>::size_type &dr,
				std::vector<std::vector<bool>>::size_type &dc, bool ccw);

		// Returns a string representation of an edge label, which is a
		// character if the label is a sentinel value, and numeric otherwise
		static std::string represent_label(char label);

	friend int main(int, char *[]);
};

#endif
