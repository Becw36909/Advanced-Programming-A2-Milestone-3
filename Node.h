#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"

class Node {
public:
    Node(Tile* tile, Node* next = nullptr);
    ~Node();

    // Copy constructor
    Node(const Node& other);

    // Copy assignment operator
    Node& operator=(const Node& other);

    // Move constructor
    Node(Node&& other);

    // Move assignment operator
    Node& operator=(Node&& other);

    Tile* getTile() const;
    Node* getNext() const;

    void setTile(Tile* tile);
    void setNext(Node* next);

private:
    Tile* tile;
    Node* next;
};

#endif // ASSIGN2_NODE_H
