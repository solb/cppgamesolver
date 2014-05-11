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

		// Number of "black box" devices that need to be placed
		const unsigned num_devices_;

		// Edge labels (for the board border described by the *_EDGE constants)
		const std::vector<std::vector<char>> edge_labels_;

		// Play board, where each true represents a mysterious device
		std::vector<std::vector<bool>> board_;

		// Which device to move in order to generate *further* configurations
		unsigned nth_device_;

		// Coordinates of the piece placed most recently (and is now fixed)
		std::vector<std::vector<char>>::size_type last_placed_row_;
		std::vector<char>::size_type last_placed_col_;

		// String representation of this instance
		mutable std::string repr_;

	public:
		// The indices of the edge labels for a given side of the board
		static const std::vector<std::vector<char>>::size_type TOP_EDGE = 0;
		static const std::vector<std::vector<char>>::size_type RIGHT_EDGE =
				1;
		static const std::vector<std::vector<char>>::size_type BOTTOM_EDGE =
				2;
		static const std::vector<std::vector<char>>::size_type LEFT_EDGE =
				3;

		BoxConfig(unsigned num_devices,
				std::vector<std::vector<char>> &&edge_labels);

		std::vector<std::shared_ptr<Configuration>> successors() const;

		bool is_nonempty() const;

		// Start tracing from the spots on the border, returning whether valid
		bool is_goal() const;

		// Getter for read-only access to edge_labels_[edge]
		const std::vector<char> &edge_labels(std::vector<char>::size_type edge)
				const;

		// Setter for an entirely new board_
		// Precondition: The new board must be the same size as the old one and
		// 					have exactly num_devices_ devices preplaced on it
		// Postcondition: The configuration will start backtracking from the
		// 					very first black box device on the board
		void set_board(const std::vector<std::vector<bool>> &board);

		// Getter for read-only access to board_[row]
		const std::vector<bool> &board(std::vector<bool>::size_type row) const;

		// Checks whether the configurations' devices are placed identically
		bool operator==(const BoxConfig &another) const;

		operator const std::string &() const;

	private:
		// Copy constructor for manufacturing successors. Moves the nth_device
		// to the first unused position after the current last device's
		// coordinates. The new config will move the next device. By default,
		// no device is acutally changed: the next one to move merely increments
		// Precondition: There *must* be space at the end of all things!
		BoxConfig(const BoxConfig &basis, unsigned nth_device = -1);

		// Trace from a specific spot on the border, returning whether the
		// labels on the two ends of the laser match
		bool trace_from_label(char edge_label,
				std::vector<std::vector<bool>>::size_type r,
				std::vector<std::vector<bool>>::size_type c,
				std::vector<std::vector<bool>>::size_type dr,
				std::vector<std::vector<bool>>::size_type dc) const;

		// Returns whether the provided edge label is a hit, and whether its
		// immediate neighbors are either hits or reflections
		inline bool valid_edge_for_touching_device(
				std::vector<std::vector<char>>::size_type edge,
				std::vector<char>::size_type offset) const;

		// Manipulate row/column deltas to reflect a CW or CCW rotation
		static void rotate_deltas(std::vector<std::vector<bool>>::size_type &dr,
				std::vector<std::vector<bool>>::size_type &dc, bool ccw);

		// Finds the index of the first device to whose placement we won't have
		// committed, which is often the first whose location changed between
		// the current board and the supplied candidate board
		unsigned first_modified_device(
				const std::vector<std::vector<bool>> &candidate) const;

	public:
		// Returns a string representation of an edge label, which is a
		// character if the label is a sentinel value, and numeric otherwise
		static std::string represent_label(char label);
};

bool BoxConfig::valid_edge_for_touching_device(
				std::vector<std::vector<char>>::size_type edge,
				std::vector<char>::size_type offset) const {
	return edge_labels_[edge][offset] == HIT_CHAR &&
			(offset - 1 >= board_[edge].size() ||
					edge_labels_[edge][offset - 1] == HIT_CHAR ||
					edge_labels_[edge][offset - 1] == REFL_CHAR) &&
			(offset + 1 >= board_[edge].size() ||
					edge_labels_[edge][offset + 1] == HIT_CHAR ||
					edge_labels_[edge][offset + 1] == REFL_CHAR);
}

#endif
