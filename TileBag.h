#ifndef ASSIGN2_TILEBAG_H
#define ASSIGN2_TILEBAG_H

#include "LinkedList.h"
#include "Tile.h"
#include <vector>
#include <random>

#define QUANTITY_OF_EACH_TILE 2

class TileBag {
public:
    TileBag();
    TileBag(std::vector<Tile*>& tiles);
    ~TileBag();

    // Copy constructor
    TileBag(const TileBag& other);

    // Copy assignment operator
    TileBag& operator=(const TileBag& other);

    // Move constructor
    TileBag(TileBag&& other);

    // Move assignment operator
    TileBag& operator=(TileBag&& other);

    void shuffle(unsigned int randSeed);
    Tile* drawTile();
    void addTile(Tile* tile);
    LinkedList* getTiles();
    bool isEmpty() const;
    
private:
    LinkedList tiles;

    // Debug function to print the number of tiles
    void printTileCount() const;

    // Function to clear all tiles from the bag
    void clear();

    std::vector<Tile*> seedTiles();
    void Initialise(std::vector<Tile*>& tiles);

};

#endif // ASSIGN2_TILEBAG_H
