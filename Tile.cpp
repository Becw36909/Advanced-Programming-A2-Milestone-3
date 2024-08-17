#include "Tile.h"

#include <iostream>
#include <sstream>

#include "TileCodes.h"

Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

// Copy Constructor
Tile::Tile(const Tile& other) : colour(other.colour), shape(other.shape) {}

Colour Tile::getColour() const { return colour; }

Shape Tile::getShape() const { return shape; }

// Overload the << operator for the Tile class
std::ostream& operator<<(std::ostream& os, const Tile& tile) {
  os << tile.getColour() << tile.getShape();
  return os;
}

std::string Tile::print() {
  return colour + std::to_string(shape);
}

// This function checks if two Tile objects are equal.
bool Tile::operator==(const Tile& other) const {
  return colour == other.getColour() && shape == other.getShape();
}

// This function checks if two Tile objects are not equal.
bool Tile::operator!=(const Tile& other) const { return !(*this == other); }

// This function checks if the tile has a match by color or shape, is the same
// or doesn't match at all
std::string Tile::matchType(const Tile& other) {
  if (colour == other.getColour() && shape == other.getShape()) {
    return "same";
  }
  if (colour == other.getColour()) {
    return "color";
  }
  if (shape == other.getShape()) {
    return "shape";
  }
  return "no-match";
}

std::string Tile::toColouredString() const {
  auto it = colorCodes.find(colour);
  std::string colorCode = (it != colorCodes.end()) ? it->second : "";
  std::stringstream ss;
  ss << colorCode << colour << shape << ANSI_COLOR_RESET;
  return ss.str();
}
