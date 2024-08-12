#ifndef ASSIGN2_RULES_H
#define ASSIGN2_RULES_H

#include "GameBoard.h"
#include "Player.h"
#include "TileBag.h"

class Rules {
public:
    // Validate a move
    static bool validateMove(GameBoard* board, Tile* tile, int x, int y);

    // Validate multiple moves
    bool validateMoveEnhanced(GameBoard* board, const std::vector<Tile*>& tiles, const std::vector<std::pair<int, int>>& positions);

    // Calculate the score of a move
    static int calculateScore(GameBoard* board, int x, int y);

    // Check if the game is over
    static bool isGameOver(Player* player1, Player* player2, TileBag* tileBag);

private:
    // Helper functions for move validation and scoring
    static bool isValidPlacement(GameBoard* board, Tile* tile, int x, int y);
    
    // Check if tile placement is valid in rows
    static bool isRowInvalid(GameBoard* board, Tile* tile, int x, int y);
};

#endif // ASSIGN2_RULES_H
