#include "TileBag.h"
#include "TileCodes.h"
#include <iostream>
#include <random>

// Constructor
TileBag::TileBag() {
    std::vector<Tile*> tiles = seedTiles();
    Initialise(tiles);
}

TileBag::TileBag(std::vector<Tile*>& tiles) {
    Initialise(tiles);
}

// Destructor
TileBag::~TileBag() {
    // Clean up the dynamically allocated tiles
    while (!tiles.isEmpty()) {
        delete tiles.removeFront();
    }
}

// Copy constructor
TileBag::TileBag(const TileBag& other) {
  // Initialize the LinkedList
  tiles = LinkedList();
  // Copy each Tile in the list
  Node* current = other.tiles.getHead();
  while (current != nullptr) {
    // Deep copy Tile
    addTile(new Tile(*current->getTile()));
    current = current->getNext();
  }
}

// Copy assignment operator
TileBag& TileBag::operator=(const TileBag& other) {
  if (this != &other) {
    // Clean up existing resources
    clear();

    // Initialize the LinkedList
    tiles = LinkedList();
    // Copy each Tile in the list
    Node* current = other.tiles.getHead();
    while (current != nullptr) {
      addTile(new Tile(*current->getTile()));  // Deep copy Tile
      current = current->getNext();
    }
  }
  return *this;
}

// Move constructor
TileBag::TileBag(TileBag&& other) : tiles(other.tiles) {
  // Transfer ownership of resources &
  // Reset the other TileBag's LinkedList
  other.tiles = LinkedList();
}

// Move assignment operator
TileBag& TileBag::operator=(TileBag&& other) {
  if (this != &other) {
    // Clean up existing resources
    clear();

    // Move the LinkedList
    tiles = std::move(other.tiles);
    // Reset the other TileBag's LinkedList
    other.tiles = LinkedList();
  }
  return *this;
}

// Function to clear all tiles from the bag
void TileBag::clear() {
  while (!tiles.isEmpty()) {
    // Remove and delete each Tile
    delete tiles.removeFront();
  }
}

// Shuffle the tiles in the bag
void TileBag::shuffle(unsigned int randSeed) {

    std::default_random_engine engine(randSeed);

    int length = tiles.getLength();
    std::vector<Tile*> v1;
    std::vector<Tile*> v2;

    for (int i = 0; i < length; i++) {
        if (randSeed == 0 || std::uniform_int_distribution<>(0, 1)(engine))
            v1.push_back(tiles.removeFront());
        else
            v2.push_back(tiles.removeFront());
    }

    v1.insert(v1.begin(), v2.begin(), v2.end());

    for (Tile* tile : v1) {
        if (randSeed == 0 || std::uniform_int_distribution<>(0, 1)(engine))
            tiles.addBack(tile);
        else
            tiles.addFront(tile);
    }
}

void TileBag::Initialise(std::vector<Tile*>& tiles) {
    for (Tile* tile : tiles) {
        addTile(tile);
    }
}

// Draw a tile from the bag
Tile* TileBag::drawTile() {
    if (tiles.getHead() == nullptr) {
        return nullptr;
    }

    Tile* tile = tiles.getHead()->getTile();
    tiles.remove(tile);
    return tile;
}

// Add a tile to the bag
void TileBag::addTile(Tile* tile) {
    tiles.addBack(tile);
}

// Getter for the tiles
LinkedList* TileBag::getTiles() {
    return &tiles;
}

// Check if the tile bag is empty
bool TileBag::isEmpty() const {
    return tiles.getHead() == nullptr;
}

std::vector<Tile*> TileBag::seedTiles()
{
        std::vector<Tile*> tilesVector;
        char colours [] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
        int shapes [] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

        int quantityOfEachTile = QUANTITY_OF_EACH_TILE;
        // Iterates through each set of tile options twice and adds them to the bag
        for (int i = 0; i < quantityOfEachTile; ++i){
            for (char colour : colours) {
                for (int shape : shapes) {
                    Tile* tile = new Tile(colour, shape);
                    tilesVector.push_back(tile);
                }
            }
        }
        return tilesVector;
}
