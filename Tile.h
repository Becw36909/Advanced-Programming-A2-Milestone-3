#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <iostream>
#include <string>

#include "TileCodes.h"

typedef char Colour;
typedef int Shape;

class Tile {
 public:
  Tile(Colour colour, Shape shape);

  // Copy Constructor
  Tile(const Tile& other);

  Colour getColour() const;
  Shape getShape() const;

  // This method returns the string representation of the tile with color
  // codes
  std::string toColouredString() const;

  std::string print();

  // Function to overload the << operator
  friend std::ostream& operator<<(std::ostream& os, const Tile& tile);

  // Equality operator for Tile comparison
  bool operator==(const Tile& other) const;

  // Inequality operator for Tile comparison
  bool operator!=(const Tile& other) const;

  // Function to check if the tile has a match by color or shape
  std::string matchType(const Tile& other);

 private:
  Colour colour;
  Shape shape;
};

#endif  // ASSIGN2_TILE_H
