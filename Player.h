#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include <string>
#include <vector>
#include "LinkedList.h"
#include "TileBag.h"

class Player {
public:
    // Constructor
    Player(const std::string& name);

    // Destructor
    ~Player();

    // Copy constructor
    Player(const Player& other);

    // Move constructor
    Player(Player&& other);

    // Copy assignment operator
    Player& operator=(const Player& other);

    // Move assignment operator
    Player& operator=(Player&& other);

    // Getter for player name
    std::string getName() const;

    // Setter for player name
    void setName(const std::string& newName);

    // Getter for player score
    int getScore() const;

    // Setter for player score
    void setScore(int newScore);

    // Add a tile to the player's hand
    void addTileToHand(Tile* tile);

    // Add quantity of tiles to player's hand
    void drawQuantityTiles(TileBag* tileBag, int quantity);

    // Remove a tile from the player's hand
    Tile* removeTileFromHand(Tile* tile);

    // Get the player's hand
    LinkedList* getHand();

    // Setter for player's hand (initializes hand with given tiles)
    void setHand(const std::vector<Tile*>& tiles);

    // Get the string representation of the player's hand
    std::string toString() const; // Added toString method

    // Confirms player has tile in player's hand
    bool containsTile(Tile* tile);

private:
    std::string name;
    int score;
    LinkedList hand;

    // Helper function to clear resources
    void clear();
};

#endif // ASSIGN2_PLAYER_H
