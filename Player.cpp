#include "Player.h"
#include <vector>
#include "Tile.h"

// Constructor
Player::Player(const std::string& name) : name(name), score(0) {}

// Destructor
Player::~Player() {
    // Cleanup the linked list of tiles in hand
    Tile* tile;
    while ((tile = hand.removeFront()) != nullptr) {
        delete tile;
    }
}

// Copy constructor
Player::Player(const Player& other) : name(other.name), score(other.score) {
  // Copy the LinkedList (deep copy of hand)
  Node* current = other.hand.getHead();
  while (current != nullptr) {
    // Deep copy Tile
    addTileToHand(new Tile(*current->getTile()));  
    current = current->getNext();
  }
}

// Move constructor
Player::Player(Player&& other)
    : name(std::move(other.name)),
      score(other.score),
      hand(std::move(other.hand)) {
  // Reset the other Player's hand
  other.score = 0;
}

// Copy assignment operator
Player& Player::operator=(const Player& other) {
  if (this != &other) {
    // Clean up existing resources
    clear();  

    name = other.name;
    score = other.score;

    // Copy the LinkedList (deep copy of hand)
    Node* current = other.hand.getHead();
    while (current != nullptr) {
        // Deep copy Tile
      addTileToHand(new Tile(*current->getTile()));  
      current = current->getNext();
    }
  }
  return *this;
}

// Move assignment operator
Player& Player::operator=(Player&& other) {
  if (this != &other) {
    // Clean up existing resources
    clear();  

    name = std::move(other.name);
    score = other.score;
    hand = std::move(other.hand);

    // Reset the other Player's hand
    other.score = 0;
  }
  return *this;
}

// Implementation of the clear function
void Player::clear() {
  Tile* tile;
  while ((tile = hand.removeFront()) != nullptr) {
    // Remove and delete each Tile
    delete tile;
  }
}

// Getter for player name
std::string Player::getName() const {
    return name;
}

// Setter for player name
void Player::setName(const std::string& newName) {
    name = newName;
}

// Getter for player score
int Player::getScore() const {
    return score;
}

// Setter for player score
void Player::setScore(int newScore) {
    score = newScore;
}

// Add a tile to the player's hand
void Player::addTileToHand(Tile* tile) {
    hand.addBack(tile);
}

// Add quantity of tiles to player's hand
void Player::drawQuantityTiles(TileBag* tileBag, int quantity) {
    for (int i = 0; i < quantity; i++) {
        Tile* newTile = tileBag->drawTile();
        if (newTile != nullptr) {
            hand.addBack(newTile);
        }
    }
}

// Remove a tile from the player's hand
Tile* Player::removeTileFromHand(Tile* tile) {
    Tile* removedTile = hand.remove(tile);
    if (removedTile == nullptr) {
        std::cout << "Error: Failed to remove tile from hand." << std::endl;
    }
    return removedTile;
}

// Getter for player's hand
LinkedList* Player::getHand() {
    return &hand;
}

// Setter for player's hand (initializes hand with given tiles)
void Player::setHand(const std::vector<Tile*>& tiles) {
    // Clear existing hand
    Tile* tile;
    while ((tile = hand.remove(nullptr)) != nullptr) {
        delete tile;
    }

    // Add tiles to hand
    for (Tile* tile : tiles) {
        hand.addBack(tile);
    }
}

// Get the string representation of the player's hand
std::string Player::toString() const {
    return hand.toString();
}

// Confirms player has tile in player's hand
bool Player::containsTile(Tile* tile) {
    for (int i = 0; i < hand.getLength(); ++i) {
        if (*(hand.get(i)) == *tile) {
            return true;
        }
    }
    return false;
}

