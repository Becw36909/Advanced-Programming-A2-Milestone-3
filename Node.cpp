#include "Node.h"

// Constructor
Node::Node(Tile* tile, Node* next) : tile(tile), next(next) {}

// Destructor
Node::~Node() {
    delete tile;
}

// Copy constructor
Node::Node(const Node& other)
    : tile(other.tile ? new Tile(*other.tile) : nullptr), next(other.next) {}

// Copy assignment operator
Node& Node::operator=(const Node& other) {
  if (this != &other) {
    delete tile;
    tile = other.tile ? new Tile(*other.tile) : nullptr;
    next = other.next;
  }
  return *this;
}

// Move constructor
Node::Node(Node&& other) : tile(other.tile), next(other.next) {
  other.tile = nullptr;
  other.next = nullptr;
}

// Move assignment operator
Node& Node::operator=(Node&& other) {
  if (this != &other) {
    delete tile;
    tile = other.tile;
    next = other.next;
    other.tile = nullptr;
    other.next = nullptr;
  }
  return *this;
}

// Getters
Tile* Node::getTile() const {
    return tile;
}

Node* Node::getNext() const {
    return next;
}

// Setters
void Node::setTile(Tile* tile) {
    this->tile = tile;
}

void Node::setNext(Node* next) {
    this->next = next;
}
